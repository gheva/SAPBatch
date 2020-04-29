#include "fs/directoryiterator.h"
#include "wavfile/wavfile.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <thread>
#include "thread/synqueue.h"

using namespace sap;
using namespace std;

mutex lock_;

void processor(DirectoryIterator& iter, Fft& fft, MultiTaper* tapers, int id, fft_buffers* buffers, MySQL& connection)
{
  string file_name;
  while ((file_name = iter.next_file()) != "")
  {
    cerr << file_name << "(" << id << ")" << endl;

    WAVFile wav(file_name, "Milliseconds");
    wav.add_tapers(tapers);
    wav(fft, *buffers, connection);
  }
}

void process(const string& root, int thread_count, vector<MySQL*>& connections)
{
  DirectoryIterator diriter(root);
  int window_size(44);
  Fft fft(window_size);
  MultiTaper* tapers = new MultiTaper(window_size);
  vector<std::thread> pool;
  vector<fft_buffers*> buffers;
  SynQueue queue;

  for (int i = 0; i < thread_count; ++i)
  {
    fft_buffers* tmp = new fft_buffers;;
    tmp->in_ = (float*)fftwf_malloc(sizeof(float) * window_size);
    tmp->out1_ = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * window_size);
    tmp->out2_ = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * window_size);
    buffers.push_back(tmp);
    pool.emplace_back(processor, std::ref(diriter), std::ref(fft), tapers, i, buffers[i], std::ref(*connections[i]));
  }

  for (int i = 0; i < thread_count; ++i)
  {
    pool[i].join();
    fft_buffers* tmp = buffers[i];
    fftwf_free(tmp->in_);
    fftwf_free(tmp->out1_);
    fftwf_free(tmp->out2_);
    delete tmp;
  }
  delete tapers;
}

int main(int argc, char** argv)
{
  if (argc < 5)
  {
    cerr << "Usage: " << argv[0] << " folder user pass db [thread_count]" << endl;
    exit(EXIT_FAILURE);
  }
  string root(argv[1]);
  int thread_count;
  if (argc == 6)
  {
    thread_count = atoi(argv[5]);
  }
  else
  {
    thread_count = 4;
  }
  MySQL connection(argv[2], argv[3], argv[4]);
  Table<MillisecondRecord> milisecond_table("Milliseconds");
  if (!milisecond_table.create(connection))
  {
    cerr << connection.error() << endl;
    exit(EXIT_FAILURE);
  }
  
  vector<MySQL*> connections;
  for (int i = 0; i < thread_count; ++i)
  {
    connections.push_back(new MySQL(argv[2], argv[3], argv[4]));
  }
  time_t start = time(nullptr);
  process(root, thread_count, connections);
  time_t end = time(nullptr);

  cout << "Runtime took " << end - start << " seconds" << endl;

  for (int i = 0; i < thread_count; ++i)
  {
    delete connections[i];
  }
  exit(EXIT_SUCCESS);
}

/* vim: set cindent sw=2 expandtab : */


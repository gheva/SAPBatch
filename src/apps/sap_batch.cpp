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

int runners;
mutex lock_;

void thread_function(DirectoryIterator& iter, Fft& fft, MultiTaper* tapers, int id, fft_buffers* buffers, SynQueue& queue)
{
  string file_name;
  while ((file_name = iter.next_file()) != "")
  {
    cerr << file_name << "(" << id << ")" << endl;

    WAVFile wav(file_name);
    wav.add_tapers(tapers);
    wav(fft, *buffers, queue);
  }
  std::lock_guard<std::mutex> lock(lock_);
  --runners;
  if (runners == 0)
  {
    queue.done();
  }
}

void sql_writer_thread(MySQL& connection, SynQueue& queue)
{
  cerr << "here " << runners;
  while (runners > 0)
  {
    Record* record = queue.dequeue();
    if (record == nullptr)
    {
      cerr << "here";
      break;
    }
    if (!record->insert(connection))
    {
      cerr << connection.error() << endl;
    }
  }
}

void multi_thread_run(const string& root, int thread_count, MySQL& connection)
{
  DirectoryIterator diriter(root);
  int window_size(44);
  Fft fft(window_size);
  MultiTaper* tapers = new MultiTaper(window_size);
  vector<std::thread> pool;
  vector<fft_buffers*> buffers;
  SynQueue queue;

  pool.emplace_back(sql_writer_thread, std::ref(connection), std::ref(queue));
  for (int i = 0; i < thread_count - 1; ++i)
  {
    fft_buffers* tmp = new fft_buffers;;
    tmp->in_ = (float*)fftwf_malloc(sizeof(float) * window_size);
    tmp->out1_ = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * window_size);
    tmp->out2_ = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * window_size);
    buffers.push_back(tmp);
    pool.emplace_back(thread_function, std::ref(diriter), std::ref(fft), tapers, i, buffers[i], std::ref(queue));
  }

  for (int i = 0; i < thread_count; ++i)
  {
    pool[i].join();
    if (i == 0) continue;
    fft_buffers* tmp = buffers[i-1];
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
  runners = thread_count - 1;
  MySQL connection(argv[2], argv[3], argv[4]);
  Table<MillisecondRecord> milisecond_table("Milliseconds");
  if (!milisecond_table.create(connection))
  {
    cerr << connection.error() << endl;
    exit(EXIT_FAILURE);
  }
  
  multi_thread_run(root, thread_count, connection);

  exit(EXIT_SUCCESS);
}

/* vim: set cindent sw=2 expandtab : */


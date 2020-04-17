#include "fs/directoryiterator.h"
#include "wavfile/wavfile.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <thread>

using namespace sap;
using namespace std;

void thread_function(DirectoryIterator& iter, Fft& fft, MultiTaper* tapers, int id, fft_buffers* buffers)
{
  string file_name;
  while ((file_name = iter.next_file()) != "")
  {
    cerr << file_name << "(" << id << ")" << endl;

    WAVFile wav(file_name);
    wav.add_tapers(tapers);
    wav(fft, *buffers);
  }
}

void single_thread_run(const string& root)
{
  DirectoryIterator fs1(root);
  int window_size(44);
  Fft fft(window_size);
  MultiTaper* tapers = new MultiTaper(window_size);
  fft_buffers* buffer = new fft_buffers;;
  buffer->in_ = (float*)fftwf_malloc(sizeof(float) * window_size);
  buffer->out1_ = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * window_size);
  buffer->out2_ = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * window_size);

  thread_function(fs1, fft, tapers, 0, buffer);
  fftwf_free(buffer->in_);
  fftwf_free(buffer->out1_);
  fftwf_free(buffer->out2_);
  delete buffer;
  delete tapers;
}

void multi_thread_run(const string& root, int thread_count)
{
  DirectoryIterator diriter(root);
  int window_size(44);
  Fft fft(window_size);
  MultiTaper* tapers = new MultiTaper(window_size);
  vector<std::thread> pool;
  vector<fft_buffers*> buffers;

  for (int i = 0; i < thread_count; ++i)
  {
    fft_buffers* tmp = new fft_buffers;;
    tmp->in_ = (float*)fftwf_malloc(sizeof(float) * window_size);
    tmp->out1_ = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * window_size);
    tmp->out2_ = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * window_size);
    buffers.push_back(tmp);
    pool.emplace_back(thread_function, std::ref(diriter), std::ref(fft), tapers, i, buffers[i]);
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
  if (argc < 2)
  {
    cerr << "Usage: " << argv[0] << " folder [thread_count]" << endl;
    exit(EXIT_FAILURE);
  }
  string root(argv[1]);
  int thread_count;
  if (argc == 3)
  {
    thread_count = atoi(argv[2]);
  }
  else
  {
    thread_count = 4;
  }
  time_t start = time(nullptr);
  single_thread_run(root);
  time_t end = time(nullptr);
  cout << "single thread took " << end - start << " seconds to complete" << endl;
  start = time(nullptr);
  multi_thread_run(root, thread_count);
  end = time(nullptr);
  cout << "multi thread took " << end - start << " seconds to complete" << endl;

  exit(EXIT_SUCCESS);
}

/* vim: set cindent sw=2 expandtab : */


#include <iostream>
#include <cstdlib>
#include <thread>
#include "wavfile/wavfile.h"
#include "tapers/multitaper.h"
#include "yin/yin.h"
#include "fft/fft.h"
#include "fs/directoryiterator.h"
#include <vector>

using namespace std;
using namespace sap;

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

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    cerr << "Usage: " << argv[0] << " folder" << endl;
    exit(EXIT_FAILURE);
  }
  //*
  if (!fftwf_init_threads())
  {
    cerr << "error initializing fftw" << endl;
    exit(EXIT_FAILURE);
  }

  fftwf_plan_with_nthreads(4);
  // */
  DirectoryIterator diriter(argv[1]);
  std::string root(argv[1]);
  int window_size(44);
  Fft fft(window_size);
  MultiTaper* tapers = new MultiTaper(window_size);
  int thread_count(4);

  std::vector<std::thread> pool;
  std::vector<fft_buffers*> buffers;
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
  }
}

/* vim: set cindent sw=2 expandtab : */


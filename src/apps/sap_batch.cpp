#include "fs/directoryiterator.h"
#include "wavfile/wavfile.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <thread>
#include "thread/synqueue.h"
#include "sql/filetable.h"

using namespace sap;
using namespace std;

mutex lock_;
int* nans;

WAVFile::options options;

void init_options(WAVFile::options& opts)
{
  opts.ms_table_name = "Milliseconds";
  opts.file_table_name = "FileTable";
  opts.min_entropy_freq = 5;
  opts.max_entropy_freq = 256;
  opts.baseline = 70;
  opts.frame_advance = 44;
  opts.upper_pitch_bound = 3;
  opts.lower_pitch_bound = 55;
  opts.spectrum_range = 260;
  opts.yin_min_freq = 300;
}

void processor(DirectoryIterator& iter, Fft& fft, Fft& cepst, MultiTaper* tapers, int id, fft_buffers* buffers, MySQL& connection)
{
  DirectoryIterator::iterator* ptr;
  while ((ptr = iter.next_file()) != nullptr)
  {
    cerr << ptr->file_name << "(" << id << ")" << endl;

    // Create a wavfile object to run the calculations
    WAVFile wav(ptr, options);
    wav.add_tapers(tapers);
    // Run the callculations on this file
    wav(fft, cepst, *buffers, connection);
    nans[id] += wav.nans();
    delete ptr;
  }
}

void process(const string& root, int thread_count, vector<MySQL*>& connections, int window_size)
{
  DirectoryIterator diriter(root);
  init_options(options);
  Fft fft(window_size);
  Fft cepstrum(512);
  MultiTaper* tapers = new MultiTaper(window_size);
  vector<std::thread> pool;
  vector<fft_buffers*> buffers;
  SynQueue queue;
  nans = new int[thread_count];

  for (int i = 0; i < thread_count; ++i)
  {
    nans[i] = 0;
    fft_buffers* tmp = new fft_buffers;;
    tmp->in_ = (float*)fftwf_malloc(sizeof(float) * window_size);
    tmp->out1_ = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * window_size);
    tmp->out2_ = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * window_size);
    tmp->cepst_in_ = (float*)fftwf_malloc(sizeof(float) * window_size);
    tmp->cepst_out_ = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * window_size);
    buffers.push_back(tmp);
    // Start a thread running the processor function
    pool.emplace_back(processor, std::ref(diriter), std::ref(fft), std::ref(cepstrum), tapers, i, buffers[i], std::ref(*connections[i]));
  }

  int nan = 0;
  for (int i = 0; i < thread_count; ++i)
  {
    nan += nans[i];
    pool[i].join();
    fft_buffers* tmp = buffers[i];
    fftwf_free(tmp->in_);
    fftwf_free(tmp->out1_);
    fftwf_free(tmp->out2_);
    fftwf_free(tmp->cepst_in_);
    fftwf_free(tmp->cepst_out_);
    delete tmp;
  }
  cout << "nans encountered " << nan << endl;

  delete[] nans;
  delete tapers;
}

int main(int argc, char** argv)
{
  int thread_count(4);
  int window_size(409);
  if (argc < 5)
  {
    cerr << "Usage: " << argv[0] << " folder user pass db [window size] [thread_count]" << endl;
    cerr << "The default values are:" << endl << "thread_count=" << thread_count << endl << "window_size=" << window_size << endl;
    exit(EXIT_FAILURE);
  }
  string root(argv[1]);
  if (argc == 6)
  {
    window_size = atoi(argv[5]);
  }

  if (argc == 7)
  {
    thread_count = atoi(argv[6]);
  }
  MySQL connection(argv[2], argv[3], argv[4]);
  Table<MillisecondRecord> milisecond_table("Milliseconds");
  if (!milisecond_table.create(connection))
  {
    cerr << connection.error() << endl;
    exit(EXIT_FAILURE);
  }

  FileTable file_table("FileTable");
  if (!file_table.create(connection))
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
  process(root, thread_count, connections, window_size);
  time_t end = time(nullptr);

  cout << "Runtime took " << end - start << " seconds" << endl;

  for (int i = 0; i < thread_count; ++i)
  {
    delete connections[i];
  }
  exit(EXIT_SUCCESS);
}

/* vim: set cindent sw=2 expandtab : */


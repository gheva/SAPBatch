#include <iostream>
#include <cstdlib>
#include "wavfile/wavfile.h"
#include "tapers/multitaper.h"
#include "yin/yin.h"
#include "fft/fft.h"

using namespace sap;
using namespace std;

void do_fft(WAVFile& wav)
{
  float* contents;
  wav.read_fully(&contents);
  fftwf_plan plan_;
  fftwf_complex* in_;
  fftwf_complex* out_;
  int size = 4400;
  in_ = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * size);
  out_ = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * size);
  plan_ = fftwf_plan_dft_1d(size, in_, out_, FFTW_FORWARD, FFTW_MEASURE);
  for (int i = 0; i < size; ++i)
  {
      in_[i][0] = contents[i];
      in_[i][1] = 0;
  }
  fftwf_execute(plan_);

  fftwf_destroy_plan(plan_);
  fftwf_free(in_);
  fftwf_free(out_);
  fftwf_cleanup();
}

void do_fft2(WAVFile& wav)
{
  float* contents;
  wav.read_fully(&contents);
  fftwf_plan plan_;
  float* in_;
  fftwf_complex* out_;
  int size = 400;
  MultiTaper tapers(size);
  in_ = (float*)fftwf_malloc(sizeof(float) * size);
  out_ = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * size);
  plan_ = fftwf_plan_dft_r2c_1d(size, in_, out_, FFTW_MEASURE);
  for (int i = 0; i < size; ++i)
  {
      in_[i] = contents[i] * tapers[0][i];
  }
  fftwf_execute(plan_);

  fftwf_destroy_plan(plan_);
  fftwf_free(in_);
  fftwf_free(out_);
  fftwf_cleanup();
}

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    exit(EXIT_FAILURE);
  }
  WAVFile wav(argv[1]);

  //int slices = total/44.1;

  int window_size = 44;
  do_fft2(wav);

  //*
  MultiTaper tapers(window_size);
  Fft fft1(window_size);
  Fft fft2(window_size);
  wav.add_tapers(&tapers);
  if (!wav(fft1, fft2))
  {
    cerr << "failed to preocees file" << endl;
  }
  else 
  {
    cerr << "success" << endl;
  }
  // */

  //initYin(sample_rate, 40);//option->minFreq);

}

/* vim: set cindent sw=2 expandtab : */


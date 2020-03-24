#include "fft.h"

namespace sap
{

Fft::Fft(int size)// : plan_(fftw_create_plan(size, FFTW_FORWARD, FFTW_MEASURE))
{
  // This might be changed to use ESTIMATE which might be faster
  in_ = (float*)fftw_malloc(sizeof(float) * size);
  out_ = (fftwf_complex*)fftw_malloc(sizeof(fftwf_complex) * size);
  plan_ = fftwf_plan_dft_r2c_1d(size, in_, out_, FFTW_MEASURE);
}

Fft::~Fft()
{
  fftwf_destroy_plan(plan_);
  fftw_free(in_);
  fftw_free(out_);
}

bool Fft::operator()()
{
  fftwf_execute(plan_);
  return true;
}

} // namespace

/* vim: set cindent sw=2 expandtab : */


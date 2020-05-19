#ifndef SAP_FFT_H__
#define SAP_FFT_H__

#include "fftw3.h"
namespace sap
{

struct fft_buffers
{
  float* in_;
  fftwf_complex* out1_;
  fftwf_complex* out2_;
  float* cepst_in_;
  fftwf_complex* cepst_out_;
};
class Fft
{
public:
  Fft(int size);
  virtual ~Fft();
  bool operator()();
  float* data() { return in_; }
  int size() { return size_; }
  void operator()(float* in, fftwf_complex* out);

protected:
private:
  fftwf_plan plan_;
  float* in_;
  fftwf_complex* out_;
  int size_;
};

} // namespace

#endif // SAP_FFT_H__

/* vim: set cindent sw=2 expandtab : */


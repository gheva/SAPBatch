#ifndef SAP_FFT_H__
#define SAP_FFT_H__

#include "fftw3.h"
namespace sap
{

class Fft
{
public:
  Fft(int size);
  virtual ~Fft();
  bool operator()();
  float* data() { return in_; }

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


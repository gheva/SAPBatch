#include "yin.h"

namespace sap
{

Yin::Yin(WAVFile wav, float min_frequency)
{
  unsigned int tmax = static_cast<unsigned int>(ceil(wav.sample_rate()/min_frequency));
  yin_.build(tmax, tmax); // ??? TODO this is from the old code the first argument should be the window size, which here seems to be the same as the tmax arg
  wav.read_fully(&buffer_);
  total_samples_ = wav.total_samples();
}

Yin::~Yin()
{
  delete[] buffer_;
}

bool Yin::operator()(float** out)
{
  int error_count = 0;
  // TODO handle errors
  float* tmp = new float[total_samples_];

  for (int i = 0; i < total_samples_; ++i)
  {
    if (error_count > 400)
    {
      tmp[i] = 0;
      continue;
    }
    try {
      float res = yin_.tick(buffer_[i]);
      if (res == -999)
      {
        ++error_count;
      }
      tmp[i] = res;
    } catch (std::exception& e) {
      if (i > 0)
      {
        tmp[i] = tmp[i-1];
      }
      else
      {
        tmp[i] = 0;
      }
    }
  }
  *out = tmp;
  return true;
}

} // namespace

/* vim: set cindent sw=2 expandtab : */


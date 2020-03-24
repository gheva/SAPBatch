#ifndef SAP_YIN_H__
#define SAP_YIN_H__

#include <exception>
#include "yinacf.h"
#include "wavfile/wavfile.h"
namespace sap
{

class Yin
{
public:
  Yin(WAVFile wav, float min_frequency);
  virtual ~Yin();
  bool operator()(float** out);
protected:
private:
  YinACF<float> yin_;
  float* buffer_;
  int total_samples_;
};

} // namespace

#endif // SAP_YIN_H__

/* vim: set cindent sw=2 expandtab : */


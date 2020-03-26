#include "wavfile.h"
#include <iostream>
#include <cstdio>
#include "yin/yin.h"

namespace sap
{

WAVFile::WAVFile(const std::string& path)
{
  audio_file_.load(path);
}

WAVFile::~WAVFile()
{
}

int WAVFile::sample_rate()
{
  return audio_file_.getSampleRate();
}

int WAVFile::total_samples()
{
  return audio_file_.getNumSamplesPerChannel();
}

bool WAVFile::fill_buffer_taper(uint64_t offset, uint64_t size, float* buffer, unsigned int taper_index)
{
  int total = total_samples();
  for (int i = offset, j=0; j < size; ++i, ++j)
  {
    if (i < total)
    {
      buffer[j] = audio_file_.samples[0][i] * tapers_->operator[](taper_index)[j];
    }
    else
    {
      buffer[j] = 0;
    }
  }
  return true;
}

bool WAVFile::fill_buffer(uint64_t offset, uint64_t size, float* buffer)
{
  // TODO handle all the cases where we end out of file
  for (int i = offset, j=0; j < size; ++i)
  {
    buffer[j] = audio_file_.samples[0][i];
  }
  return true;
}

bool WAVFile::read_fully(float** ret)
{
  float* tmp = new float[total_samples()];
  int total = total_samples();
  for (int i = 0; i < total; ++i)
  {
    tmp[i] = audio_file_.samples[0][i];
  }

  *ret = tmp;
  return true;
}

bool WAVFile::operator()(Fft& fft1, Fft& fft2)
{
  Yin yin(*this, 400);
  float* result;
  bool ret = yin(&result);
  if (!ret)
  {
    return ret;
  }
  int slices = float(total_samples())/44.1;
  pitches_ = new float[slices];
  float sr = sample_rate();
  float div = 44.1;
  for (int i = 0; i< slices; ++i)
  {
    pitches_[i] = sr * result[int(div * (float)i)];
  }
  int offset = 0;
  int advance = 44;
  while (offset < total_samples())
  {
    fill_buffer_taper(offset, fft1.size(), fft1.data(), 0);
    fill_buffer_taper(offset, fft2.size(), fft2.data(), 1);

    if (!fft1())
    {
      return false;
    }
    if (!fft2())
    {
      return false;
    }
    offset += fft1.size();
  }

  return true;
}

} // namespace

/* vim: set cindent sw=2 expandtab : */


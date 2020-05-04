#include "wavfile.h"
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cmath>
#include "yin/yin.h"
#include "thread/synqueue.h"

namespace sap
{

WAVFile::WAVFile(DirectoryIterator::iterator* iter, const std::string& table) : table_(table), file_name_(iter->file_name), nans_(0)
{
  audio_file_.load(iter->file_path);
  auto siter = file_name_.find('_');
  bird_id_ = file_name_.substr(0, siter);
  sscanf(file_name_.substr(siter).c_str(), "_%d.%lld_%d_%d_%d_%d_%d.wav", &serial_number_, &ms_from_midnight_, &month_, &day_, &hour_, &minute_, &second_);
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

bool WAVFile::operator()(Fft& fft, fft_buffers& buffers, MySQL& connection)
{
  Yin yin(*this, 300);
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
    fill_buffer_taper(offset, fft.size(), buffers.in_, 0);
    fft(buffers.in_, buffers.out1_);
    fill_buffer_taper(offset, fft.size(), buffers.in_, 1);
    fft(buffers.in_, buffers.out2_);
    offset += fft.size();
  }

  for (int i = 0; i < slices; ++i)
  {
    MillisecondRecord* record = table_.new_record();
    record->set("file", file_name_);
    record->set("index_in_file", i);
    if (std::isnan(pitches_[i]))
    {
      ++nans_;
      pitches_[i] = 0.0;
    }
        
    record->set("pitch", pitches_[i]);
    if (!record->insert(connection))
    {
      std::cerr << connection.error() << std::endl;
      std::cerr << file_name_ << "," << i << "," << pitches_[i] << std::endl;
    }
  }
  return true;
}


} // namespace

/* vim: set cindent sw=2 expandtab : */


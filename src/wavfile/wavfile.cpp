#include "wavfile.h"
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cmath>
#include "yin/yin.h"
#include "thread/synqueue.h"

namespace sap
{

WAVFile::WAVFile(DirectoryIterator::iterator* iter, const std::string& ms_table, const std::string& file_table_name) : ms_table_(ms_table), file_table_(file_table_name), file_name_(iter->file_name), nans_(0), file_index_(iter->file_index)
{
  audio_file_.load(iter->file_path);
  auto siter = file_name_.find('_');
  bird_id_ = file_name_.substr(0, siter);
  sscanf(file_name_.substr(siter).c_str(), "_%d.%lld_%d_%d_%d_%d_%d.wav", &serial_number_, &ms_from_midnight_, &month_, &day_, &hour_, &minute_, &second_);

  record_ = file_table_.new_record();
  record_->set("file_index", file_index_);
  record_->set("bird_ID", bird_id_);
  record_->set("file_name", iter->file_name);
  record_->set("file_age", 0.0);
  record_->set("bird_age", 0.0);
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

bool WAVFile::calculate_pitchse()
{
  Yin yin(*this, 300);
  float* result;
  bool ret = yin(&result);
  if (!ret)
  {
    return ret;
  }
  int slices = float(total_samples())/44.1;
  if (pitches_ != nullptr)
  {
    delete[] pitches_;
  }
  pitches_ = new float[slices];
  float sr = sample_rate();
  float div = 44.1;
  for (int i = 0; i< slices; ++i)
  {
    pitches_[i] = sr * result[int(div * (float)i)];
  }
  return true;
}

void WAVFile::calculate_frame(Fft& fft, fft_buffers& buffers, int offset)
{
  fill_buffer_taper(offset, fft.size(), buffers.in_, 0);
  fft(buffers.in_, buffers.out1_);
  fill_buffer_taper(offset, fft.size(), buffers.in_, 1);
  fft(buffers.in_, buffers.out2_);

  // 260 = spectrum range
  for (int i = 0; i < 260; ++i)
  {
    float fReal1, fReal2, fImag1, fImag2;

    fReal1 = buffers.out1_[i][0];
    fImag1 = buffers.out1_[i][1];
    fReal2 = buffers.out2_[i][0];
    fImag2 = buffers.out2_[i][1];
    float power_spec_i = fReal1 * fReal1 + fReal2 * fReal2 + fImag1 * fImag1 + fImag2 * fImag2;
    power_spectrum_.push_back(power_spec_i);
    float time_deriv_i = -fReal1 * fReal2 - fImag1 * fImag2; 
    float freq_deriv_i = fImag1 * fReal2 - fReal1 * fImag2;
    // TODO Sono
  }
}

void WAVFile::store_frame(int frame, MySQL& connection)
{
  int slices = float(total_samples())/44.1;
  MillisecondRecord* record = ms_table_.new_record();
  record->set("file_index", file_index_);
  record->set("index_in_file", frame);
  if (frame >= slices)
  {
    record->set("pitch", 0.0);
  }
  else
  {
    if (std::isnan(pitches_[frame]))
    {
      ++nans_;
      pitches_[frame] = 0.0;
    }
        
    record->set("pitch", pitches_[frame]);
  }
  if (!record->insert(connection))
  {
    std::cerr << connection.error() << std::endl;
    std::cerr << file_name_ << "," << frame << "," << pitches_[frame] << std::endl;
  }
}

bool WAVFile::operator()(Fft& fft, fft_buffers& buffers, MySQL& connection)
{
  if (!record_->insert(connection))
  {
    std::cerr << connection.error() << std::endl;
    std::cerr << "Failed to insert the file record for " << file_name_ << std::endl;
  }
  delete record_;
  if (!calculate_pitchse())
  {
    return false;
  }
  int offset = 0;
  int advance = 44;
  int index = 0;
  while (offset < total_samples())
  {
    calculate_frame(fft, buffers, offset);
    store_frame(index, connection);
    offset += fft.size();
    ++index;
  }

  return true;
}


} // namespace

/* vim: set cindent sw=2 expandtab : */


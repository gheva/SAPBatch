#include "wavfile.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cmath>
#include "yin/yin.h"
#include "thread/synqueue.h"

namespace sap
{

WAVFile::WAVFile(DirectoryIterator::iterator* iter, options opt) : options_(opt), ms_table_(options_.ms_table_name), file_table_(options_.file_table_name), file_name_(iter->file_name), nans_(0), file_index_(iter->file_index)
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
  entropy_range_ = (options_.max_entropy_freq - options_.min_entropy_freq);
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
  Yin yin(*this, options_.yin_min_freq);
  float* result;
  bool ret = yin(&result);
  if (!ret)
  {
    return ret;
  }
  int slices = float(total_samples())/44.1;
  pitches_.clear();
  float sr = sample_rate();
  float div = 44.1;
  for (int i = 0; i< slices; ++i)
  {
    pitches_.push_back(sr * result[int(div * (float)i)]);
  }
  return true;
}

void WAVFile::calculate_frame(Fft& fft, Fft& cepstrum, fft_buffers& buffers, int offset, MillisecondRecord* record, int frame)
{
  fill_buffer_taper(offset, fft.size(), buffers.in_, 0);
  fft(buffers.in_, buffers.out1_);
  fill_buffer_taper(offset, fft.size(), buffers.in_, 1);
  fft(buffers.in_, buffers.out2_);

  power_spectrum_.clear();
  time_derivative_.clear();
  freq_derivative_.clear();
  float F1 = 0, F2 = 0, F3 = 0, F4 = 0;
  float po1 = 0, po2 = 0, po3 = 0, po4 = 0;
  // 260 = spectrum range
  float time_deriv_max = 0, freq_deriv_max = 0, amplitude = 0, max_power = 0;
  float sum_log = 0, log_sum = 0, peak_freq = 0, log_power = 0;
  float gravity_center = 0, gc_base = 0, AM = 0;
  float noise_power = 0;
  for (int i = 0; i < options_.spectrum_range; ++i)
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
    time_derivative_.push_back(time_deriv_i);
    freq_derivative_.push_back(freq_deriv_i);
    // TODO Sono
    float xx = freq_deriv_i * freq_deriv_i + time_deriv_i * time_deriv_i;
    xx = sqrt(xx);
#define SET_POF(from, to, p, f) \
    if (i < to && i > from) {\
      if (p < xx) {\
        p = xx; f = i; \
      }}
    SET_POF(5, 50, po1, F1);
    SET_POF(50, 100, po2, F2);
    SET_POF(100, 150, po3, F3);
    SET_POF(149, 260, po4, F4);

    if (i >= options_.min_entropy_freq && i < options_.max_entropy_freq && power_spec_i != 0 && std::isnormal(power_spec_i))
    {
      float tmp = time_deriv_i * time_deriv_i;
      if (tmp > time_deriv_max) time_deriv_max = tmp;

      tmp = freq_deriv_i * freq_deriv_i;
      if (tmp > freq_deriv_max) freq_deriv_max = tmp;

      if (max_power < power_spec_i)
      {
        max_power = power_spec_i;
        peak_freq = i;
      } // TODO Maximum
      amplitude += power_spec_i;
      log_power = freq_deriv_i * freq_deriv_i + time_deriv_i * time_deriv_i;
      sum_log += log(power_spec_i);
      gravity_center += i * log_power;
      gc_base += log_power;
      log_sum += power_spec_i;
      AM += time_deriv_i;
    }
  } // end for frequencies in the frame
  // TODO pos and fs
  if (amplitude == 0)
  {
    amplitude = 1;
  }
  AM /= amplitude;
  if (AM != 0)
  {
    record->set("AM", (double)(AM * 100));
  }
  amplitude = log10(amplitude + 1) * 10 - options_.baseline;
  noise_power /= std::max(amplitude, float(1.0));
  if (noise_power > 0.5)/*noise_ratio*/
  {
    amplitude = 0;
  }
  record->set("amplitude", (double)(amplitude * 10));
  unsigned int mfa_int = gravity_center;
  double mfa = 0;
  for (int l = -5; l < 6; ++l)
  {
    if (mfa_int > 4 && mfa_int < 200)
    {
      mfa += power_spectrum_[mfa_int + l];
    }
  }
  mfa = log10(mfa + 1) * 10 - options_.baseline;
  record->set("mfa", (double)(mfa * 10));
  // we now compute the features of this fft window:
  //////////////////////////////////////////////////////////////////////
  // compute Wiener entropy
  float entropy(0);
  if (log_sum != 0)
  {
    log_sum = log(log_sum / entropy_range_);
    entropy = (sum_log / entropy_range_) - log_sum;
  }
  record->set("entropy", entropy * 100);
  // Compute FM
  float FM(0);
  if (freq_deriv_max != 0 && time_deriv_max != 0)
  {
    FM = atan(time_deriv_max / freq_deriv_max);
  }
  float sFM(sin(FM)), cFM(cos(FM));

  FM *= 57.2957795; // change units of FM to degrees
  record->set("FM", FM * 10);
  memset(buffers.cepst_in_, 0, cepstrum.size() * sizeof(float));
  for (int i = 7; i < 255; ++i)
  {
    float spec_deriv_i = time_derivative_[i] * sFM + freq_derivative_[i] * cFM;
    float tmp = power_spectrum_[i];
    buffers.cepst_in_[i] = 0;
    if (tmp != 0 && i >= options_.min_entropy_freq && i < options_.max_entropy_freq)
    {
      buffers.cepst_in_[i] = spec_deriv_i / tmp;
    }
  }
  // run the fft
  cepstrum(buffers.cepst_in_, buffers.cepst_out_);
  float pitch_goodness(1);
  float pitch(1);
  for (int i = options_.upper_pitch_bound; i < options_.lower_pitch_bound; ++i)
  {
    float x, y;
    x = buffers.cepst_out_[i][0];
    y = buffers.cepst_out_[i][1];
    x = x * x + y * y;
    if (x > pitch_goodness)
    {
      pitch_goodness = x;
      pitch = i;
    }
  }
  record->set("pgoodness", pitch_goodness);

  if (frame >= pitches_.size())
  {
    record->set("pitch", 0.0);
  }
  else
  {
    record->set("pitch", (double)pitches_[frame]);
  }
}

void WAVFile::store_frame(MillisecondRecord* record, MySQL& connection)
{
  if (record == nullptr)
  {
    return;
  }
  if (!record->insert(connection))
  {
    std::cerr << connection.error() << std::endl;
  }
}

bool WAVFile::operator()(Fft& fft, Fft& cepstrum, fft_buffers& buffers, MySQL& connection)
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
  int advance = options_.frame_advance;
  int index = 0;
  long long frame(ms_from_midnight_);
  while (offset < total_samples())
  {
    MillisecondRecord* record = ms_table_.new_record();
    record->set("file_index", file_index_);
    record->set("index_in_file", frame++);
    calculate_frame(fft, cepstrum, buffers, offset, record, index);
    store_frame(record, connection);
    offset += advance;
    ++index;
  }

  return true;
}


} // namespace

/* vim: set cindent sw=2 expandtab : */


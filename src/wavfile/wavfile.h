#ifndef SAP_WAVFILE_H__
#define SAP_WAVFILE_H__

#include <string>
#include <cstdio>
#include "AudioFile.h"
#include "tapers/multitaper.h"
#include "fft/fft.h"
#include "sql/millisecondtable.h"

namespace sap
{

class SynQueue;

class WAVFile
{
public:
  WAVFile(const std::string& file_name, const std::string& table_name="Milliseconds");
  virtual ~WAVFile();

  int sample_rate();
  int total_samples();
  bool fill_buffer(uint64_t offset, uint64_t size, float*);
  bool read_fully(float** ret);
  void add_tapers(MultiTaper* tapers) { tapers_ = tapers; }
  bool operator()(Fft& fft1, Fft& fft2);
  bool operator()(Fft& fft, fft_buffers& buffers);
  bool operator()(Fft& fft, fft_buffers& buffers, SynQueue& write_queue);
  bool operator()(Fft& fft, fft_buffers& buffers, MySQL& connection);
  int nans() { return nans_; }
protected:
private:

  bool fill_buffer_taper(uint64_t offset, uint64_t size, float*, unsigned int);

  AudioFile<float> audio_file_;
  MultiTaper* tapers_;
  float* pitches_;
  MillisecondTable table_;
  std::string file_name_;
  int nans_;
};

} // namespace

#endif // SAP_WAVFILE_H__

/* vim: set cindent sw=2 expandtab : */


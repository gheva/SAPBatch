#ifndef SAP_WAVFILE_H__
#define SAP_WAVFILE_H__

#include <string>
#include <cstdio>
#include <vector>
#include "AudioFile.h"
#include "tapers/multitaper.h"
#include "fft/fft.h"
#include "sql/millisecondtable.h"
#include "sql/filetable.h"
#include "fs/directoryiterator.h"

namespace sap
{

class SynQueue;

class WAVFile
{
public:
  struct options
  {
    std::string ms_table_name;
    std::string file_table_name;
    int min_entropy_freq;
    int max_entropy_freq;
    int baseline;
    int frame_advance;
    int upper_pitch_bound;
    int lower_pitch_bound;
  };
  WAVFile(DirectoryIterator::iterator* iter, options opts);
  virtual ~WAVFile();

  int sample_rate();
  int total_samples();
  bool fill_buffer(uint64_t offset, uint64_t size, float*);
  bool read_fully(float** ret);
  void add_tapers(MultiTaper* tapers) { tapers_ = tapers; }
  bool operator()(Fft& fft, Fft& cepstrum, fft_buffers& buffers, MySQL& connection);
  int nans() { return nans_; }
protected:
private:
  options options_;

  bool fill_buffer_taper(uint64_t offset, uint64_t size, float*, unsigned int);
  bool calculate_pitchse();
  void calculate_frame(Fft& fft, Fft& cepstrum, fft_buffers& buffers, int offset, MillisecondRecord* record, int frame);
  void store_frame(MillisecondRecord* rec, MySQL& connection);

  AudioFile<float> audio_file_;
  MultiTaper* tapers_;
  std::vector<float> pitches_;
  MillisecondTable ms_table_;
  FileTable file_table_;
  FileRecord* record_;
  std::string file_name_;
  int file_index_;
  int nans_;

  std::string bird_id_;
  int serial_number_;
  long long ms_from_midnight_;
  int month_;
  int day_;
  int hour_;
  int minute_;
  int second_;
  float entropy_range_;
  std::vector<float> power_spectrum_;
  std::vector<float> time_derivative_;
  std::vector<float> freq_derivative_;
};

} // namespace

#endif // SAP_WAVFILE_H__

/* vim: set cindent sw=2 expandtab : */


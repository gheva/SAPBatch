#ifndef SAP_WAVFILE_H__
#define SAP_WAVFILE_H__

#include <string>
#include <cstdio>
#include "AudioFile.h"

namespace sap
{

class WAVFile
{
public:
  WAVFile(const std::string& file_name);
  virtual ~WAVFile();

  int sample_rate();
  int total_samples();
  bool fill_buffer(uint64_t offset, uint64_t size, float*);
  bool read_fully(float** ret);
protected:
private:
  AudioFile<float> audio_file_;
};

} // namespace

#endif // SAP_WAVFILE_H__

/* vim: set cindent sw=2 expandtab : */


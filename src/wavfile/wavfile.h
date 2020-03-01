#ifndef SAP_WAVFILE_H__
#define SAP_WAVFILE_H__

#include <string>
#include <cstdio>

namespace sap
{

class WAVFile
{
public:
  struct header
  {
    char RIFF[4]; // The chars RIFF
    uint32_t file_size; // size of the file without the RIFF and size (-8)
    char WAVE[4]; // The chars WAVE
    char fmt[4]; // The chars "fmt "
    uint32_t chunk1_size;  // Size of the wav section chunck
    uint16_t audio_format; // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t mono_streo; // 0x01 mono 0x02 streo
    uint32_t sample_rate;  // Sampling Frequency in Hz
    uint32_t bytes_per_second; // bytes per second 
    uint16_t block_alignment; // 2=16-bit mono, 4=16-bit stereo 
    uint16_t bits_per_sample; // Number of bits per sample      
    char data[4]; // The chars "data"
    uint32_t data_chunk_size; // Sampled data length  
  };
  WAVFile(const std::string& file_name);
  virtual ~WAVFile();

  int sample_rate();
protected:
private:
  header header_;
  FILE* file_;
};

} // namespace

#endif // SAP_WAVFILE_H__

/* vim: set cindent sw=2 expandtab : */


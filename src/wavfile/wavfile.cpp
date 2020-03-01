#include "wavfile.h"
#include <iostream>

namespace sap
{

WAVFile::WAVFile(const std::string& path)
{
  file_ = fopen(path.c_str(), "rb");
  if (file_ != nullptr)
  {
    fread(&header_, sizeof(header_), 1, file_);
    if (header_.data[0] != 'd' || header_.data[1] != 'a' || header_.data[2] != 't' || header_.data[3] != 'a')
    {
      std::cerr << "Header is not OK" << std::endl;
    }
  }
}

WAVFile::~WAVFile()
{
  if (file_ != nullptr)
  {
    fclose(file_);
  }
}

int WAVFile::sample_rate()
{
  return header_.sample_rate;
}

} // namespace

/* vim: set cindent sw=2 expandtab : */


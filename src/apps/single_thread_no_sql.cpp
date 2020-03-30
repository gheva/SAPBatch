#include <iostream>
#include <cstdlib>
#include "wavfile/wavfile.h"
#include "tapers/multitaper.h"
#include "yin/yin.h"
#include "fft/fft.h"
#include "fs/directoryiterator.h"

using namespace sap;
using namespace std;

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    cerr << "Usage: " << argv[0] << " folder" << endl;
    exit(EXIT_FAILURE);
  }

  DirectoryIterator diriter(argv[1]);
  std::string file_name;
  int window_size = 44;

  MultiTaper tapers(window_size);
  Fft fft1(window_size);
  Fft fft2(window_size);
  std::string root = argv[1];

  while ((file_name = diriter.next_file()) != "")
  {
    cerr << file_name << endl;
    WAVFile wav(file_name);
    wav.add_tapers(&tapers);
    if (!wav(fft1, fft2))
    {
      cerr << "failed to preocees file" << endl;
    }
    else 
    {
      cerr << "success" << endl;
    }
  }

}

/* vim: set cindent sw=2 expandtab : */


#include <iostream>
#include <cstdlib>
#include "wavfile/wavfile.h"
#include "tapers/multitaper.h"
#include "yin/yin.h"
#include "fft/fft.h"

using namespace sap;
using namespace std;

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    exit(EXIT_FAILURE);
  }
  WAVFile wav(argv[1]);

  //int slices = total/44.1;

  int window_size = 44;

  MultiTaper tapers(window_size);
  Fft fft1(window_size);
  Fft fft2(window_size);
  wav.add_tapers(&tapers);
  if (!wav(fft1, fft2))
  {
    cerr << "failed to preocees file" << endl;
  }
  else 
  {
    cerr << "success" << endl;
  }

  //initYin(sample_rate, 40);//option->minFreq);

}

/* vim: set cindent sw=2 expandtab : */


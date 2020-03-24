#include <iostream>
#include <cstdlib>
#include "wavfile/wavfile.h"
#include "yin/yin.h"

using namespace sap;
using namespace std;

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    exit(EXIT_FAILURE);
  }
  WAVFile wav(argv[1]);
  cout << wav.sample_rate() << endl;

  int sample_rate = wav.sample_rate();
  int total = wav.total_samples();
  Yin yin(wav, 400);
  float* result;
  yin(&result);
  int slices = total/44.1;
  float* pithces = new float[slices];
  float div = 44.1;
  for (int i = 0; i< slices; ++i)
  {
    pithces[i] = (float)sample_rate * result[int(div * (float)i)];
  }

  //initYin(sample_rate, 40);//option->minFreq);

}

/* vim: set cindent sw=2 expandtab : */


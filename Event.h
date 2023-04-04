#ifndef EVENT_H
#define EVENT_H

#include <stdint.h>
#include <stdio.h>
#include <TFile.h>
#include <TH1D.h>
#include "Waveform.h"

#define NCHANNELS 16

using namespace std;

class Event{
  
 public:
  
  Event();
  virtual ~Event();

  void ReadEvent(string inname, int* pos);
  void Clear();
  
 private:

  vector<Waveform *>myWaveforms;
  double tot_charge;
  double time;
  
};

#endif

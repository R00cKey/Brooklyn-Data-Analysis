#ifndef ANATOOLS_H
#define ANATOOLS_H

#include <stdint.h>
#include <stdio.h>
#include <TFile.h>
#include <TH1D.h>
#include "Waveform.h"
#include "Event.h"

using namespace std;

class AnaTools{
  
 public:
  
  AnaTools(TFile *f,Event *myEvent);
  virtual ~AnaTools();

  void BookingHistograms();
  void Process();
  void Clear();

  //methods for analysis to be implemented here
  
 private:

  TFile *outfile;
  Event *event;
  
};

#endif

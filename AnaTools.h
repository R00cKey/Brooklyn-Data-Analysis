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
  
  //COSTRUTTORE E DISTRUTTORE
  AnaTools(TFile *f, Event *myEvent);
  virtual ~AnaTools();

  //METODI PER ANALISI DATI
  void BookingHistograms();
  void Process();
  void Clear();
  //Print Histogram to File method
  void FillHistogram(int directory);
 private:

  TFile *outfile;
  Event *event;
  TH1D* hist_vector[3][16];
  TH1D* hc_vector[16];
  TH1D *hctot;
};

#endif

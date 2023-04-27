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
  
  //Constructor and Destructor
  AnaTools(TFile *f, Event *myEvent);
  virtual ~AnaTools();

  //Getters
  double getGain();
  double getQE();
  double getEtr();

  //Data Analysis Methods
  void BookingHistograms();
  void Process();
  double getLightYield(double Q, double gain, double qe, double etr); //qe: Quantum Efficiency etr: Transimission efficiency
  //Print Histogram to File method
  void FillHistogram(int directory);
  void Clear();


 private:

  TFile *outfile;
  Event *event;
  TH1D* hist_vector[3][NCHANNELS];
  TH1D* hc_vector[NCHANNELS];
  TH1D *hctot;
  TH1D *hly_vector[NCHANNELS];
  TH1D *hlytot;
  double gain_=1.e7;
  double qe_=14.e-2;
  double etr_=4.88e-2;
};

#endif

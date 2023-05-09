#ifndef ANATOOLS_H
#define ANATOOLS_H

#include <stdint.h>
#include <stdio.h>
#include <TFile.h>
#include <TH1D.h>
#include "Waveform.h"
#include "Event.h"
#include <fstream>
#include <sstream>

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
	void TOF();
	void f_TOF();
	void Pedestal(string inname);
 private:

  TFile *outfile;
  Event *event;
  TH1D* hist_vector[3][NCHANNELS];
  TH1D* hc_vector[NCHANNELS];
  TH1D* hc_vector_shifted[NCHANNELS];
  TH1D *hctot;
  TH1D *hly_vector[NCHANNELS];
  TH1D *hlytot;
  TH1D *hTOF_cfm[NCHANNELS];
  TH1D *hTOF_ft;
  TF1 *fit_TOF1[12];
  double gain_=1.e7;
  double qe_=14.e-2;
  double etr_=4.88e-2;
  double ped_mean[16];
	double ped_sigma[16];
	double mean2[16];
	double sigma2[16];
};

#endif

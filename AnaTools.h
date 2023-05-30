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
  AnaTools(TFile *f, Event *myEvent, double cf_, double th_);
  virtual ~AnaTools();

  //Getters
  double getGain(int i);
  double getQE(int i);
  double getEtr(int i);

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
	int PeakTimeFinder(double ch);
 private:
 
	double cf;
	double th;
  TFile *outfile;
  Event *event;
  TH1D* hist_vector[3][NCHANNELS];
  TH1D* hc_vector[NCHANNELS];
  TH1D* hc_vector_shifted[NCHANNELS];
  TH1D* hc_vector_shifted_cut[NCHANNELS];
  TH1D* hc_vector_ap[NCHANNELS];
  TH1D *hctot;
  TH1D *hly_vector[NCHANNELS];
  TH1D *hlytot;
  TH1D *hTOF_cfm[NCHANNELS];
  TH1D *hTOF_cfm_cut[NCHANNELS];
  TH1D *hTOF_ft[NCHANNELS];
  TH1D *hTOF_ft_cut[NCHANNELS];
  TF1 *fit_TOF_cfm[12];
  TF1 *fit_TOF_cfm_cut[12];
  double gain_[16];
  double qe_[16];
  double etr_[16];
  double ped_mean[16];
  double ped_sigma[16];
  double mean2[16];
  double sigma2[16];
};

#endif

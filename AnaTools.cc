#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <TFile.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TMath.h>
#include <TSpectrum.h>
#include "AnaTools.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <string.h>
#include <TMinuit.h>
#include <math.h>


#define NEVENTS  20; //bisogna prenderlo dal file di output

using namespace std;

//Constructor
AnaTools::AnaTools(TFile *f, Event *myEvent){ 
  outfile = f;
  event = myEvent;
}

//Destructor
AnaTools::~AnaTools(){
	return;

}
//Getters
double AnaTools::getGain(){return gain_;}
double AnaTools::getQE(){return qe_;}
double AnaTools::getEtr(){return etr_;}


void AnaTools::BookingHistograms(){
	outfile->cd("..");
	//CHARGE HISTOGRAMS
	gDirectory->mkdir("Hist_Total_Charge");
	gDirectory->cd("Hist_Total_Charge");
  TString name = Form("Hist_total_charge");
  TString title = Form("Total Charge distribution; charge[]; Frequency(#)");
  hctot = new TH1D(name, title, 100, -0.1e-13, 0.1e-13);
  hctot->SetCanExtend(TH1::kAllAxes);
  gDirectory->cd("..");
  gDirectory->mkdir("Hist_Cariche_Canali");
  gDirectory->cd("Hist_Cariche_Canali");
	for(unsigned int k=1; k<=NCHANNELS;k++){
  		TString name = Form("Hist_Channel_%d",k);
  		TString title = Form("Charge distribution channel %d; charge[C]; Frequency(#)", k);
  		hc_vector[k-1] = new TH1D(name, title, 100, -0.1e-13, 0.1e-13);
  		hc_vector[k-1]->SetCanExtend(TH1::kAllAxes);
  	}
  gDirectory->cd("..");
  //LIGHT YIELD HISTOGRAMS
  gDirectory->mkdir("Hist_Total_Light_Yield");
  gDirectory->cd("Hist_Total_Light_Yield");
  name = Form("Hist_Total_Light_Yield");
  title = Form("Hodoscope Light Yield Distribution; LY[# produced photons/Energy loss]; Frequency(#)");
  hlytot = new TH1D(name, title, 100, -0.1e-21, 0.1e-21);
  hlytot->SetCanExtend(TH1::kAllAxes);
  gDirectory->cd("..");
  gDirectory->mkdir("Hist_Channels_Light_Yield");
  gDirectory->cd("Hist_Channels_Light_Yield");
	for(unsigned int k=1; k<=NCHANNELS;k++){
  		TString name = Form("Hist_LY_Channel_%d",k);
  		TString title = Form("Light Yield Distribution Channel %d; LY[# produced photons/Energy loss]; Frequency(#)", k);
  		hly_vector[k-1] = new TH1D(name, title, 100, -0.1e-21, 0.1e-21);
  		hly_vector[k-1]->SetCanExtend(TH1::kAllAxes);
  	}
	gDirectory->cd("..");
	
	for(int i=1; i<=3; i++){       
	
		//WAVEFORMS HISTOGRAMS                
		char dir[10]="Evento";
  	char *newEvent=strcat(dir, to_string(i).c_str());        
  	gDirectory->mkdir(&newEvent[0]);
  	gDirectory->cd(&newEvent[0]);
  	for(unsigned int k=1; k<=NCHANNELS;k++){
  		TString name = Form("Event_%d_Channel_%d", i,k);
  		TString title = Form("Event %d, Channel %d; time[s]; Amplitude(V)", i,k);
  		hist_vector[i-1][k-1] = new TH1D(name, title, 1024, 0, 1024*SAMPLINGPERIOD);
  	}
  gDirectory->cd("..");	
	}
}

//Method for Data Analysis: Gets charges and light yields and puts them into histograms
void AnaTools::Process(){
	double tot_charge=0;
	for(unsigned int i =0; i < event->getWaveforms().size(); i++){
		double charge=0;
 		for(int k=0; k < NSAMPLING; k++){
 			charge +=  event->getWaveforms()[i]->getv_amplitude()[k]*SAMPLINGPERIOD;
 		}
 		event->getWaveforms()[i]->setcharge(charge/50); //I divide by R=50 Ohm
 		tot_charge += event->getWaveforms()[i]->getcharge(); //il getter dovrebbe contenere la charge modificata dal setter che ho appena chiamato
	}
	event->settot_charge(tot_charge);

	outfile->cd();
	
	//CHARGE HISTOGRAMS FILLING
  gDirectory->cd("Hist_Cariche_Canali");
	for(unsigned int i =0; i < event->getWaveforms().size(); i++){
		hc_vector[i]->SetCanExtend(TH1::kAllAxes);
		hc_vector[i]->Fill(event->getWaveforms()[i]->getcharge());
  }
  gDirectory->cd("..");
  gDirectory->cd("Hist_Total_Charge");
  hctot->SetCanExtend(TH1::kAllAxes);
  hctot->Fill(event->GetTot_charge());
  gDirectory->cd("..");
  
	//LIGHT YIELD HISTOGRAMS FILLING
  gDirectory->cd("Hist_Channels_Light_Yield");
	for(unsigned int i =0; i < event->getWaveforms().size(); i++){
		hc_vector[i]->SetCanExtend(TH1::kAllAxes);
		hly_vector[i]->Fill(getLightYield(event->getWaveforms()[i]->getcharge(), getGain(), getQE(), getEtr()));
  }
  gDirectory->cd("..");
  gDirectory->cd("Hist_Total_Light_Yield");
  hlytot->SetCanExtend(TH1::kAllAxes);
  hlytot->Fill(getLightYield(event->GetTot_charge(), getGain(), getQE(), getEtr()));
  gDirectory->cd("..");


  return;
}


void AnaTools::FillHistogram(int directory){
	char dir[10]="Evento";
  char *newEvent=strcat(dir, to_string(directory).c_str());
  outfile->cd();
  gDirectory->cd(&newEvent[0]);
	
	for(unsigned int k=1; k<=event->getWaveforms().size();k++){
  		for(int j=0; j<NSAMPLING; j++){
  			hist_vector[directory-1][k-1]->SetBinContent(j+1, event->getWaveforms()[k-1]->getv_amplitude()[j]);
  		}
 	}
}

double AnaTools::getLightYield(double Q, double gain, double qe, double etr){
	//NOTA TEMPORANEA: DOPO DOVREMO IMPLEMENTARE L'ENERGIA RILASCIATA DAL MUONE. PER ORA ly=ly*E
	return Q/(gain*qe*etr);
}

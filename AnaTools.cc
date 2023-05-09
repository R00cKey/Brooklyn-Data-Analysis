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
#include <TAxis.h>


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
  TString title = Form("Total Charge distribution; charge[C]; Counts(#)");
  hctot = new TH1D(name, title, 500, -20.e-11, 20.e-11);
  gDirectory->cd("..");
  gDirectory->mkdir("Hist_Channels_Charge");
  gDirectory->cd("Hist_Channels_Charge");
	for(unsigned int k=1; k<=NCHANNELS;k++){
  		TString name = Form("Hist_Channel_%d",k-1);
  		TString title = Form("Charge distribution channel %d; charge[C]; Counts(#)", k-1);
  		hc_vector[k-1] = new TH1D(name, title, 500, -2.e-11, 2.e-11);
  	}
  gDirectory->cd("..");
   
  //TOF HISTOGRAMS
  for(int i=2;i<14;i++){
  	char dir[17]="Hist_TOF_cfm_ch";
	  char *newHist=strcat(dir, to_string(i).c_str());        
 		gDirectory->mkdir(&newHist[0]);
 		gDirectory->cd(&newHist[0]);

  	TString name = Form("Hist_TOF_cfm_ch%d",i);
  	TString title = Form("Distribution of TOF of ch%d wrt mean TOF of ch 0,1,14,15 using Constant Fraction Method; Time of arrival[s]; Counts (#)",i);
  	hTOF_cfm[i] = new TH1D(name, title, 500, -1.e-7, 1.e-7);
  	gDirectory->cd("..");
  	
  	/*gDirectory->mkdir("Hist_TOF_ft");
  	gDirectory->cd("Hist_TOF_ft");
  	name = "Hist_TOF_ft";
  	title = ("Distribution of TOF wrt mean TOF of ch 0,1,14,15 using a fixed threshold; Time of flight[s]; Counts (#)");
  	hTOF_ft = new TH1D(name, title, 500, -1.e-7, 1.e-7);
  	gDirectory->cd("..");*/
  	
  	}
   
  //LIGHT YIELD HISTOGRAMS
  gDirectory->mkdir("Hist_Total_Light_Yield");
  gDirectory->cd("Hist_Total_Light_Yield");
  name = Form("Hist_Total_Light_Yield");
  title = Form("Hodoscope Light Yield Distribution; LY[# produced photons/Energy loss]; Counts(#)");
  hlytot = new TH1D(name, title, 100, -1.e5, 1.e5);
  gDirectory->cd("..");
  gDirectory->mkdir("Hist_Channels_Light_Yield");
  gDirectory->cd("Hist_Channels_Light_Yield");
	for(unsigned int k=1; k<=NCHANNELS;k++){
  		TString name = Form("Hist_LY_Channel_%d",k);
  		TString title = Form("Light Yield Distribution Channel %d; LY[# produced photons/Energy loss]; Counts(#)", k);
  		hly_vector[k-1] = new TH1D(name, title, 100, -1000., 1000.);
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
 		event->getWaveforms()[i]->setcharge(-charge/50); //I divide by R=50 Ohm
 		tot_charge += event->getWaveforms()[i]->getcharge(); //il getter dovrebbe contenere la charge modificata dal setter che ho appena chiamato
	}
	event->settot_charge(tot_charge);

	outfile->cd();
	
	//CHARGE HISTOGRAMS FILLING
  gDirectory->cd("Hist_Channels_Charge");
	for(unsigned int i =0; i < event->getWaveforms().size(); i++){
		hc_vector[i]->Fill(event->getWaveforms()[i]->getcharge());
  }
  gDirectory->cd("..");
  gDirectory->cd("Hist_Total_Charge");
  hctot->Fill(event->GetTot_charge());
  gDirectory->cd("..");
  
	//LIGHT YIELD HISTOGRAMS FILLING
  gDirectory->cd("Hist_Channels_Light_Yield");
	for(unsigned int i =0; i < event->getWaveforms().size(); i++){
		hly_vector[i]->Fill(getLightYield(event->getWaveforms()[i]->getcharge(), getGain(), getQE(), getEtr()));
  }
  gDirectory->cd("..");
  gDirectory->cd("Hist_Total_Light_Yield");
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
	return Q/(gain*qe*etr*ECHARGE);
}





void AnaTools::TOF(){
	double V_peak[16];
	double t_in_cfm[16];
	double tpeak[16];
		for(unsigned int i =0; i < event->getWaveforms().size(); i++){
			V_peak[i]=0;
			for(int k=0; k < NSAMPLING; k++){
				if(V_peak[i]>event->getWaveforms()[i]->getv_amplitude()[k]){
					V_peak[i]=event->getWaveforms()[i]->getv_amplitude()[k];
					tpeak[i]=k;
				}
			//	cout << "tpeak " << tpeak[i] << endl;
			}
			
			t_in_cfm[i]=tpeak[i];
			while(event->getWaveforms()[i]->getv_amplitude()[int(t_in_cfm[i])]<V_peak[i]*0.2){
			t_in_cfm[i]=t_in_cfm[i]-1;
		 }
		
		}
	double TOF_mean_cfm=(t_in_cfm[0]+t_in_cfm[1]+t_in_cfm[14]+t_in_cfm[15])*0.25*SAMPLINGPERIOD;
	
	for(int l=2;l<14;l++){
		
		t_in_cfm[l]=t_in_cfm[l]*SAMPLINGPERIOD-TOF_mean_cfm;
		hTOF_cfm[l]->Fill(t_in_cfm[l]);
	}
	
	
	
}





void AnaTools::f_TOF(){

	for(int l=2;l<14;l++){
	
		
		fit_TOF1[l-2] = new TF1("fit_TOF","gaus(0)+gaus(3)+[6]",-0.5,0.5);
		fit_TOF1[l-2] ->SetParameter(0,hTOF_cfm[l]->GetBinContent(hTOF_cfm[l]->GetMaximumBin()));
		fit_TOF1[l-2] ->SetParameter(1,hTOF_cfm[l]->GetBinCenter(hTOF_cfm[l]->GetMaximumBin()));
		fit_TOF1[l-2] ->SetParameter(2,hTOF_cfm[l]->GetStdDev());
		fit_TOF1[l-2] ->SetParLimits(2,0.5e-9,2.2e-9); 
		fit_TOF1[l-2] ->SetParameter(3,hTOF_cfm[l]->GetBinContent(hTOF_cfm[l]->GetMaximumBin()));
		fit_TOF1[l-2] ->SetParameter(4,hTOF_cfm[l]->GetBinCenter(hTOF_cfm[l]->GetMaximumBin()));
		fit_TOF1[l-2] ->SetParameter(5,hTOF_cfm[l]->GetStdDev());
		fit_TOF1[l-2] ->SetParLimits(5,2e-9,4e-9); 
		fit_TOF1[l-2]	->SetNpx(10000);
		
		hTOF_cfm[l]->Fit(fit_TOF1[l-2],"R");
		
	 //Usare SetRangeUser per zoommare
 	
		
		hTOF_cfm[l]->SetAxisRange(-0.05e-6,0.05e-6);
		//fit_TOF1[l-2]->SetRange(-30e-9,30e-9);
		
	 /*auto* legend = new TLegend(0.65,0.7,0.89,0.89);
   legend->AddEntry(fit_TOF1[l-2],"Fit result", "L");
   legend->Draw();*/
		
	}
}






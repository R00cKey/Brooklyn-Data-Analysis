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

//COSTRUTTORE E DISTRUTTORE
AnaTools::AnaTools(TFile *f, Event *myEvent){ 
  outfile = f;
  event = myEvent;
}

//METODO CHE PRENOTA GLI ISTOGRAMMI
void AnaTools::BookingHistograms(){
	outfile->cd("..");
	gDirectory->mkdir("Hist_Total_Charge");
	gDirectory->cd("Hist_Total_Charge");
  	TString name = Form("Hist_total_charge");
  	TString title = Form("Total Charge distribution; charge[]; Frequency(#)"); //da controllare le unità di misura
  	hctot = new TH1D(name, title, 100, -0.1e-13, 0.1e-13);
  	hctot->SetCanExtend(TH1::kAllAxes);
  	gDirectory->cd("..");
  	gDirectory->mkdir("Hist_Cariche_Canali");
  	gDirectory->cd("Hist_Cariche_Canali");
	for(unsigned int k=1; k<=NCHANNELS;k++){
  		TString name = Form("Hist_Channel_%d",k);
  		TString title = Form("Charge distribution channel %d; charge[]; Frequency(#)", k); //da controllare le unità di misura
  		hc_vector[k] = new TH1D(name, title, 100, -0.1e-13, 0.1e-13);
  		hc_vector[k]->SetCanExtend(TH1::kAllAxes);
  		/*for(int j=1; j<=NSAMPLING; j++){
  			h[k]->SetBinContent(j, event->getWaveforms()[k]->getv_amplitude()[j]);
  		}*/
  	}
	gDirectory->cd("..");
	
	for(int i=1; i<=3; i++){       
	
		//assegniamo il nome a ciascun evento                
		char dir[10]="Evento";
  	char *newEvent=strcat(dir, to_string(i).c_str());        
  	             
  	//booking dei 16 istogrammi per ogni evento
  	gDirectory->mkdir(&newEvent[0]);

  	gDirectory->cd(&newEvent[0]);

  	for(unsigned int k=1; k<=NCHANNELS;k++){
  		TString name = Form("Event_%d_Channel_%d", i,k);
  		TString title = Form("Event %d, Channel %d; time[ps]; Amplitude(mV)", i,k); //da controllare le unità di misura
  		hist_vector[i-1][k-1] = new TH1D(name, title, 1024, 0, 1024*SAMPLINGPERIOD);
  	}
  gDirectory->cd("..");	
	}
}

//METODO CHE FA ANALISI DATI E CALCOLA LA CARICA
void AnaTools::Process(){
	double tot_charge=0;
	for(unsigned int i =0; i < event->getWaveforms().size(); i++){
		double charge=0;
 		for(int k=0; k < NSAMPLING; k++)
 			charge +=  event->getWaveforms()[i]->getv_amplitude()[k]*SAMPLINGPERIOD;
 		event->getWaveforms()[i]->setcharge(charge/50); //divido per R=50 Ohm
 		//cout << "Channel " << i << " charge: " << event->getWaveforms()[i]->getcharge() << endl;
 		tot_charge += event->getWaveforms()[i]->getcharge(); //il getter dovrebbe contenere la charge modificata dal setter che ho appena chiamato
	}
	event->settot_charge(tot_charge);

	outfile->cd();
  gDirectory->cd("Hist_Cariche_Canali");
	for(unsigned int i =0; i < event->getWaveforms().size(); i++){
		hc_vector[i+1]->SetCanExtend(TH1::kAllAxes);
		hc_vector[i+1]->Fill(event->getWaveforms()[i]->getcharge());
  }
  gDirectory->cd("..");
  /*gDirectory->cd("Hist_Total_Charge");
  hctot->SetCanExtend(TH1::kAllAxes);
  hctot->Fill(event->GetTot_charge());
  gDirectory->cd("..");*/

  return;
}

//METODO CHE LIBERA MEMORIA
AnaTools::~AnaTools(){

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
 		// hc_vector[]
 	}
}
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <TFile.h>
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
AnaTools::~AnaTools(){};

//METODO CHE PRENOTA GLI ISTOGRAMMI
void AnaTools::BookingHistograms(){
	outfile->cd();
	for(int i=1; i<=20; i++){       
	
		//assegnamo il nome a ciascun evento                
		char dir[10]="Evento";
  	char *newEvent=strcat(dir, to_string(i).c_str());        
  	             
  	//booking&filling dei 16 istogrammi per ogni evento
  	TH1D *h[16]={(TH1D*)gDirectory->mkdir(&newEvent[0])}; //curly brackets perchè è un array di histo
  	gDirectory->cd(&newEvent[0]);
  	
  	for(unsigned int k=1; k<=NCHANNELS;k++){
  		TString name = Form("Event_%d_Channel_%d", i,k);
  		TString title = Form("Event %d, Channel %d; time[ps]; Amplitude(mV)", i,k); //da controllare le unità di misura
  		h[k] = new TH1D(name, title, 1024, 0, 1024*SAMPLINGPERIOD);
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
 		tot_charge += event->getWaveforms()[i]->getcharge(); //il getter dovrebbe contenere la charge modificata dal setter che ho appena chiamato
	}
	event->settot_charge(tot_charge);
  return;
}

//METODO CHE LIBERA MEMORIA
void AnaTools::Clear(){

  return;

}


void AnaTools::FillHistogram(int directory){

	char dir[10]="Evento";
  char *newEvent=strcat(dir, to_string(directory).c_str());
  outfile->cd();	
  TH1D *h=(TH1D*)gDirectory->cd(&newEvent[0]);
  //booking dei 16 istogrammi per ogni evento
	for(int k=1; k<=event->getWaveforms().size();k++){
  int k=1;
		string histostring = "Event_";
		histostring.append(to_string(directory));
  	histostring.append("_Channel_");
  	histostring.append(to_string(k));
  	gDirectory->Get(histostring.data());
  	for(int j=0; j<NSAMPLING; j++){
  		cout << "Dentro for" << endl;	//arriva qui e poi dà Segmentation Violation
  		h->SetBinContent(j+1, event->getWaveforms()[k-1]->getv_amplitude()[j]);
  	}
  histostring.clear();
 	}
}
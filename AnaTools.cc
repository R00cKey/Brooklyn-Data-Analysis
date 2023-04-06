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
#include <TMinuit.h>
#include <math.h>


using namespace std;

//COSTRUTTORE E DISTRUTTORE
AnaTools::AnaTools(TFile *f, Event *myEvent){ 
  outfile = f;
  event = myEvent;
}
AnaTools::~AnaTools(){};

//METODO CHE PRENOTA GLI ISTOGRAMMI
void AnaTools::BookingHistograms(){
  TH1D *h; 
  outfile->cd();
  //here define histograms
  h = new TH1D("histo_test","",100,-5,5);
}

//METODO CHE FA ANALISI DATI E CALCOLA LA CARICA
void AnaTools::Process(){
	double tot_charge=0;
	for(unsigned int i =0; i < event->getWaveforms().size(); i++){
		cout <<"i" << endl;
		double charge=0;
 		for(int k=0; k < NSAMPLING; k++)
 			charge +=  event->getWaveforms()[i]->getv_amplitude()[k]*SAMPLINGPERIOD;
 		event->getWaveforms()[i]->setcharge(charge/50); //divido per R=50Ohm
 		tot_charge += event->getWaveforms()[i]->getcharge(); //il getter dovrebbe contenere la charge modificata dal setter che ho appena chiamato
	}
	event->settot_charge(tot_charge);
  return;
}

//METODO CHE LIBERA MEMORIA
void AnaTools::Clear(){


 

  return;

}

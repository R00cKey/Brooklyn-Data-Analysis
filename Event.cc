#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TMath.h>
#include <TSpectrum.h>
#include "Event.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <TMinuit.h>
#include <math.h>


using namespace std;

//COSTRUTTORE E DISTRUTTORE
Event::Event(){ 
  tot_charge = -1000; //valori di inizializzazione a caso solo per avere dei getter da usare (credo)
  time = -1000;
}
Event::~Event(){};

//METODO CHE LEGGE DA FILE
char* Event::ReadEvent(FILE *file){
	//da capire come si fa, userei la funzione fgets
	return NULL;
}

//GETTERS
vector<Waveform*> Event::getWaveforms(){
	return myWaveforms;
} 
double Event::gettot_charge(){
	return tot_charge;
}
double Event::gettime(){
	return time;
}

//SETTERS
void Event::settot_charge(double caricaTotale){
	tot_charge=caricaTotale;
}

void Event::settime(double tempo){
	time=tempo;
}

//METODO CHE LIBERA MEMORIA
void Event::Clear(){ 
	for(int iW=0;iW<myWaveforms.size();iW++){
    delete myWaveforms.at(iW);
  }
  myWaveforms.clear();
}



//return;


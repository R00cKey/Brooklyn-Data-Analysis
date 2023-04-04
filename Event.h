#ifndef EVENT_H
#define EVENT_H

#include <stdint.h>
#include <stdio.h>
#include <TFile.h>
#include <TH1D.h>
#include "Waveform.h"

#define NCHANNELS 16

using namespace std;

class Event{
  
 public:
  
  //COSTRUTTORE E DISTRUTTORE
  Event();
  virtual ~Event();
  
  //METODO CHE LEGGE DA FILE
  char* ReadEvent(FILE*); //da file si prende myWaveforms
  
  //GETTERS
  vector<Waveform*> getWaveforms(); 
  double gettot_charge();
  double gettime();
  
  //SETTERS
  void settot_charge(double caricaTotale);
  void settime(double tempo);
	
	//METODO CHE LIBERA MEMORIA  
  void Clear();
 
 private:

  vector<Waveform *> myWaveforms; 
  double tot_charge;
  double time;
  
};

#endif

//DUBBIO:
//un evento è un vettore di 16 forme d'onda (ciascuna relativa ad ogni canale dei 16 canali), perchè allora <Waveform*> con "*"? Così sembra un vettore di array di classi di tipo Waveform

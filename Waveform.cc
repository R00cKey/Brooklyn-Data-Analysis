#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TMath.h>
#include <TSpectrum.h>
#include "Waveform.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <TMinuit.h>
#include <math.h>


using namespace std;

Waveform::Waveform(double *vt, double *va){

  
  charge = -1000;
  time = -1000;

  //set time and amplitudes array with the values read from file
  memcpy(v_time, vt, sizeof(v_time));
  memcpy(v_amplitude, va, sizeof(v_amplitude));

  
}



Waveform::~Waveform(){}
//Getters
double* Waveform::getV_Time(){return &v_time[0];}
double* Waveform::getV_Amplitude(){return &v_amplitude[0];}
double Waveform::getCharge(){return charge;}
double Waveform::getTime(){return time;}
//Setters
void Waveform::setV_Time(int index, double value){v_time[index]=value;}
void Waveform::setV_Amplitude(int index, double value){v_amplitude[index]=value;}
void Waveform::setCharge(double Q){charge=Q;}
void Waveform::setTime(double t){charge=t;}
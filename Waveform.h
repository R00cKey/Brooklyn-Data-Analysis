#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <stdint.h>
#include <stdio.h>
#include <TFile.h>
#include <TH1D.h>


#define NSAMPLING 1024 //to be checked
#define SAMPLINGPERIOD 312.5e-12 //s

using namespace std;

class Waveform{
  
 public:
  
  Waveform(double *vt, double *va);
  virtual ~Waveform();

  //Getters
  double* getV_Time(); //Returns pointer to v_time[0]
  double* getV_Amplitude(); //Returns pointer to v_amplitude[0]
  double getCharge(); //Returns Charge
  double getTime(); //Returns Time

  //Setters
  void setV_Time(int index, double value); //Sets value at v_time[index]
  void setV_Amplitude(int index, double value); //Sets value at v_amplitude[index]
  void setCharge(double Q); //Sets charge
  void setTime(double t); //Sets time
  
 private:


  double v_time[NSAMPLING];
  double v_amplitude[NSAMPLING];
  
  double charge;
  double time;
  
};

#endif

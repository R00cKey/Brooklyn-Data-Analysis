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

AnaTools::AnaTools(TFile *f, Event *myEvent){

  outfile = f;
  event = myEvent;
 
 
   
}



AnaTools::~AnaTools(){

  


}


void AnaTools::BookingHistograms(){

  TH1D *h;
  
  outfile->cd();



  //here define histograms
  h = new TH1D("histo_test","",100,-5,5);

  


}


void AnaTools::Process(){



  //qua si fa il processing







  return;

}

void AnaTools::Clear(){


 

  return;

}

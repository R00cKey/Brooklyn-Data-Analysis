#include <TCanvas.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <TTree.h>
#include <TFile.h>
#include <TH1D.h>
#include <TF1.h>
#include <TH2D.h>
#include <TGraph.h>
#include <TSystem.h>
#include <stdio.h>
#include <TStyle.h>
#include <stdint.h>
#include "AnaTools.h"
#include "Event.h"

using namespace std;


int main(int argc, char *argv[]){

  string inname("in.dat");
  string outname("out.root");

  int nevent=1;
  unsigned long int filepos=0;
  unsigned long int filelength;


  for(int i=0; i< argc; i++){
    if(strcmp("-in",argv[i])==0){
      inname.assign(argv[++i]);
    }
    if(strcmp("-out",argv[i])==0){
      outname.assign(argv[++i]);
    }
    if(strcmp("-nev",argv[i])==0){
      nevent = atoi(argv[++i]);
    }
  }


  //open output file
  TFile *f = new TFile(outname.data(), "RECREATE");
  f->cd();


  //create an Event object
  Event * myEvent = new Event();


  //create an Analysys Tool object and create histograms
  AnaTools *myAnaTools = new AnaTools(f,myEvent);
  myAnaTools->BookingHistograms();

  //open input file
  //to do according to the file format, example:
  ifstream infile;
  infile.open(inname);
  if(!infile.is_open()){
   printf("***************88File %s not found, exiting from the program*****************\n",inname.data());
    exit(-1);
  }
  infile.seekg (0, infile.end);
  filelength = infile.tellg();
	myAnaTools->Pedestal("gain.dat");
  //loop on events, read an event until file is finished
  while(filepos<filelength){
    myEvent->ReadEvent(inname, &filepos);
    if(nevent<=3){myAnaTools->FillHistogram(nevent);}
    myAnaTools->Process();
		myAnaTools->TOF();
    myEvent->Clear();
    nevent++;
    if(nevent%100==0){cout << "\r" <<"Processed " << 100*filepos/filelength << "%" << " of file" <<flush;}
  }
  cout << "\r" <<"Processed 100" << "%" << "of file" <<flush;
  cout << " " << endl;
  myAnaTools->f_TOF();
	
  cout<< "Total No. Events read:" << nevent-1 << endl; 
 
  infile.close();
  
  //close output file
  f->Write();
  f->Close();

  delete myAnaTools;


  return 0;

}

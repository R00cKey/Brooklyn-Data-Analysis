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
AnaTools::AnaTools(TFile *f, Event *myEvent, double cf_, double th_){ 
  outfile = f;
  event = myEvent;
  cf=cf_;
  th=th_;

  ifstream qeinfile; //File "QE.dat" stream
  string fileline; //A line of the File

	int ch=0;
	qeinfile.open("QE.dat");// file containing numbers in 5 columns
	getline(qeinfile,fileline);
     if(qeinfile.fail()) // checks to see if file opended 
   	 { 
		  	cout << "error= input file of pedestal not found." << endl; 
   	 } 
		  while(!qeinfile.eof()) // reads file to end of file
      { 
 						
						getline(qeinfile,fileline);
      			istringstream ss(fileline);
       		 	ss >> qe_[ch];
       		 	ss >> qe_[ch];
            ch++;
      }
	qeinfile.close();

  //INITIALIZING TRAPPING EFFICIENCY
  for(int k=0; k<16; k++){
		etr_[k]=4.88e-2;
	}
}

//Destructor
AnaTools::~AnaTools(){
	return;

}
//Getters
double AnaTools::getGain(int channel){return gain_[channel];}
double AnaTools::getQE(int channel){return qe_[channel];}
double AnaTools::getEtr(int channel){return etr_[channel];}


void AnaTools::BookingHistograms(){
	outfile->cd("..");

	//CHARGE HISTOGRAMS
	gDirectory->mkdir("Hist_Total_Charge");
	gDirectory->cd("Hist_Total_Charge");
  TString name = Form("Hist_total_charge");
  TString title = Form("Total Charge distribution; Charge[C]; Counts[#]");
  hctot = new TH1D(name, title, 500, -20.e-11, 20.e-11);
  gDirectory->cd("..");
  gDirectory->mkdir("Hist_Channels_Charge");
  gDirectory->cd("Hist_Channels_Charge");
	for(unsigned int k=1; k<=NCHANNELS;k++){
  		TString name = Form("Hist_Channel_%d",k-1);
  		TString title = Form("Charge distribution, channel %d; Charge[C]; Counts[#]", k-1);
  		hc_vector[k-1] = new TH1D(name, title, 500, -2.e-11, 2.e-11);
  	}

  gDirectory->cd("..");

  
  gDirectory->mkdir("Hist_Channels_Charge_shifted");
  gDirectory->cd("Hist_Channels_Charge_shifted");
	for(unsigned int k=1; k<=NCHANNELS;k++){
  		TString name = Form("Hist_Channel_%d_shifted",k-1);
  		TString title = Form("Charge distribution shifted by the mean value of the pedestal, channel %d; Charge[C]; Counts[#]", k-1);
  		hc_vector_shifted[k-1] = new TH1D(name, title, 500, -2.e-11, 2.e-11);
  	}
  gDirectory->cd("..");
  
  gDirectory->mkdir("Hist_Channels_Charge_shifted_cut");
  gDirectory->cd("Hist_Channels_Charge_shifted_cut");
	for(unsigned int k=1; k<=NCHANNELS;k++){
  		TString name = Form("Hist_Channel_%d_shifted_cut",k-1);
  		TString title = Form("Charge distribution shifted by the mean value of the pedestal and cut, channel %d; Charge[C]; Counts[#]", k-1);
  		hc_vector_shifted_cut[k-1] = new TH1D(name, title, 500, -2.e-11, 2.e-11);
  	}
  gDirectory->cd("..");
  
  gDirectory->mkdir("Hist_Channels_Charge_around_peak");
  gDirectory->cd("Hist_Channels_Charge_around_peak");
	for(unsigned int k=1; k<=NCHANNELS;k++){
  		TString name = Form("Hist_Channel_%d_around_peak",k-1);
  		TString title = Form("Charge distribution evaluated around the peak of the waveform, channel %d; Charge[C]; Counts[#]", k-1);
  		hc_vector_ap[k-1] = new TH1D(name, title, 400, -10.e-11, 60.e-11);
  	}
  gDirectory->cd("..");
  
   
  //TOF HISTOGRAMS
  for(int i=2;i<14;i++){
 		//CONSTANT FRACTION HISTOGRAMS
 		if(cf!=0){
  		char dir[17]="Hist_TOF_cfm_ch";
	  	char *newHist=strcat(dir, to_string(i).c_str());        
 			gDirectory->mkdir(&newHist[0]);
 			gDirectory->cd(&newHist[0]);

			TString name = Form("Hist_TOF_cfm_ch%d",i);
			TString title = Form("Distribution of time of arrival using Constant Fraction Method, constant=%g, channel %d, not cut; Time of arrival[s]; Counts [#]",cf,i);
  		hTOF_cfm[i] = new TH1D(name, title, 500, -1.e-7, 1.e-7);

  		name = Form("Hist_TOF_cfm_ch%d_cut",i);
  		title = Form("Distribution of time of arrival using Constant Fraction Method, constant=%g, channel %d; Time of arrival[s]; Counts [#]",cf,i);
  		hTOF_cfm_cut[i] = new TH1D(name, title, 500, -1.e-7, 1.e-7);
  		gDirectory->cd("..");
  		}
  	
  	//FIXED THRESHOLD HISTOGRAMS
  	if(th!=0){
  		char dir2[17]="Hist_TOF_ft_ch";
	  	char *newHist2=strcat(dir2, to_string(i).c_str());        
 			gDirectory->mkdir(&newHist2[0]);
 			gDirectory->cd(&newHist2[0]);
	
  		name = Form("Hist_TOF_ft_ch%d",i);
  		title = Form("Distribution of time of arrival using Fixed Threshold Method, threshold=%gV, channel %d, not cut; Time of arrival[s]; Counts [#]",th,i);
  		hTOF_ft[i] = new TH1D(name, title, 500, -1.e-7, 1.e-7);

  		name = Form("Hist_TOF_ft_ch%d_cut",i);
  		title = Form("Distribution of time of arrival using Fixed Threshold Method, threshold=%gV, channel %d; Time of arrival[s]; Counts [#]",th,i);
  		hTOF_ft_cut[i] = new TH1D(name, title, 500, -1.e-7, 1.e-7);
 			gDirectory->cd("..");
  	}
  	
 
  	
  	}
   
  //LIGHT YIELD HISTOGRAMS
  gDirectory->mkdir("Hist_Channels_Light_Yield");
  gDirectory->cd("Hist_Channels_Light_Yield");
	for(unsigned int k=1; k<=NCHANNELS;k++){
  		TString name = Form("Hist_LY_Channel_%d",k-1);
  		TString title = Form("Light Yield Distribution Channel %d; LY[# produced photons/Energy loss]; Counts[#]", k-1);
  		hly_vector[k-1] = new TH1D(name, title, 100, -100., 2000.);
  	}
	gDirectory->cd("..");
	
	for(int i=1; i<=3; i++){       
	
		//WAVEFORMS HISTOGRAMS                
		char dir[10]="Evento";
  	char *newEvent=strcat(dir, to_string(i).c_str());        
  	gDirectory->mkdir(&newEvent[0]);
  	gDirectory->cd(&newEvent[0]);
  	for(unsigned int k=1; k<=NCHANNELS;k++){
  		TString name = Form("Event_%d_Channel_%d", i,k-1);
  		TString title = Form("Event %d, Channel %d; time[s]; Amplitude[V]", i,k-1);
  		hist_vector[i-1][k-1] = new TH1D(name, title, 1024, 0, 1024*SAMPLINGPERIOD);
  	}
  gDirectory->cd("..");	
	}
	
	//CORRELATION HISTOGRAMS
	
	gDirectory->mkdir("Hist_Corr");
	gDirectory->cd("Hist_Corr");
	for(unsigned int k=1; k<=NCHANNELS;k++){
		if(k==1){
  		TString name = "Hist_Corr_Channel_0_WRT_1";
  		TString title = "Correlation histogram of channel 0 with respect to channel 1; Amplitude of channel 0 [V]; Amplitude of channel 1[V]; Counts[#]";
  		hCORR_succ[k-1] = new TH2D(name, title, 800, -0.1, 0, 800, -0.1,0);
  	}
  	else if(k==NCHANNELS){
  		name = "Hist_Corr_Channel_15_WRT_14";
  		title = "Correlation histogram of channel 15 with respect to channel 14; Amplitude of channel 15 [V]; Amplitude of channel 14[V]; Counts[#]";
  		hCORR_prec[k-1] = new TH2D(name, title, 800, -0.1, 0, 800, -0.1, 0);
  	}
  	else{
  		name = Form("Hist_Corr_Channel_%d_WRT_%d", k-1, k-2 );
  		title = Form("Correlation histogram of channel %d with respect to channel %d ; Amplitude of channel %d [V]; Amplitude of channel %d[V]; Counts[#]", k-1,k-2, k-1, k-2);
  		hCORR_prec[k-1] = new TH2D(name, title, 800, -0.1, 0, 800, -0.1, 0);
  		
  		name = Form("Hist_Corr_Channel_%d_WRT_%d", k-1, k);
  		title = Form("Correlation histogram of channel %d with respect to channel %d; Amplitude of channel %d [V]; Amplitude of channel %d[V]; Counts[#]", k-1,k, k-1, k);
  		hCORR_succ[k-1] = new TH2D(name, title, 800, -0.1, 0, 800, -0.1, 0);
  	}	
  }
	gDirectory->cd("..");
	
	
}

//Method for Data Analysis: Gets charges and light yields and puts them into histograms
void AnaTools::Process(){
	double tot_charge=0;
	int tpeak[16];
	double t_in1[16];
	double t_in2[16];
	double charge_ap[16];
	for(unsigned int i =0; i < event->getWaveforms().size(); i++){
		tpeak[i]=PeakTimeFinder(i);
		double charge=0;
		charge_ap[i]=0;
		
		
		
		t_in1[i]=tpeak[i];
				while(event->getWaveforms()[i]->getv_amplitude()[int(t_in1[i])]<event->getWaveforms()[i]->getv_amplitude()[tpeak[i]]*0.1){
				t_in1[i]=t_in1[i]-1;
			 }
		t_in2[i]=tpeak[i];
				while(event->getWaveforms()[i]->getv_amplitude()[int(t_in2[i])]<event->getWaveforms()[i]->getv_amplitude()[tpeak[i]]*0.1){
				t_in2[i]=t_in2[i]+1;
			 }
		
		
 		for(int k=0; k < NSAMPLING; k++){
 		
 			charge +=  event->getWaveforms()[i]->getv_amplitude()[k]*SAMPLINGPERIOD;
 			if(abs(k-tpeak[i])<30){
 			//if((k-tpeak[i]<t_in2[i])&&(k-tpeak[i]<t_in1[i])){//?
 				charge_ap[i] +=  event->getWaveforms()[i]->getv_amplitude()[k]*SAMPLINGPERIOD;
 				}
 		}
 		event->getWaveforms()[i]->setcharge(-charge/50); //I divide by R=50 Ohm
 		tot_charge += event->getWaveforms()[i]->getcharge(); //il getter dovrebbe contenere la charge modificata dal setter che ho appena chiamato
	}
	event->settot_charge(tot_charge);

	outfile->cd();
	//CHARGE HISTOGRAMS FILLING
	for(unsigned int i =0; i < event->getWaveforms().size(); i++){
			
			hc_vector[i]->Fill(event->getWaveforms()[i]->getcharge());
			hc_vector_shifted[i]->Fill(event->getWaveforms()[i]->getcharge()-ped_mean[i]);
			if(event->getWaveforms()[i]->getcharge()>ped_mean[i]+3*ped_sigma[i]){
				hc_vector_shifted_cut[i]->Fill(event->getWaveforms()[i]->getcharge()-ped_mean[i]);
				//hc_vector_ap[i]->Fill(-charge_ap[i]);
			}
			hc_vector_ap[i]->Fill(-charge_ap[i]);
  }
  gDirectory->cd("Hist_Total_Charge");
  hctot->Fill(event->GetTot_charge());
  gDirectory->cd("..");
  
	//LIGHT YIELD HISTOGRAMS FILLING
  gDirectory->cd("Hist_Channels_Light_Yield");
	for(unsigned int i =0; i < event->getWaveforms().size(); i++){
		hly_vector[i]->Fill(getLightYield(event->getWaveforms()[i]->getcharge()-ped_mean[i], getGain(i), getQE(i), getEtr(i)));
  }


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
	return 2*Q/(gain*qe*etr*ECHARGE);
}





void AnaTools::TOF(){
	double V_peak[16];
	double t_in_cfm[16];
	double t_in_ft[16];
	double tpeak[16];
		for(unsigned int i =0; i < event->getWaveforms().size(); i++){
			/*V_peak[i]=0;
			for(int k=0; k < NSAMPLING; k++){
				if(V_peak[i]>event->getWaveforms()[i]->getv_amplitude()[k]){
					V_peak[i]=event->getWaveforms()[i]->getv_amplitude()[k];
					tpeak[i]=k;
				}
				
			}*/
				
			tpeak[i]=PeakTimeFinder(i);
			V_peak[i]=event->getWaveforms()[i]->getv_amplitude()[int(tpeak[i])];
			//	cout << "tpeak " << tpeak[i] << endl;
		
		if(cf!=0){
				t_in_cfm[i]=tpeak[i];
				while(event->getWaveforms()[i]->getv_amplitude()[int(t_in_cfm[i])]<V_peak[i]*cf){
				t_in_cfm[i]=t_in_cfm[i]-1;
			 }
		}
		if(th!=0){
		 		t_in_ft[i]=tpeak[i];
				while(event->getWaveforms()[i]->getv_amplitude()[int(t_in_ft[i])]<-th){
					t_in_ft[i]=t_in_ft[i]-1;
		 	}
		}
		}
	double TOF_mean_cfm=(t_in_cfm[0]+t_in_cfm[1]+t_in_cfm[14]+t_in_cfm[15])*0.25*SAMPLINGPERIOD;
	double TOF_mean_ft=(t_in_ft[0]+t_in_ft[1]+t_in_ft[14]+t_in_ft[15])*0.25*SAMPLINGPERIOD;
	for(int l=2;l<14;l++){
		if(cf!=0){
			t_in_cfm[l]=t_in_cfm[l]*SAMPLINGPERIOD-TOF_mean_cfm;
			hTOF_cfm[l]->Fill(t_in_cfm[l]);
			if(event->getWaveforms()[l]->getcharge()>ped_mean[l]+3*ped_sigma[l]){
					hTOF_cfm_cut[l]->Fill(t_in_cfm[l]);
				}
			}
		if(th!=0){
				t_in_ft[l]=t_in_ft[l]*SAMPLINGPERIOD-TOF_mean_ft;
				hTOF_ft[l]->Fill(t_in_ft[l]);
				if(event->getWaveforms()[l]->getcharge()>ped_mean[l]+3*ped_sigma[l]){
					hTOF_ft_cut[l]->Fill(t_in_ft[l]);
				}
		}
	}
	
	
	
}



/*
void AnaTools::f_TOF(){   //f_TOF() is now built in macro ConstVarMacro.C

	for(int l=2;l<14;l++){
	
		
		fit_TOF_cfm[l-2] = new TF1("fit_TOF","gaus(0)+gaus(3)+[6]",-0.5,0.5);
		fit_TOF_cfm[l-2] ->SetParameter(0,hTOF_cfm[l]->GetBinContent(hTOF_cfm[l]->GetMaximumBin()));
		fit_TOF_cfm[l-2] ->SetParameter(1,hTOF_cfm[l]->GetBinCenter(hTOF_cfm[l]->GetMaximumBin()));
		fit_TOF_cfm[l-2] ->SetParameter(2,hTOF_cfm[l]->GetStdDev());
		fit_TOF_cfm[l-2] ->SetParLimits(2,0.5e-9,2.2e-9); 
		fit_TOF_cfm[l-2] ->SetParameter(3,hTOF_cfm[l]->GetBinContent(hTOF_cfm[l]->GetMaximumBin()));
		fit_TOF_cfm[l-2] ->SetParameter(4,hTOF_cfm[l]->GetBinCenter(hTOF_cfm[l]->GetMaximumBin()));
		fit_TOF_cfm[l-2] ->SetParameter(5,hTOF_cfm[l]->GetStdDev());
		fit_TOF_cfm[l-2] ->SetParLimits(5,2e-9,4e-9); 
		fit_TOF_cfm[l-2]	->SetNpx(10000);
		
		hTOF_cfm[l]->Fit(fit_TOF_cfm[l-2],"R");
		
	 //Usare SetRangeUser per zoommare
 	
		
		hTOF_cfm[l]->SetAxisRange(-0.05e-6,0.05e-6);
		
		
			fit_TOF_cfm_cut[l-2] = new TF1("fit_TOF","gaus(0)+gaus(3)+[6]",-0.5,0.5);
		fit_TOF_cfm[l-2] ->SetParameter(0,hTOF_cfm_cut[l]->GetBinContent(hTOF_cfm_cut[l]->GetMaximumBin()));
		fit_TOF_cfm_cut[l-2] ->SetParameter(1,hTOF_cfm_cut[l]->GetBinCenter(hTOF_cfm_cut[l]->GetMaximumBin()));
		fit_TOF_cfm_cut[l-2] ->SetParameter(2,hTOF_cfm_cut[l]->GetStdDev());
		fit_TOF_cfm_cut[l-2] ->SetParLimits(2,0.5e-9,2.2e-9); 
		fit_TOF_cfm_cut[l-2] ->SetParameter(3,hTOF_cfm_cut[l]->GetBinContent(hTOF_cfm_cut[l]->GetMaximumBin()));
		fit_TOF_cfm_cut[l-2] ->SetParameter(4,hTOF_cfm_cut[l]->GetBinCenter(hTOF_cfm_cut[l]->GetMaximumBin()));
		fit_TOF_cfm_cut[l-2] ->SetParameter(5,hTOF_cfm_cut[l]->GetStdDev());
		fit_TOF_cfm_cut[l-2] ->SetParLimits(5,2e-9,4e-9); 
		fit_TOF_cfm_cut[l-2]	->SetNpx(10000);
		
		hTOF_cfm_cut[l]->Fit(fit_TOF_cfm_cut[l-2],"R");
		
	 //Usare SetRangeUser per zoommare
 	
		
		hTOF_cfm_cut[l]->SetAxisRange(-0.05e-6,0.05e-6);
	}
}
*/

void AnaTools::Pedestal(string inname){
	ifstream infile; //File "inname.dat" stream
  string fileline; //A line of the File
	int ch=0;
	infile.open(inname);// file containing numbers in 5 columns
	getline(infile,fileline);
     if(infile.fail()) // checks to see if file opended 
   	 { 
		  	cout << "error= input file of pedestal not found." << endl; 
   	 } 
		  while(!infile.eof()) // reads file to end of file
      { 
 						
						getline(infile,fileline);
      			istringstream ss(fileline);
       		 	ss >> ped_mean[ch];
       		 	ss >> ped_mean[ch];
       		 	ss >> ped_sigma[ch];
            ss >> mean2[ch];
            ss >> sigma2[ch];
            ch++;
      }
  //EVALUATING GAIN    
  for(int ch=0; ch<NCHANNELS; ch++){
  	gain_[ch]=mean2[ch]/ECHARGE;
  }
  infile.close();

  /*for(int i=0;i<16;i++)
           { 
   	cout << "\tped mean= "<< ped_mean[i] << "\tped sigma= " << ped_sigma[i] << endl;
   	
   	
   	
   	
   	}*/
}   	
   	
   	int AnaTools::PeakTimeFinder(double ch){
   		double V_peak=0;
   		int tpeak;
   		for(int k=0; k < NSAMPLING; k++){
				if(V_peak>event->getWaveforms()[ch]->getv_amplitude()[k]){
					V_peak=event->getWaveforms()[ch]->getv_amplitude()[k];
					tpeak=k;
				
				}
			}	
			return tpeak;
   	}
   	
   	
   	
   int AnaTools::TriggeredChannel(){	
   	double Vpeak=0;
   	int ch_trigger=0;
   	int time_peak;
   	for (int ch=0; ch<16; ch++){
			time_peak=PeakTimeFinder(ch);
			if (Vpeak < -(event->getWaveforms()[ch]->getv_amplitude()[time_peak])){
					Vpeak = -(event->getWaveforms()[ch]->getv_amplitude()[time_peak]);
   				ch_trigger=ch;
   			}
   		}
   		return ch_trigger;
   }
   	
   	
   	
  void AnaTools::Correlation(){
  	double A_tr;
  	double A_prec;
  	double A_succ;
  	int tr_ch;
  	tr_ch=TriggeredChannel();
  	A_tr=event->getWaveforms()[tr_ch]->getv_amplitude()[PeakTimeFinder(tr_ch)];
  	if(tr_ch==0){
  	
  		A_succ=event->getWaveforms()[tr_ch+1]->getv_amplitude()[PeakTimeFinder(tr_ch+1)];
  		hCORR_succ[tr_ch]->Fill(A_tr,A_succ);
  	
  	
  	}else if(tr_ch==15){
  	
  		A_prec=event->getWaveforms()[tr_ch-1]->getv_amplitude()[PeakTimeFinder(tr_ch-1)];
  		hCORR_prec[tr_ch]->Fill(A_tr,A_prec);
  		
  	}else {
  	
  		A_prec=event->getWaveforms()[tr_ch-1]->getv_amplitude()[PeakTimeFinder(tr_ch-1)];
  		A_succ=event->getWaveforms()[tr_ch+1]->getv_amplitude()[PeakTimeFinder(tr_ch+1)];
			hCORR_succ[tr_ch]->Fill(A_tr,A_succ);
			hCORR_prec[tr_ch]->Fill(A_tr,A_prec);
		}  
  
  }
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
   	
 

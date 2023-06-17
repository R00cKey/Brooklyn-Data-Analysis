void QaroundpeakMacro(){

	TFile *file=new TFile("outputMuon_6.root", "READ");
	ofstream infofile;
	infofile.open("Qaround_6.dat");
	infofile<< "CHANNEL\tQ[C]\tSTDDEV[C]\n";	
	for(int k=0;k<16;k++){
	file->cd();
	gDirectory->cd("Hist_Channels_Charge_around_peak");	
	TString name = Form("Hist_Channel_%d_around_peak",k);
	TH1D *h=(TH1D*)(gDirectory->Get(name));

	infofile << k << "\t" << h->GetMean() << "\t" << h->GetMeanError() << endl;

	}
	infofile.close();
	file->Close();
}
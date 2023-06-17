void LYMacro(){

	TFile *file=new TFile("outputMuon_6.root", "READ");
	ofstream infofile;
	infofile.open("ly_6.dat");
	infofile<< "ly[#Photons]\tSTDDEV[#Photons]\n";	
	for(int k=0;k<16;k++){
	file->cd();
	gDirectory->cd("Hist_Channels_Light_Yield");	
	TString name = Form("Hist_LY_Channel_%d",k);
	TH1D *h=(TH1D*)(gDirectory->Get(name));

	infofile << k << "\t" << h->GetMean() << "\t" << h->GetMeanError() << endl;

	}
	infofile.close();
	file->Close();
}
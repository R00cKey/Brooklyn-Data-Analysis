void QshiftedMacro(){

	TFile *file=new TFile("outputMuon_6.root", "READ");
	ofstream infofile;
	infofile.open("Qshifted_6.dat");
	infofile<< "CHANNEL\tQSHIFTED[C]\tSTDDEV[C]\n";	
	for(int k=0;k<16;k++){
	file->cd();
	gDirectory->cd("Hist_Channels_Charge_shifted");	
	TString name = Form("Hist_Channel_%d_shifted",k);
	TH1D *h=(TH1D*)(gDirectory->Get(name));

	infofile << k << "\t" << h->GetMean() << "\t" << h->GetMeanError() << endl;

	}
	infofile.close();
	file->Close();
}
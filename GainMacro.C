void GainMacro(){

	TFile *file=new TFile("outputGain.root", "READ");
	TFile *outfile=new TFile("gainfits.root", "RECREATE");
	ofstream infofile;
	infofile.open("gain.dat");
	infofile<< "CHANNEL\tMEAN[Gaus1]\tSTDDEV[Gaus1]\tMEAN[Gaus2-Gaus1]\tSTDDEV[Gaus2]\n";	
	for(int k=0;k<16;k++){
	file->cd();
	gDirectory->cd("Hist_Channels_Charge");	
	TString name = Form("Hist_Channel_%d",k);
	TH1D *h=(TH1D*)((TH1D*)gDirectory->Get(name)->Clone(Form("Gain_Fit_Channel_%d",k)));
	h->Draw();

	//Fitting data
	TF1 *fun=new TF1("FitFun1", "gaus(0)+gaus(3)", -1.e-11 ,5.);  //0:ampiezza 1:media 2:sigma +gaus(3) 3:ampiezza 4:media ...
	fun->SetParameter(0, h->GetBinContent(h->GetMaximumBin())); //Prendi valore nel bin(del bin più pieno)
	fun->SetParameter(1, h->GetBinCenter(h->GetMaximumBin())); //Prendi il centro del bin(del bin più pieno)
	fun->SetParameter(2, h->GetStdDev());
	fun->SetParameter(3, h->GetBinContent(h->GetMaximumBin())/1000);
	fun->SetParameter(4, h->GetBinCenter(h->GetMaximumBin())+2e-12);
	fun->SetParameter(5, 1e-12);
	fun->SetParLimits(1, -5.e-12, 5.e-12);
	fun->SetParLimits(2, -1.e-12, 1.e-12);
	fun->SetParLimits(4, -1.e-12, 5.e-12);
	fun->SetParLimits(5, -1.e-12, 1.e-12);
	h->Fit(fun, "R"); //In alternativa a fun "FitFun1"
	outfile->cd();
	h->Write();

	infofile << k << "\t" << fun->GetParameter(1) << "\t" << fun->GetParameter(2) << "\t" << fun->GetParameter(4)-fun->GetParameter(1) << "\t" << fun->GetParameter(5) << endl;

	}
	infofile.close();
	outfile->Close();
}
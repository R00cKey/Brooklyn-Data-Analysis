void QpeAPMacro(){

	TFile *file=new TFile("outputNoise_1.root", "READ");
	TFile *outfile=new TFile("gainfitsAround05_1.root", "RECREATE");
	ofstream infofile;
	infofile.open("QpeAround05_1.dat");
	infofile<< "CHANNEL\tMEAN[Gaus2-Gaus1]\tSTDDEV[Mean]\n";	
	for(int k=0;k<16;k++){
	file->cd();
	gDirectory->cd("Hist_Channels_Charge_around_peak");	
	TString name = Form("Hist_Channel_%d_around_peak",k);
	TH1D *h=(TH1D*)((TH1D*)gDirectory->Get(name)->Clone(Form("Gain_Fit_Channel_%d_Around_Peak",k)));
	TString YTitle= Form("Counts [#], Bin Width = %g [C]", h->GetBinWidth(10));
	h->SetYTitle(YTitle);
	h->Rebin(4);
	h->Draw();

	//Fitting data
	TF1 *fun=new TF1("FitFun1", "gaus(0)", -4.e-11 ,2.e-11);  //0:ampiezza 1:media 2:sigma +gaus(3) 3:ampiezza 4:media ...
	TF1 *fun2=new TF1(Form("FitFun2_%d", k), "gaus(0)+gaus(3)", -2.e-10 ,5.e-10);
	fun->SetParameter(0, h->GetBinContent(h->GetMaximumBin())); //Prendi valore nel bin(del bin più pieno)
	fun->SetParameter(1, h->GetBinCenter(h->GetMaximumBin())); //Prendi il centro del bin(del bin più pieno)
	fun->SetParameter(2, h->GetStdDev());
	fun->SetParLimits(1, -5.e-12, 5.e-12);
	fun->SetParLimits(2, -1.e-12, 1.e-12);
	h->Fit(fun, "R");
	fun2->FixParameter(0, fun->GetParameter(0));
	fun2->FixParameter(1, fun->GetParameter(1));
	fun2->FixParameter(2, fun->GetParameter(2));
	fun2->SetParameter(3, fun->GetParameter(0)/500);
	fun2->SetParameter(4, fun->GetParameter(1)+1.e-10);
	fun2->SetParameter(5, 1.e-13);
	fun2->SetParLimits(3, 5, fun->GetParameter(0)/10);
	fun2->SetParLimits(4, fun->GetParameter(1)+5.e-11, fun->GetParameter(1)+1.5e-10);
	fun2->SetParLimits(5, 1.e-11, 0.8e-10);
	fun2->SetParName(0,"A_{1}(#)");
	fun2->SetParName(1,"Q_{pedestal}(C)");
	fun2->SetParName(2,"#sigma_{1}(C)");		
	fun2->SetParName(3,"A_{2}(#)");
	fun2->SetParName(4,"Q_{pe}(C)");	
	fun2->SetParName(5,"#sigma_{2}(C)");
	h->Fit(fun2, "R");
	fun2->ReleaseParameter(0);
	fun2->ReleaseParameter(1);
	fun2->ReleaseParameter(2);
	fun->SetParLimits(1, -5.e-12, 5.e-12);
	fun->SetParLimits(2, -1.e-12, 1.e-12);
	h->Fit(fun2, "R");
	outfile->cd();
	h->Write();
	fun2->SetLineColor(kGreen+1);
	fun2->Write();

	infofile << k << "\t" << fun2->GetParameter(4)-fun2->GetParameter(1) << "\t" << sqrt(fun2->GetParError(1)*fun2->GetParError(1)+fun2->GetParError(4)*fun2->GetParError(4)) << endl;

	}
	infofile.close();
	outfile->Close();
}
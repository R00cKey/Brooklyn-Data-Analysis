void GainMacro(){

	TFile *file=new TFile("outputNoise_1.root", "READ");
	TFile *outfile=new TFile("gainfits05_1.root", "RECREATE");
	ofstream infofile;
	ofstream infofile2;
	infofile.open("gain05_1.dat");
	infofile2.open("Qpe05_1.dat");
	infofile<< "CHANNEL\tMEAN[Gaus1]\tSTDDEV[Gaus1]\tMEAN[Gaus2-Gaus1]\tSTDDEV[Gaus2]\n";
	infofile2<< "CHANNEL\tMEAN[Gaus2-Gaus1]\tSTDDEV[Mean]\n";
	for(int k=0;k<16;k++){
	file->cd();
	gDirectory->cd("Hist_Channels_Charge");
	TString name = Form("Hist_Channel_%d",k);
	TH1D *h=(TH1D*)((TH1D*)gDirectory->Get(name)->Clone(Form("Gain_Fit_Channel_%d",k)));
	TString YTitle= Form("Counts [#], Bin Width = %g [C]", h->GetBinWidth(10));
	h->SetYTitle(YTitle);
	h->Draw();

	//Fitting data
	TF1 *fun=new TF1("FitFun1", "gaus(0)", -1.e-11 ,5.e-12);  //0:Height 1:mean 2:sigma +gaus(3) 3:Height 4:mean ...
	TF1 *fun2=new TF1(Form("FitFun2_%d", k), "gaus(0)+gaus(3)", -1.e-11 ,1.e-11);
	TF1 *fun3=new TF1(Form("FitFun3_%d", k), "gaus(0)", -1.e-11 ,5.e-12);
	TF1 *fun4=new TF1(Form("FitFun4_%d", k), "gaus(0)", -1.e-11 ,5.e-12);
	fun->SetParameter(0, h->GetBinContent(h->GetMaximumBin())); //Get the value of the the maximum bin
	fun->SetParameter(1, h->GetBinCenter(h->GetMaximumBin())); //Get the location of the center of the maximum bin
	fun->SetParameter(2, h->GetStdDev());
	fun->SetParLimits(1, -5.e-12, 5.e-12);
	fun->SetParLimits(2, -1.e-12, 1.e-12);
	h->Fit(fun, "R");
	fun2->FixParameter(0, fun->GetParameter(0));
	fun2->FixParameter(1, fun->GetParameter(1));
	fun2->FixParameter(2, fun->GetParameter(2));
	fun2->SetParameter(3, fun->GetParameter(0)/1000);
	fun2->SetParameter(4, fun->GetParameter(1)+2.e-12);
	fun2->SetParameter(5, 1.e-13);
	fun2->SetParLimits(4, fun->GetParameter(1)+5.e-13, fun->GetParameter(1)+3.e-12);
	fun2->SetParLimits(5, -1.e-12, 1.5e-12);
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
	fun2->SetParLimits(1, -5.e-11, fun->GetParameter(1)+2.e-12);
	fun2->SetParLimits(2, -1.e-12, 1.e-12);
	h->Fit(fun2, "R");
	outfile->cd();
	h->Write();
	fun3->FixParameter(0, fun2->GetParameter(0));
	fun3->FixParameter(1, fun2->GetParameter(1));
	fun3->FixParameter(2, fun2->GetParameter(2));
	fun4->FixParameter(0, fun2->GetParameter(3));
	fun4->FixParameter(1, fun2->GetParameter(4));
	fun4->FixParameter(2, fun2->GetParameter(5));
	fun3->SetLineColor(kGreen+1);
	fun4->SetLineColor(kBlue);
	fun3->Write();
	fun4->Write();

	infofile << k << "\t" << fun2->GetParameter(1) << "\t" << fun2->GetParameter(2) << "\t" << fun2->GetParameter(4)-fun2->GetParameter(1) << "\t" << fun2->GetParameter(5) << endl;
	infofile2 << k << "\t" << fun2->GetParameter(4)-fun2->GetParameter(1) << "\t" << sqrt(fun2->GetParError(1)*fun2->GetParError(1)+fun2->GetParError(4)*fun2->GetParError(4)) << endl;
	}
	infofile.close();
	outfile->Close();
}
void MacroFixedThreshold(string file_in, string file_out, double run, double ft){
  
  
	TFile *file=new TFile(file_in.data(), "READ"); 
	TFile *outfile=new TFile(file_out.data(), "RECREATE"); //DISCLAIMER: "RECREATE" WILL OVERWRITE THE CONTENT OF THE OUTUPUT FILE
	ofstream infofile;
	infofile.open(file_out);
	

	ofstream infofile_fit;
	infofile_fit.open(Form("FitParRun%gFt%g.dat", run, ft));
	
	ofstream infofile_fit_cut;
	infofile_fit_cut.open(Form("FitParRun%gFt%gCut.dat", run, ft));
	
	infofile_fit<< "THRESHOLD\t"<<ft<<endl;
	infofile_fit_cut<< "THRESHOLD\t"<<ft<<endl;

	infofile_fit<<"CHANNEL\tENTRIES\tMEAN[Gaus1]\tSTDDEV[Gaus1]\tSTDDEV_ERR[Gaus1]\tMEAN[Gaus2]\tSTDDEV[Gaus2]\t VerticalShift\n";
	infofile_fit_cut<<"CHANNEL\tENTRIES\tMEAN[Gaus1]\tSTDDEV[Gaus1]\tSTDDEV_ERR[Gaus1]\tMEAN[Gaus2]\tSTDDEV[Gaus2]\t VerticalShift\n";
	
	
	TH1D *hTOF_ft[12];
	TH1D *hTOF_ft_cut[12];
	TF1 *fit_TOF_ft[12];
  TF1 *fit_TOF_ft_cut[12];
	for(int l=2;l<14;l++){
		file->cd();
		TString name_dir = Form("Hist_TOF_ft_ch%d", l);
		gDirectory->cd(name_dir);	
		TString name_1 = Form("Hist_TOF_ft_ch%d", l);
		TString name_2 = Form("Hist_TOF_ft_ch%d_cut", l);
		hTOF_ft[l-2]=(TH1D*)((TH1D*)gDirectory->Get(name_1)->Clone(Form("Hist_TOF_ft_ch%d", l)));
		hTOF_ft_cut[l-2]=(TH1D*)((TH1D*)gDirectory->Get(name_2)->Clone(Form("Hist_TOF_ft_ch%d_cut", l)));
		hTOF_ft[l-2]->Rebin(2);
		hTOF_ft_cut[l-2]->Rebin(2);
	
		fit_TOF_ft[l-2] = new TF1(Form("fit_TOF_ft_ch%d",l),"gaus(0)+gaus(3)+[6]",-0.5,0.5);
		cout <<l<<endl;
		fit_TOF_ft[l-2] ->SetParameter(0,hTOF_ft[l-2]->GetBinContent(hTOF_ft[l-2]->GetMaximumBin()));
		fit_TOF_ft[l-2] ->SetParLimits(0,0,hTOF_ft[l-2]->GetBinContent(hTOF_ft[l-2]->GetMaximumBin())*1.5); 
		fit_TOF_ft[l-2] ->SetParameter(1,hTOF_ft[l-2]->GetBinCenter(hTOF_ft[l-2]->GetMaximumBin()));
		fit_TOF_ft[l-2] ->SetParLimits(1,-20e-9,20e-9); 
		fit_TOF_ft[l-2] ->SetParameter(2,1e-9);
		fit_TOF_ft[l-2] ->SetParLimits(2,0.5e-9,2.2e-9); 
		fit_TOF_ft[l-2] ->SetParameter(3,hTOF_ft[l-2]->GetBinContent(hTOF_ft[l-2]->GetMaximumBin())*0.1);
		fit_TOF_ft[l-2] ->SetParLimits(3,0,hTOF_ft[l-2]->GetBinContent(hTOF_ft[l-2]->GetMaximumBin())*0.5); 
		fit_TOF_ft[l-2] ->SetParameter(4,hTOF_ft[l-2]->GetBinCenter(hTOF_ft[l-2]->GetMaximumBin()));
		fit_TOF_ft[l-2] ->SetParLimits(4,-20e-9,20e-9); 
		fit_TOF_ft[l-2] ->SetParameter(5,hTOF_ft[l-2]->GetStdDev());
		fit_TOF_ft[l-2] ->SetParLimits(5,1e-9,20e-9);
		fit_TOF_ft[l-2] ->SetParLimits(6,0,10); 
		fit_TOF_ft[l-2] ->SetNpx(10000);
		
		hTOF_ft[l-2]->Fit(fit_TOF_ft[l-2],"R");
		
	 //Usare SetRangeUser per zoommare
 	
 	
		
		hTOF_ft[l-2]->SetAxisRange(-0.05e-6,0.05e-6);
		
		
		fit_TOF_ft_cut[l-2] = new TF1(Form("fit_TOF_ft_cut_ch%d",l),"gaus(0)+gaus(3)+[6]",-0.5,0.5);
		fit_TOF_ft_cut[l-2] ->SetParameter(0,hTOF_ft_cut[l-2]->GetBinContent(hTOF_ft_cut[l-2]->GetMaximumBin()));
		fit_TOF_ft_cut[l-2] ->SetParLimits(0,0,hTOF_ft_cut[l-2]->GetBinContent(hTOF_ft_cut[l-2]->GetMaximumBin())*1.5); 
		fit_TOF_ft_cut[l-2] ->SetParameter(1,hTOF_ft_cut[l-2]->GetBinCenter(hTOF_ft_cut[l-2]->GetMaximumBin()));
		fit_TOF_ft_cut[l-2] ->SetParLimits(1,-20e-9,20e-9); 
		fit_TOF_ft_cut[l-2] ->SetParameter(2,1e-9);
		fit_TOF_ft_cut[l-2] ->SetParLimits(2,0.5e-9,2.2e-9); 
		fit_TOF_ft_cut[l-2] ->SetParameter(3,hTOF_ft_cut[l-2]->GetBinContent(hTOF_ft_cut[l-2]->GetMaximumBin())*0.1);
		fit_TOF_ft_cut[l-2] ->SetParLimits(3,0,hTOF_ft_cut[l-2]->GetBinContent(hTOF_ft_cut[l-2]->GetMaximumBin())*0.5);
		fit_TOF_ft_cut[l-2] ->SetParameter(4,hTOF_ft_cut[l-2]->GetBinCenter(hTOF_ft_cut[l-2]->GetMaximumBin()));
		fit_TOF_ft_cut[l-2] ->SetParLimits(4,-20e-9,20e-9); 
		fit_TOF_ft_cut[l-2] ->SetParameter(5,hTOF_ft_cut[l-2]->GetStdDev());
		fit_TOF_ft_cut[l-2] ->SetParLimits(5,1e-9,20e-9); 
		fit_TOF_ft_cut[l-2] ->SetParameter(6,0.5);
		fit_TOF_ft_cut[l-2] ->SetParLimits(6,0,10);
		fit_TOF_ft_cut[l-2]	->SetNpx(10000);
		
		hTOF_ft_cut[l-2]->Fit(fit_TOF_ft_cut[l-2],"R");
		
	 //Usare SetRangeUser per zoommare
 	
		
		hTOF_ft_cut[l-2]->SetAxisRange(-0.05e-6,0.05e-6);
		
		//gDirectory->cd("..");
		
		outfile->cd();
		
		
		
		hTOF_ft[l-2]->Write();
		hTOF_ft_cut[l-2]->Write();

		
		infofile_fit << l << "\t" << hTOF_ft[l-2]->GetEntries()<<"\t"<<fit_TOF_ft[l-2]->GetParameter(1) << "\t" << fit_TOF_ft[l-2]->GetParameter(2) << "\t" << fit_TOF_ft[l-2]->GetParError(2) << "\t" << fit_TOF_ft[l-2]->GetParameter(4)<< "\t" << 		fit_TOF_ft[l-2]->GetParameter(5) <<"\t"<< fit_TOF_ft[l-2]->GetParameter(6)<<endl;
}		

}

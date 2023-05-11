void ConstVarMacro(int argc, char* argv[]){

  
  string inname("in.dat");
  string outname("out.root");

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
  
  
  
  
	TFile *file=new TFile(file_in, "READ");
	TFile *outfile=new TFile(file_out, "RECREATE");
	ofstream infofile;
	infofile.open(file.out);
	infofile<< "CHANNEL\tMEAN[Gaus1]\tSTDDEV[Gaus1]\tMEAN[Gaus2-Gaus1]\tSTDDEV[Gaus2]\n";	
	TH1D *hTOF_cfm[12];
	TH1D *hTOF_cfm_cut[12];
	TF1 *fit_TOF_cfm[12];
  TF1 *fit_TOF_cfm_cut[12];
	for(int l=2;l<14;l++){
		file->cd();
		TString name_dir = Form("Hist_TOF_cfm_ch%d", l);
		gDirectory->cd(name_dir);	
		TString name_1 = Form("Hist_TOF_cfm_ch%d", l);
		Tstring name_2 = Form("Hist_TOF_cfm_ch%d_cut", l);
		hTOF_cfm[l-2]->Get(name_1);
		hTOF_cfm[l-2]->Get(name_2);
		//TH1D *h=(TH1D*)((TH1D*)gDirectory->Get(name_1)->Clone(Form("Hist_TOF_cfm_ch%d", l)));
		//TH1D *h=(TH1D*)((TH1D*)gDirectory->Get(name_1)->Clone(Form("Hist_TOF_cfm_ch%d", l)));
	
		fit_TOF_cfm[l-2] = new TF1("fit_TOF","gaus(0)+gaus(3)+[6]",-0.5,0.5);
		fit_TOF_cfm[l-2] ->SetParameter(0,hTOF_cfm[l-2]->GetBinContent(hTOF_cfm[l-2]->GetMaximumBin()));
		fit_TOF_cfm[l-2] ->SetParameter(1,hTOF_cfm[l-2]->GetBinCenter(hTOF_cfm[l-2]->GetMaximumBin()));
		fit_TOF_cfm[l-2] ->SetParameter(2,hTOF_cfm[l-2]->GetStdDev());
		fit_TOF_cfm[l-2] ->SetParLimits(2,0.5e-9,2.2e-9); 
		fit_TOF_cfm[l-2] ->SetParameter(3,hTOF_cfm[l-2]->GetBinContent(hTOF_cfm[l-2]->GetMaximumBin()));
		fit_TOF_cfm[l-2] ->SetParameter(4,hTOF_cfm[l-2]->GetBinCenter(hTOF_cfm[l-2]->GetMaximumBin()));
		fit_TOF_cfm[l-2] ->SetParameter(5,hTOF_cfm[l-2]->GetStdDev());
		fit_TOF_cfm[l-2] ->SetParLimits(5,2e-9,4e-9); 
		fit_TOF_cfm[l-2]	->SetNpx(10000);
		
		hTOF_cfm[l]->Fit(fit_TOF_cfm[l-2],"R");
		
	 //Usare SetRangeUser per zoommare
 	
		
		hTOF_cfm[l]->SetAxisRange(-0.05e-6,0.05e-6);
		
		
			fit_TOF_cfm_cut[l-2] = new TF1("fit_TOF","gaus(0)+gaus(3)+[6]",-0.5,0.5);
		fit_TOF_cfm[l-2] ->SetParameter(0,hTOF_cfm_cut[l-2]->GetBinContent(hTOF_cfm_cut[l-2]->GetMaximumBin()));
		fit_TOF_cfm_cut[l-2] ->SetParameter(1,hTOF_cfm_cut[l-2]->GetBinCenter(hTOF_cfm_cut[l-2]->GetMaximumBin()));
		fit_TOF_cfm_cut[l-2] ->SetParameter(2,hTOF_cfm_cut[l-2]->GetStdDev());
		fit_TOF_cfm_cut[l-2] ->SetParLimits(2,0.5e-9,2.2e-9); 
		fit_TOF_cfm_cut[l-2] ->SetParameter(3,hTOF_cfm_cut[l-2]->GetBinContent(hTOF_cfm_cut[l-2]->GetMaximumBin()));
		fit_TOF_cfm_cut[l-2] ->SetParameter(4,hTOF_cfm_cut[l-2]->GetBinCenter(hTOF_cfm_cut[l-2]->GetMaximumBin()));
		fit_TOF_cfm_cut[l-2] ->SetParameter(5,hTOF_cfm_cut[l-2]->GetStdDev());
		fit_TOF_cfm_cut[l-2] ->SetParLimits(5,2e-9,4e-9); 
		fit_TOF_cfm_cut[l-2]	->SetNpx(10000);
		
		hTOF_cfm_cut[l-2]->Fit(fit_TOF_cfm_cut[l-2],"R");
		
	 //Usare SetRangeUser per zoommare
 	
		
		hTOF_cfm_cut[l-2]->SetAxisRange(-0.05e-6,0.05e-6);
	}
	

}

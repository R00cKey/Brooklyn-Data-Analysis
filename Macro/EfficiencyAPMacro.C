void EfficiencyAPMacro(){

	TFile *file=new TFile("outputMuon_1.root", "READ");
	ifstream infofile;
	string fileline; //A line of the File
	infofile.open("gainAP.dat");
	getline(infofile,fileline);
	file->cd();
	gDirectory->cd("Hist_Total_Charge");	
	TString name = Form("Hist_total_charge");
	TH1D *h=(TH1D*)(gDirectory->Get(name));
	for(int k=0;k<16;k++){
		int nevent=0;
		double ped_mean, ped_std;
		getline(infofile,fileline);
		istringstream ss(fileline);
		ss >> ped_mean;
    	ss >> ped_mean;
    	ss >> ped_std;
		file->cd();
		gDirectory->cd("Hist_Channels_Charge_around_peak");	
		TString name = Form("Hist_Channel_%d_around_peak",k);
		TH1D *h=(TH1D*)((TH1D*)gDirectory->Get(name)->Clone(Form("Gain_Fit_Channel_%d",k)));
		for(int i=1; i<=500;i++){
			if(h->GetBinCenter(i)>ped_mean+3*ped_std){nevent+=h->GetBinContent(i);}
		}
	cout << k << "\t" << nevent/h->GetEntries() << endl;
	}
	infofile.close();
}
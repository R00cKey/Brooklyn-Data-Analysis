void MacroFTPlot(double run, double ft1, double ft2, double ft3, double ft4, double ft5, double ft6, double ft7, double ft8, double ft9){

	double cfs[]={ft1, ft2, ft3, ft4 ,ft5 ,ft6 , ft7, ft8 ,ft9};
	
	ifstream infile[9];
	ifstream infile_cut[9];
  string fileline; //appoggio
  string fileline_cut;
 
	
	double err_x[]={0,0,0,0,0,0,0,0,0};
  double stdDev[12][9];
  double stdDev_cut[12][9];
  double stdDev_err[12][9];
  double stdDev_cut_err[12][9];
  int ch; 	 
  int ch_cut;
  
  for(int i=0; i <=8; i++){
  
		infile[i].open(Form("FitParRun%gFt%g.dat", run, cfs[i]));
		infile_cut[i].open(Form("FitParRun%gFt%gCut.dat", run, cfs[i]));
    if(infile[i].fail()){ // checks if file opended 
   		cout << "error=file of fit parameters not found" << endl; 
   	 } 
   	

		getline(infile[i],fileline);
		getline(infile[i],fileline);
		
		getline(infile_cut[i],fileline_cut);
		getline(infile_cut[i],fileline_cut);
		
		while(!infile[i].eof()){// reads file to end of file
			
			getline(infile[i],fileline);
		  istringstream ss(fileline);
		  ss >> ch;
		  ss >> stdDev[ch-2][i];
		  ss >> stdDev[ch-2][i];	
		  ss >> stdDev[ch-2][i];
     	ss >> stdDev_err[ch-2][i];
     }
     
     
     while(!infile_cut[i].eof()){
     	getline(infile_cut[i],fileline_cut);
		  istringstream zz(fileline_cut);
		  zz >> ch_cut;
		  zz >> stdDev_cut[ch_cut-2][i];
		  zz >> stdDev_cut[ch_cut-2][i];	
		  zz >> stdDev_cut[ch_cut-2][i];
		   zz >> stdDev_cut_err[ch_cut-2][i];
     // cout << ch <<endl; 
      //if(ch==13)
      	//break;   
    }
   //	cout << i<<endl;
   	infile[i].close();
   	infile_cut[i].close();
	}
		
	TGraph* graph[12];
	TFile *outfile=new TFile(Form("TimeResVSthresholdRun%g.root", run), "RECREATE");
	
	TGraph* graph_cut[12];
	TFile *outfile_cut=new TFile(Form("TimeResVSthresholdRun%g_cut.root", run), "RECREATE");
	
	for(int d=0; d<=11; d++){
		
		outfile->cd();
		graph[d]=new TGraphErrors(8, cfs, stdDev[d],err_x,stdDev_err[d]);
		graph[d]->SetTitle(Form("Time Resolution vs Fixed Threshold, channel %d, not cut",d+2));
		graph[d]->GetXaxis()->SetTitle("Fixed Threshold[V]");
		graph[d]->GetYaxis()->SetTitle("Time Resolution[s]");
		graph[d]->Draw("AC*");
		graph[d]->Write();
		
		outfile_cut->cd();
		graph_cut[d]=new TGraphErrors(8, cfs, stdDev_cut[d],err_x,stdDev_cut_err[d]);
		graph_cut[d]->SetTitle(Form("Time Resolution vs Fixed Threshold, channel %d ",d+2));
		graph_cut[d]->GetXaxis()->SetTitle("Fixed Threshold[V]");
		graph_cut[d]->GetYaxis()->SetTitle("Time Resolution[s]");
		graph_cut[d]->Draw("AC*");
		graph_cut[d]->Write();
	
	}
	
	//infile[0].close();
	//infile[1].close();
	outfile->Close();
	outfile_cut->Close();	
		
}		
		
		

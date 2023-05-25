void MacroCFMPlot(double run, double cf1, double cf2, double cf3, double cf4, double cf5, double cf6, double cf7, double cf8, double cf9){

	double cfs[]={cf1, cf2, cf3, cf4 ,cf5 ,cf6 , cf7, cf8 ,cf9};
	
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
  
		infile[i].open(Form("FitParRun%gCfm%g.dat", run, cfs[i]));
		infile_cut[i].open(Form("FitParRun%gCfm%gCut.dat", run, cfs[i]));
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
     	
     	getline(infile_cut[i],fileline_cut);
		  istringstream zz(fileline);
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
	TFile *outfile=new TFile(Form("TimeResVSconstRun%g.root", run), "RECREATE");
	
	TGraph* graph_cut[12];
	TFile *outfile_cut=new TFile(Form("TimeResVSconstRun%g_cut.root", run), "RECREATE");
	
	for(int d=0; d<=11; d++){
		
		outfile->cd();
		graph[d]=new TGraphErrors(8, cfs, stdDev[d],err_x,stdDev_err[d]);
		graph[d]->SetTitle(Form("Time Resolution vs Constant for ch. %d",d+2));
		graph[d]->GetXaxis()->SetTitle("Constant");
		graph[d]->GetYaxis()->SetTitle("Time Resolution");
		graph[d]->Draw("AC*");
		graph[d]->Write();
		
		outfile_cut->cd();
		graph_cut[d]=new TGraphErrors(8, cfs, stdDev_cut[d],err_x,stdDev_cut_err[d]);
		graph_cut[d]->SetTitle(Form("Time Resolution vs Constant for ch. %d (of cut histograms)",d+2));
		graph_cut[d]->GetXaxis()->SetTitle("Constant");
		graph_cut[d]->GetYaxis()->SetTitle("Time Resolution");
		graph_cut[d]->Draw("AC*");
		graph_cut[d]->Write();
	
	}
	
	//infile[0].close();
	//infile[1].close();
	outfile->Close();
	outfile_cut->Close();	
		
		
		
		
		
		
		
		
		
		
		
		
		

		
	 /*	
	 getline(infile2,fileline);
	 getline(infile2,fileline);
	  while(!infile2.eof()) // reads file to end of file
      { 		
						getline(infile2,fileline);
      			istringstream ss(fileline);
       		 	ss >> ped_mean[ch];
       		 	ss >> ped_mean[ch];
       		 	ss >> ped_sigma[ch];
            ss >> mean2[ch];
            ss >> sigma2[ch];
            ch++;
      }
      getline(infile3,fileline);
      getline(infile3,fileline);
       while(!infile3.eof()) // reads file to end of file
      { 		
						getline(infile,fileline);
      			istringstream ss(fileline);
       		 	ss >> ped_mean[ch];
       		 	ss >> ped_mean[ch];
       		 	ss >> ped_sigma[ch];
            ss >> mean2[ch];
            ss >> sigma2[ch];
            ch++;
      }
      getline(infile4,fileline);
      getline(infile,fileline);
       while(!infile4.eof()) // reads file to end of file
      { 		
						getline(infile,fileline);
      			istringstream ss(fileline);
       		 	ss >> ped_mean[ch];
       		 	ss >> ped_mean[ch];
       		 	ss >> ped_sigma[ch];
            ss >> mean2[ch];
            ss >> sigma2[ch];
            ch++;
      }
      getline(infile5,fileline);
     	getline(infile5,fileline); 
       while(!infile5.eof()) // reads file to end of file
      { 		
						getline(infile,fileline);
      			istringstream ss(fileline);
       		 	ss >> ped_mean[ch];
       		 	ss >> ped_mean[ch];
       		 	ss >> ped_sigma[ch];
            ss >> mean2[ch];
           
      }
	
		*/
	
	
	
	
		
}

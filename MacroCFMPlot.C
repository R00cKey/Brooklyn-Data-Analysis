void MacroCFMPlot(double run, double cf1, double cf2, double cf3, double cf4, double cf5){

	double cfs[]={cf1, cf5};
	
	ifstream infile[2];
  string fileline; //appoggio
  
 
  double stdDev[12][2];
  int ch; 	 
   
  
  for(int i=0; i <=1; i++){
  
		infile[i].open(Form("FitParRun%gConst%g.dat", run, cfs[i]));
	
    if(infile[i].fail()){ // checks if file opended 
   		cout << "error=file of fit parameters not found" << endl; 
   	 } 
   	

		getline(infile[i],fileline);
		getline(infile[i],fileline);
		
		while(!infile[i].eof()){// reads file to end of file
			
			getline(infile[i],fileline);
		  istringstream ss(fileline);
		  ss >> ch;
		  ss >> stdDev[ch-2][i];
		  ss >> stdDev[ch-2][i];	
		  ss >> stdDev[ch-2][i];
     
     // cout << ch <<endl; 
      //if(ch==13)
      	//break;   
    }
   //	cout << i<<endl;
   	infile[i].close();
   	
	}
		
	TGraph* graph[12];
	TFile *outfile=new TFile(Form("TimeResVSconstRun%g", run), "RECREATE");
	for(int d=0; d<=11; d++){
		
		outfile->cd();
		graph[d]=new TGraph(2, cfs, stdDev[d]);
		graph[d]->SetTitle(Form("Time Resolution vs Constant for ch. %d",d));
		graph[d]->GetXaxis()->SetTitle("Constant");
		graph[d]->GetYaxis()->SetTitle("Time Resolution");
		graph[d]->Draw("AC*");
	
	}
	
	//infile[0].close();
	//infile[1].close();
	outfile->Close();
		
		
		
		
		
		
		
		
		
		
		
		
		
		

		
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

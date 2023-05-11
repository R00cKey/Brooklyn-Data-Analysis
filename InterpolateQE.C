double getinterp(double x1, double x2, double y1, double y2, double x){
	double m=(y2-y1)/(x2-x1);
	double q=y1-m*x1;

	return m*x+q;
}

int getindex(vector<double> x, double xinter){
	int k=0;
	while(xinter>=x[k]&&k<x.size()){
		k++;
	}
	return k-1;
}

double getIntegral(vector<double> x, vector<double> y){
	double integral=0.;
	for(int k=0; k<x.size()-1; k++){
		integral+=(y[k]+y[k+1])*(x[k+1]-x[k])*0.5;
	}
	return integral;
}


void InterpolateQE(){


	ofstream outfile;
	ifstream infile1;
	ifstream infile2;
	ifstream infile3;


  	string fileelement;
  	int i=0;
  	vector<double> x1;
  	vector<double> y1;
  	vector<double> x2;
  	vector<double> y2;
  	vector<double> x3;
  	vector<double> y3;
  	vector<double> steps1;
  	vector<double> steps2;
  	vector<double> spectrum1;
  	vector<double> spectrum2;

  	infile1.open("EmissionSpectraBCF12.dat");
  	while(!infile1.eof()){
  	infile1 >> fileelement;
  	if(i%2==0){x1.push_back(stod(fileelement));}
  	else{y1.push_back(stod(fileelement));}
  	i++;
  	}
  	x1.erase(x1.end()-1, x1.end());
	infile1.close();
	i=0;

  	infile2.open("EmissionSpectraSCSF7830cm.dat");
	while(!infile2.eof()){
  	infile2 >> fileelement;
  	if(i%2==0){x2.push_back(stod(fileelement));}
  	else{y2.push_back(stod(fileelement));}
  	i++;
  	}
  	x2.erase(x2.end()-1, x2.end());
 	infile2.close();
	i=0;

  	infile3.open("PMTQE.dat");
  	while(!infile3.eof()){
  	infile3 >> fileelement;
  	if(i%2==0){x3.push_back(stod(fileelement));}
  	else{y3.push_back(stod(fileelement));}
  	i++;
  	}
  	x3.erase(x3.end()-1, x3.end());
	infile3.close();
	i=0;
	double interstep=(x1.back()-x1.front())/100;
	for(int k=0; k<100; k++){
		int i=getindex(x1, x1.front()+interstep*k);
		int j=getindex(x3, x3.front()+interstep*k);
		steps1.push_back(interstep*k);
		spectrum1.push_back(getinterp(x1[i], x1[i+1], y1[i], y1[i+1], x1.front()+interstep*k)*getinterp(x3[j],x3[j+1],y3[j], y3[j+1], x3.front()+interstep*k)/getIntegral(x1, y1));
	
	}
	interstep=(x2.back()-x2.front())/100;
	for(int k=0; k<100; k++){
		int i=getindex(x2, x2.front()+interstep*k);
		int j=getindex(x3, x3.front()+interstep*k);
		steps2.push_back(interstep*k);
		spectrum2.push_back(getinterp(x2[i], x2[i+1], y2[i], y2[i+1], x2.front()+interstep*k)*getinterp(x3[j],x3[j+1],y3[j], y3[j+1], x3.front()+interstep*k)/getIntegral(x2, y2));
	}

 	outfile.open("QE.dat");
 	outfile << "CHANNEL\tQE\n";
 	for(int k=0; k<16; k++){
 	if(k==6||k==7||k==8){outfile << k << "\t" << getIntegral(steps2, spectrum2) <<"\n";}
 	else{outfile << k << "\t" << getIntegral(steps1, spectrum1) <<"\n";}
 	}
 	
}
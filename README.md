# Brooklyn-Data-Analysis

The software is build with the following header structure:

- Waveform.* : Class and methods dedicated to saving the waveform Raw Data .
- Event.* : Class and methods dedicated to collecting the waveform of the 16 channels. Contains the method to read
 the raw data file.
- AnaTools.* : Extracts data from waveforms.

Analyze.cpp is the main() file.

## Executing the program
First of all, we recommend changing your .bashrc file (hidden in \Home) to include the following lines:
```
alias root='root --web=off'

startroot(){
    export ROOTSYS=/'path preceding root directory'/root/
    export LD_LIBRARY_PATH=$ROOTSYS/lib
    . ROOTSYS/bin/thisroot.sh
    echo "Root sourced successfully"
}
```
While on bash, simply write `startroot` to source root and TBrowser will open the local app. Now you can compile the program with `make` 
inside the directory with all the headers.
To execute, write the following: 
```
./Analyze -in 'Input File directory and name' -out 'Output File name.root'
```

The progress of input file reading should be printed on bash.

## Macro
- **GainMacro.C**:
Fits **Noise Data** with two gaussians to extract the pedestal charge and single photoelectron charge. The files used are:
   - file: ROOT Noise file.
   - outfile: ROOT file to save the fitted data.
   - infofile: File to save fit parameters.
   - infofile2: File to save single photoelectron charge (aka: mean_2-mean_1) and its standard deviation.

- **QpeAPMacro.C**:
Works like GainMacro.C but considers the charge evaluated around the first peak. Only saves single photoelectron charge and its standard deviation. Used on **Noise files**

- **QaroundpeakMacro.C**:
Gets charge evaluated around the peak and saves it in the defined infofile. Used on **Muons files**.

- **QshiftedMacro.C**:
Gets charge evaluated around the peak. and seves it in the defined infofile. Used on **Muons files**.

- **EfficiencyMacro.C**:
Calculates the efficiency of the different channels by analyzing the whole waveform and prints it on shell. To change the files to be used, modify the following:
```
3    TFile *file=new TFile("Output File name.root", "READ");
6    infofile.open("GainMacro infofile.dat");

```
Used on **Muons files**.

- **EfficiencyMacroAP.C**:
Calculates the efficiency of the different channels by analyzing the waveform around the first peak and prints it on shell. To change the files to be used, modify the following:
```
3    TFile *file=new TFile("Output File name.root", "READ");
6    infofile.open(".dat");

```
Used on **Muons files**.

- **InterpolateQE.C**:
Computes the integral to get the Quantum Efficiency of the different channels. Uses Trapezoidal Rule. Two files are the emission spectra and one for the Response function of the PMT. 

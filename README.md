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
./Analyze -in 'Input File directory and name' -out 'Output File name'
```

The progress of input file reading should be printed on bash.

## Macros

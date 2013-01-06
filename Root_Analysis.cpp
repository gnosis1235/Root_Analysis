// Root_Analysis.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
//#include <stdio.h>
//#include <string>


//
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TNtupleD.h"
#include "TSystem.h"
//
#include "rootstuff.h"
#include "event_data.h"
#include "Root_file_handler.h"
#include "console.h"

using namespace std;



void ProcessRootFile(string name, rootstuff * rt){
	
		
	//while(current_entry_inputfile < 10){
	//	inputfileRootTree->GetEntry(current_entry_inputfile);
	//	printf("r1x=%f, r1y=%f, r1tof=%f, e1x=%f, e1y=%f, e1tof=%f", rx[0], ry[0], rtof[0], ex[0], ey[0], etof[0]);
	//	printf(", r2x=%f, r2y=%f, r2tof=%f, e2x=%f, e2y=%f, e2tof=%f\n", rx[1], ry[1], rtof[1], ex[1], ey[1], etof[1]);
	//	current_entry_inputfile++;
	//}
	return;
}

int main(__int32 argc, char* argv[], char* envp[])
{
	Red(true);
	printf("Don't panic! Everything will be fine.\n");
	White(false);

	Root_file_handler * input_root = new Root_file_handler();
	

	event_data * single_event ;
	for(int idx=0;idx<10;idx++){
		single_event = input_root->get_next_event();
		printf("reaction=%f\n",single_event->reaction);
	}

	//rootstuff * rt = new rootstuff();
	//string name = "C:/Users/Joshua/Documents/Visual Studio 2012/Projects/Root_Analysis/test.root";
	//ProcessRootFile(name, rt);

	return 0;
}
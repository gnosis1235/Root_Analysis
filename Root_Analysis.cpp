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

#include "console.h"

using namespace std;


void ProcessRootFile(string name, rootstuff * rt){
	


	static TTree * inputfileRootTree = 0;

	inputfileRootTree = rt->OpenRootFileGetTree("test.root","Data");
	__int64 current_entry_inputfile =0;

	
	__int64 Total_Events_inputfile = (__int64)(inputfileRootTree->GetEntries());

	printf("total entries in file:%d", Total_Events_inputfile);
	if(!inputfileRootTree){
		printf("\n could not find NTuple \"Data\" in this rootfile:\n%s\n",name);
		return;
	}


	double reaction=-1., ehit=-1., rhit=-1., phit=-1., bunchmarker=-1.;
	double rx[32],ry[32],rmcp[32],rtof[32],rflag[32];
	double ex[32],ey[32],emcp[32],etof[32],eflag[32];
	double px[32],py[32],pmcp[32],ptof[32],pflag[32];

	if(current_entry_inputfile == 0) {

		char ntuple_identifier[500];
		bool Check = 0;
		// set branch addresses...

		inputfileRootTree->SetBranchAddress("reaction",&reaction);
		//inputfileRootTree->SetBranchAddress("ehit",&ehit);
		//inputfileRootTree->SetBranchAddress("rhit",&rhit);
		//inputfileRootTree->SetBranchAddress("phit",&phit);
		//inputfileRootTree->SetBranchAddress("bunchmarker",&bunchmarker);

		// find out how many recoils are in Ntuple
		int MaxRec = 0;
		do {
			sprintf(ntuple_identifier,"r%ix",++MaxRec);
		} while(inputfileRootTree->GetBranch(ntuple_identifier));

		for(int i=0;i<MaxRec-1;i++) {
			sprintf(ntuple_identifier,"r%ix",i+1);
			inputfileRootTree->SetBranchAddress(ntuple_identifier,&rx[i]);
			sprintf(ntuple_identifier,"r%iy",i+1);
			inputfileRootTree->SetBranchAddress(ntuple_identifier,&ry[i]);
			//sprintf(ntuple_identifier,"r%imcp",i+1);
			//inputfileRootTree->SetBranchAddress(ntuple_identifier,&rmcp[i]);
			sprintf(ntuple_identifier,"r%itof",i+1);
			inputfileRootTree->SetBranchAddress(ntuple_identifier,&rtof[i]);
			//sprintf(ntuple_identifier,"r%iflag",i+1);
			//inputfileRootTree->SetBranchAddress(ntuple_identifier,&rflag[i]);		
		}

		// find out how many electrons are in Ntuple
		int MaxElec = 0;
		do {
			sprintf(ntuple_identifier,"e%ix",++MaxElec);
		} while(inputfileRootTree->GetBranch(ntuple_identifier));

		for(int i=0;i<MaxElec-1;i++) {
			sprintf(ntuple_identifier,"e%ix",i+1);
			inputfileRootTree->SetBranchAddress(ntuple_identifier,&ex[i]);
			sprintf(ntuple_identifier,"e%iy",i+1);
			inputfileRootTree->SetBranchAddress(ntuple_identifier,&ey[i]);
			//sprintf(ntuple_identifier,"e%imcp",i+1);
			//inputfileRootTree->SetBranchAddress(ntuple_identifier,&emcp[i]);
			sprintf(ntuple_identifier,"e%itof",i+1);
			inputfileRootTree->SetBranchAddress(ntuple_identifier,&etof[i]);
			//sprintf(ntuple_identifier,"e%iflag",i+1);
			//inputfileRootTree->SetBranchAddress(ntuple_identifier,&eflag[i]);		
		}

		// find out how many projectyles are in Ntuple
		int MaxProj = 0;
		do {
			sprintf(ntuple_identifier,"p%ix",++MaxProj);
		} while(inputfileRootTree->GetBranch(ntuple_identifier));
		
		for(int i=0;i<MaxProj-1;i++) {
			sprintf(ntuple_identifier,"p%iy",i+1);
			inputfileRootTree->SetBranchAddress(ntuple_identifier,&py[i]);
			sprintf(ntuple_identifier,"p%imcp",i+1);
			inputfileRootTree->SetBranchAddress(ntuple_identifier,&pmcp[i]);
			sprintf(ntuple_identifier,"p%itof",i+1);
			inputfileRootTree->SetBranchAddress(ntuple_identifier,&ptof[i]);
			sprintf(ntuple_identifier,"p%iflag",i+1);
			inputfileRootTree->SetBranchAddress(ntuple_identifier,&pflag[i]);		
		}
	}

	
	while(current_entry_inputfile < 10){
		inputfileRootTree->GetEntry(current_entry_inputfile);
		printf("r1x=%f, r1y=%f, r1tof=%f, e1x=%f, e1y=%f, e1tof=%f", rx[0], ry[0], rtof[0], ex[0], ey[0], etof[0]);
		printf(", r2x=%f, r2y=%f, r2tof=%f, e2x=%f, e2y=%f, e2tof=%f\n", rx[1], ry[1], rtof[1], ex[1], ey[1], etof[1]);
		current_entry_inputfile++;
	}
	return;
}

int main(__int32 argc, char* argv[], char* envp[])
{
	Red(true);
	printf("Don't panic! Everything will be fine.\n");
	White(false);

	//---------------------------------------------------------
	// don't know why these are need, but they are required to get the Ttree without errors.  I think something is wrong with the project settings
	gSystem->Load("libTree");
	TH1F* dummy = new TH1F("dummy", "dummy", 10, 0, 1);
	//---------------------------------------------------------


	rootstuff * rt = new rootstuff();
	string name = "C:/Users/Joshua/Documents/Visual Studio 2012/Projects/Root_Analysis/test.root";
	ProcessRootFile(name, rt);

	return 0;
}
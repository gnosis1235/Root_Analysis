

#include "stdlib.h"

#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TNtupleD.h"
#include "TSystem.h"

//#include "TBrowser.h"
//#include "TNtuple.h"

#include "Root_file_handler.h"
#include "rootstuff.h"
#include "event_data.h"

Root_file_handler::Root_file_handler(void)//std::string filename)
{
	
	inputfileRootTree = 0;
	//---------------------------------------------------------
	// don't know why these are need, but they are required to get the Ttree without errors.  I think something is wrong with the project settings
	gSystem->Load("libTree");
	TH1F* dummy = new TH1F("dummy", "dummy", 10, 0, 1);
	//---------------------------------------------------------
	rt =  new rootstuff();
	inputfileRootTree = rt->OpenRootFileGetTree("test.root","Data");
	current_entry_inputfile =0;
	Total_Events_inputfile = (__int64)(inputfileRootTree->GetEntries());

	//printf("total entries in file:%d", Total_Events_inputfile);
	//if(!inputfileRootTree){
	//	printf("\n could not find NTuple \"Data\" in this rootfile:\n%s\n",filename.c_str());
	//	return;
	//}

	
}


Root_file_handler::~Root_file_handler(void)
{
}


event_data * Root_file_handler::get_next_event(){
	event_data * single_event;
	single_event = new event_data();

		char ntuple_identifier[500];
		bool Check = 0;
		// set branch addresses...

		
		inputfileRootTree->SetBranchAddress("reaction",&single_event->reaction);
		//inputfileRootTree->SetBranchAddress("ehit",&ehit);
		//inputfileRootTree->SetBranchAddress("rhit",&rhit);
		//inputfileRootTree->SetBranchAddress("phit",&phit);
		//inputfileRootTree->SetBranchAddress("bunchmarker",&bunchmarker);

		// find out how many recoils are in Ntuple
		//int MaxRec = 0;
		//do {
		//	sprintf(ntuple_identifier,"r%ix",++MaxRec);
		//} while(inputfileRootTree->GetBranch(ntuple_identifier));

		//for(int i=0;i<MaxRec-1;i++) {
		//	sprintf(ntuple_identifier,"r%ix",i+1);
		//	inputfileRootTree->SetBranchAddress(ntuple_identifier,&rx[i]);
		//	sprintf(ntuple_identifier,"r%iy",i+1);
		//	inputfileRootTree->SetBranchAddress(ntuple_identifier,&ry[i]);
		//	//sprintf(ntuple_identifier,"r%imcp",i+1);
		//	//inputfileRootTree->SetBranchAddress(ntuple_identifier,&rmcp[i]);
		//	sprintf(ntuple_identifier,"r%itof",i+1);
		//	inputfileRootTree->SetBranchAddress(ntuple_identifier,&rtof[i]);
		//	//sprintf(ntuple_identifier,"r%iflag",i+1);
		//	//inputfileRootTree->SetBranchAddress(ntuple_identifier,&rflag[i]);		
		//}

		//// find out how many electrons are in Ntuple
		//int MaxElec = 0;
		//do {
		//	sprintf(ntuple_identifier,"e%ix",++MaxElec);
		//} while(inputfileRootTree->GetBranch(ntuple_identifier));

		//for(int i=0;i<MaxElec-1;i++) {
		//	sprintf(ntuple_identifier,"e%ix",i+1);
		//	inputfileRootTree->SetBranchAddress(ntuple_identifier,&ex[i]);
		//	sprintf(ntuple_identifier,"e%iy",i+1);
		//	inputfileRootTree->SetBranchAddress(ntuple_identifier,&ey[i]);
		//	//sprintf(ntuple_identifier,"e%imcp",i+1);
		//	//inputfileRootTree->SetBranchAddress(ntuple_identifier,&emcp[i]);
		//	sprintf(ntuple_identifier,"e%itof",i+1);
		//	inputfileRootTree->SetBranchAddress(ntuple_identifier,&etof[i]);
		//	//sprintf(ntuple_identifier,"e%iflag",i+1);
		//	//inputfileRootTree->SetBranchAddress(ntuple_identifier,&eflag[i]);		
		//}

		//// find out how many projectyles are in Ntuple
		//int MaxProj = 0;
		//do {
		//	sprintf(ntuple_identifier,"p%ix",++MaxProj);
		//} while(inputfileRootTree->GetBranch(ntuple_identifier));
		//
		//for(int i=0;i<MaxProj-1;i++) {
		//	sprintf(ntuple_identifier,"p%iy",i+1);
		//	inputfileRootTree->SetBranchAddress(ntuple_identifier,&py[i]);
		//	sprintf(ntuple_identifier,"p%imcp",i+1);
		//	inputfileRootTree->SetBranchAddress(ntuple_identifier,&pmcp[i]);
		//	sprintf(ntuple_identifier,"p%itof",i+1);
		//	inputfileRootTree->SetBranchAddress(ntuple_identifier,&ptof[i]);
		//	sprintf(ntuple_identifier,"p%iflag",i+1);
		//	inputfileRootTree->SetBranchAddress(ntuple_identifier,&pflag[i]);		
		//}

	inputfileRootTree->GetEntry(current_entry_inputfile);
	current_entry_inputfile++;

	return single_event;
}
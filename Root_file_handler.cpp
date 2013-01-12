

#include "stdlib.h"

#include "TText.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TNtupleD.h"
#include "TNtuple.h"
#include "TSystem.h"
#include "TApplication.h"
#include <TDirectory.h>
//#include "TBrowser.h"
//#include "TNtuple.h"

#include <sys/stat.h> 
#include <io.h>
#include <iostream>

#include "Root_file_handler.h"
//#include "rootstuff.h"
#include "event_data.h"
#include "Simple_Hist.h"


	#ifdef _DEBUG
		#include "assert.h"
	#endif

#include <thread>
#include <mutex>

using namespace std;

void set_branches(event_data * single_event, TTree * inputfileRootTree);


Root_file_handler::Root_file_handler(void)
{
	std::lock_guard<std::mutex> guard(mutex); //auto lock thread

	inputfileRootTree = 0;
	current_entry_inputfile =0;
	Total_Events_inputfile =0;
	printf("Error: No file name.\n");
	return;
	
}
Root_file_handler::Root_file_handler(std::string filename, std::string Option_read_write)
{
	std::lock_guard<std::mutex> guard(mutex); //auto lock thread

	rootfilename = filename;

	inputfileRootTree = 0;
	current_entry_inputfile = 0;
	MyTNtuple = 0;
	RootFile =0;
	TNtupleD_started=false;
	reading = false;
	writing = false;
	stop_reading = false;

	//---------------------------------------------------------
	// Don't know why these are need, but they are required to get the Ttree without errors.  I think something is wrong with the project settings.  
	// I think this forces the linker to include the libs.
	//gSystem->Load("libTree");
	TH1F * dummy = new TH1F("dummy", "dummy", 10, 0, 1);
	TTree * dummy2 = new TTree();
	TText * dummy3 = new TText();
	//---------------------------------------------------------

	//open file to be read
	if (strcmp(Option_read_write.c_str(),"read")==0){

		reading = true;
		printf("\n\nInput file is: %s\n",rootfilename.c_str());
		if(!FileExists(rootfilename.c_str())) {
				printf("\n \n %s does not be exist!\n\n",rootfilename.c_str());
				return;
		}
		
		inputfileRootTree = OpenRootFileGetTree("Data");
		if(!inputfileRootTree){
			printf("\n could not find NTuple \"Data\" in this rootfile:\n%s\n",rootfilename.c_str());
			return;
		}
		else{
			single_event = new event_data();
			set_branches(single_event, inputfileRootTree);
		}

		current_entry_inputfile =0;
		Total_Events_inputfile = (__int64)(inputfileRootTree->GetEntries());
	
	}


	//open file to write 
	if (strcmp(Option_read_write.c_str(),"write")==0){

		writing = true;
		printf("Output file is: %s\n",rootfilename.c_str());
		if(FileExists(rootfilename.c_str())) {
				printf("Output file exist: File will be overwritten\n");
		}
		
		//tuple_size = 100;
		//tuple_array = new TObject*[tuple_size];
		//for (__int32 i =0;i<tuple_size;++i){
		//	tuple_array[i] = 0;
		//}
		RootFile =  new TFile(filename.c_str(),"RECREATE");

		//RootFile->SetWritable(kTRUE);

		RootFile->cd("/");
		//Histograms_dir = RootFile->mkdir("Histograms"); 
		//RootFile->cd("/");
		Data_dir = RootFile->mkdir("Ntuple_data"); 
		RootFile->cd("/");
	}
	return;
}


//void Root_file_handler::set_branches(){
//			//single_event = new event_data();
//			char ntuple_identifier[500];
//			bool Check = 0;
//
//			inputfileRootTree->SetBranchAddress("reaction",&single_event->reaction);
//			inputfileRootTree->SetBranchAddress("ehit",&single_event->ehit);
//			inputfileRootTree->SetBranchAddress("rhit",&single_event->rhit);
//			inputfileRootTree->SetBranchAddress("phit",&single_event->phit);
//			inputfileRootTree->SetBranchAddress("bunchmarker",&single_event->bunchmarker);
//			
//			// find out how many recoils are in Ntuple
//			int MaxRec = 0;
//			do {
//				sprintf(ntuple_identifier,"r%ix",++MaxRec);
//			} while(inputfileRootTree->GetBranch(ntuple_identifier));
//
//			for(int i=0;i<MaxRec-1;i++) {
//				sprintf(ntuple_identifier,"r%ix",i+1);
//				inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->rx[i]);
//				sprintf(ntuple_identifier,"r%iy",i+1);
//				inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->ry[i]);
//				sprintf(ntuple_identifier,"r%imcp",i+1);
//				inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->rmcp[i]);
//				sprintf(ntuple_identifier,"r%itof",i+1);
//				inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->rtof[i]);
//				sprintf(ntuple_identifier,"r%iflag",i+1);
//				inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->rflag[i]);		
//			}
//
//			// find out how many electrons are in Ntuple
//			int MaxElec = 0;
//			do {
//				sprintf(ntuple_identifier,"e%ix",++MaxElec);
//			} while(inputfileRootTree->GetBranch(ntuple_identifier));
//
//			for(int i=0;i<MaxElec-1;i++) {
//				sprintf(ntuple_identifier,"e%ix",i+1);
//				inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->ex[i]);
//				sprintf(ntuple_identifier,"e%iy",i+1);
//				inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->ey[i]);
//				sprintf(ntuple_identifier,"e%imcp",i+1);
//				inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->emcp[i]);
//				sprintf(ntuple_identifier,"e%itof",i+1);
//				inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->etof[i]);
//				sprintf(ntuple_identifier,"e%iflag",i+1);
//				inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->eflag[i]);		
//			}
//
//			// find out how many projectyles are in Ntuple
//			int MaxProj = 0;
//			do {
//				sprintf(ntuple_identifier,"p%ix",++MaxProj);
//			} while(inputfileRootTree->GetBranch(ntuple_identifier));
//		
//			for(int i=0;i<MaxProj-1;i++) {
//				sprintf(ntuple_identifier,"p%iy",i+1);
//				inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->py[i]);
//				sprintf(ntuple_identifier,"p%imcp",i+1);
//				inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->pmcp[i]);
//				sprintf(ntuple_identifier,"p%itof",i+1);
//				inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->ptof[i]);
//				sprintf(ntuple_identifier,"p%iflag",i+1);
//				inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->pflag[i]);		
//			}
//}

Root_file_handler::~Root_file_handler(void)
{
	std::lock_guard<std::mutex> guard(mutex); //auto lock thread
}


TTree * Root_file_handler::OpenRootFileGetTree(const char *TreeName)
{
	//should only be called my the consturcter
	RootFile = new TFile(rootfilename.c_str(),"READ");

	if (RootFile->IsZombie()) {
		return 0;
	}
	
    TTree * tree = (TTree*)RootFile->Get(TreeName);
	return tree;
}

TDirectory* Root_file_handler::getDir(TFile *rootfile, TString dirName)
{
	//first find out whether directory exists
#ifdef _DEBUG
	assert(rootfile);
#endif
	//printf("here1: %d \n",rootfile);
	if (!rootfile) return 0;
	rootfile->cd("/"); 
	TDirectory * direc = rootfile->GetDirectory(dirName.Data()); 
	if (!direc)
	{
		//direc->SetWritable(true);
		//if not create it//
		TString lhs;
		TString rhs;
		TString tmp = dirName;
		while (1)
		{
			//if there is no / then this is the last subdir
			if (tmp.Index("/") == -1)
			{
				lhs = tmp;
			}
			else //otherwise split the string to lefthandside and righthandside of "/"
			{
				lhs = tmp(0,tmp.Index("/"));
				rhs = tmp(tmp.Index("/")+1,tmp.Length());
			}

			//check wether subdir exits//
			direc = gDirectory->GetDirectory(lhs.Data());
			if (direc){
				gDirectory->cd(lhs.Data());//cd into it
			}
			else
			{
				direc = gDirectory->mkdir(lhs.Data()); //create it
				gDirectory->cd(lhs.Data()); //and cd into it
			}

			//when there is no "/" anymore break here//
			if (tmp.Index("/") == -1)
				break;

			//the new temp is all that is on the right hand side
			tmp = rhs;
		}
	}
	//return to root Path//
	rootfile->cd("/");
	return direc;
}

event_data * Root_file_handler::get_next_event(){
	std::lock_guard<std::mutex> guard(mutex); //auto lock thread
	
	if(!reading) { //don't try to read the file if is for write
		printf("Error:File %s does not appear to be an input file. You cannot call get_next_event on and output file.\n", rootfilename.c_str());
		return single_event;
	}
	if(current_entry_inputfile <= Total_Events_inputfile){
		//read new row from the NTuple
		inputfileRootTree->GetEntry(current_entry_inputfile);
		current_entry_inputfile++;

		event_data * event = new event_data();
		//copy data to a disposible event_data (will be deleted when analysis is finsihed)
		//should change this a memcpy
		(*event).bunchmarker = (*single_event).bunchmarker;
		(*event).rhit		= (*single_event).rhit;
		(*event).ehit		= (*single_event).ehit;
		(*event).phit		= (*single_event).phit;
		(*event).reaction	= (*single_event).reaction ;
	
		for(int i=0;i<32; ++i)	(*event).rflag[i]		= (*single_event).rflag[i];
		for(int i=0;i<32; ++i)	(*event).rx[i]			= (*single_event).rx[i]	;
		for(int i=0;i<32; ++i)	(*event).ry[i]			= (*single_event).ry[i]	;
		for(int i=0;i<32; ++i)	(*event).rmcp[i]		= (*single_event).rmcp[i];
		for(int i=0;i<32; ++i)	(*event).rtof[i]		= (*single_event).rtof[i];
								  						    
		for(int i=0;i<32; ++i)	(*event).eflag[i]		= (*single_event).eflag[i];
		for(int i=0;i<32; ++i)	(*event).ex[i]			= (*single_event).ex[i]	;
		for(int i=0;i<32; ++i)	(*event).ey[i]			= (*single_event).ey[i]	;
		for(int i=0;i<32; ++i)	(*event).emcp[i]		= (*single_event).emcp[i];
		for(int i=0;i<32; ++i)	(*event).etof[i]		= (*single_event).etof[i];
								  						    
		for(int i=0;i<32; ++i)	(*event).pflag[i]		= (*single_event).pflag[i];
		for(int i=0;i<32; ++i)	(*event).px[i]			= (*single_event).px[i]	;
		for(int i=0;i<32; ++i)	(*event).py[i]			= (*single_event).py[i]	;
		for(int i=0;i<32; ++i)	(*event).pmcp[i]		= (*single_event).pmcp[i];
		for(int i=0;i<32; ++i)	(*event).ptof[i]		= (*single_event).ptof[i];
	
		return event;
	}else{
		stop_reading=true;
		return single_event;
	}
}








void Root_file_handler::NTupleD( const char *name, const char * title, const char *varlist, __int32 buffersize, double *data, const char * dir)
{
	std::lock_guard<std::mutex> guard(mutex); //auto lock thread

	Data_dir->cd();

	if(!TNtupleD_started){

		MyTNtuple = new TNtupleD(name,title,varlist,buffersize);
		
		//MyTNtuple->SetDirectory(getDir(RootFile,"Data")); //put it in the wanted directory
		//printf("here");
		TNtupleD_started = true;
	}
	//--check also if name is the same one--//
	if (strcmp(MyTNtuple->GetName(),name))
	{
		printf("name doesn't match: %s and should be:%s \n", (MyTNtuple->GetName()), name);
	}

	//--now fill it--//
	if (MyTNtuple) MyTNtuple->Fill(data);
	++eventswritten;
}

void Root_file_handler::EventsWrittenCounter() {
	std::lock_guard<std::mutex> guard(mutex); //auto lock thread
	eventswritten++;
}


void Root_file_handler::write_TNtupleD(){
	Data_dir->cd();

	if (MyTNtuple) {
		MyTNtuple->Write();
	}
	else{
		printf("MyTNtuple does not appear to exsits \n");
	}
}


void Root_file_handler::close_file(){
	if (RootFile) {
		RootFile->Close();
		if(reading)		printf("Closing input root file: %s ...\n", rootfilename.c_str());
		if(writing)		printf("Closing output root file: %s ...\n", rootfilename.c_str());
	}
	else{
		printf("RootFile does not appear to exsits");
	}
}





void Root_file_handler::add_hist(H1i * hist){
	//change/make directory 
	Histograms_dir = getDir(RootFile, hist->get_dir() );
	Histograms_dir->cd();

	// create root hist
	TH1I * root_hist =new TH1I(hist->get_name().c_str(), hist->get_title().c_str(), hist->get_X_n_bins(), hist->get_X_min(), hist->get_X_max() );
	// copy contents
	root_hist->SetBinContent( 0 , hist->get_X_underflow() );
	for(int i=0; i < hist->get_X_n_bins(); ++i){
		root_hist->SetBinContent( i+1, hist->bins[i]);
	}
	root_hist->SetBinContent( hist->get_X_n_bins()+1 , hist->get_X_overflow() );
	//set axis title
	root_hist->SetXTitle( hist->get_X_title().c_str() );
	

	//write to root file
	root_hist->Write(hist->get_name().c_str() );
}



void Root_file_handler::add_hist(H2i * hist){
	//change/make directory 
	Histograms_dir = getDir(RootFile, hist->get_dir() );
	Histograms_dir->cd();

	// create root hist
	TH2I * root_hist =new TH2I(hist->get_name().c_str(), hist->get_title().c_str(), hist->get_X_n_bins(), hist->get_X_min(), hist->get_X_max(), hist->get_Y_n_bins(), hist->get_Y_min(), hist->get_Y_max() );
	// copy contents
	root_hist->SetBinContent( 0, 1, hist->get_X_underflow() );
	root_hist->SetBinContent( 1, 0, hist->get_Y_underflow() );
	for(int i=0; i < hist->get_X_n_bins(); ++i){
		for(int j=0; j < hist->get_Y_n_bins(); ++j){
			root_hist->SetBinContent( i+1, j+1, hist->bins[i][j]);
		}
	}
	root_hist->SetBinContent( hist->get_X_n_bins()+1, 1 , hist->get_X_overflow() );
	root_hist->SetBinContent( 1, hist->get_Y_n_bins()+1 , hist->get_Y_overflow() );
	//set axis title
	root_hist->SetXTitle( hist->get_X_title().c_str() );
	root_hist->SetYTitle( hist->get_Y_title().c_str() );

	root_hist->SetOption("colz");
	//write to root file
	root_hist->Write(hist->get_name().c_str() );
}



/////////////////////////////////////////////////////////////////////////////
bool Root_file_handler::FileExists(const char * strFilename) {
/////////////////////////////////////////////////////////////////////////////
	#ifdef LINUX

		if ( access(strFilename,F_OK) )
			return(false);
		else if ( access(strFilename,R_OK) ) {
			return(false);
		} else	return(true);

	#else

		FILE * fi;
		fi = fopen(strFilename,"rb");
		if (!fi) return false;
		if (ferror(fi)) {
			clearerr(fi); fclose(fi);	fi = 0;
			return false;
		}
		char dummy_char;
		fread(&dummy_char,1,1,fi);
		if (ferror(fi)) {
			clearerr(fi); fclose(fi);	fi = 0;			
			return false;
		}
		if (fi) {
			fclose(fi);	fi = 0;
		}
		return true;
	#endif
}

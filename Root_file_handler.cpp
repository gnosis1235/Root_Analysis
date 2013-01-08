

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

#include "Root_file_handler.h"
//#include "rootstuff.h"
#include "event_data.h"

#include <thread>
#include <mutex>

using namespace std;

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

	reading = false;
	writing = false;

	//---------------------------------------------------------
	// Don't know why these are need, but they are required to get the Ttree without errors.  I think something is wrong with the project settings.  
	// I think this forces the linker to include the libs.
	//gSystem->Load("libTree");
	TH1F * dummy = new TH1F("dummy", "dummy", 10, 0, 1);
	TTree * dummy2 = new TTree();
	TText * dummy3 = new TText();
	//---------------------------------------------------------

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

		current_entry_inputfile =0;
		Total_Events_inputfile = (__int64)(inputfileRootTree->GetEntries());
	
	}
	if (strcmp(Option_read_write.c_str(),"write")==0){

		writing = true;
		printf("Output file is: %s\n\n",rootfilename.c_str());
		if(FileExists(rootfilename.c_str())) {
				printf("File exist: File will be overwritten");
				return;
		}
		
		tuple_size = 100;
		tuple_array = new TObject*[tuple_size];
		for (__int32 i =0;i<tuple_size;++i){
			tuple_array[i] = 0;
		}
		RootFile =  new TFile(filename.c_str(),"RECREATE","");
		
	}
	return;
}


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

	if (!rootfile) return 0;
	rootfile->cd("/");
	TDirectory * direc = rootfile->GetDirectory(dirName.Data());
	if (!direc)
	{
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
			if (direc)
				gDirectory->cd(lhs.Data());//cd into it
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
	
	event_data * single_event;
	single_event = new event_data();
	
	
	if(!reading) { //don't try to read the file if is for write
		printf("Error:File %s does not appear to be an input file. You cannot call get_next_event on and output file.\n", rootfilename.c_str());
		return single_event;
	}
	


	char ntuple_identifier[500];
	bool Check = 0;
	// set branch addresses...

			
	inputfileRootTree->SetBranchAddress("reaction",&single_event->reaction);
	inputfileRootTree->SetBranchAddress("ehit",&single_event->ehit);
	inputfileRootTree->SetBranchAddress("rhit",&single_event->rhit);
	inputfileRootTree->SetBranchAddress("phit",&single_event->phit);
	inputfileRootTree->SetBranchAddress("bunchmarker",&single_event->bunchmarker);
			
	// find out how many recoils are in Ntuple
	int MaxRec = 0;
	do {
		sprintf(ntuple_identifier,"r%ix",++MaxRec);
	} while(inputfileRootTree->GetBranch(ntuple_identifier));

	for(int i=0;i<MaxRec-1;i++) {
		sprintf(ntuple_identifier,"r%ix",i+1);
		inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->rx[i]);
		sprintf(ntuple_identifier,"r%iy",i+1);
		inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->ry[i]);
		sprintf(ntuple_identifier,"r%imcp",i+1);
		inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->rmcp[i]);
		sprintf(ntuple_identifier,"r%itof",i+1);
		inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->rtof[i]);
		sprintf(ntuple_identifier,"r%iflag",i+1);
		inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->rflag[i]);		
	}

	// find out how many electrons are in Ntuple
	int MaxElec = 0;
	do {
		sprintf(ntuple_identifier,"e%ix",++MaxElec);
	} while(inputfileRootTree->GetBranch(ntuple_identifier));

	for(int i=0;i<MaxElec-1;i++) {
		sprintf(ntuple_identifier,"e%ix",i+1);
		inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->ex[i]);
		sprintf(ntuple_identifier,"e%iy",i+1);
		inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->ey[i]);
		sprintf(ntuple_identifier,"e%imcp",i+1);
		inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->emcp[i]);
		sprintf(ntuple_identifier,"e%itof",i+1);
		inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->etof[i]);
		sprintf(ntuple_identifier,"e%iflag",i+1);
		inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->eflag[i]);		
	}

	// find out how many projectyles are in Ntuple
	int MaxProj = 0;
	do {
		sprintf(ntuple_identifier,"p%ix",++MaxProj);
	} while(inputfileRootTree->GetBranch(ntuple_identifier));
		
	for(int i=0;i<MaxProj-1;i++) {
		sprintf(ntuple_identifier,"p%iy",i+1);
		inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->py[i]);
		sprintf(ntuple_identifier,"p%imcp",i+1);
		inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->pmcp[i]);
		sprintf(ntuple_identifier,"p%itof",i+1);
		inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->ptof[i]);
		sprintf(ntuple_identifier,"p%iflag",i+1);
		inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->pflag[i]);		
	}

	inputfileRootTree->GetEntry(current_entry_inputfile);
	current_entry_inputfile++;

	return single_event;
}


TNtuple * Root_file_handler::newNTuple(char *name, char * title, char *varlist, __int32 buffersize)
{
   TNtuple * nTuple = new TNtuple(name,title,varlist,buffersize);
   return nTuple;
}




void Root_file_handler::NTupleD(__int32 id, const char *name, const char * title, const char *varlist, __int32 buffersize, double *data, const char * dir)
{
	std::lock_guard<std::mutex> guard(mutex); //auto lock thread

	MyNTuple = 0;
	if (id <0 || id >= tuple_size) {
		printf("\nError: NTuple-Id is smaller than 0 or greater than %i\n",tuple_size-1);
		return;
	}
	//printf("here");
	MyNTuple = (TNtupleD *) tuple_array[id];

	//--if the histo does not exist create it first--//
	if (!MyNTuple)
	{
		MyNTuple = new TNtupleD(name,title,varlist,buffersize);
		MyNTuple->SetDirectory(getDir(RootFile,dir)); //put it in the wanted directory

		//--now add it to the list--//
		tuple_array[id]= (TObject *) MyNTuple;
		//--write it into root file--//
	}
	//--check also if name is the same one--//
	else if (strcmp(MyNTuple->GetName(),name))
	{
		printf("name doesn't match(%d) is: %s and should be:%s \n",id, (MyNTuple->GetName()), name);
	}

	//--now fill it--//
	if (MyNTuple) MyNTuple->Fill(data);
}

void Root_file_handler::EventsWrittenCounter() {
	std::lock_guard<std::mutex> guard(mutex); //auto lock thread
	eventswritten++;
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

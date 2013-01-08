#pragma once
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

#include <sys/stat.h> //file io

#include "Root_file_handler.h"
//#include "rootstuff.h"
#include "event_data.h"

#include <thread>
#include <mutex>

using namespace std;


class TCanvas;
class TFile;
//class TMinuit;
class TH1D;
class TH2D;
class TNtuple;

class TTree;

class rootstuff;
struct event_data;



class Root_file_handler
{
	std::mutex mutex;
	string rootfilename;
	TFile * RootFile;
	//rootstuff * rt;
	TTree * inputfileRootTree;
	__int64 Total_Events_inputfile ;
	__int64 current_entry_inputfile;
	__int64 eventswritten;

	bool reading;
	bool writing;

	TNtupleD * MyTNtuple;
	bool TNtupleD_started;
	__int32 tuple_size;

	TDirectory* getDir(TFile *rootfile, TString dirName);
	TTree * OpenRootFileGetTree(const char *TreeName);
	bool FileExists(const char * strFilename);

public:
	Root_file_handler(void);//
	Root_file_handler(std::string filename, std::string Option_read_write);
	~Root_file_handler(void);

    event_data *	get_next_event();
	void NTupleD(const char *name, const char * title, const char *varlist, __int32 buffersize, double *data, const char *  dir = "Data");
	void EventsWrittenCounter();
	void write_TNtupleD();
	void close_file();
};


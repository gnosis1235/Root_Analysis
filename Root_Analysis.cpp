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
//#include "rootstuff.h"
#include "event_data.h"
#include "Root_file_handler.h"
#include "console.h"

#include <thread>
#include <mutex>
#include <math.h>


 #include <sstream> //for string_to_double

using namespace std;


 double string_to_double( const std::string& s )
 {
   std::istringstream i(s);
   double x;
   if (!(i >> x))
     return 0;
   return x;
 }


void analysis(Root_file_handler * input_root_file, Root_file_handler * output_root_file, int thread_id){
	
	event_data * single_event ;
	double NTupleData[5];
	bool WriteNTuple = false;

	for(int idx=0;idx<10000;idx++){
		single_event = input_root_file->get_next_event();
		
		//std::chrono::milliseconds sleepDuration(50);
		//std::this_thread::sleep_for(sleepDuration);
		//cout<<std::this_thread::get_id()<<endl;

		//printf("\nreaction=%f, idx=%d, slow=%f\n",single_event->reaction, idx, pow((double)idx,100));
		//printf("r1x=%f, r1y=%f, r1tof=%f,\n e1x=%f, e1y=%f, e1tof=%f\n", single_event->rx[0], single_event->ry[0], single_event->rtof[0], single_event->ex[0], single_event->ey[0], single_event->etof[0]);
		//printf("p1x=%f, p1y=%f, p1tof=%f,\n", single_event->px[0], single_event->py[0], single_event->ptof[0]);
		//printf("r2x=%f, r2y=%f, r2tof=%f,\n e2x=%f, e2y=%f, e2tof=%f\n", single_event->rx[1], single_event->ry[1], single_event->rtof[1], single_event->ex[1], single_event->ey[1], single_event->etof[1]);
		//printf("p2x=%f, p2y=%f, p2tof=%f,\n", single_event->px[1], single_event->py[1], single_event->ptof[1]);
		//
		//single_event->reaction = pow((double)idx,1000);
		
		NTupleData[0]=idx;
		NTupleData[1]=thread_id;

		WriteNTuple = true;
		if(WriteNTuple) {
			output_root_file->NTupleD("Data","test_file","idx:threadID",32000,NTupleData);
			output_root_file->EventsWrittenCounter();
		}
		
	}	

}

void ProcessRootFile(string inputfilename, string outputfilename){
	
	Root_file_handler * input_root_file = new Root_file_handler(inputfilename, "read");
	Root_file_handler * output_root_file = new Root_file_handler(outputfilename, "write");
	//event_data * single_event ;



	std::thread t1(analysis, input_root_file, output_root_file, 1.);
	std::thread t2(analysis, input_root_file, output_root_file, 2.);
	//std::thread t3(analysis, input_root_file);
	//std::thread t4(analysis, input_root_file);
	//std::thread t5(analysis, input_root_file);
	//std::thread t6(analysis, input_root_file);
	//std::thread t7(analysis, input_root_file);
	//std::thread t8(analysis, input_root_file);
	
	//analysis( input_root_file, output_root_file, 1.);

	t1.join();
	t2.join();
	//t3.join();
	//t4.join();
	//t5.join();
	//t6.join();
	//t7.join();
	//t8.join();
	
	

	input_root_file->close_file();
	output_root_file->write_TNtupleD();
	output_root_file->close_file();
	
	return;
}



int main(__int32 argc, char* argv[], char* envp[])
{
	Red(true);
	printf("Don't panic! Everything will be fine.\n");
	White(false);


	string inputfilename = "test.root";
	string outputfilename = "output_test.root";
	ProcessRootFile(inputfilename, outputfilename);

	return 0;
}
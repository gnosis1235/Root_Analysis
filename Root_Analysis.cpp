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
#include "Simple_Hist.h"

#include <thread>
#include <mutex>
#include <math.h>
#include <unordered_map> 

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


void analysis(Root_file_handler * input_root_file, Root_file_handler * output_root_file, int thread_id, histo_handler * Hist, __int64 num_events){
	
	
	double NTupleData[5];
	bool WriteNTuple = false;

	for(__int64 idx=0;idx<num_events;idx++){
		event_data * event  = input_root_file->get_next_event();
		
		//std::chrono::milliseconds sleepDuration(50);
		//std::this_thread::sleep_for(sleepDuration);
		//cout<<std::this_thread::get_id()<<endl;

		//printf("\nreaction=%f, idx=%d, slow=%f\n",event->reaction, idx, pow((double)idx,100));
		//printf("r1x=%f, r1y=%f, r1tof=%f,\n e1x=%f, e1y=%f, e1tof=%f\n", event->rx[0], event->ry[0], event->rtof[0], event->ex[0], event->ey[0], event->etof[0]);
		//printf("p1x=%f, p1y=%f, p1tof=%f,\n", event->px[0], event->py[0], event->ptof[0]);
		//printf("r2x=%f, r2y=%f, r2tof=%f,\n e2x=%f, e2y=%f, e2tof=%f\n", event->rx[1], event->ry[1], event->rtof[1], event->ex[1], event->ey[1], event->etof[1]);
		//printf("p2x=%f, p2y=%f, p2tof=%f,\n", event->px[1], event->py[1], event->ptof[1]);
		
		//single_event->reaction = pow((double)idx,1000);
		
		
		Hist->fill1("test", 5.6,"test title", 1000, 0., 10., "testx", "Histograms");
		Hist->fill1("test", 15.6,"test title", 1000, 0., 10., "testx", "Histograms");

		Hist->fill1("test1", idx,"test title", 1000, -1., 1000., "testx", "Histograms");

		Hist->fill1("test2", idx*2,"test title", 1000, -1, 1000., "testx", "Histograms");

		Hist->fill2("test1", idx, idx,"test title", 1000, 0., 1000., "testx", 1000, 0., 1000., "testy", "Histograms");
		Hist->fill2("test2", idx/2., idx/2.,"test title", 1000, 0., 1000., "testx", 1000, 0., 1000., "testy", "Histograms");
		Hist->fill2("test3", idx/2., idx/2.,"test title", 1000, 0., 100., "testx", 1000, 0., 1000., "testy", "Histograms");

		NTupleData[0]=idx;
		NTupleData[1]=thread_id;
		NTupleData[2]=event->reaction;
		NTupleData[3]=event->rx[0];

		WriteNTuple = true;
		if(WriteNTuple) {
			output_root_file->NTupleD("Data","test_file","idx:threadID:reation:rx",32000,NTupleData);
			output_root_file->EventsWrittenCounter();
		}
		delete event;
	}	

}

//void ProcessRootFile(string inputfilename, string outputfilename){
//	
//	Root_file_handler * input_root_file = new Root_file_handler(inputfilename, "read");
//	Root_file_handler * output_root_file = new Root_file_handler(outputfilename, "write");
//	//event_data * single_event ;
//
//
//
//	//std::thread t1(analysis, input_root_file, output_root_file, 1.);
//	//std::thread t2(analysis, input_root_file, output_root_file, 2.);
//	//std::thread t3(analysis, input_root_file);
//	//std::thread t4(analysis, input_root_file);
//	//std::thread t5(analysis, input_root_file);
//	//std::thread t6(analysis, input_root_file);
//	//std::thread t7(analysis, input_root_file);
//	//std::thread t8(analysis, input_root_file);
//	
//	analysis( input_root_file, output_root_file, 1);
//
//	//t1.join();
//	//t2.join();
//	//t3.join();
//	//t4.join();
//	//t5.join();
//	//t6.join();
//	//t7.join();
//	//t8.join();
//	
//	H1i * hist1 = new H1i("test1", "test title", 100, 0., 10., "testx", "test_dir");
//	//hist1->print_bin_contents();
//	
//	hist1->fill(5.5);
//	//hist1->fill1(3.0);
//	hist1->print_bin_contents();
//
//	input_root_file->close_file();
//	output_root_file->write_TNtupleD();
//	output_root_file->close_file();
//	
//	return;
//}



int main(__int32 argc, char* argv[], char* envp[])
{
	Red(true);
	printf("Don't panic! Everything will be fine.\n");
	White(false);

	int number_of_threads=1;

	string inputfilename = "test.root";
	string outputfilename = "output_test.root";
	//ProcessRootFile(inputfilename, outputfilename);

	Root_file_handler * input_root_file = new Root_file_handler(inputfilename, "read");
	Root_file_handler * output_root_file = new Root_file_handler(outputfilename, "write");
	//event_data * single_event ;


	histo_handler * Histogram_Handler_array[8];
	for(int i=0; i<number_of_threads;++i){
		Histogram_Handler_array[i]= new histo_handler();
	}

	__int64 num_events_per_thread = input_root_file->get_Total_Events_inputfile()/((__int64)number_of_threads) -1; 
	//lanuch threads
	std::vector<std::thread> threads;
	for(int i = 0; i < number_of_threads; ++i){
		threads.push_back(std::thread(analysis, input_root_file, output_root_file, i, Histogram_Handler_array[i], num_events_per_thread));
	}

	for(auto& thread : threads){
		thread.join();
	}


	//std::thread t1(analysis, input_root_file, output_root_file, 1., Histogram_Handler_array[0], num_events_per_thread);
	//std::thread t2(analysis, input_root_file, output_root_file, 2., Histogram_Handler_array[1], num_events_per_thread);
	//std::thread t3(analysis, input_root_file, output_root_file, 3., Histogram_Handler_array[2], num_events_per_thread);
	//std::thread t4(analysis, input_root_file, output_root_file, 4., Histogram_Handler_array[3], num_events_per_thread);
	//std::thread t5(analysis, input_root_file, output_root_file, 5., Histogram_Handler_array[4], num_events_per_thread);
	//std::thread t6(analysis, input_root_file, output_root_file, 6., Histogram_Handler_array[5], num_events_per_thread);
	//std::thread t7(analysis, input_root_file, output_root_file, 7., Histogram_Handler_array[6], num_events_per_thread);
	//std::thread t8(analysis, input_root_file, output_root_file, 8., Histogram_Handler_array[7], num_events_per_thread);	
	////analysis( input_root_file, output_root_file, 1);

	//t1.join();
	//t2.join();
	//t3.join();
	//t4.join();
	//t5.join();
	//t6.join();
	//t7.join();
	//t8.join();
	
	//H1i * hist1 = new H1i("test1", "test title", 100, 0., 10., "testx", "test_dir");
	//H1i * hist1 = new H1i("test1", "test title", 100, 0., 10., "testx", "test_dir");
	//H1i * hist2 = new H1i("test1", "test title", 100, 0., 10., "testx", "test_dir");
	//hist1->print_bin_contents();
	
	
	

	//combine the historams form each thread
	// iterate over the Histogram_Handler_array
	// store the results in Histogram_Handler_array[0]
	for(int i=0; i<number_of_threads;++i){
		//the 1d histograms
		for (	Histogram_Handler_array[i]->h1i_map_iterator =  Histogram_Handler_array[i]->h1i_map.begin(); 
				Histogram_Handler_array[i]->h1i_map_iterator != Histogram_Handler_array[i]->h1i_map.end();
				++Histogram_Handler_array[i]->h1i_map_iterator	) {

			//Histogram_Handler1->h1i_map[ Histogram_Handler2->h1i_map_iterator->first ]->print_bin_contents();
			Histogram_Handler_array[0]->combine_hist(Histogram_Handler_array[i]->h1i_map_iterator->second );
			//Histogram_Handler1->h1i_map[ Histogram_Handler2->h1i_map_iterator->first ]->print_bin_contents();
		}
		//the 2d histograms
		for (	Histogram_Handler_array[i]->h2i_map_iterator =  Histogram_Handler_array[i]->h2i_map.begin(); 
				Histogram_Handler_array[i]->h2i_map_iterator != Histogram_Handler_array[i]->h2i_map.end();
				++Histogram_Handler_array[i]->h2i_map_iterator	) {

			//Histogram_Handler1->h2i_map[ Histogram_Handler2->h2i_map_iterator->first ]->print_bin_contents();
			Histogram_Handler_array[0]->combine_hist(Histogram_Handler_array[i]->h2i_map_iterator->second );
			//Histogram_Handler1->h2i_map[ Histogram_Handler2->h2i_map_iterator->first ]->print_bin_contents();
		}
	}


	//write 1D histograms to the root file
	for (	Histogram_Handler_array[0]->h1i_map_iterator =  Histogram_Handler_array[0]->h1i_map.begin(); 
			Histogram_Handler_array[0]->h1i_map_iterator != Histogram_Handler_array[0]->h1i_map.end();
			++Histogram_Handler_array[0]->h1i_map_iterator	) {

				output_root_file->add_hist( Histogram_Handler_array[0]->h1i_map_iterator->second );
	}

	//write 2D histograms to the root file
	for (	Histogram_Handler_array[0]->h2i_map_iterator =  Histogram_Handler_array[0]->h2i_map.begin(); 
			Histogram_Handler_array[0]->h2i_map_iterator != Histogram_Handler_array[0]->h2i_map.end();
			++Histogram_Handler_array[0]->h2i_map_iterator	) {

				output_root_file->add_hist( Histogram_Handler_array[0]->h2i_map_iterator->second );
	}


	input_root_file->close_file();
	output_root_file->write_TNtupleD();
	output_root_file->close_file();

	return 0;
}



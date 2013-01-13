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
//#include "config_file_parser.h"
#include "config_file_reader.h"

#include <thread>
#include <mutex>
#include <math.h>
#include <unordered_map> 
#include <sys/stat.h> //needed for FileExists
#include <sstream> //for string_to_double

using namespace std;

void analysis(Root_file_handler * input_root_file, Root_file_handler * output_root_file, int thread_id, histo_handler * Hist, __int64 num_events);
//kbhit get keyboard input
#ifndef LINUX

	__int32 my_kbhit()
	{
		if (!_kbhit()) return 0;
		return _getch();
	}

#else

	__int32 my_kbhit(void)
	{
		struct termios term, oterm;
		__int32 fd = 0;
		__int32 c = 0;
		tcgetattr(fd, &oterm);
		memcpy(&term, &oterm, sizeof(term));
		term.c_lflag = term.c_lflag & (!ICANON);
		term.c_cc[VMIN] = 0;
		term.c_cc[VTIME] = 1;
		tcsetattr(fd, TCSANOW, &term);
		c = getchar();
		tcsetattr(fd, TCSANOW, &oterm);
		return ((c != -1) ? c : 0);
	}

#endif


 double string_to_double( const std::string& s )
 {
   std::istringstream i(s);
   double x;
   if (!(i >> x))
     return 0;
   return x;
 }

 
/////////////////////////////////////////////////////////////////////////////
bool FileExists(const char * strFilename) {
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


 void GUI(Root_file_handler * input_root_file ){


	printf("Number of events in file: %.0lf\n", double(input_root_file->get_Total_Events_inputfile()));
	printf("\n0%%           25%%          50%%          75%%        100%%\n");
	printf("|------------|------------|------------|-----------|\n");
	COORD start_COORD= getXY();
	printf("\n\n\n\n\n");
	__int64 total_events = input_root_file->get_Total_Events_inputfile();
	__int64 prev_position=0;
	__int64 current_position=0;
	double rate=0;
	
	string p_bar;

	double percentage_complete = 0;
	int num_star =0;
	__int32 c = 0;

	while(input_root_file->get_current_entry_inputfile() < total_events && !input_root_file->stop_reading){
		prev_position = input_root_file->get_current_entry_inputfile();
		
		//sleep thread for 0.25s
		std::chrono::milliseconds sleepDuration(750);
		std::this_thread::sleep_for(sleepDuration);

		current_position =input_root_file->get_current_entry_inputfile();
		rate = ((double)(current_position - prev_position))/0.75;

		gotoXY(start_COORD.X, start_COORD.Y-5);
		percentage_complete = (double)current_position / (double)total_events ;
		num_star = (int)(51. * percentage_complete);
		p_bar = "*";
		for(int i=0; i < num_star; ++i){
			p_bar += "*";
		}
		printf("%s \nrate=%G /s							\n", p_bar.c_str(),rate);
		printf("percentage_complete = %f                \n",percentage_complete);
		printf("current_position = %I64i                \n",current_position);
		printf("total_events	 = %I64i                \n",total_events);
		
		

		c = my_kbhit();
		if (c) {
			while (my_kbhit());
			if(c=='q') {printf("\nq was pressed -> skipping all file. \n"); input_root_file->stop_reading = true;}
			if(c=='Q') {printf("\nQ was pressed -> skipping all files.\n"); input_root_file->stop_reading = true;}
		}

	}
 }





int main(__int32 argc, char* argv[], char* envp[])
{
	#ifdef _DEBUG
		Red(true);
		printf("\n***********************\n    SLOW DEBUG VERSION !\n***********************\n");
		Red(false);
	#endif
	Green(true);
	printf("\nCRAP (Coltrims Root Analysis Program).\n");
	White(false);	
	printf("Verison 0.5\n");

	Red(true);
	printf("Don't panic! Everything will be fine.\n");
	White(false);



	config_file_reader * config_file;// = new config_file_reader("config.txt");

	//////////////////////////////// read config file //////////////////////////////////////////

	FILE * ascii_file = 0;
	if (argc>1) {
		if (!strcmp(argv[1],"help") || !strcmp(argv[1],"-help")) {
		}
		else{
			ascii_file = fopen(argv[1],"rt");
			if (!ascii_file || !FileExists(argv[1])) {
				printf("Error:Problem with config file.");
			}
			else if (ascii_file) {
				fclose(ascii_file); 
				ascii_file = 0;
			}
				config_file = new config_file_reader(argv[1]);
		}
	}else {
		cout << "Please give a config file" << endl;
		return 0;
	}
	///////////////////////////// end read config file /////////////////////////////////////////
	//roughly find how many events per thread
	__int64 num_events_per_thread;
	__int64 extras;
	Root_file_handler * input_root_file ;
	std::vector<std::thread> threads;

	//start output file handlers
	string outputfilename = config_file->outputfilename;
	Root_file_handler * output_root_file = new Root_file_handler(outputfilename, "write");
	if ( output_root_file->IsZombie()){
		cout << "Error opening the output root files"<< endl;
		return 0;
	}

	double temp_test = config_file->parameter[1001];
	cout << "temp_test " << temp_test << endl;
	//set the number of theads
	int number_of_threads=(int) config_file->parameter[1000];

	//Start the histogram_handlers (one for each thread)
	std::vector<histo_handler*> Histogram_Handler_vector;
	histo_handler * temp;
	for(int i = 0; i < number_of_threads; ++i){
		temp= new histo_handler();
		Histogram_Handler_vector.push_back(temp);
	}

	//loop over all of the input files
	for(int i=0; i < (int)config_file->inputfilename.size(); ++i){
	
		if( !FileExists( config_file->inputfilename[i].c_str() ) ){
			cout << "File does not exist. Skipping file." << endl;
			continue; //skip the rest of the loop and return to the begin of the loop 
		}


		//start input file handlers
		input_root_file = new Root_file_handler(config_file->inputfilename[i], "read");
		if (input_root_file->IsZombie() ){
			cout << "Error opening the input root files"<< endl;
			return 0;
		}



	
		//roughly find how many events per thread
		num_events_per_thread = input_root_file->get_Total_Events_inputfile()/((__int64)number_of_threads) ; 
		//find the left overs
		extras = input_root_file->get_Total_Events_inputfile() - (num_events_per_thread * number_of_threads);
	
	
		//lanuch threads
		//start thread 0 (this one has the extra event)
		threads.push_back(std::thread(analysis, input_root_file, output_root_file, 0, Histogram_Handler_vector[0], num_events_per_thread + extras));
		//start the rest of the threads 
		for(int i = 1; i < number_of_threads; ++i){
			threads.push_back(std::thread(analysis, input_root_file, output_root_file, i, Histogram_Handler_vector[i], num_events_per_thread));
		}
		std::thread so_called_gui(GUI, input_root_file);
	
	
		for(auto& thread : threads){
			thread.join();
		}
	
		so_called_gui.join();
	
	
		input_root_file->close_file();

	}

	cout<<endl;
	//combine the historams form each thread
	// iterate over the Histogram_Handler_vector
	// store the results in Histogram_Handler_vector[0]
	printf("Combining histgrams from each thread ...\n");
	for(int i=0; i<number_of_threads;++i){
		//the 1d histograms
		for (	Histogram_Handler_vector[i]->h1i_map_iterator =  Histogram_Handler_vector[i]->h1i_map.begin(); 
				Histogram_Handler_vector[i]->h1i_map_iterator != Histogram_Handler_vector[i]->h1i_map.end();
				++Histogram_Handler_vector[i]->h1i_map_iterator	) {

			//Histogram_Handler1->h1i_map[ Histogram_Handler2->h1i_map_iterator->first ]->print_bin_contents();
			Histogram_Handler_vector[0]->combine_hist(Histogram_Handler_vector[i]->h1i_map_iterator->second );
			//Histogram_Handler1->h1i_map[ Histogram_Handler2->h1i_map_iterator->first ]->print_bin_contents();
		}
		//the 2d histograms
		for (	Histogram_Handler_vector[i]->h2i_map_iterator =  Histogram_Handler_vector[i]->h2i_map.begin(); 
				Histogram_Handler_vector[i]->h2i_map_iterator != Histogram_Handler_vector[i]->h2i_map.end();
				++Histogram_Handler_vector[i]->h2i_map_iterator	) {

			//Histogram_Handler1->h2i_map[ Histogram_Handler2->h2i_map_iterator->first ]->print_bin_contents();
			Histogram_Handler_vector[0]->combine_hist(Histogram_Handler_vector[i]->h2i_map_iterator->second );
			//Histogram_Handler1->h2i_map[ Histogram_Handler2->h2i_map_iterator->first ]->print_bin_contents();
		}
	}


	//write 1D histograms to the root file
	for (	Histogram_Handler_vector[0]->h1i_map_iterator =  Histogram_Handler_vector[0]->h1i_map.begin(); 
			Histogram_Handler_vector[0]->h1i_map_iterator != Histogram_Handler_vector[0]->h1i_map.end();
			++Histogram_Handler_vector[0]->h1i_map_iterator	) {

				output_root_file->add_hist( Histogram_Handler_vector[0]->h1i_map_iterator->second );
	}

	//write 2D histograms to the root file
	for (	Histogram_Handler_vector[0]->h2i_map_iterator =  Histogram_Handler_vector[0]->h2i_map.begin(); 
			Histogram_Handler_vector[0]->h2i_map_iterator != Histogram_Handler_vector[0]->h2i_map.end();
			++Histogram_Handler_vector[0]->h2i_map_iterator	) {

				output_root_file->add_hist( Histogram_Handler_vector[0]->h2i_map_iterator->second );
	}



	output_root_file->write_TNtupleD();
	output_root_file->close_file();

	return 0;
}



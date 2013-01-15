#include "event_data.h"
#include "Root_file_handler.h"
#include "console.h"
#include "Simple_Hist.h"
#include "config_file_reader.h"



//////////////////////////////////

//#include <Windows.h>
//#include "TCanvas.h"
//#include "TH1D.h"
//#include "TH2D.h"
//#include "TApplication.h"
//#include "TFile.h"
//#include "TTree.h"
//#include "TF1.h"
#include "TTimeStamp.h"
TTimeStamp * TimeWatch = 0;
////////////////////////////////////////////////////
double	get_time_s()
////////////////////////////////////////////////////
{
	if (!TimeWatch) TimeWatch = new TTimeStamp(0,0);
	TimeWatch->Set();
	return double(TimeWatch->GetSec()) + 1.e-9*TimeWatch->GetNanoSec();
}





////////////////////////



void set_branches(event_data * single_event, TTree * inputfileRootTree){

			char ntuple_identifier[500];
			bool Check = 0;

			if(inputfileRootTree->GetBranch("reaction"))
				inputfileRootTree->SetBranchAddress("reaction",&single_event->reaction);
			
			if(inputfileRootTree->GetBranch("ehit"))
				inputfileRootTree->SetBranchAddress("ehit",&single_event->ehit);
			
			if(inputfileRootTree->GetBranch("rhit"))
				inputfileRootTree->SetBranchAddress("rhit",&single_event->rhit);
			
			if(inputfileRootTree->GetBranch("phit"))
				inputfileRootTree->SetBranchAddress("phit",&single_event->phit);
			
			if(inputfileRootTree->GetBranch("bunchmarker"))
				inputfileRootTree->SetBranchAddress("bunchmarker",&single_event->bunchmarker);
			
			// find out how many recoils are in Ntuple
			int MaxRec = 0;
			do {
				sprintf(ntuple_identifier,"r%ix",++MaxRec);
			} while(inputfileRootTree->GetBranch(ntuple_identifier));

			for(int i=0;i<MaxRec-1;i++) {
				sprintf(ntuple_identifier,"r%ix",i+1);
				if(inputfileRootTree->GetBranch(ntuple_identifier))
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->rx[i]);
				
				sprintf(ntuple_identifier,"r%iy",i+1);
				if(inputfileRootTree->GetBranch(ntuple_identifier))
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->ry[i]);
				
				sprintf(ntuple_identifier,"r%imcp",i+1);
				if(inputfileRootTree->GetBranch(ntuple_identifier))
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->rmcp[i]);
				
				sprintf(ntuple_identifier,"r%itof",i+1);
				if(inputfileRootTree->GetBranch(ntuple_identifier))
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->rtof[i]);
				
				sprintf(ntuple_identifier,"r%iflag",i+1);
				if(inputfileRootTree->GetBranch(ntuple_identifier))
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->rflag[i]);		
			}

			// find out how many electrons are in Ntuple
			int MaxElec = 0;
			do {
				sprintf(ntuple_identifier,"e%ix",++MaxElec);
			} while(inputfileRootTree->GetBranch(ntuple_identifier));

			for(int i=0;i<MaxElec-1;i++) {
				sprintf(ntuple_identifier,"e%ix",i+1);
				if(inputfileRootTree->GetBranch(ntuple_identifier))
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->ex[i]);
				sprintf(ntuple_identifier,"e%iy",i+1);
				if(inputfileRootTree->GetBranch(ntuple_identifier))
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->ey[i]);
				sprintf(ntuple_identifier,"e%imcp",i+1);
				if(inputfileRootTree->GetBranch(ntuple_identifier))
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->emcp[i]);
				sprintf(ntuple_identifier,"e%itof",i+1);
				if(inputfileRootTree->GetBranch(ntuple_identifier))
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->etof[i]);
				sprintf(ntuple_identifier,"e%iflag",i+1);
				if(inputfileRootTree->GetBranch(ntuple_identifier))
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->eflag[i]);		
			}

			// find out how many projectyles are in Ntuple
			int MaxProj = 0;
			do {
				sprintf(ntuple_identifier,"p%ix",++MaxProj);
			} while(inputfileRootTree->GetBranch(ntuple_identifier));
		
			for(int i=0;i<MaxProj-1;i++) {
				sprintf(ntuple_identifier,"p%iy",i+1);
				if(inputfileRootTree->GetBranch(ntuple_identifier))
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->py[i]);
				
				sprintf(ntuple_identifier,"p%imcp",i+1);
				if(inputfileRootTree->GetBranch(ntuple_identifier))
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->pmcp[i]);
				
				sprintf(ntuple_identifier,"p%itof",i+1);
				if(inputfileRootTree->GetBranch(ntuple_identifier))
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->ptof[i]);
				
				sprintf(ntuple_identifier,"p%iflag",i+1);
				if(inputfileRootTree->GetBranch(ntuple_identifier))
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->pflag[i]);		
			}
}


void analysis(Root_file_handler * input_root_file, Root_file_handler * output_root_file, histo_handler * Hist, config_file_reader * config_file, __int64 num_events){
	
	unordered_map<int, double> parameter = config_file->parameter;
	vector<command> config_commands = config_file->config_commands;

	////////////////////////////////////
	double startTime, endTime;

	//////////////////////////////////
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
		
		//Hist->fill1("test1d", 0.0,"test title", 1000, 0., 10., "testx", "Histograms");
		//Hist->fill1("test1d", 5.6,"test title", 1000, 0., 10., "testx", "Histograms");
		//Hist->fill1("test1d", 15.6,"test title", 1000, 0., 10., "testx", "Histograms");

		//Hist->fill1("test1d1", idx,"test title", 1000, -1., 1000., "testx", "Histograms");

		//Hist->fill1("test1d2", idx*2,"test title", 1000, -1, 1000., "testx", "Histograms");

		//Hist->fill2("test1", idx, idx,"test title", 1000, 0., 1000., "testx", 1000, 0., 1000., "testy", "Histograms");
		//Hist->fill2("test2", idx/2., idx/2.,"test title", 1000, 0., 1000., "testx", 1000, 0., 1000., "testy", "Histograms");
		//Hist->fill2("test3", idx/2., idx/2.,"test title", 1000, 0., 100., "testx", 1000, 0., 1000., "testy", "Histograms");

		//string sub_name = "Basic";
		//
		//Hist->fill2("pipico1 spectrum ",event->rtof[0],event->rtof[1],"PIPICO spectrum " ,750,0.,6000.,"rec1 TOF [ns]",750,0.,6000.,"rec2 TOF [ns]",sub_name);
		//Hist->fill1(" tof hit 1 ",event->rtof[0],"r1tof ",500,0.,5000.,"rec1 TOF [ns]",sub_name);
		//Hist->fill1(" tof hit 2 ",event->rtof[1],"r2tof ",500,0.,5000.,"rec2 TOF [ns]",sub_name);
		//				
		//Hist->fill1("e1_tof",event->etof[0],"e1_tof",500,-10.,350.,"e1tof [ns]",sub_name);
		//Hist->fill1("e2_tof",event->etof[1],"e2_tof",500,-10.,350.,"e1tof [ns]",sub_name);
		//				
		//Hist->fill2("e1 X fish",event->etof[0],event->ex[0],"e1 X fish",500,-10.,100.,"e1tof [ns]",500,-50.,50.,"e1z [mm]",sub_name);
		//Hist->fill2("e1 Y fish",event->etof[0],event->ey[0],"e1 Y fish",500,-10.,100.,"e1tof [ns]",500,-50.,50.,"e1y [mm]",sub_name);
		//Hist->fill2("e2 X fish",event->etof[1],event->ex[1],"e2 X fish",500,-10.,100.,"e2tof [ns]",500,-50.,50.,"e2z [mm]",sub_name);
		//Hist->fill2("e2 Y fish",event->etof[1],event->ey[1],"e2 Y fish",500,-10.,100.,"e2tof [ns]",500,-50.,50.,"e2y [mm]",sub_name);
		//				
		//Hist->fill2("e1 Wiggles",event->etof[0],sqrt(event->ex[0]*event->ex[0] + event->ey[0]*event->ey[0]),"e1 Wiggles",500,-10.,150.,"e1tof [ns]",500,0.,50.,"Radius [mm]",sub_name);
		//Hist->fill2("e2 Wiggles",event->etof[1],sqrt(event->ex[1]*event->ex[1] + event->ey[1]*event->ey[1]),"e2 Wiggles",500,-10.,150.,"e2tof [ns]",500,0.,50.,"Radius [mm]",sub_name);

		//Hist->fill2("r1x vs r1y",event->rx[0],event->ry[0],"Recoil Position",500,-75.,75.,"r1z [mm]",500,-75.,75.,"r1y [mm]",sub_name);
		//Hist->fill2("r2x vs r2y",event->rx[1],event->ry[1],"Recoil Position",500,-75.,75.,"r2z [mm]",500,-75.,75.,"r2y [mm]",sub_name);
		//					
		//Hist->fill2("e1z vs e1y",event->ex[0],event->ey[0],"Recoil Position",500,-50.,50.,"e1z [mm]",500,-50.,50.,"e1y [mm]",sub_name);
		//Hist->fill2("e2x vs e2y",event->ex[1],event->ey[1],"Recoil Position",500,-75.,75.,"e2z [mm]",500,-75.,75.,"e2y [mm]",sub_name);
		//
		//Hist->fill1(" r1y " ,event->ry[0],"r1y ",500,-70.,70.,"rec1 y [mm]",sub_name);
		//Hist->fill1(" r1x " ,event->rx[0],"r1z ",500,-70.,70.,"rec1 z [mm]",sub_name);
		//				
		//Hist->fill1("reaction",event->reaction,"Reaction Flag",400,-1.,15.,"Reaction Number",sub_name);

		////gated to be in the center of y (low y momentum)
		//if( fabs(event->ey[0]) <= 5. ){
		//	Hist->fill2("e1 X fish ygated",event->etof[0],event->ex[0],"e1 X fish ygated",150,30.,85.,"e1tof [ns]",150,-50.,50.,"e1z [mm]",sub_name);
		//}
		////gated to be in the center of z (low z momentum)
		//if( fabs(event->ex[0]) <= 5. ){
		//	Hist->fill2("e1 Y fish zgated",event->etof[0],event->ey[0],"e1 Y fish zgated",150,30.,85.,"e1tof [ns]",150,-50.,50.,"e1y [mm]",sub_name);
		//}

		//Hist->fill2("r1 X fish",event->rtof[0],event->rx[0],"r1 X fish",500,-10.,8000.,"r1tof [ns]",500,-70.,70.,"r1z [mm]",sub_name);
		//Hist->fill2("r1 Y fish",event->rtof[0],event->ry[0],"r1 Y fish",500,-10.,8000.,"r1tof [ns]",500,-70.,70.,"r1y [mm]",sub_name);


startTime = get_time_s();


Hist->fill1(401,"ehit_ntuple0",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(402,"ehit_ntuple1",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(403,"ehit_ntuple2",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(404,"ehit_ntuple3",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(405,"ehit_ntuple4",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(406,"ehit_ntuple5",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(407,"ehit_ntuple6",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(408,"ehit_ntuple7",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(409,"ehit_ntuple8",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(410,"ehit_ntuple9",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill2(411,"elec1xy_ntuple0",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(412,"elec1xy_ntuple1",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(413,"elec1xy_ntuple2",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(414,"elec1xy_ntuple3",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(415,"elec1xy_ntuple4",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(416,"elec1xy_ntuple5",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(417,"elec1xy_ntuple6",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(418,"elec1xy_ntuple7",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(419,"elec1xy_ntuple8",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(420,"elec1xy_ntuple9",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");


Hist->fill1(401,"ehit_ntuple10",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(402,"ehit_ntuple11",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(403,"ehit_ntuple12",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(404,"ehit_ntuple13",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(405,"ehit_ntuple14",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(406,"ehit_ntuple15",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(407,"ehit_ntuple16",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(408,"ehit_ntuple17",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(409,"ehit_ntuple18",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(410,"ehit_ntuple19",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill2(411,"elec1xy_ntuple10",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(412,"elec1xy_ntuple11",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(413,"elec1xy_ntuple12",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(414,"elec1xy_ntuple13",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(415,"elec1xy_ntuple14",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(416,"elec1xy_ntuple15",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(417,"elec1xy_ntuple16",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(418,"elec1xy_ntuple17",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(419,"elec1xy_ntuple18",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(420,"elec1xy_ntuple19",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");

Hist->fill1(401,"ehit_ntuple20",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(402,"ehit_ntuple21",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(403,"ehit_ntuple22",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(404,"ehit_ntuple23",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(405,"ehit_ntuple24",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(406,"ehit_ntuple25",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(407,"ehit_ntuple26",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(408,"ehit_ntuple27",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(409,"ehit_ntuple28",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(410,"ehit_ntuple29",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill2(411,"elec1xy_ntuple20",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(412,"elec1xy_ntuple21",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(413,"elec1xy_ntuple22",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(414,"elec1xy_ntuple23",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(415,"elec1xy_ntuple24",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(416,"elec1xy_ntuple25",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(417,"elec1xy_ntuple26",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(418,"elec1xy_ntuple27",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(419,"elec1xy_ntuple28",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(420,"elec1xy_ntuple29",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");


Hist->fill1(401,"ehit_ntuple30",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(402,"ehit_ntuple31",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(403,"ehit_ntuple32",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(404,"ehit_ntuple33",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(405,"ehit_ntuple34",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(406,"ehit_ntuple35",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(407,"ehit_ntuple36",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(408,"ehit_ntuple37",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(409,"ehit_ntuple38",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(410,"ehit_ntuple39",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill2(411,"elec1xy_ntuple30",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(412,"elec1xy_ntuple31",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(413,"elec1xy_ntuple32",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(414,"elec1xy_ntuple33",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(415,"elec1xy_ntuple34",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(416,"elec1xy_ntuple35",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(417,"elec1xy_ntuple36",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(418,"elec1xy_ntuple37",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(419,"elec1xy_ntuple38",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(420,"elec1xy_ntuple39",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");


endTime = get_time_s();

Hist->fill1(1,"cputime",(endTime-startTime)*pow(10,9),1.,"cputime",1000,0,100,"time (ns)","benchmark");
Hist->fill1(2,"cputime2",(endTime-startTime)*pow(10,9),1.,"cputime",1000,0,1000,"time (ns)","benchmark");
Hist->fill1(3,"cputime3",(endTime-startTime)*pow(10,9),1.,"cputime",1000000,0,2000000,"time (ns)","benchmark");
Hist->fill1(4,"cputime4",(endTime-startTime)*pow(10,9),1.,"cputime",1000000,0,1000000000,"time (ns)","benchmark");
//printf("time= %e\n",(endTime-startTime)*pow(10,9) );

		WriteNTuple = false;
		
		if(WriteNTuple) {
			NTupleData[0]=endTime-startTime;
			NTupleData[1]=endTime;
			NTupleData[2]=event->reaction;
			NTupleData[3]=event->rx[0];


			output_root_file->NTupleD("Data","test_file","cputime",32000,NTupleData);
			output_root_file->EventsWrittenCounter();
		}
		delete event;
		if(	input_root_file->stop_reading) idx=num_events+1;  //exit
	}	

}
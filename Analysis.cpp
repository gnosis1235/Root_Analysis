#include "event_data.h"
#include "Root_file_handler.h"
#include "console.h"
#include "Simple_Hist.h"
#include "config_file_reader.h"




//#include <Windows.h>
//#include <tuple>
//#include <time.h>
//#include <iostream>
//
//class ProcessorTimer
//{
//  public:
//    ProcessorTimer() { start(); }
//    void start() { ::GetProcessTimes(::GetCurrentProcess(), &ft_[3], &ft_[2], &ft_[1], &ft_[0]); }
//    std::tuple<double, double> stop()
//    {
//        ::GetProcessTimes(::GetCurrentProcess(), &ft_[5], &ft_[4], &ft_[3], &ft_[2]);
//        ULARGE_INTEGER u[4];
//        for (size_t i = 0; i < 4; ++i)
//        {
//            u[i].LowPart = ft_[i].dwLowDateTime;
//            u[i].HighPart = ft_[i].dwHighDateTime;
//        }
//        double user = (u[2].QuadPart - u[0].QuadPart) / 10000000.0;
//        double kernel = (u[3].QuadPart - u[1].QuadPart) / 10000000.0;
//        return std::make_tuple(user, kernel);
//    }
//  private:
//    FILETIME ft_[6];
//};
//
//
//class ScopedProcessorTimer
//{
//  public:
//    ScopedProcessorTimer(std::ostream& os = std::cerr) : timer_(ProcessorTimer()), os_(os) { }
//    ~ScopedProcessorTimer()
//    {
//        std::tuple<double, double> t = timer_.stop();
//		printf("user %10.9e	kernal %10.9e \n", std::get<0>(t), std::get<1>(t) );
//        //os_ << "user " << std::get<0>(t) << "\n";
//        //os_ << "kernel " << std::get<1>(t) << "\n";
//    }
//  private:
//    ProcessorTimer timer_;
//    std::ostream& os_;
//};





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
			
			cout << "The MaxRec are: " << MaxRec-1 << endl;
			
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
			
			cout << "The MaxElec are: " << MaxElec-1 << endl;

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
			
			cout << "The MaxProj are: " << MaxProj-1 << endl;
		
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


	//////////////////////////////////
	double NTupleData[5];
	bool WriteNTuple = false;

	//event_data * event  = input_root_file->get_next_event();	
	event_data * event = new event_data();
	for(__int64 idx=0;idx<num_events;idx++){

		input_root_file->get_next_event(event);	


		//char * sub_name = "Basic";
		//int num = 0;
		//Hist->fill2(1,"pipico1 spectrum ",event->rtof[0],event->rtof[1],1.,"PIPICO spectrum " ,750,0.,6000.,"rec1 TOF [ns]",750,0.,6000.,"rec2 TOF [ns]",sub_name);
		//Hist->fill1(2," tof hit 1 ",event->rtof[0],1.,"r1tof ",500,0.,5000.,"rec1 TOF [ns]",sub_name);
		//Hist->fill1(3," tof hit 2 ",event->rtof[1],1.,"r2tof ",500,0.,5000.,"rec2 TOF [ns]",sub_name);
		//Hist->fill1(4,"e1_tof",event->etof[0],1.,"e1_tof",500,-10.,350.,"e1tof [ns]",sub_name);
		//Hist->fill1(5,"e2_tof",event->etof[1],1.,"e2_tof",500,-10.,350.,"e1tof [ns]",sub_name);
		//Hist->fill2(6,"e1 X fish",event->etof[0],event->ex[0],1.,"e1 X fish",500,-10.,100.,"e1tof [ns]",500,-50.,50.,"e1z [mm]",sub_name);
		//Hist->fill2(7,"e1 Y fish",event->etof[0],event->ey[0],1.,"e1 Y fish",500,-10.,100.,"e1tof [ns]",500,-50.,50.,"e1y [mm]",sub_name);
		//Hist->fill2(8,"e2 X fish",event->etof[1],event->ex[1],1.,"e2 X fish",500,-10.,100.,"e2tof [ns]",500,-50.,50.,"e2z [mm]",sub_name);
		//Hist->fill2(9,"e2 Y fish",event->etof[1],event->ey[1],1.,"e2 Y fish",500,-10.,100.,"e2tof [ns]",500,-50.,50.,"e2y [mm]",sub_name);
		//Hist->fill2(11,"e1 Wiggles",event->etof[0],sqrt(event->ex[0]*event->ex[0] + event->ey[0]*event->ey[0]),1.,"e1 Wiggles",500,-10.,150.,"e1tof [ns]",500,0.,50.,"Radius [mm]",sub_name);
		//Hist->fill2(12,"e2 Wiggles",event->etof[1],sqrt(event->ex[1]*event->ex[1] + event->ey[1]*event->ey[1]),1.,"e2 Wiggles",500,-10.,150.,"e2tof [ns]",500,0.,50.,"Radius [mm]",sub_name);
		//Hist->fill2(13,"r1x vs r1y",event->rx[0],event->ry[0],1.,"Recoil Position",500,-75.,75.,"r1z [mm]",500,-75.,75.,"r1y [mm]",sub_name);
		//Hist->fill2(14,"r2x vs r2y",event->rx[1],event->ry[1],1.,"Recoil Position",500,-75.,75.,"r2z [mm]",500,-75.,75.,"r2y [mm]",sub_name);
		//Hist->fill2(15,"e1z vs e1y",event->ex[0],event->ey[0],1.,"Recoil Position",500,-50.,50.,"e1z [mm]",500,-50.,50.,"e1y [mm]",sub_name);
		//Hist->fill2(16,"e2x vs e2y",event->ex[1],event->ey[1],1.,"Recoil Position",500,-75.,75.,"e2z [mm]",500,-75.,75.,"e2y [mm]",sub_name);
		//Hist->fill1(17," r1y " ,event->ry[0],1.,"r1y ",500,-70.,70.,"rec1 y [mm]",sub_name);
		//Hist->fill1(18," r1x " ,event->rx[0],1.,"r1z ",500,-70.,70.,"rec1 z [mm]",sub_name);
		//Hist->fill1(19,"reaction",event->reaction,1.,"Reaction Flag",400,-1.,15.,"Reaction Number",sub_name);

		////gated to be in the center of y (low y momentum)
		//if( fabs(event->ey[0]) <= 5. ){
		//	Hist->fill2(20,"e1 X fish ygated",event->etof[0],event->ex[0],1.,"e1 X fish ygated",150,30.,85.,"e1tof [ns]",150,-50.,50.,"e1z [mm]",sub_name);
		//}
		////gated to be in the center of z (low z momentum)
		//if( fabs(event->ex[0]) <= 5. ){
		//	Hist->fill2(21,"e1 Y fish zgated",event->etof[0],event->ey[0],1.,"e1 Y fish zgated",150,30.,85.,"e1tof [ns]",150,-50.,50.,"e1y [mm]",sub_name);
		//}

		//Hist->fill2(22,"r1 X fish",event->rtof[0],event->rx[0],1.,"r1 X fish",500,-10.,8000.,"r1tof [ns]",500,-70.,70.,"r1z [mm]",sub_name);
		//Hist->fill2(23,"r1 Y fish",event->rtof[0],event->ry[0],1.,"r1 Y fish",500,-10.,8000.,"r1tof [ns]",500,-70.,70.,"r1y [mm]",sub_name);




//ScopedProcessorTimer timer1= ScopedProcessorTimer();
//for(int i=0; i<1; ++i){
Hist->fill1(101,"ehit_ntuple0",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(102,"ehit_ntuple1",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(103,"ehit_ntuple2",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(104,"ehit_ntuple3",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(105,"ehit_ntuple4",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(106,"ehit_ntuple5",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(107,"ehit_ntuple6",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(108,"ehit_ntuple7",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(109,"ehit_ntuple8",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(110,"ehit_ntuple9",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill2(111,"elec1xy_ntuple0",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(112,"elec1xy_ntuple1",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(113,"elec1xy_ntuple2",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(114,"elec1xy_ntuple3",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(115,"elec1xy_ntuple4",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(116,"elec1xy_ntuple5",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(117,"elec1xy_ntuple6",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(118,"elec1xy_ntuple7",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(119,"elec1xy_ntuple8",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(120,"elec1xy_ntuple9",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");


Hist->fill1(201,"ehit_ntuple10",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(202,"ehit_ntuple11",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(203,"ehit_ntuple12",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(204,"ehit_ntuple13",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(205,"ehit_ntuple14",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(206,"ehit_ntuple15",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(207,"ehit_ntuple16",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(208,"ehit_ntuple17",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(209,"ehit_ntuple18",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(210,"ehit_ntuple19",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill2(211,"elec1xy_ntuple10",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(212,"elec1xy_ntuple11",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(213,"elec1xy_ntuple12",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(214,"elec1xy_ntuple13",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(215,"elec1xy_ntuple14",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(216,"elec1xy_ntuple15",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(217,"elec1xy_ntuple16",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(218,"elec1xy_ntuple17",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(219,"elec1xy_ntuple18",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(220,"elec1xy_ntuple19",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");

Hist->fill1(301,"ehit_ntuple20",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(302,"ehit_ntuple21",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(303,"ehit_ntuple22",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(304,"ehit_ntuple23",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(305,"ehit_ntuple24",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(306,"ehit_ntuple25",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(307,"ehit_ntuple26",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(308,"ehit_ntuple27",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(309,"ehit_ntuple28",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill1(310,"ehit_ntuple29",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
Hist->fill2(311,"elec1xy_ntuple20",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(312,"elec1xy_ntuple21",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(313,"elec1xy_ntuple22",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(314,"elec1xy_ntuple23",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(315,"elec1xy_ntuple24",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(316,"elec1xy_ntuple25",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(317,"elec1xy_ntuple26",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(318,"elec1xy_ntuple27",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(319,"elec1xy_ntuple28",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
Hist->fill2(320,"elec1xy_ntuple29",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");


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
//}
//timer1.~ScopedProcessorTimer();

		WriteNTuple = false;
		
		if(WriteNTuple) {
			NTupleData[0]= 0;
			NTupleData[1]= 0;
			NTupleData[2]=event->reaction;
			NTupleData[3]=event->rx[0];


			output_root_file->NTupleD("Data","test_file","cputime",32000,NTupleData);
			output_root_file->EventsWrittenCounter();
		}
		//delete event;
		if(	input_root_file->stop_reading) idx=num_events+1;  //exit
	}	

}
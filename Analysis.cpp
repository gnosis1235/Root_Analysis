#include "event_data.h"
#include "Root_file_handler.h"
#include "console.h"
#include "Simple_Hist.h"


void set_branches(event_data * single_event, TTree * inputfileRootTree){

			char ntuple_identifier[500];
			bool Check = 0;

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
		
		Hist->fill1("test1d", 0.0,"test title", 1000, 0., 10., "testx", "Histograms");
		Hist->fill1("test1d", 5.6,"test title", 1000, 0., 10., "testx", "Histograms");
		Hist->fill1("test1d", 15.6,"test title", 1000, 0., 10., "testx", "Histograms");

		Hist->fill1("test1d1", idx,"test title", 1000, -1., 1000., "testx", "Histograms");

		Hist->fill1("test1d2", idx*2,"test title", 1000, -1, 1000., "testx", "Histograms");

		Hist->fill2("test1", idx, idx,"test title", 1000, 0., 1000., "testx", 1000, 0., 1000., "testy", "Histograms");
		Hist->fill2("test2", idx/2., idx/2.,"test title", 1000, 0., 1000., "testx", 1000, 0., 1000., "testy", "Histograms");
		Hist->fill2("test3", idx/2., idx/2.,"test title", 1000, 0., 100., "testx", 1000, 0., 1000., "testy", "Histograms");

		string sub_name = "Basic";
		
		Hist->fill2("pipico1 spectrum ",event->rtof[0],event->rtof[1],"PIPICO spectrum " ,750,0.,6000.,"rec1 TOF [ns]",750,0.,6000.,"rec2 TOF [ns]",sub_name);
		Hist->fill1(" tof hit 1 ",event->rtof[0],"r1tof ",500,0.,5000.,"rec1 TOF [ns]",sub_name);
		Hist->fill1(" tof hit 2 ",event->rtof[1],"r2tof ",500,0.,5000.,"rec2 TOF [ns]",sub_name);
						
		Hist->fill1("e1_tof",event->etof[0],"e1_tof",500,-10.,350.,"e1tof [ns]",sub_name);
		Hist->fill1("e2_tof",event->etof[1],"e2_tof",500,-10.,350.,"e1tof [ns]",sub_name);
						
		Hist->fill2("e1 X fish",event->etof[0],event->ex[0],"e1 X fish",500,-10.,100.,"e1tof [ns]",500,-50.,50.,"e1z [mm]",sub_name);
		Hist->fill2("e1 Y fish",event->etof[0],event->ey[0],"e1 Y fish",500,-10.,100.,"e1tof [ns]",500,-50.,50.,"e1y [mm]",sub_name);
		Hist->fill2("e2 X fish",event->etof[1],event->ex[1],"e2 X fish",500,-10.,100.,"e2tof [ns]",500,-50.,50.,"e2z [mm]",sub_name);
		Hist->fill2("e2 Y fish",event->etof[1],event->ey[1],"e2 Y fish",500,-10.,100.,"e2tof [ns]",500,-50.,50.,"e2y [mm]",sub_name);
						
		Hist->fill2("e1 Wiggles",event->etof[0],sqrt(event->ex[0]*event->ex[0] + event->ey[0]*event->ey[0]),"e1 Wiggles",500,-10.,150.,"e1tof [ns]",500,0.,50.,"Radius [mm]",sub_name);
		Hist->fill2("e2 Wiggles",event->etof[1],sqrt(event->ex[1]*event->ex[1] + event->ey[1]*event->ey[1]),"e2 Wiggles",500,-10.,150.,"e2tof [ns]",500,0.,50.,"Radius [mm]",sub_name);

		Hist->fill2("r1x vs r1y",event->rx[0],event->ry[0],"Recoil Position",500,-75.,75.,"r1z [mm]",500,-75.,75.,"r1y [mm]",sub_name);
		Hist->fill2("r2x vs r2y",event->rx[1],event->ry[1],"Recoil Position",500,-75.,75.,"r2z [mm]",500,-75.,75.,"r2y [mm]",sub_name);
							
		Hist->fill2("e1z vs e1y",event->ex[0],event->ey[0],"Recoil Position",500,-50.,50.,"e1z [mm]",500,-50.,50.,"e1y [mm]",sub_name);
		Hist->fill2("e2x vs e2y",event->ex[1],event->ey[1],"Recoil Position",500,-75.,75.,"e2z [mm]",500,-75.,75.,"e2y [mm]",sub_name);
		
		Hist->fill1(" r1y " ,event->ry[0],"r1y ",500,-70.,70.,"rec1 y [mm]",sub_name);
		Hist->fill1(" r1x " ,event->rx[0],"r1z ",500,-70.,70.,"rec1 z [mm]",sub_name);
						
		Hist->fill1("reaction",event->reaction,"Reaction Flag",400,-1.,15.,"Reaction Number",sub_name);

		//gated to be in the center of y (low y momentum)
		if( fabs(event->ey[0]) <= 5. ){
			Hist->fill2("e1 X fish ygated",event->etof[0],event->ex[0],"e1 X fish ygated",150,30.,85.,"e1tof [ns]",150,-50.,50.,"e1z [mm]",sub_name);
		}
		//gated to be in the center of z (low z momentum)
		if( fabs(event->ex[0]) <= 5. ){
			Hist->fill2("e1 Y fish zgated",event->etof[0],event->ey[0],"e1 Y fish zgated",150,30.,85.,"e1tof [ns]",150,-50.,50.,"e1y [mm]",sub_name);
		}

		Hist->fill2("r1 X fish",event->rtof[0],event->rx[0],"r1 X fish",500,-10.,8000.,"r1tof [ns]",500,-70.,70.,"r1z [mm]",sub_name);
		Hist->fill2("r1 Y fish",event->rtof[0],event->ry[0],"r1 Y fish",500,-10.,8000.,"r1tof [ns]",500,-70.,70.,"r1y [mm]",sub_name);



		NTupleData[0]=idx;
		NTupleData[1]=thread_id;
		NTupleData[2]=event->reaction;
		NTupleData[3]=event->rx[0];

		WriteNTuple = false;
		if(WriteNTuple) {
			output_root_file->NTupleD("Data","test_file","idx:threadID:reation:rx",32000,NTupleData);
			output_root_file->EventsWrittenCounter();
		}
		delete event;
		if(	input_root_file->stop_reading) idx=num_events+1;  //exit
	}	

}
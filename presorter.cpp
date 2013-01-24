#include "OS_Version.h"
#include "presorter.h"
#include <math.h>
#include <string>
#include <unordered_map>

#include "event_data.h"

//#include "functions.h"
//#include "lowlevel_functions.h"

presorter_class::presorter_class()
{
	this->Number_Of_Electrons   = 0;
	this->Number_Of_Recoils     = 0;
	this->Number_Of_Projectiles = 0;
}


presorter_class::~presorter_class(){}

//void presorter_class::init(void * Ueber_, int Type, int Flag, Histo * Hist, double * parameter)
//{
//	this->Number_Of_Electrons   = 0;
//	this->Number_Of_Recoils     = 0;
//	this->Number_Of_Projectiles = 0;
//
//	this->Type = Type;
//	this->Flag = Flag;
//	this->H = Hist;
//	this->par = parameter;
//
//	sprintf(dir,"_presorter check/channel %d",Flag);
//}

void presorter_class::sayHi()
{
	printf("Hi!\n");
}

bool presorter_class::Run(string command, event_data * event, histo_handler * H, unordered_map<int, double> parameter )
{
	this->BMspacing = parameter[1010];

	if(command == "ELECTRONTOF")
		return electron_tof( event, H);

	//switch (Type) {

	//	case 0 :
	//		return ;//single_tof(elec,rec,tdc_ns,cnt);
	//		break;
	//	case 1 :
	//		return electron_tof(event_data * event, histo_handler * H);
	//		break;
	//	case 2 :
	//		return ;//pipico(elec,rec,tdc_ns,cnt);
	//		break;
	//	case 3 :
	//		return ;//three_ions(elec,rec,tdc_ns,cnt);
	//		break;
	//	case 4 :
	//		return ;//pipico_AA(elec,rec,tdc_ns,cnt);
	//		break;
	//	case 5 :
	//		return ;//proj_rec_tof(elec,rec,proj);
	//		break;
	//	case 6 :
	//		return;// proj_pipico_AA(elec,rec,proj);
	//		break;
	//	case 7 :
	//		return;// proj_pipico(elec,rec,proj);
	//		break;

	//	//xxxxm

	//	default:
	//		break;
	//}
	return false;
}
	



//////////////////////////   E L E C T R O N _ T O F   ///////////////////////////////////////
// electron TOF (no recoils!)

void presorter_class::init_electron_tof(double etof_center, double etof_width)
{
	this->etof_center = etof_center;
	this->etof_width = etof_width;
}

bool presorter_class::electron_tof(event_data * event, histo_handler * H)
{	// begin wiggle (only electron ist stored)
	//Number_Of_Electrons = 0;
	//Number_Of_Recoils = 0;
	//bool found = false;

	//

	//for (int j=0;j<(event->ehit);j++)
	//	{	// begin for: electrons
	//		H->fill1("electron tof (no prs)",fmod((event->emcp[j] - event->bunchmarker + BMspacing*1000),BMspacing),"Electron TOF",int(6*BMspacing),-1.5*BMspacing,1.5*BMspacing,"TOF [ns]",dir);
	//		if ( (fabs( (fmod((event->emcp[j] - event->bunchmarker + BMspacing*1000),BMspacing)) - etof_center) < etof_width ) ) // good electron
	//		{	// begin if: correct TOF found and electron
	//			found = true;
	//			Elec[Number_Of_Electrons++] = j;
	//			H->fill1("electron tof",fmod((event->emcp[j] - event->bunchmarker + BMspacing*1000),BMspacing),"Electron TOF",int(6*BMspacing),-1.5*BMspacing,1.5*BMspacing,"TOF [ns]",dir);
	//		}	// end if: correct TOF found and electron
	//}	// end for: electrons


	//if(event->rhit >0)
	//{
	//	Number_Of_Recoils = 1;
	//	Rec[0]=0;
	//}

	//return found;
	return true;
}	// end wiggle (only electron is stored)



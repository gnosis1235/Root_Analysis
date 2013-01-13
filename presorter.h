#ifndef PRESORTER_ALREADY_INCLUDED
	#define PRESORTER_ALREADY_INCLUDED

	#include "OS_Version.h"
	#include <math.h>
	#include "event_data.h"
	#include "Simple_Hist.h"
	#include <unordered_map>

	#define PRESORTERVERSION (0.1)

	class presorter_class
	{	
		private:

			int Type;
			double * par;

			string dir;
			double BMspacing;

			//single tof variables
			double etof_center;
			double etof_width;
			double rtof_center;
			double rtof_width;

			//additional pipico variables
			double mass1_amu;
			double charge1_au;
			double mass2_amu;
			double charge2_au;
			double efield_Vcm;
			double acceleration_cm;
			double efield2_Vcm;
			double acceleration2_cm;
			double pipico_width_ns;
			double shift_ns;

			//additional three ions variables
			double mass3_amu;
			double charge3_au;

			//if projectile detector is used
			double px_center;
			double py_center;
			double p_radius;

		public:

		
			int Flag;
			int Rec[32];
			int Elec[32];
			int Proj[32];
			int Number_Of_Electrons;
			int Number_Of_Recoils;
			int Number_Of_Projectiles;

		public:

			presorter_class();
			~presorter_class();

			//void init(int Type, int Flag,  double * parameter);
			void sayHi();
			bool Run(string command, event_data * event, histo_handler * H, unordered_map<int, double> parameter );

			//////////////////////////   E L E C T R O N _ T O F   ///////////////////////////////////////
			// electron TOF (no recoils!)
			void init_electron_tof(double etof_center, double etof_width);
			bool electron_tof(event_data * event, histo_handler * H);

	};	// end presorter_class

#endif
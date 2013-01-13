#ifndef FUNCTIONS_ALREADY_INCLUDED
	#define FUNCTIONS_ALREADY_INCLUDED

	#include "OS_Version.h"

	// **************************************************************
	// **
	// **  momentum calculation from t_0 (two field regions)
	// **
	// **************************************************************
	double tof2mom21(double tof_ns, double acc_mm, double drift_mm, double charge_au, double mass_au, double Efeld_Vpm, double  parameter[]);


	// **************************************************************
	// **
	// **  momentum calculation from t_0 (homogeneous acceleration)
	// **
	// **************************************************************
	double tof2mom(double tof_ns, double acc_mm, double Efield_Vpcm, double  parameter[]);
	

	// **************************************************************
	// **
	// ** position correction for electron (E-B-drift)
	// **
	// **************************************************************
	double elec_pos_corr(double e1x_mm, double e1y_mm, double e1t_ns, char direction, double  parameter[]);	


	// **************************************************************
	// **
	// ** calculate momenta for electron (magnetic field)
	// **
	// **************************************************************
	double elec2mom(double e1x_mm, double e1y_mm, double e1t_ns, char direction, double  parameter[]);


	// **************************************************************
	// **
	// ** find channels in PIPICO: t1 [ns], m1,m2 [au], q1,q2 [e], s[mm], fieldE [V/cm] 
	// **
	// **************************************************************
	double t2(double t1,double m1,double m2, double q1, double q2, double s, double fieldE, double  parameter[]); 


	// **************************************************************
	// **
	// ** find channels in PIPIPICO: t1,t2 [ns], m1,m2,m2 [u], q1,q2,q3 [e], s[mm], fieldE [V/cm] 
	// **
	// **************************************************************
	double t3(double t1, double t2, double m1,double m2, double m3, double q1, double q2, double q3, double s, double fieldE);

	// **************************************************************
	// **
	// ** calculate momenta for electron x,y direction (magnetic field)
	// ** using Mirko Hattass' functions.
	// **
	// **************************************************************

	double electron_px(double etof, double xe, double ye, double  parameter[]);
	double electron_py(double etof, double xe, double ye, double  parameter[]);


	// **************************************************************
	// **
	// ** calculate momenta for tof-direction /w McLaren geometry
	// **
	// **************************************************************
	double toftomom(double tof, double  parameter[]);



	// ********************************************************************
	// **
	// ** calculate angle of Delta_Phi between Recoil and projectile
	// **
	// ********************************************************************
	double deltaphi(double phi_projectile, double phi_molecule);


	// **************************************************************
	// **
	// ** Labframe transformation - 3 vectors are transformed into a new
	// ** frame of reference
	// **
	// **************************************************************
	void labframe_transformation(__int32 direction, double a[], double b[], double c[]);

#endif
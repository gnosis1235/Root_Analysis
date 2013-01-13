#ifndef LOWLEVEL_FUNCTIONS_ALREADY_DEFINED
	#define LOWLEVEL_FUNCTIONS_ALREADY_DEFINED

	//#include "DetStruct.h"
	#include "Ueberstruct.h"

	#define MUKG 1.66053886e-27			// u in Kg
	#define COULOMB	1.60217733E-19		// Elementary charge
	#define PI 3.14159265				// PI

	void calcPhi(Ueberstruct *Ueber, Det_struct *part);
	double cubiceq(double a, double b, double c);
	double T2_AA(double t1,double m1_amu,double m2_amu, double q1_au, double q2_au, double s1_mm, double fieldE1, double s2_mm, double fieldE2, double shift_ns);

	// **************************************************************
	// **
	// ** find channels in PIPICO: t1 [ns], m1,m2 [au], q1,q2 [e], s[mm], fieldE [V/cm], shift of tofs [ns] 
	// **
	// **************************************************************
	double CH_t2(double t1,double m1,double m2, double q1, double q2, double s, double fieldE, double shift_ns);

	//-------------------------------------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------
	// calculate TOF using different methods
	// using a projectile detector 
	//----------------------------------------------------------------------------------------------------
	void getTOFs(Ueberstruct *Ueber, Det_struct *elec, Det_struct *rec,  Det_struct *proj, int ref_particle);

	//----------------------------------------------------------------------------------------------------
	// calculate TOF using different methods
	// w/o using a projectile detector 
	//----------------------------------------------------------------------------------------------------
	void getTOFs(Ueberstruct *Ueber, Det_struct *elec, Det_struct *rec, double (*tdc_ns)[NUM_IONS],__int32 *cnt, int ref_elec, int bm_channel, double bm_spacing, double mod_shift);
	
	// no mod, e.g. for low rep.rate lasers etc.
	void getTOFs(Ueberstruct *Ueber, Det_struct *elec, Det_struct *rec, double (*tdc_ns)[NUM_IONS],__int32 *cnt, int ref_elec, int bm_channel);

	//----------------------------------------------------------------------------------------------------
	// calculate TOF (ions or electrons only)
	//----------------------------------------------------------------------------------------------------
	void getTOFs(Ueberstruct *Ueber, Det_struct *particle, double (*tdc_ns)[NUM_IONS],__int32 *cnt, int bm_channel);

	void CalcTOFs(Ueberstruct *Ueber, double (*tdc_ns)[NUM_IONS],__int32 *cnt, int ref_particle);

#endif
#include "lowlevel_functions.h"
#include <math.h>
#include "Ueberstruct.h"



void calcPhi(Ueberstruct* Ueber, Det_struct *part)
{
	if (!part) {
		Ueber->error = 1;
		printf("\nError 1 in function CalcPhi\n");
		return;
	}
	if (!part->use_this_detector) {
		Ueber->error = 2;
		printf("\nError 2 in function CalcPhi\n");
		return;
	}

	for(int i=0;i<part->number_of_reconstructed_hits;i++) {
		part->phi[i] = atan2(part->x[i],part->y[i])*180./3.14152;
	}
}


double cubiceq(double a, double b, double c) {
	double n, aa, bb, cc, q, r, d, s, t, z, z2, z3;

	n  = exp(((log(sqrt(a*a))+2*log(sqrt(b*b))+3*log(sqrt(c*c))))/14.);
	aa = a/n;
	bb = b/(n*n);
	cc = c/(n*n*n);

	q  = (3.*bb-aa*aa)/9.;
	r  = (9.*aa*bb-27.*cc-2.*aa*aa*aa)/54.;
	d  =  q*q*q + r*r;

	if(d<=0.) {
		t  = acos(r/sqrt(-q*q*q));
		z3 = 2.*sqrt(-q)*cos(t/3.) - aa/3; // real root #3 right most
		z2 = 2.*sqrt(-q)*cos((t+2.*PI)/3.) - aa/3.; // real root #2
		z  = 2.*sqrt(-q)*cos((t+4.*PI)/3.) - aa/3.; // real root  same if D=0
	}
	else {
		if((r+sqrt(d))>0.)
			s = pow(r + sqrt(d),1./3.);
		else
			s = -pow(-r - sqrt(d),1./3.);

		if((r-sqrt(d))>0.)
			t = pow(r - sqrt(d),1./3.);
		else
			t = -pow(-r + sqrt(d),1./3.);

			z = s + t - aa/3.;
	}

	return z*n;
}


double T2_AA(double t1,double m1_amu,double m2_amu, double q1_au, double q2_au, double s1_mm, double fieldE1, double s2_mm, double fieldE2, double shift_ns) {
	double E1 = fieldE1 * 100.0;  //  in v/m
	double E2 = fieldE2 * 100.0;  //  in v/m
	double s1 = s1_mm / 1000.0; //  in m
	double s2 = s2_mm / 1000.0; //  in m
	
	double m1 =  m1_amu * MUKG;
	double m2 =  m2_amu * MUKG;
	
	double q1 = q1_au * COULOMB;
	double q2 = q2_au * COULOMB;

	if (E2<0.01) E2=0.0001; // in case second "acceleration" region is a drift

	double a11   = E1 * q1 / m1;      // acc. first ion, first region
	double a12   = E2 * q1 / m1;      // acc. first ion, second region 
	double a21   = E1 * q2 / m2;      // acc. second ion, first region
	double a22   = E2 * q2 / m2;      // acc. second ion, second region 

	t1 = (t1 + shift_ns) * 1.E-9;  //  in s

	double t_reg1 = cubiceq(t1*(2*a12-a11)/(a11-a12),(2*s1+2*s2-a12*t1*t1)/(a11-a12),t1*2*s1/(a12-a11));

	double v1 =  s1/t_reg1 - a11 * t_reg1/2;
	double v2 = -m1 * v1 / m2;

	double t2 = (sqrt(v2*v2+2*a21*s1+2*a22*s2) - sqrt(v2*v2+2*a21*s1))/a22  + (sqrt(v2*v2+2*a21*s1)-v2)/a21;
	t2  *= 1.E9; // back to ns

	return t2;
}




// **************************************************************
// **
// ** find channels in PIPICO: t1 [ns], m1,m2 [au], q1,q2 [e], s[mm], fieldE [V/cm], shift of tofs [ns] 
// **
// **************************************************************

double CH_t2(double t1,double m1,double m2, double q1, double q2, double s, double fieldE, double shift_ns) {
	
	//convert to mks

	m1 = m1 / 1836. * 1.661e-27;
	m2 = m2 / 1836. * 1.661e-27;
	
	q1 = q1 * 1.60322e-19;
	q2 = q2 * 1.60322e-19;

	t1 = (t1 + shift_ns) * 1e-9;

	s = s / 1000.0; 
	fieldE = fieldE * 100.0;

	double z = s * m1 / (q2 * fieldE * t1) - q1 * t1 / (2. * q2);
	double t2 = z + sqrt(2.* s * m2 / (q2 * fieldE) + z * z);
	t2 = t2 * 1e+9;

	return t2;
}





//-------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
// calculate TOF using different methods
// using a projectile detector 
//----------------------------------------------------------------------------------------------------

void getTOFs(Ueberstruct*Ueber, Det_struct *elec, Det_struct *rec,  Det_struct *proj, int ref_particle)
{
	// electrons & recoils
	// calculate TOF with respect to projectile MCP

	if (!proj) {
		Ueber->error = 1;
		printf("\nError 1 in function getTOFs\n");
		return;
	}
	if (!proj->use_this_detector) {
		Ueber->error = 2;
		printf("\nError 2 in function getTOFs\n");
		return;
	}

	if(proj->number_of_reconstructed_hits > ref_particle) {
		if (!elec) {
			Ueber->error = 3;
			printf("\nError 3 in function getTOFs\n");
			return;
		}

		if (elec->use_this_detector) {
			for(int i=0;i<elec->number_of_reconstructed_hits;i++)
			{	// begin for: calculate electron TOFs
				elec->tof[i] = elec->time[i] - proj->time[ref_particle];
			}	// end for: calculate electron TOFs
		}

		if (!rec) {
			Ueber->error = 5;
			printf("\nError 5 in function getTOFs\n");
			return;
		}

		if (rec->use_this_detector) {
			for(int j=0;j<rec->number_of_reconstructed_hits;j++) {	// begin for: calculate recoil TOFs
				rec->tof[j] = rec->time[j] - proj->time[ref_particle];
			}	// end for: calculate recoil TOFs
		}
	} else {
		if (elec) {if (elec->use_this_detector) {for(int i=0;i<elec->number_of_reconstructed_hits;i++) elec->tof[i] = -1.e150;}}
		if (rec)  {if (rec->use_this_detector) {for(int i=0;i<rec->number_of_reconstructed_hits;i++) rec->tof[i] = -1.e200;}}
	}
}





//----------------------------------------------------------------------------------------------------
// calculate TOF using different methods
// without using a projectile detector
//----------------------------------------------------------------------------------------------------

void getTOFs(Ueberstruct* Ueber, Det_struct *elec, Det_struct *rec, double (*tdc_ns)[NUM_IONS],__int32 *cnt, int ref_elec, int bm_channel, double bm_spacing, double mod_shift = 0.0)
{
	// electrons
	// calculate tof with respect to bunchmarker/photodiode

	if (bm_channel < 0 || bm_channel >= Ueber->number_of_channels) {
		Ueber->error = 6;
		printf("\nError 6 in getTOF: no bunchmarker channel defined\n");
		return;
	}

	if (!elec) {
		Ueber->error = 7;
		printf("\nError 7 in function getTOFs\n");
		return;
	}
	if (!rec) {
		Ueber->error = 9;
		printf("\nError 9 in function getTOFs\n");
		return;
	}

	if(!cnt[bm_channel]) {
		if (elec->use_this_detector) {for(int i=0;i<elec->number_of_reconstructed_hits;i++) elec->tof[i] = -1.e150;}
		if (rec->use_this_detector)  {for(int i=0;i< rec->number_of_reconstructed_hits;i++) rec->tof[i]  = -1.e200;}
		return;
	}

	if (elec->use_this_detector) {
		for(int i=0;i<elec->number_of_reconstructed_hits;i++) {
			elec->tof[i] = fmod(elec->time[i] - tdc_ns[bm_channel][0] + mod_shift,bm_spacing);
			if(elec->tof[i] < 0.0) elec->tof[i] += bm_spacing;
		}
	}

	// recoils
	// calculate tof with respect to electron[ref_elec]
	// if the reference electron is not present in the event tof will be set to -1

	if (rec->use_this_detector && elec->use_this_detector) {
		if (elec->number_of_reconstructed_hits > ref_elec) {
			for(int i=0;i<rec->number_of_reconstructed_hits;i++) {
				rec->tof[i] = -elec->time[ref_elec] + rec->time[i] + elec->tof[ref_elec];
			}
		} else {
			for(int i=0;i<rec->number_of_reconstructed_hits;i++) rec->tof[i] = -1.e200;
		}
		return;
	}
	
	if (rec->use_this_detector && (!elec->use_this_detector)) {
		for(int i=0;i<rec->number_of_reconstructed_hits;i++) {
			rec->tof[i] = fmod(rec->time[i] - tdc_ns[bm_channel][0] + mod_shift,bm_spacing);
			if(rec->tof[i] < 0.0) rec->tof[i] += bm_spacing;
		}
		return;
	}
}





// no modulo, e.g. for low rep.rate lasers etc.
void getTOFs(Ueberstruct* Ueber, Det_struct *elec, Det_struct *rec, double (*tdc_ns)[NUM_IONS],__int32 *cnt, int ref_elec, int bm_channel)
{
	// electrons
	// calculate tof with respect to bunchmarker/photodiode

	if (bm_channel < 0 || bm_channel >= Ueber->number_of_channels) {
		Ueber->error = 6;
		printf("\nError 6 in getTOF: no bunchmarker channel defined\n");
		return;
	}

	if (!elec) {
		Ueber->error = 11;
		printf("\nError 11 in function getTOFs\n");
		return;
	}
	if (!rec) {
		Ueber->error = 12;
		printf("\nError 12 in function getTOFs\n");
		return;
	}

	if(!cnt[bm_channel]) {
		if (elec->use_this_detector) {for(int i=0;i<elec->number_of_reconstructed_hits;i++) elec->tof[i] = -1.e150;}
		if (rec->use_this_detector)  {for(int i=0;i< rec->number_of_reconstructed_hits;i++) rec ->tof[i] = -1.e200;}
		return;
	}

	if (elec->use_this_detector) {
		for(int i=0;i<elec->number_of_reconstructed_hits;i++) {
			elec->tof[i] = elec->time[i] - tdc_ns[bm_channel][0];
		}
	}

	// recoils
	// calculate tof with respect to bunchmarker/photodiode

	if (rec->use_this_detector) {
		for(int i=0;i<rec->number_of_reconstructed_hits;i++) {
			rec->tof[i] = rec->time[i] - tdc_ns[bm_channel][0];
		}
	}
}






//----------------------------------------------------------------------------------------------------
// calculate TOF (ions or electrons only) versus bunch marker
//----------------------------------------------------------------------------------------------------

void getTOFs(Ueberstruct* Ueber, Det_struct *particle, double (*tdc_ns)[NUM_IONS],__int32 *cnt, int bm_channel)
{
	if (bm_channel < 0 || bm_channel >= Ueber->number_of_channels) {
		Ueber->error = 6;
		printf("\nError 6 in getTOF: no bunchmarker channel defined\n");
		return;
	}

	if (!particle) {
		Ueber->error = 15;
		printf("\nError 15 in function getTOFs\n");
		return;
	}
	if (!particle->use_this_detector) {
		Ueber->error = 16;
		printf("\nError 16 in function getTOFs\n");
		return;
	}

	if(!cnt[bm_channel]) {
		if (particle->use_this_detector) {for(int i=0;i< particle->number_of_reconstructed_hits;i++) particle ->tof[i] = -1.e175;}
		return;
	}

	// calculate tof with respect to bunchmarker/photodiode:
	for(int i=0;i<particle->number_of_reconstructed_hits;i++) {
		particle->tof[i] = particle->time[i] - tdc_ns[bm_channel][0];
	}
}



void CalcTOFs(Ueberstruct *Ueber, double (*tdc_ns)[NUM_IONS],__int32 *cnt, int ref_particle)
{
	switch(Ueber->tofType)
	{
		case 0:
			if(Ueber->BMspacing>0)
				getTOFs(Ueber, Ueber->elec, Ueber->rec, tdc_ns, cnt, ref_particle, Ueber->BMchannel, Ueber->BMspacing, 0);
			else
				getTOFs(Ueber, Ueber->elec, Ueber->rec, tdc_ns, cnt, ref_particle, Ueber->BMchannel);
		break;

		case 1:
			getTOFs(Ueber, Ueber->elec,Ueber->rec,Ueber->proj,0);
		break;

		case 2:
			getTOFs(Ueber, Ueber->elec,Ueber->rec,Ueber->proj,ref_particle);
		break;
	}
}

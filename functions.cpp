#include "OS_Version.h"
#include <math.h>

#include "Ueberstruct.h"
#include "lowlevel_functions.h"


// **************************************************************
// **
// ** momentum calculation from t_0 (2:1 time-focusing geometry)
// **
// **************************************************************

double tof2mom21(double tof_ns, double acc_mm, double drift_mm, double charge_au, double mass_au, double Efeld_Vpm, double  parameter[])
{
	double s1 = acc_mm * 1e-3;
	double s2 = drift_mm * 1e-3;
	double t = tof_ns * 1e-9;
	double q = charge_au * 1.6022e-19;
	double mass = mass_au * 9.1095e-31;
	
	double a = Efeld_Vpm * q/mass;

	double v0 = -t*a + sqrt(2.*a*s1) + s2*sqrt(a/2.* s1);
	for (__int32 i=0;i<10;++i)
	{
		double test1Tof = -v0/a + sqrt(v0*v0 + 2.*a*s1)/a + s2/sqrt(v0*v0 + 2.*a*s1);
		double v1 = 1.01*v0;
		double test2Tof = -v1/a + sqrt(v1*v1 + 2.*a*s1)/a + s2/sqrt(v1*v1 + 2.*a*s1);
		double dtdvo = (test2Tof - test1Tof) / (v1-v0);
		v0 = v0 + 0.7*(t - test1Tof)/dtdvo;
	}

	double mom = v0 * mass/ (9.1095e-31*2.1877e6);
	return mom;
}


// **************************************************************
// **
// **  momentum calculation from t_0 (homogeneous acceleration)
// **
// **************************************************************

double tof2mom(double tof_ns, double acc_mm, double Efield_Vpcm, double  parameter[])
{
	double s1 = acc_mm * 1e-3;
	double t = tof_ns * 1e-9;
	double vau = 2.1877e+6;                              // unity velocity in atomic units [m/s]
	double mom;

	mom = (s1 / t - 0.5 * 1.7588e13*Efield_Vpcm / 1. * t ) / vau * 1.;

	return mom;
}
	
// **************************************************************
// **
// ** position correction for electron (E-B-drift)
// **
// **************************************************************

double elec_pos_corr(double e1x_mm, double e1y_mm, double e1t_ns, char direction, double  parameter[])
{
	double outputvalue=754653.;

	
	// position correction for different times (E-B-drift)
	double e1x_corr		= -1. * ((e1x_mm - ( parameter[355] + (parameter[360] - parameter[355]) * (e1t_ns - parameter[357]) / (parameter[362] - parameter[357]) ) ));
	double e1y_corr		= (e1y_mm - ( parameter[356] + (parameter[361] - parameter[356]) * (e1t_ns - parameter[357]) / (parameter[362] - parameter[357]) ) );
	
	if (direction=='x') {outputvalue=e1x_corr;}
	else {if (direction=='y') {outputvalue=e1y_corr;} else {outputvalue=85734.;} }

	return outputvalue;
}

	
// **************************************************************
// **
// ** calculate momenta for electron (magnetic field)
// **
// **************************************************************

double elec2mom(double e1x_mm, double e1y_mm, double e1t_ns, char direction, double  parameter[])
{
	double e_beta, epx1, epy1;
	double pi = 3.14159265359;
	double outputvalue=754653.;

	

	// magnetic field, nodes etc.
	double b_field		= 357.238755341 / fabs (parameter[357] - parameter[362]);										// magnetic field in Gauss from wiggle [ns]
	double e_alpha		= fmod((( (e1t_ns - parameter[357]) * 360. / fabs(parameter[357] - parameter[362])) + 720.), 360.);	// angle in gyration phase
	double rad_callib	= sqrt(2. - 2. * cos(e_alpha * pi / 180) );

	if (rad_callib >= parameter[353])
		{						// Begin IF: not in the node
			e_beta		= -1 * fmod( ((e_alpha/2.) +360.), 180. ) + parameter[352];				// additional detector rotation
			rad_callib	= 1/rad_callib * b_field / 124.38406963662;

			epx1	= ( cos(e_beta * pi / 180.) * (e1x_mm) - sin(e_beta * pi / 180.) * e1y_mm) * rad_callib;
			epy1	= ( sin(e_beta * pi / 180.) * (e1x_mm) + cos(e_beta * pi / 180.) * e1y_mm) * rad_callib;
		}						// End IF: not in the node

	
	if (direction=='x') {outputvalue=epx1;}
	else {if (direction=='y') {outputvalue=epy1;} else {outputvalue=85734.;} }


	return outputvalue;
}



// **************************************************************
// **
// ** find channels in PIPICO: t1 [ns], m1,m2 [au], q1,q2 [e], s[mm], fieldE [V/cm] 
// **
// **************************************************************

double t2(double t1,double m1,double m2, double q1, double q2, double s, double fieldE, double  parameter[], double shift_ns = 0.0) {
	
	return CH_t2(t1, m1, m2, q1, q2, s, fieldE, shift_ns);
}


double t3(double t1, double t2, double m1,double m2, double m3, double q1, double q2, double q3, double s, double fieldE) 
{
	//convert to mks

	m1 = m1 * 1.661e-27;
	m2 = m2 * 1.661e-27;
	m3 = m3 * 1.661e-27;
	
	q1 = q1 * 1.60322e-19;
	q2 = q2 * 1.60322e-19;
	q3 = q3 * 1.60322e-19;
  
	t1 = t1 * 1e-9;
	t2 = t2 * 1e-9;

	s = s / 1000.0; 
	fieldE = fieldE * 100.0;

	double t3 = 1./(2.*fieldE*t1*t2*q3)*(-fieldE*t1*t1*t2*q1-fieldE*t1*t2*t2*q2+2.*m1*t2*s+2.*m2*t1*s+sqrt((-fieldE*t1*t1*t2*q1-fieldE*t1*t2*t2*q2+2.*m1*t2*s+2.*m2*t1*s)*(-fieldE*t1*t1*t2*q1-fieldE*t1*t2*t2*q2+2.*m1*t2*s+2.*m2*t1*s)+8*fieldE*m3*t1*t1*t2*t2*q3*s));
	//printf("%f\n",t3*1e+9);
	return t3 * 1e+9;
}


// **************************************************************
// **
// ** calculate momenta for electron x,y direction (magnetic field)
// ** using Mirko's functions.
// **
// **************************************************************

double electron_px(double etof, double xe, double ye, double  parameter[]) {

	double pex,pey,qe,w,me,a,b,pau,wigglepos;

	me = 9.1083E-31;
	qe = 1.602E-19;
	pau = me*300.e6/137.;

	double fieldB = parameter[1080] / 10000.;
	double fieldBns = parameter[1081];


	wigglepos =  fieldBns * 1e-9;
	if(1==0)fieldB = 2.*me*3.14152 / (qe * wigglepos);

	w = qe / me * fieldB;
	a = (1. - cos(w * etof*1.e-9)) / w;
	b = (sin(w * etof*1.e-9)) / w;

	pey = me * (-xe/1000. * a - b*ye/1000.) / (a*a + b*b);
	pex = me * (xe/1000. * b - a*ye/1000.) / (a*a + b*b);
	pex = pex / pau;
	pey = pey / pau;
	return pex;
}
double electron_py(double etof, double xe, double ye, double  parameter[]) {

	double pex,pey,qe,w,me,a,b,pau,wigglepos;

	me = 9.1083E-31;
	qe = 1.602E-19;
	pau = me*300.e6/137.;

	double fieldB = parameter[1080] / 10000.;
	double fieldBns = parameter[1081];

	wigglepos =  fieldBns * 1e-9;
	if(1==0)fieldB = 2.*me*3.14152 / (qe * wigglepos);

	w = qe / me * fieldB;
	a = (1. - cos(w * etof*1.e-9)) / w;
	b = (sin(w * etof*1.e-9)) / w;

	pey = me * (-xe/1000. * a - b*ye/1000.) / (a*a + b*b);
	pex = me * (xe/1000. * b - a*ye/1000.) / (a*a + b*b);
	pex = pex / pau;
	pey = pey / pau;
	return pey;
}


// **************************************************************
// **
// ** calculate momenta for tof-direction /w McLaren geometry
// **
// **************************************************************
double toftomom(double tof, double  parameter[]) {

// *******************************************
// this is lothars subroutine
// ********************************************

	double s1 = parameter[1300] / 1000.;
	double s2 = parameter[1301] / 1000.;

	double a = 1.7588E13 * parameter[1070];
	double etof = tof * 1.e-9;


	double vo = - etof*a + sqrt(2.*a*s1) + s2*sqrt(a/2./s1);

	double dt = 10.;               

	for(__int32 i=1;i<7;i++) {
		
		if((vo*vo + 2.*a*s1) > 1e-9) {
    
			double test1tof = -vo/a +  sqrt(vo*vo + 2.*a*s1 )/a + s2/sqrt(vo*vo + 2.*a*s1);
			double v1 = 1.01 * vo;
			double test2tof = -v1/a + sqrt(v1*v1 + 2.*a*s1 )/a + s2/sqrt(v1*v1 + 2.*a*s1);
			double dtdvo = (test2tof - test1tof) / (v1 - vo);
			vo = vo + 0.7 * (etof - test1tof) / dtdvo;
		}
	}
	
	return vo/2.18E6;
}





// ********************************************************************
// **
// ** calculate angle of Delta_Phi between Recoil and projectile
// **
// ********************************************************************

double deltaphi(double phi_projectile, double phi_molecule)
{
	double phi_p = phi_projectile + 180.;
	double phi_m = phi_molecule + 180.;
	double relphi;

	relphi = phi_p - phi_m;
		if (phi_p > phi_m) {
			if (relphi > 180.) {
				relphi = relphi - 360.;
			}
		}else {
			if (phi_p <= phi_m) {
				if (relphi < -180.) {
					relphi = relphi + 360.;
				}
			}
		}
			
	return relphi;
}


// ********************************************************************
// **
// ** Labframe transformation - 3 vectors are transformed into a new
// ** frame of reference
// **
// ********************************************************************

void labframe_transformation(__int32 direction, double a[], double b[], double c[]) {

//   Author: Achim Czasch, email: czasch@atom.uni-frankfurt.de
//
//   Function: the 3 vectors are transformed into a new frame of reference.
//             The new frame of reference is defined in the following way:
//             Vector  a  becomes the x-axis (|a|,0,0) of the new frame of reference.
//             The y-axis of the new system lies in the half plane which is spanned
//             by a and b. (It is not necessarily parallel to b)
//             The orientation of the new system is in accordance to the right-hand-rule:
//             Thumb is vector a. The new y-axis is in the half-plane which is spanned
//             by thumb and index-finger. The middle finger points in the direction of the
//             the z-axis.
//             The first argument (direction )can be +1 or -1. In case of -1 the inverse transformation
//             is performed.


	double norm_vector_x[3];
	double norm_vector_y[3];
	double norm_vector_z[3];

	double n;
	__int32 i;

	double transformation_matrix[3][3];
	
	n=sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
	norm_vector_x[0] = a[0]/n;
	norm_vector_x[1] = a[1]/n;
	norm_vector_x[2] = a[2]/n;

	norm_vector_z[0] = -b[1]*a[2]+b[2]*a[1];
	norm_vector_z[1] = -b[2]*a[0]+b[0]*a[2];
	norm_vector_z[2] = -b[0]*a[1]+b[1]*a[0];

	n=sqrt(norm_vector_z[0]*norm_vector_z[0] + norm_vector_z[1]*norm_vector_z[1] + norm_vector_z[2]*norm_vector_z[2]);
	norm_vector_z[0] = norm_vector_z[0]/n;
	norm_vector_z[1] = norm_vector_z[1]/n;
	norm_vector_z[2] = norm_vector_z[2]/n;

	norm_vector_y[0] = -a[1]*norm_vector_z[2]+a[2]*norm_vector_z[1];
	norm_vector_y[1] = -a[2]*norm_vector_z[0]+a[0]*norm_vector_z[2];
	norm_vector_y[2] = -a[0]*norm_vector_z[1]+a[1]*norm_vector_z[0];

	n=sqrt(norm_vector_y[0]*norm_vector_y[0] + norm_vector_y[1]*norm_vector_y[1] + norm_vector_y[2]*norm_vector_y[2]);
	norm_vector_y[0] = norm_vector_y[0]/n;
	norm_vector_y[1] = norm_vector_y[1]/n;
	norm_vector_y[2] = norm_vector_y[2]/n;

	for (i=0;i<3;i++) transformation_matrix[0][i] = norm_vector_x[i];
	for (i=0;i<3;i++) transformation_matrix[1][i] = norm_vector_y[i];
	for (i=0;i<3;i++) transformation_matrix[2][i] = norm_vector_z[i];


//         If direction =  +1: This transforms into the new system
//         If direction =  -1: Reverse transformation

	
	double vec_new[3];

	if (direction==-1) {
			vec_new[0]=a[0]*transformation_matrix[0][0]+a[1]*transformation_matrix[1][0]+a[2]*transformation_matrix[2][0];
			vec_new[1]=a[0]*transformation_matrix[0][1]+a[1]*transformation_matrix[1][1]+a[2]*transformation_matrix[2][1];
			vec_new[2]=a[0]*transformation_matrix[0][2]+a[1]*transformation_matrix[1][2]+a[2]*transformation_matrix[2][2];
			a[0] = vec_new[0];	a[1] = vec_new[1];	a[2] = vec_new[2];

			vec_new[0]=b[0]*transformation_matrix[0][0]+b[1]*transformation_matrix[1][0]+b[2]*transformation_matrix[2][0];
			vec_new[1]=b[0]*transformation_matrix[0][1]+b[1]*transformation_matrix[1][1]+b[2]*transformation_matrix[2][1];
			vec_new[2]=b[0]*transformation_matrix[0][2]+b[1]*transformation_matrix[1][2]+b[2]*transformation_matrix[2][2];
			b[0] = vec_new[0];	b[1] = vec_new[1];	b[2] = vec_new[2];

			vec_new[0]=c[0]*transformation_matrix[0][0]+c[1]*transformation_matrix[1][0]+c[2]*transformation_matrix[2][0];
			vec_new[1]=c[0]*transformation_matrix[0][1]+c[1]*transformation_matrix[1][1]+c[2]*transformation_matrix[2][1];
			vec_new[2]=c[0]*transformation_matrix[0][2]+c[1]*transformation_matrix[1][2]+c[2]*transformation_matrix[2][2];
			c[0] = vec_new[0];	c[1] = vec_new[1];	c[2] = vec_new[2];
	} else {
		if (direction==+1) {
				vec_new[0]=a[0]*transformation_matrix[0][0]+a[1]*transformation_matrix[0][1]+a[2]*transformation_matrix[0][2];
				vec_new[1]=a[0]*transformation_matrix[1][0]+a[1]*transformation_matrix[1][1]+a[2]*transformation_matrix[1][2];
				vec_new[2]=a[0]*transformation_matrix[2][0]+a[1]*transformation_matrix[2][1]+a[2]*transformation_matrix[2][2];
				a[0] = vec_new[0];	a[1] = vec_new[1];	a[2] = vec_new[2];

				vec_new[0]=b[0]*transformation_matrix[0][0]+b[1]*transformation_matrix[0][1]+b[2]*transformation_matrix[0][2];
				vec_new[1]=b[0]*transformation_matrix[1][0]+b[1]*transformation_matrix[1][1]+b[2]*transformation_matrix[1][2];
				vec_new[2]=b[0]*transformation_matrix[2][0]+b[1]*transformation_matrix[2][1]+b[2]*transformation_matrix[2][2];
				b[0] = vec_new[0];	b[1] = vec_new[1];	b[2] = vec_new[2];

				vec_new[0]=c[0]*transformation_matrix[0][0]+c[1]*transformation_matrix[0][1]+c[2]*transformation_matrix[0][2];
				vec_new[1]=c[0]*transformation_matrix[1][0]+c[1]*transformation_matrix[1][1]+c[2]*transformation_matrix[1][2];
				vec_new[2]=c[0]*transformation_matrix[2][0]+c[1]*transformation_matrix[2][1]+c[2]*transformation_matrix[2][2];
				c[0] = vec_new[0];	c[1] = vec_new[1];	c[2] = vec_new[2];
		} else {
				a[0] = 0.; a[1] = 0.; a[2] = 0.;
				b[0] = 0.; b[1] = 0.; b[2] = 0.;
				c[0] = 0.; c[1] = 0.; c[2] = 0.;
		}
	}
}

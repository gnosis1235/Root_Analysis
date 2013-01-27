#ifndef event_data_ALREADY_INCLUDED
	#define event_data_ALREADY_INCLUDED

#include <vector>

struct event_data{
	double reaction, ehit, rhit, phit, bunchmarker;
	double rx[5],ry[5],rmcp[5],rtof[5],rflag[5];
	double ex[6],ey[6],emcp[6],etof[6],eflag[6];
	double px[5],py[5],pmcp[5],ptof[5],pflag[5];


};

//class event_data{
//
//public:
//	double reaction; 
//	double ehit; 
//	double rhit; 
//	double phit; 
//	double bunchmarker;
//	double * rx;
//	double * ry;
//	double * rmcp;
//	double * rtof;
//	double * rflag;
//	double * ex;
//	double * ey;
//	double * emcp;
//	double * etof;
//	double * eflag;
//	double * px;
//	double * py;
//	double * pmcp;
//	double * ptof;
//	double * pflag;
//
//	int numRec, numElec, numProj;
//
//	event_data(int numRec,int numElec, int numProj){
//		if(numRec>0){
//			rx	   = new double[numRec];
//			ry	   = new double[numRec];
//			rmcp  = new double[numRec];
//			rtof  = new double[numRec];
//			rflag = new double[numRec];
//		}
//		if(numElec>0){
//			ex	   = new double[numElec];
//			ey	   = new double[numElec];
//			emcp  = new double[numElec];
//			etof  = new double[numElec];
//			eflag = new double[numElec];
//		}
//		if(numProj>0){
//			px	   = new double[numProj];
//			py	   = new double[numProj];
//			pmcp  = new double[numProj];
//			ptof  = new double[numProj];
//			pflag = new double[numProj];
//		}
//	}
//	
//
//};

#endif


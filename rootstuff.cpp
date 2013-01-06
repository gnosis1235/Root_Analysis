// rootstuff.cpp: implementation of the rootstuff class.
//////////////////////////////////////////////////////////////////////

#pragma warning(disable : 4996)
#pragma warning(disable : 4800)

/////////////////////////////////////////////////////////////////////////

// Project/Settings/C++/Precompiled Headers/Not using precompiled Headers !!!!

/////////////////////////////////////////////////////////////////////////
#include "OS_Version.h"

#include "stdlib.h"
#include "rootstuff.h"
//#include "TApplication.h"
//#include "TCanvas.h"
//#include "TF1.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
//#include "TMinuit.h"

//#include "TBrowser.h"
#include "TNtuple.h"

//#include "afx.h"
//#include "TKey.h" //Must be after afx include since they both have a GetClassName member!



////////////////////////////////////////////////////////////////
//For ROOT//////////////////////////////////////////////////////
//#include "TROOT.h"
//extern void InitGui();
//VoidFuncPtr_t initfuncs[] = { InitGui, 0 };
//TROOT root("rint", "The ROOT Interactive Interface", initfuncs);
////////////////////////////////////////////////////////////////



rootstuff::rootstuff()
{
}



rootstuff::~rootstuff()
{
}



TTree * rootstuff::OpenRootFileGetTree(const char *name,const char *TreeName)
{
    TFile * m_RootFile = new TFile(name,"READ");

	if (m_RootFile->IsZombie()) {
		return 0;
	}

    TTree * m_RootTree = (TTree*)m_RootFile->Get(TreeName);
	return m_RootTree;
}


TFile * rootstuff::RecreateRootFile(const char *name,const char *comment)
{
    TFile * m_RootFile = new TFile(name,"RECREATE",comment);
	return m_RootFile;
}

TCanvas * rootstuff::newCanvas(char *name,char *titel,__int32 xposition,__int32 yposition,__int32 pixelsx,__int32 pixelsy)
{
	TCanvas * canvaspointer;
	canvaspointer = new TCanvas(name,titel,xposition,yposition,pixelsx,pixelsy);
	return canvaspointer;
}

TNtuple * rootstuff::newNTuple(char *name, char * title, char *varlist, __int32 buffersize)
{
   TNtuple * nTuple = new TNtuple(name,title,varlist,buffersize);
   

   return nTuple;
}




TH1D * rootstuff::newTH1D(__int32 number,char *comment,__int32 bins,double xmin,double xmax,char *titelx,char *option)
{
   char name[200];
   TH1D * hist;
   sprintf(name,"%i",number);
   hist = new TH1D(name,comment,bins,xmin,xmax);
   hist->SetOption(option);
   hist->GetXaxis()->SetTitle(titelx);
   return hist;
}



TH1D * rootstuff::newTH1D(char *name,char *comment,__int32 bins,double xmin,double xmax,char *titelx,char *option)
{
   TH1D * hist;
   hist = new TH1D(name,comment,bins,xmin,xmax);
   hist->SetOption(option);
   hist->GetXaxis()->SetTitle(titelx);
   return hist;
}


TH2D * rootstuff::newTH2D(__int32 number,char *comment,__int32 xbins,double xmin,double xmax,char *titelx,__int32 ybins,double ymin,double ymax,char *titely,char *option)
{
   char name[200];
   TH2D * hist;
   sprintf(name,"%i",number);
   hist = new TH2D(name,comment,xbins,xmin,xmax,ybins,ymin,ymax);
   hist->SetOption(option);
   hist->GetXaxis()->SetTitle(titelx);
   hist->GetYaxis()->SetTitle(titely);
   return hist;
}


TH2D * rootstuff::newTH2D(char *name,char *comment,__int32 xbins,double xmin,double xmax,char *titelx,__int32 ybins,double ymin,double ymax,char *titely,char *option)
{
   TH2D * hist;
   hist = new TH2D(name,comment,xbins,xmin,xmax,ybins,ymin,ymax);
   hist->SetOption(option);
   hist->GetXaxis()->SetTitle(titelx);
   hist->GetYaxis()->SetTitle(titely);
   return hist;
}


#include "Simple_Hist.h"
#include <string>
#include <vector>
#include <thread>
#include <mutex>
//using namespace std;


H1i::H1i(string Name, string Title,int N_bins, double XMin, double XMax, string X_axis_label){
	std::lock_guard<std::mutex> guard(mutex); //auto lock thread
	n_bins = N_bins;
	xMax = XMax; 
	xMin = XMin;
	title = Title;
	name = Name;
	x_axis_label = X_axis_label;
	vector<int> bins(n_bins); //initialise all bins to 0
	int counter=0;
	for (vector<int>::iterator it = bins.begin(); it != bins.end(); it++)
	{
		bins[counter]=0;
		//printf("bin%i:%i   ",counter,bins[counter]);
		counter++;
	}
	printf("\nnumber of bins=%i\n",bins.size());
	//bins;
 }

void H1i::fill1(double x){
	std::lock_guard<std::mutex> guard(mutex); //auto lock thread
	int bin = 1 + int (n_bins*(x-xMin)/(xMax-xMin) );
	printf("here\n");
	bins[bin] = 1 + bins[bin];
		printf("\n");
	int counter=0;
	//for(vector<int>::iterator it = bins.begin(); it != bins.end(); ++it) {
	for (vector<int>::iterator it = bins.begin(); it != bins.end(); it++)
	{
		printf("bin%i:%i   ",counter,bins[counter]);
		counter++;
	}
	printf("\n");
	return;
}

//vector  * H1i::get_bins(){
//	return &bins;
//}
void H1i::print(){
	std::lock_guard<std::mutex> guard(mutex); //auto lock thread
	printf("\n");
	int counter=0;
	//for(vector<int>::iterator it = bins.begin(); it != bins.end(); ++it) {
	for (vector<int>::iterator it = bins.begin(); it != bins.end(); it++)
	{
		printf("bin%i:%i   ",counter,bins[counter]);
		counter++;
	}
	printf("\n");
	return;
}


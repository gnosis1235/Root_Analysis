#include "Simple_Hist.h"
#include <string>
#include <vector>
#include "console.h"
#include <map>
//#include <thread>
//#include <mutex>

using namespace std;


////////////////////////////////////////////  axis  ////////////////////////////////////////////
axis::axis(string Name, string Title,int N_bins, double Min, double Max){
	//std::lock_guard<std::mutex> guard(mutex); //auto lock thread
	n_bins = N_bins;
	max = Max; 
	min = Min;
	title = Title;
	name = Name;
	overflow =0;
	underflow =0;

	return;
 }
axis::~axis(){

}

int axis::get_bin_address(double x){
	//std::lock_guard<std::mutex> guard(mutex); //auto lock thread
	if(x>max){
		overflow++;
		return -1;
	}
	if(x<min){
		underflow++;
		return -1;
	}
	int bin = int (n_bins*(x-min)/(max-min) );
	//bins[bin] = 1 + bins[bin];
	return bin;
}
////////////////////////////////////////////  end  ////////////////////////////////////////////


////////////////////////////////////////////  H1i  ////////////////////////////////////////////


H1i::H1i(string Name, string Title,int N_bins, double Min, double Max, string X_axis_label, string Dir){
		
	title = Title;
	name = Name;
	dir = Dir;

	Xaxis = new axis("xaxis", X_axis_label, N_bins, Min, Max );

	//initialise all bins to 0
	for (int i=0; i<N_bins; i++)
	{
		bins.push_back(0);
		//printf("bin%i:%i   ",i,bins[i]);
	}
}

H1i::~H1i(){}

void H1i::fill(double x){
	int bin_id = Xaxis->get_bin_address(x);
	printf("max%f:\n",Xaxis->max);
	printf("min%f:\n",Xaxis->min);
	if (bin_id<0){
		printf("bin_id%i:",bin_id);
		return;
	}
	++bins[bin_id];
}

//vector  * H1i::get_bins(){
//	return &bins;
//}

void H1i::print_bin_contents(){
	//std::lock_guard<std::mutex> guard(mutex); //auto lock thread
	printf("\n");
	printf("\nnumber of bins=%i\n",bins.size());
	double bin_width = (Xaxis->max-Xaxis->min)/Xaxis->n_bins;
	printf("bin widh=%f\n", bin_width);
	for (int i=0; i<((signed int)bins.size()); i++)
	{
		if (bins[i]!=0)	Red(true);
			printf("%2.2f=<(bin%i)<%2.2f:%i   ",(double)i*bin_width+Xaxis->min,i,((double)i+1.)*bin_width+Xaxis->min,bins[i]);
		if (bins[i]!=0)	White(false);
	
		if((i+1)%10==0 ) printf("\n");
		
	}
	
	printf("\n");
	return;
}


////////////////////////////////////////////  end  ////////////////////////////////////////////


////////////////////////////////////////////  histo_container  ////////////////////////////////////////////

histo_container::histo_container(){
	string Name = "test1";
	string Name2 = "test2";

	H1i temp = H1i("test1", "test title", 100, 0., 10., "testx", "test_dir");
	Table_1d_hists["test1"] = 1;
	//std::map<std::string, double>  test = { {Name, 5.5} , {Name2, 5.5}};
	//map<int, int> m = {{1,2}, {3,4}, {5,6}, {7,8}};
}

//histo_container::~histo_container(){
//
//}
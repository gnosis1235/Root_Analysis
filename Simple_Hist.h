#include <string>
#include <vector>
#include <map>
//#include <thread>
//#include <mutex>

using namespace std;


class axis
{
//	std::mutex mutex;


public:
	int n_bins;

	double max, min;
	string title;
	string name;
	string axis_label;
	__int32 overflow;
	__int32 underflow;

	axis(string Name, string Title,int N_bins, double Min, double Max);
	~axis();

	int get_bin_address(double x);

	//vector * get_bins();
	//void print_bin_contents();

};

/////////////////////////////////////////////////////
//1d histogram of ints (so no weighting is allows)
class H1i 
{

//	std::mutex mutex;
	//int n_bins;
	
	axis * Xaxis;
	vector<int> bins;
	string title;
	string name;
	string dir;
public:
	H1i(string Name, string Title,int N_bins, double Min, double Max, string X_axis_label, string Dir);
	~H1i();

	

	//vector * get_bins();
	void print_bin_contents();
	void H1i::fill(double x);
};



///////////////////////////////////////////////////////
//1d histogram of ints (so no weighting is allows)
class histo_container
{
	map<string, int>  Table_1d_hists;
	string name;
public:
	histo_container();
	//~histo_container();

};
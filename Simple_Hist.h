#include <string>
#include <vector>
#include <unordered_map> 

//#include <thread>
//#include <mutex>
//#include <functional> // for std::hash<int>
using namespace std;


class axis
{
//	std::mutex mutex;


public:
	int n_bins;

	double max, min;
	string title;
	string name;
	//string axis_label;
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
	
	string title;
	string name;
	string dir;
public:
	H1i();
	H1i(string Name, string Title,int N_bins, double Min, double Max, string X_axis_label, string Dir);
	~H1i();

	bool operator == (const H1i& rhs) const {
		return this->name == rhs.name;
	} 
	vector<int> bins;
	//vector * get_bins();
	void print_bin_contents();
	void H1i::fill(double x);
	bool match(string NAME, string TITLE, int N_BINS, double MIN, double MAX, string X_LABEL, string DIR);
	void print_info();
	
};




///////////////////////////////////////////////////////
//1d histogram of ints (so no weighting is allows)
class histo_handler
{
	string name;
	int size_h1i;

public:
	histo_handler();
	//~histo_handler();

	void fill1(string NAME, double x, string TITLE, int N_BINS, double MIN, double MAX, string X_LABEL, string DIR);
	unordered_map<string, H1i*> h1i_map;
	void combine_hist(H1i * newhist);
};
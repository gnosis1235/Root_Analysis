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

	inline __int32 get_overflow(){return overflow;}
	inline __int32 get_underflow(){return underflow;}
	inline void set_overflow(__int32 value){ overflow = value;}
	inline void set_underflow(__int32 value){ underflow = value;}

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
		return this->name == rhs.name && rhs.title==this->title && rhs.Xaxis->n_bins==this->Xaxis->n_bins && rhs.Xaxis->min==this->Xaxis->min && rhs.Xaxis->max==this->Xaxis->max && rhs.Xaxis->title==this->Xaxis->title && rhs.dir==this->dir;
	} 
	vector<int> bins;
	//vector * get_bins();
	void print_bin_contents();
	void H1i::fill(double x);
	bool match(string NAME, string TITLE, int N_BINS, double MIN, double MAX, string X_LABEL, string DIR);
	void print_info();


	inline string get_name(){return name;}
	inline string get_dir(){return dir;}	
	inline string get_title(){return title;}	
	inline string get_X_title(){return Xaxis->title;}	
	inline double get_X_max(){return Xaxis->max;}	
	inline double get_X_min(){return Xaxis->min;}	
	inline int	  get_X_n_bins(){return Xaxis->n_bins;}

	inline __int32 get_X_overflow(){return Xaxis->overflow;}
	inline __int32 get_X_underflow(){return Xaxis->underflow;}
	inline void set_X_overflow(__int32 value){ Xaxis->overflow = value;}
	inline void set_X_underflow(__int32 value){ Xaxis->underflow = value;}

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
	unordered_map<string, H1i*>::iterator h1i_map_iterator;
	void combine_hist(H1i * hist2);
};
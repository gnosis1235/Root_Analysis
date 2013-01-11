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
	axis * Xaxis;
	
	string title;
	string name;
	string dir;
public:
	H1i();
	H1i(string Name, string Title,int N_bins, double Min, double Max, string X_axis_label, string Dir);
	~H1i();

	//bool operator == (const H1i& rhs) const {
	//	return this->name == rhs.name && rhs.title==this->title && rhs.Xaxis->n_bins==this->Xaxis->n_bins && rhs.Xaxis->min==this->Xaxis->min && rhs.Xaxis->max==this->Xaxis->max && rhs.Xaxis->title==this->Xaxis->title && rhs.dir==this->dir;
	//} 
	vector<int> bins;
	//vector * get_bins();
	void print_bin_contents();
	void fill(double x);
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


/////////////////////////////////////////////////////
//1d histogram of ints (so no weighting is allows)
class H2i 
{
	axis * Xaxis;
	axis * Yaxis;
	string title;
	string name;
	string dir;
public:
	H2i();
	H2i(string Name, string Title,int X_N_bins, double X_Min, double X_Max, string X_axis_label,int Y_N_bins, double Y_Min, double Y_Max, string Y_axis_label, string Dir);
	~H2i();


	vector<vector<int>> bins;

	//void print_bin_contents();
	void fill(double x, double y);
	bool match(string NAME, string TITLE, int X_N_BINS, double X_MIN, double X_MAX, string X_LABEL, int Y_N_BINS, double Y_MIN, double Y_MAX, string Y_LABEL,  string DIR);
	void print_info();


	inline string get_name(){return				name;}
	inline string get_dir(){return				dir;}	
	inline string get_title(){return			title;}	

	inline string get_X_title(){return			Xaxis->title;}	
	inline double get_X_max(){return			Xaxis->max;}	
	inline double get_X_min(){return			Xaxis->min;}	
	inline int	  get_X_n_bins(){return			Xaxis->n_bins;}

	inline __int32 get_X_overflow(){return		Xaxis->overflow;}
	inline __int32 get_X_underflow(){return		Xaxis->underflow;}
	inline void set_X_overflow(__int32 value){	Xaxis->overflow = value;}
	inline void set_X_underflow(__int32 value){ Xaxis->underflow = value;}

	inline string get_Y_title(){return			Yaxis->title;}	
	inline double get_Y_max(){return			Yaxis->max;}	
	inline double get_Y_min(){return			Yaxis->min;}	
	inline int	  get_Y_n_bins(){return			Yaxis->n_bins;}

	inline __int32 get_Y_overflow(){return		Yaxis->overflow;}
	inline __int32 get_Y_underflow(){return		Yaxis->underflow;}
	inline void set_Y_overflow(__int32 value){	Yaxis->overflow = value;}
	inline void set_Y_underflow(__int32 value){ Yaxis->underflow = value;}


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

	unordered_map<string, H1i*> h1i_map;
	unordered_map<string, H1i*>::iterator h1i_map_iterator;
	void fill1(string NAME, double x, string TITLE, int N_BINS, double MIN, double MAX, string X_LABEL, string DIR);
	void combine_hist(H1i * hist2);

	unordered_map<string, H2i*> h2i_map;
	unordered_map<string, H2i*>::iterator h2i_map_iterator;
	void fill2(string NAME, double x, double y, string TITLE, int X_N_BINS, double X_MIN, double X_MAX, string X_LABEL, int Y_N_BINS, double Y_MIN, double Y_MAX, string Y_LABEL, string DIR);
	void combine_hist(H2i * hist2);
};
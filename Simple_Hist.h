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
	char * title;
	char * name;
	//string axis_label;
	__int32 overflow;
	__int32 underflow;

	axis(char * Name, char * Title,int N_bins, double Min, double Max);
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

	
	char * title;
	char * name;
	char * dir;

public:
	H1i();
	H1i(char * Name, char * Title,int N_bins, double Min, double Max, char * X_axis_label, char * Dir);
	~H1i();

	char * key;

	//bool operator == (const H1i& rhs) const {
	//	return this->name == rhs.name && rhs.title==this->title && rhs.Xaxis->n_bins==this->Xaxis->n_bins && rhs.Xaxis->min==this->Xaxis->min && rhs.Xaxis->max==this->Xaxis->max && rhs.Xaxis->title==this->Xaxis->title && rhs.dir==this->dir;
	//} 
	axis * Xaxis;
	float * bins;
	//vector<int> bins;
	void print_bin_contents();
	void fill(double x);
//	bool match(string NAME, string TITLE, int N_BINS, double MIN, double MAX, string X_LABEL, string DIR);
	void print_info();


	//inline string get_name(){return name;}
	//inline string get_dir(){return dir;}	
	//inline string get_title(){return title;}	
	//inline string get_X_title(){return Xaxis->title;}	
	inline char * get_name(){return name;}
	inline char * get_dir(){return dir;}	
	inline char * get_title(){return title;}	
	inline char * get_X_title(){return Xaxis->title;}	

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
	char * title;
	const	char * name;
	char * dir;

public:
	H2i();
	H2i(const char * Name, char * Title,int X_N_bins, double X_Min, double X_Max, char * X_axis_label,int Y_N_bins, double Y_Min, double Y_Max, char * Y_axis_label, char * Dir);
	~H2i();

		char * key;
	vector<vector<int>> bins;

	//void print_bin_contents();
	void fill(double x, double y);
	//bool match(string NAME, string TITLE, int X_N_BINS, double X_MIN, double X_MAX, string X_LABEL, int Y_N_BINS, double Y_MIN, double Y_MAX, string Y_LABEL,  string DIR);
	void print_info();


	inline const char * get_name(){return				name;}
	inline char * get_dir(){return				dir;}	
	inline char * get_title(){return			title;}	

	inline char * get_X_title(){return			Xaxis->title;}	


	inline double get_X_max(){return			Xaxis->max;}	
	inline double get_X_min(){return			Xaxis->min;}	
	inline int	  get_X_n_bins(){return			Xaxis->n_bins;}

	inline __int32 get_X_overflow(){return		Xaxis->overflow;}
	inline __int32 get_X_underflow(){return		Xaxis->underflow;}
	inline void set_X_overflow(__int32 value){	Xaxis->overflow = value;}
	inline void set_X_underflow(__int32 value){ Xaxis->underflow = value;}

	inline char * get_Y_title(){return			Yaxis->title;}	

	inline double get_Y_max(){return			Yaxis->max;}	
	inline double get_Y_min(){return			Yaxis->min;}	
	inline int	  get_Y_n_bins(){return			Yaxis->n_bins;}

	inline __int32 get_Y_overflow(){return		Yaxis->overflow;}
	inline __int32 get_Y_underflow(){return		Yaxis->underflow;}
	inline void set_Y_overflow(__int32 value){	Yaxis->overflow = value;}
	inline void set_Y_underflow(__int32 value){ Yaxis->underflow = value;}


};


//struct eqstr {
//    inline size_t operator()(const char *s) const {
//        size_t hash = 1;
//        for (; *s; ++s) hash = hash * 5 + *s;
//        return hash;
//    }
//    inline bool operator()(const char *s1, const char *s2) const {
//        return strcmp(s1, s2) == 0;
//    }
//};


struct my_hash {
  inline size_t operator()(const char* val) const
  {
    unsigned long h = 0;
    for (; *val; ++val)
      h = 5 * h + *val;
    return size_t(h);
  }
};

struct eqstr {
  inline bool operator()(const char *s1, const char *s2) const {
    return strcmp(s1, s2) == 0;
  }
};


//  typedef std::tr1::unordered_map<const char * , int, my_hash, eqstr> strhash;


///////////////////////////////////////////////////////
//1d histogram of ints (so no weighting is allows)
class histo_handler
{
	string name;
	int size_h1i;
	int hist_check_counter;
public:
	histo_handler();
	//~histo_handler();

	unordered_map<const char*, H1i*, my_hash, eqstr> h1i_map;
	unordered_map<const char*, H1i*, my_hash, eqstr>::iterator h1i_map_iterator;
	void fill1(char * NAME, double x, char * TITLE, int N_BINS, double MIN, double MAX, char * X_LABEL, char * DIR);
	inline void fill1(int UNUSED_id,char * NAME, double x, double UNUSED_weight, char * TITLE, int N_BINS, double MIN, double MAX, char * X_LABEL, char * DIR){
		fill1(NAME,  x,  TITLE,  N_BINS,  MIN,  MAX,  X_LABEL,  DIR);
	}
	void combine_hist(H1i * hist2);

	//vector<H1i*>	h1i_vector;
	//void fill1(int UNUSED_id, char * NAME, double x, char * TITLE, int N_BINS, double MIN, double MAX, char * X_LABEL, char * DIR);
	//inline void fill1(int id,char * NAME, double x, double UNUSED_weight, char * TITLE, int N_BINS, double MIN, double MAX, char * X_LABEL, char * DIR){
	//	fill1(id ,NAME,  x,  TITLE,  N_BINS,  MIN,  MAX,  X_LABEL,  DIR);
	//}
	//void combine_hist(H1i * hist2, int pos);

	unordered_map<const char*, H2i*, my_hash, eqstr> h2i_map;
	unordered_map<const char*, H2i*, my_hash, eqstr>::iterator h2i_map_iterator;
	void fill2(const char * NAME, double x, double y, char * TITLE, int X_N_BINS, double X_MIN, double X_MAX, char * X_LABEL, int Y_N_BINS, double Y_MIN, double Y_MAX, char * Y_LABEL, char * DIR);
	inline void fill2(int UNUSED_id, const char * NAME, double x, double y, double UNUSED_weight, char * TITLE, int X_N_BINS, double X_MIN, double X_MAX, char * X_LABEL, int Y_N_BINS, double Y_MIN, double Y_MAX, char * Y_LABEL, char * DIR){
		fill2( NAME,  x,  y,  TITLE,  X_N_BINS,  X_MIN,  X_MAX,  X_LABEL,  Y_N_BINS,  Y_MIN,  Y_MAX,  Y_LABEL,  DIR);
	}
	void combine_hist(H2i * hist2);

	//vector<H2i*>	h2i_vector;
	//void fill2(int id, char * NAME, double x, double y, char * TITLE, int X_N_BINS, double X_MIN, double X_MAX, char * X_LABEL, int Y_N_BINS, double Y_MIN, double Y_MAX, char * Y_LABEL, char * DIR);
	//inline void fill2(int id, char * NAME, double x, double y, double UNUSED_weight, char * TITLE, int X_N_BINS, double X_MIN, double X_MAX, char * X_LABEL, int Y_N_BINS, double Y_MIN, double Y_MAX, char * Y_LABEL, char * DIR){
	//	fill2( id ,NAME,  x,  y,  TITLE,  X_N_BINS,  X_MIN,  X_MAX,  X_LABEL,  Y_N_BINS,  Y_MIN,  Y_MAX,  Y_LABEL,  DIR);
	//}
	//void combine_hist(H2i * hist2, int pos);
};
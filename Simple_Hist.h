#include <string>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;


class H1i
{
	std::mutex mutex;
	int n_bins;
	vector<int> bins;
	double xMax, xMin;
	string title;
	string name;
	string x_axis_label;
public:
	H1i(string Name, string Title,int N_bins, double XMin, double XMax, string X_axis_label);
	//~H1i();

	void fill1(double x);

	//vector * get_bins();
	void print();

};


//class Simple_Hist
//{
//
//	public:
//		//Simple_Hist();
//		//~Simple_Hist();
//
//}
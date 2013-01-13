#include <fstream>
#include <iostream>
#include <stdio.h>
#include <sstream>

#include <string>
#include <vector>
#include <unordered_map> 
using namespace std;



class config_file_reader
{
public:
	config_file_reader(string filename);
	~config_file_reader();

	unordered_map<int, double> parameter;
	vector<string> inputfilename;
	string outputfilename;

};


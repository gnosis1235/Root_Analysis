#include "Simple_Hist.h"
#include <string>
#include <vector>
#include "console.h"
#include <map>
//#include <thread>
//#include <mutex>

using namespace std;


////////////////////////////////////////////  axis  ////////////////////////////////////////////
axis::axis(char * Name, char * Title,int N_bins, double Min, double Max){
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
	if(x>=max){
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

H1i::H1i(){};

H1i::H1i(char * Name, char * Title,int N_bins, double Min, double Max, char * X_axis_label, char * Dir){
		
	title = Title;
	name = Name;
	dir = Dir;

	Xaxis = new axis("Xaxis", X_axis_label, N_bins, Min, Max );

	////initialise all bins to 0
	//for (int i=0; i<N_bins; i++)
	//{
	//	bins.push_back(0);
	//	//printf("bin%i:%i   ",i,bins[i]);
	//}

	float * temp = new float[N_bins];

	bins = temp;

}

H1i::~H1i(){}

void H1i::fill(double x){
	int bin_id = Xaxis->get_bin_address(x);
	// add one to the bin if it isn't overflow are underflow
	
	if (bin_id !=-1) ++bins[bin_id];
}


void H1i::print_bin_contents(){
	//std::lock_guard<std::mutex> guard(mutex); //auto lock thread
	printf("\n");
	//printf("\nnumber of bins=%i\n",bins.size());
	double bin_width = (Xaxis->max-Xaxis->min)/Xaxis->n_bins;
	//printf("bin widh=%f\n", bin_width);
	for (int i=0; i<(Xaxis->n_bins); i++)
	{
		if (bins[i]!=0)	Red(true);
			printf("%2.f=<(bin%i)<%2.f:%i   ",(double)i*bin_width+Xaxis->min,i,((double)i+1.)*bin_width+Xaxis->min,bins[i]);
		if (bins[i]!=0)	White(false);
	
		if((i+1)%10==0 ) printf("\n");
		
	}
	
	printf("\n");
	return;
}

//bool H1i::match(string NAME, string TITLE, int N_BINS, double MIN, double MAX, string X_LABEL, string DIR){
//	return NAME==this->name && TITLE==this->title && N_BINS==this->Xaxis->n_bins && MIN==this->Xaxis->min && MAX==this->Xaxis->max && X_LABEL==this->Xaxis->title && DIR==this->dir; 
//}

void H1i::print_info(){
	cout<<name<< ", "<< title<< ", "<< Xaxis->n_bins << ", "<< Xaxis->min<< ", "<< Xaxis->max<< ", "<< Xaxis->title<< ", "<< dir <<endl;

}
////////////////////////////////////////////  H1i end  ////////////////////////////////////////////

////////////////////////////////////////////  H2i  ////////////////////////////////////////////

H2i::H2i(){};

H2i::H2i(char * Name, char * Title,int X_N_bins, double X_Min, double X_Max, char * X_axis_label,int Y_N_bins, double Y_Min, double Y_Max, char * Y_axis_label, char * Dir){
		
	title = Title;
	name = Name;
	dir = Dir;

	Xaxis = new axis("Xaxis", X_axis_label, X_N_bins, X_Min, X_Max );
	Yaxis = new axis("Yaxis", Y_axis_label, Y_N_bins, Y_Min, Y_Max );

	//initialise all bins to 0
	//bins[x][y]
	vector<int> col;
	for (int j=0; j<Y_N_bins; j++)
	{
		col.push_back(0);
	}

	for (int i=0; i<X_N_bins; i++)
	{
		bins.push_back(col);
	}
}

H2i::~H2i(){}

void H2i::fill(double x, double y){
	int bin_id_x = Xaxis->get_bin_address(x);
	int bin_id_y = Yaxis->get_bin_address(y);
	// add one to the bin if it isn't overflow are underflow
	if (bin_id_x !=-1 && bin_id_y !=-1) ++bins[bin_id_x][bin_id_y];
	return;
}


//void H2i::print_bin_contents(){
//	//std::lock_guard<std::mutex> guard(mutex); //auto lock thread
//	printf("\n");
//	//printf("\nnumber of bins=%i\n",bins.size());
//	double bin_width = (Xaxis->max-Xaxis->min)/Xaxis->n_bins;
//	//printf("bin widh=%f\n", bin_width);
//	for (int i=0; i<((signed int)bins.size()); i++)
//	{
//		if (bins[i]!=0)	Red(true);
//			printf("%2.f=<(bin%i)<%2.f:%i   ",(double)i*bin_width+Xaxis->min,i,((double)i+1.)*bin_width+Xaxis->min,bins[i]);
//		if (bins[i]!=0)	White(false);
//	
//		if((i+1)%10==0 ) printf("\n");
//		
//	}
//	
//	printf("\n");
//	return;
//}

//bool H2i::match(string NAME, string TITLE, int X_N_BINS, double X_MIN, double X_MAX, string X_LABEL, int Y_N_BINS, double Y_MIN, double Y_MAX, string Y_LABEL,  string DIR){
//	return NAME==this->name && TITLE==this->title && X_N_BINS==this->Xaxis->n_bins && X_MIN==this->Xaxis->min && X_MAX==this->Xaxis->max && X_LABEL==this->Xaxis->title && Y_N_BINS==this->Yaxis->n_bins && Y_MIN==this->Yaxis->min && Y_MAX==this->Yaxis->max && Y_LABEL==this->Yaxis->title && DIR==this->dir; 
//}

void H2i::print_info(){
	cout<<name<< ", "<< title<< ", "<< Xaxis->n_bins << ", "<< Xaxis->min<< ", "<< Xaxis->max<< ", "<< Xaxis->title<< ", ";
	cout<< Yaxis->n_bins << ", "<< Yaxis->min<< ", "<< Yaxis->max<< ", "<< Yaxis->title<< ", "<< dir <<endl;
}
////////////////////////////////////////////  H2i end  ////////////////////////////////////////////


////////////////////////////////////////////  histo_container  ////////////////////////////////////////////

histo_handler::histo_handler(){
	hist_check_counter=0;

	for(int i=0; i<10000; ++i){
		h1i_vector.push_back(0);
		h2i_vector.push_back(0);
	}
}

//void histo_handler::fill1(string NAME, double x, string TITLE, int N_BINS, double MIN, double MAX, string X_LABEL, string DIR){
//	
//	if(h1i_map.count(NAME+DIR) == 0){
//		H1i * hist = new H1i(NAME, TITLE, N_BINS, MIN, MAX, X_LABEL, DIR);
//		h1i_map[NAME+DIR]=hist;
//	}else{
//		//checking to see if the histogram matches is slow so we will only do this in the very begining 
//		if ( hist_check_counter < 10000 && !(h1i_map[NAME+DIR]->match(NAME, TITLE, N_BINS, MIN, MAX, X_LABEL, DIR)) ){
//			Red(true);
//			cout<<"ERROR: Histogram is:"<<NAME<< ", "<< TITLE<< ", "<< N_BINS<< ", "<< MIN<< ", "<< MAX<< ", "<< X_LABEL<< ", "<< DIR<<endl;
//			cout<<"And it should be   :";
//			h1i_map[NAME+DIR]->print_info();
//			White(false);
//		}
//		else {
//			h1i_map[NAME+DIR]->fill(x);
//			hist_check_counter++;
//		}
//	}
//	return;
//
//}

void histo_handler::fill1(int id, char * NAME, double x, char * TITLE, int N_BINS, double MIN, double MAX, char * X_LABEL, char * DIR){
	//if( id > 10000) {
	//	printf("The current max number of histograms are 10000.  Please change the max (in Simple_Hist.cpp -> histo_handler::histo_handler()).\n");
	//	return;
	//}

	if( h1i_vector[id] == 0){
		H1i * hist = new H1i(NAME, TITLE, N_BINS, MIN, MAX, X_LABEL, DIR);
		h1i_vector[id]=hist;
	}else{
		////checking to see if the histogram matches is slow so we will only do this in the very begining 
		//if ( hist_check_counter < 1 && !(h1i_vector[id]->match(NAME, TITLE, N_BINS, MIN, MAX, X_LABEL, DIR)) )
		//{
		//	Red(true);
		//	cout<<endl;
		//	cout<<"ERROR: Histogram is:" << NAME<< ", "<< TITLE<< ", "<< N_BINS<< ", "<< MIN<< ", "<< MAX<< ", "<< X_LABEL<< ", "<< DIR<<endl;
		//	cout<<"And it should be   :";
		//	h1i_vector[id]->print_info();
		//	cout<<endl;
		//	White(false);
		//}
		//else {
			h1i_vector[id]->fill(x);
			hist_check_counter++;
		//}
	}
	return;

}

void histo_handler::combine_hist(H1i * hist2, int pos){
	
	if(hist2 == 0) return;

	if(h1i_vector[pos] == 0){ //if it dosen't exit just add it
		h1i_vector[pos]=hist2;
	}
	else{
		if ( true)//h1i_vector[pos]->match(hist2->get_name(), hist2->get_title(), hist2->get_X_n_bins(), hist2->get_X_min(), hist2->get_X_max(), hist2->get_X_title(), hist2->get_dir() ))
		{
		//if( *h1i_map[key] == *hist2){
			for(int i=0; i < hist2->Xaxis->n_bins; ++i){
				h1i_vector[pos]->bins[i] = h1i_vector[pos]->bins[i] + hist2->bins[i];
			}
			h1i_vector[pos]->set_X_overflow(  h1i_vector[pos]->get_X_overflow()  +	hist2->get_X_overflow() );
			h1i_vector[pos]->set_X_underflow( h1i_vector[pos]->get_X_underflow() +	hist2->get_X_underflow() );
		}
		else {
			Red(true);
			cout<<"ERROR in combine_hist()"<<endl;
			cout<<"ERROR: Histogram is:";	hist2->print_info();
			cout<<"   And it should be:";	h1i_vector[pos]->print_info();
				
			White(false);
		}

		}
}

//void histo_handler::combine_hist(H1i * hist2){
//
//	string key = hist2->get_name() + hist2->get_dir();
//	
//	if(h1i_map.count(key) == 0){ //if it dosen't exit just add it
//		h1i_map[key]=hist2;
//	}
//	else{
//		if ( h1i_map[key]->match(hist2->get_name(), hist2->get_title(), hist2->get_X_n_bins(), hist2->get_X_min(), hist2->get_X_max(), hist2->get_X_title(), hist2->get_dir() )){
//		//if( *h1i_map[key] == *hist2){
//			for(int i=0; i < (int)hist2->bins.size(); ++i){
//				h1i_map[key]->bins[i] = h1i_map[key]->bins[i] + hist2->bins[i];
//			}
//			h1i_map[key]->set_X_overflow(  h1i_map[key]->get_X_overflow()  +	hist2->get_X_overflow() );
//			h1i_map[key]->set_X_underflow( h1i_map[key]->get_X_underflow() +	hist2->get_X_underflow() );
//		}
//		else {
//			Red(true);
//			cout<<"ERROR in combine_hist()"<<endl;
//			cout<<"ERROR: Histogram is:";	hist2->print_info();
//			cout<<"   And it should be:";	h1i_map[key]->print_info();
//				
//			White(false);
//		}
//
//		}
//}

//void histo_handler::fill2(string NAME, double x, double y, string TITLE, int X_N_BINS, double X_MIN, double X_MAX, string X_LABEL, int Y_N_BINS, double Y_MIN, double Y_MAX, string Y_LABEL, string DIR){
//	
//	if(h2i_map.count(NAME+DIR) == 0){
//		H2i * hist = new H2i(NAME, TITLE, X_N_BINS, X_MIN, X_MAX, X_LABEL, Y_N_BINS, Y_MIN, Y_MAX, Y_LABEL, DIR);
//		h2i_map[NAME+DIR]=hist;
//		//printf("add new\n");
//	}else{
//		//checking to see if the histogram matches is slow so we will only do this in the very begining 
//		if ( hist_check_counter < 10000 &&  !(h2i_map[NAME+DIR]->match(NAME, TITLE, X_N_BINS, X_MIN, X_MAX, X_LABEL,  Y_N_BINS, Y_MIN, Y_MAX, Y_LABEL, DIR)) ){
//			Red(true);
//			cout<<"ERROR: Histogram is:"<<NAME<< ", "<< TITLE<< ", "<< X_N_BINS<< ", "<< X_MIN<< ", "<< X_MAX<< ", "<< X_LABEL<< ", "<< DIR<<endl;
//			cout<<"And it should be   :";
//			h2i_map[NAME+DIR]->print_info();
//			White(false);
//		}
//		else {
//			h2i_map[NAME+DIR]->fill(x,y);
//			hist_check_counter++;	
//		}
//	}
//	return;
//
//}
	
void histo_handler::fill2(int id, char * NAME, double x, double y, char * TITLE, int X_N_BINS, double X_MIN, double X_MAX, char * X_LABEL, int Y_N_BINS, double Y_MIN, double Y_MAX, char * Y_LABEL, char * DIR){

	if( id > 10000) {
		printf("The current max number of histograms are 10000.  Please change the max (in Simple_Hist.cpp -> histo_handler::histo_handler()).\n");
		return;
	}

	if( h2i_vector[id] == 0){
		H2i * hist = new H2i(NAME, TITLE, X_N_BINS, X_MIN, X_MAX, X_LABEL, Y_N_BINS, Y_MIN, Y_MAX, Y_LABEL, DIR);
		h2i_vector[id]=hist;
		h2i_vector[id]->fill(x,y);
	}else{
		////checking to see if the histogram matches is slow so we will only do this in the very begining 
		//if ( hist_check_counter < 1 && !(h2i_vector[id]->match(NAME, TITLE, X_N_BINS, X_MIN, X_MAX, X_LABEL,  Y_N_BINS, Y_MIN, Y_MAX, Y_LABEL, DIR)) ){
		//	Red(true);
		//	cout<<endl;
		//	cout<<"ERROR: Histogram is:"<<NAME<< ", "<< TITLE<< ", "<< X_N_BINS<< ", "<< X_MIN<< ", "<< X_MAX<< ", "<< X_LABEL<< ", "<< DIR<<endl;
		//	cout<<"And it should be   :";
		//	h2i_vector[id]->print_info();
		//	cout<<endl;
		//	White(false);
		//}
		//else {
			h2i_vector[id]->fill(x,y);
			hist_check_counter++;
		//}
	}
	return;

}

void histo_handler::combine_hist(H2i * hist2, int pos){
	if(hist2 == 0) return;
	
	if(h2i_vector[pos] == 0){ //if it dosen't exit just add it
		h2i_vector[pos]=hist2;
	}
	else{
		if (true)// h2i_vector[pos]->match(hist2->get_name(), hist2->get_title(), hist2->get_X_n_bins(), hist2->get_X_min(), hist2->get_X_max(), hist2->get_X_title(), hist2->get_Y_n_bins(), hist2->get_Y_min(), hist2->get_Y_max(), hist2->get_Y_title(), hist2->get_dir() ))
		{
			for(int i=0; i < (int)hist2->get_X_n_bins(); ++i){
				for(int j=0; j < (int)hist2->get_Y_n_bins(); ++j){
					h2i_vector[pos]->bins[i][j] = h2i_vector[pos]->bins[i][j] + hist2->bins[i][j];
				}
			}

			h2i_vector[pos]->set_X_overflow(  h2i_vector[pos]->get_X_overflow()  +	hist2->get_X_overflow() );
			h2i_vector[pos]->set_X_underflow( h2i_vector[pos]->get_X_underflow() +	hist2->get_X_underflow() );
			
			h2i_vector[pos]->set_Y_overflow(  h2i_vector[pos]->get_Y_overflow()  +	hist2->get_Y_overflow() );
			h2i_vector[pos]->set_Y_underflow( h2i_vector[pos]->get_Y_underflow() +	hist2->get_Y_underflow() );
			
		}
		else {
			Red(true);
			cout<<"ERROR in combine_hist()"<<endl;
			cout<<"ERROR: Histogram is:";	hist2->print_info();
			cout<<"   And it should be:";	h2i_vector[pos]->print_info();
				
			White(false);
		}

		}
}	
	
//void histo_handler::combine_hist(H2i * hist2){
//	string key = hist2->get_name() + hist2->get_dir();
//	
//	if(h2i_map.count(key) == 0){ //if it dosen't exit just add it
//		h2i_map[key]=hist2;
//	}
//	else{
//		if ( h2i_map[key]->match(hist2->get_name(), hist2->get_title(), hist2->get_X_n_bins(), hist2->get_X_min(), hist2->get_X_max(), hist2->get_X_title(), hist2->get_Y_n_bins(), hist2->get_Y_min(), hist2->get_Y_max(), hist2->get_Y_title(), hist2->get_dir() )){
//			for(int i=0; i < (int)hist2->get_X_n_bins(); ++i){
//				for(int j=0; j < (int)hist2->get_Y_n_bins(); ++j){
//					h2i_map[key]->bins[i][j] = h2i_map[key]->bins[i][j] + hist2->bins[i][j];
//				}
//			}
//
//			h2i_map[key]->set_X_overflow(  h2i_map[key]->get_X_overflow()  +	hist2->get_X_overflow() );
//			h2i_map[key]->set_X_underflow( h2i_map[key]->get_X_underflow() +	hist2->get_X_underflow() );
//			
//			h2i_map[key]->set_Y_overflow(  h2i_map[key]->get_Y_overflow()  +	hist2->get_Y_overflow() );
//			h2i_map[key]->set_Y_underflow( h2i_map[key]->get_Y_underflow() +	hist2->get_Y_underflow() );
//			
//		}
//		else {
//			Red(true);
//			cout<<"ERROR in combine_hist()"<<endl;
//			cout<<"ERROR: Histogram is:";	hist2->print_info();
//			cout<<"   And it should be:";	h2i_map[key]->print_info();
//				
//			White(false);
//		}
//
//		}
//}																															
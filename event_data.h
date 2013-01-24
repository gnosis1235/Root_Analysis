#ifndef event_data_ALREADY_INCLUDED
	#define event_data_ALREADY_INCLUDED

struct event_data{
	double reaction, ehit, rhit, phit, bunchmarker;
	double rx[5],ry[5],rmcp[5],rtof[5],rflag[5];
	double ex[6],ey[6],emcp[6],etof[6],eflag[6];
	double px[5],py[5],pmcp[5],ptof[5],pflag[5];
};

#endif
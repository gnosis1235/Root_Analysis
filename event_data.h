#ifndef event_data_ALREADY_INCLUDED
	#define event_data_ALREADY_INCLUDED

typedef struct event_data{
	double reaction, ehit, rhit, phit, bunchmarker;
	double rx[32],ry[32],rmcp[32],rtof[32],rflag[32];
	double ex[32],ey[32],emcp[32],etof[32],eflag[32];
	double px[32],py[32],pmcp[32],ptof[32],pflag[32];
};

#endif
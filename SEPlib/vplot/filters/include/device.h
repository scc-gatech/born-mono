#ifndef VP_DEVICE_H
#define VP_DEVICE_H
/*
 * device routine table structure
 */

#include "../include/vertex.h"
struct device{

	/* control routines */
	void (*open)(void);
	void (*reset)(void);
	void (*message)(int,char *);
	void (*erase)(int);
	void (*close)(int);

	/* high level output */
	void (*vector)(int,int,int,int,int,int);
	void (*marker)(int,int,int,int *);
	void (*text)(char *,float,float,float,float);
	void (*area)(int,struct vertex *);
	void (*raster)(int,int,int,int,int,int,unsigned short *,int,int,int);
	void (*point)(int,int);
	int  (*attributes)(int,int,int,int,int);

	/* input */
	int (*getpoint)(int *,int *);
	int (*interact)(int,FILE *,char *);

	/* low level output
	 * - these are called by the generic high level output routines
	 */
	void (*plot)(int,int,int);
	void (*startpoly)(int);
	void (*midpoly)(int,int);
	void (*endpoly)(int);
};
#endif

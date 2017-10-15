/*
*
*
*This routine will set the the device parameters
* namely color of the background, plot color and ploot fat
*
* Modifications:
* 05-14-91	W. Bauske
*		Make it use a pointer instead of a structure on the stack
*
*/
#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_dash ## SUF (struct dashinfo ## SUF *dash)
#else
#define A1(FTYP,SUF) \
int gl_dash ## SUF (dash) \
    struct dashinfo ##SUF *dash;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
    vp_setdash ## SUF  (dash->dash, dash->gap, 2); \
	return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

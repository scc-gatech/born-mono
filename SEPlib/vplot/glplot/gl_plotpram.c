/*
*
*
*This routine will set the the device parameters
* namely color of the background, plot color and plot fat
*
*/
#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_plotpram ## SUF (struct colorinfo ## SUF *color,struct coordinfo ## SUF * coordinate)
#else
#define A1(FTYP,SUF) \
int gl_plotpram ## SUF (color, coordinate) \
struct colorinfo ## SUF  *color; \
struct coordinfo ## SUF  *coordinate;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
        if (color->fillcol[0] !=color->backcol[0] || color->fillcol[1] != color->backcol[1] || color->fillcol[2] != color->backcol[2] ) \
            gl_fillin ## SUF (coordinate, color); \
            vp_coltab ## SUF (0, color->backcol[0], color->backcol[1], color->backcol[2]); \
	return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

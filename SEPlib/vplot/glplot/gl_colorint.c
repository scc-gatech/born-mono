/*
*
*
* The routine will fetch and initialize plot and axis parameters.
*
* 
*/
#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_colorint ## SUF (struct colorinfo ## SUF *color)
#else
#define A1(FTYP,SUF) \
int gl_colorint ## SUF (color) \
    struct colorinfo ## SUF *color;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
int             ii; \
    for (ii = 0; ii < 3; ii++) \
    { \
	color->backcol[ii] = 0.; \
    } \
    getch ("backcol", "f", color->backcol); \
    for (ii = 0; ii < 3; ii++) \
    { \
	color->fillcol[ii] = color->backcol[ii]; \
    } \
    getch ("fillcol", "f", color->fillcol); \
	return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

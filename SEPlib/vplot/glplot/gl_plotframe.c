/*
*
*
*This routine will plot the frame around the picture
*
*
*/
#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_plotframe ## SUF (struct coordinfo ## SUF  *coord, int col)
#else
#define A1(FTYP,SUF) \
int gl_plotframe ## SUF (coord, col) \
struct coordinfo ## SUF  *coord; \
int col;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
    gl_color ## SUF (col); \
    gl_umove ## SUF (coord->min1, coord->min2); \
    gl_udraw ## SUF (coord->min1, coord->max2); \
    gl_udraw ## SUF (coord->max1, coord->max2); \
    gl_udraw ## SUF (coord->max1, coord->min2); \
    gl_udraw ## SUF (coord->min1, coord->min2); \
	return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

/*
gl_arrow


*/
#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_arrow ## SUF (FTYP x0,FTYP y0,FTYP x,FTYP y ,FTYP r)
#else
#define A1(FTYP,SUF) \
int gl_arrow ## SUF (x0, y0, x, y, r) \
    FTYP           x0, y0, x, y, r;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
	vp_arrow ## SUF (x0, y0, x, y, r); \
	return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)
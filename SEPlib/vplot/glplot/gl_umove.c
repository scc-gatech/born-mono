#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_umove ## SUF (FTYP point1, FTYP point2)
#else
#define A1(FTYP,SUF) \
int gl_umove ## SUF (point1, point2) \
FTYP point1, point2;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
return(	vp_umove ## SUF (point1, point2)); \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

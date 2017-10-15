#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_where ## SUF (FTYP *xc, FTYP *yc)
#else
#define A1(FTYP,SUF) \
int gl_where ## SUF (xc, yc) \
FTYP *xc, *yc;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
   int rc; \
   rc = vp_where ## SUF (xc, yc); \
   return rc; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_upmark ## SUF (int npts, int mtype, int msize, FTYP *xp,  FTYP *yp)
#else
#define A1(FTYP,SUF) \
int gl_upmark ## SUF (npts, mtype, msize,xp, yp) \
int npts, mtype, msize; \
 FTYP *xp, *yp;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
  return(  vp_upmark ## SUF (npts, mtype, msize, xp, yp)); \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

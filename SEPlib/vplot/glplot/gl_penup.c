#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_penup ## SUF (void)
#else
#define A1(FTYP,SUF) \
int gl_penup ## SUF ()
#endif
#define A2(FTYP,FMT,SUF) \
{ \
return(vp_penup ## SUF ()); \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

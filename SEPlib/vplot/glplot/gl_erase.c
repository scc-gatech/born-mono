#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_erase ## SUF (void)
#else
#define A1(FTYP,SUF) \
int gl_erase  ## SUF()
#endif
#define A2(FTYP,FMT,SUF) \
{ \
return (vp_erase ## SUF ()); \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

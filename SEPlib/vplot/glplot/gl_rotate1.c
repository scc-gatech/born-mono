#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_rotate1 ## SUF (FTYP *x, FTYP min, FTYP max)
#else
#define A1(FTYP,SUF) \
int gl_rotate1 ## SUF (x, min, max) \
FTYP *x, min, max;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
FTYP temp; \
temp = *x; \
*x = (min + max) - temp; \
return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_rotate ## SUF (float* x,FTYP  min,FTYP  max,struct datainfo ## SUF * data)
#else
#define A1(FTYP,SUF) \
int gl_rotate ## SUF (x, min, max, data) \
float *x; \
FTYP min, max; \
struct datainfo ## SUF *data;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
int i, j; \
j = 0; \
for ( i = 0; i < data->n2; i++ ) \
    j =  j + data->n1[i]; \
for (i  = 0; i < j; i++) \
{ \
 x[i] = (min + max) - x[i]; \
}  \
return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

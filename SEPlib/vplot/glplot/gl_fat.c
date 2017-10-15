#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_fat ## SUF (int fat)
#else
#define A1(FTYP,SUF) \
int gl_fat ## SUF (fat) \
int fat;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
return(vp_fat ## SUF (fat)); \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_color ## SUF (int color)
#else
#define A1(FTYP,SUF) \
int gl_color ## SUF (color) \
int color;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
vp_color ## SUF (color); \
return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

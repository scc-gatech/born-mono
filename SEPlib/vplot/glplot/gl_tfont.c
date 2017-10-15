#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_tfont ## SUF (int font, int prec, int ovly)
#else
#define A1(FTYP,SUF) \
int gl_tfont ## SUF (font, prec, ovly) \
int font, prec, ovly;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
return(vp_tfont ## SUF (font, prec, ovly)); \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

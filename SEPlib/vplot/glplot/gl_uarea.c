#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_uarea ## SUF ( FTYP *px, FTYP *py, int iii, int fatp, int ymask, int xmask, int fun )
#else
#define A1(FTYP,SUF) \
int gl_uarea ## SUF ( px, py, iii, fatp, ymask, xmask, fun) \
FTYP *px, *py; \
int iii, fatp, xmask, ymask, fun;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
int j; \
char string[10]; \
return(vp_uarea ## SUF (px, py, iii, fatp, ymask, xmask)); \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

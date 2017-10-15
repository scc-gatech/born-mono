#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_clip ## SUF (FTYP pos1, FTYP pos2, FTYP pos3, FTYP pos4)
#else
#define A1(FTYP,SUF) \
int gl_clip ## SUF (pos1, pos2, pos3, pos4) \
FTYP pos1, pos2, pos3, pos4;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
vp_clip ## SUF (pos1, pos2, pos3, pos4); \
return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

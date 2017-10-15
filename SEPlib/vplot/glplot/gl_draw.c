#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_draw ## SUF (FTYP position1, FTYP position2)
#else
#define A1(FTYP,SUF) \
int gl_draw ## SUF (position1, position2) \
    FTYP           position1, position2;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
    vp_draw ## SUF  (position1, position2); \
	return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

/*  fetch min and maxes   will also take care of transposing*/ 
#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_minmax ## SUF (struct coordinfo ## SUF *coordinate)
#else
#define A1(FTYP,SUF) \
int gl_minmax ## SUF (coordinate) \
    struct         coordinfo ## SUF *coordinate;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
int   temp1; \
float temp; \
    coordinate->fmin1 = 0;  \
    coordinate->fmax1 = 0;  \
    coordinate->fmin2 = 0;  \
    coordinate->fmax2 = 0;  \
    if (getch ("min1", FMT, &coordinate->min1)) \
    coordinate->fmin1 = 1;  \
    if (getch ("max1", FMT, &coordinate->max1)) \
    coordinate->fmax1 = 1;  \
    if (getch ("min2", FMT, &coordinate->min2)) \
    coordinate->fmin2 = 1;  \
    if (getch ("max2", FMT, &coordinate->max2)) \
    coordinate->fmax2 = 1;  \
 \
 \
    if (coordinate->transp) \
    { \
      temp = coordinate->min1; \
      coordinate->min1 = coordinate->min2; \
      coordinate->min2 = temp; \
      temp = coordinate->max1; \
      coordinate->max1 = coordinate->max2; \
      coordinate->max2 = temp; \
      temp1 = coordinate->fmin1; \
      coordinate->fmin1 = coordinate->fmin2; \
      coordinate->fmin2 = temp1; \
      temp1 = coordinate->fmax1; \
      coordinate->fmax1 = coordinate->fmax2; \
      coordinate->fmax2 = temp; \
    } \
 \
	return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

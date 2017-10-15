#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_invmassage ##SUF (FTYP* min, FTYP* max, FTYP mid, FTYP dev)
#else
#define A1(FTYP,SUF) \
int gl_invmassage ##SUF (min, max, mid, dev)
    FTYP           *min, *max,  mid, dev;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
double          temp1num, temp2num, temp3num, tempnum, tempmin, tempmax; \
    tempmin = *min; \
    tempmax = *max; \
    temp1num = (tempmin * dev) + mid; \
    temp2num = (tempmax * dev) + mid; \
    if (temp1num == temp2num)  \
     { \
        temp1num = temp1num - temp1num ; \
        temp2num = temp2num + temp2num ; \
     } \
    *min = temp1num; \
    *max = temp2num; \
	return 0; \
     \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

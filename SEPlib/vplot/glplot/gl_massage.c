/*
 * Modifications:
 * W. Bauske IBM 04-01-91
 *	Removed re-declare of malloc()
 */
#include "glplot.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_massage ## SUF (FTYP *min, FTYP *max, FTYP *mid, FTYP *dev)
#else
#define A1(FTYP,SUF) \
int gl_massage ## SUF (min, max, mid, dev) \
    FTYP          *min, *max, *mid, *dev;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
double          xtemp[2], mintemp, maxtemp, midtemp, nextnum; \
double          devtemp; \
int             ii, jj, kk; \
 \
/* first calculate max and min  of original data*/ \
    mintemp = *min; \
    maxtemp = *max; \
    midtemp = (mintemp + maxtemp) / 2.0; \
    devtemp = 0.; \
    xtemp[0] = mintemp - midtemp; \
    xtemp[1] = maxtemp - midtemp; \
    for (ii = 0; ii < 2; ii++) \
    { \
	devtemp = (fabs (xtemp[ii]) > devtemp) ? fabs (xtemp[ii]) : devtemp; \
    } \
    if (devtemp != 0.) \
    { \
	*min = (mintemp - midtemp) / devtemp; \
	*max = (maxtemp - midtemp) / devtemp; \
	*mid = midtemp; \
	*dev = devtemp; \
    } \
    else \
    { \
	*min = mintemp - midtemp ; \
	*max = mintemp - midtemp ; \
	*mid = midtemp; \
	*dev = devtemp; \
    } \
 \
	return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

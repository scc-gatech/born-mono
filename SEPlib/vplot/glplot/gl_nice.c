#include <stdio.h>
#include <math.h>
#include <limits.h>
#define FLT_EPSILON    1.192092896e-07F        /* smallest such that 1.0+FLT_EPSILON != 1.0 */
#define DBL_EPSILON 2.2204460492503131e-016
/* Algorithm for internal labeling from Tom Steppe, "Composing
 * well-tempered linear scales", Computer Language, September 1989,
 * 49-65. */

#define A1(FTYP,SUF) \
int gl_optimal_scale ## SUF (int n, FTYP min, FTYP max, \
                         /*@out@*/ FTYP *onum, FTYP *dnum)
#define A2(FTYP,EPS,SUF) \
{ \
    int lo, hi; \
    FTYP d, nice; \
    extern FTYP gl_nice_number ## SUF (FTYP); \
 \
    d = ((FTYP) fabs(max-min))/n; \
 \
    nice = gl_nice_number ## SUF (d); \
    FTYP range=fabs(max-min); \
    FTYP delta=.02; \
 \
    if (min <= max) { \
        lo = (int) ((FTYP) ceil((FTYP) ((min+delta*range)/nice))); \
        if ((lo-1)*nice+FLT_EPSILON >= min+delta*range) lo--; \
 \
        hi = (int) ((FTYP) floor((FTYP) ((max-delta*range)/nice))); \
        if ((hi+1)*nice <= (max-delta*range)+EPS) hi++; \
        *onum = lo * nice; \
        *dnum = nice; \
    } else { \
        lo = (int) ((FTYP) ceil((FTYP) ((max+delta*range)/nice))); \
        if ((lo-1)*nice+EPS >= max+delta*range) lo--; \
 \
        hi = (int) ((FTYP) floor((FTYP) ((min-delta*range)/nice))); \
        if ((hi+1)*nice <= min+EPS+delta*range) hi++; \
 \
        *onum = hi * nice; \
        *dnum = -nice; \
    } \
   \
 \
    return (hi-lo+1); \
}

/* smallest nice number >= d */
#define A3(FTYP,SUF) \
FTYP gl_nice_number ## SUF (FTYP d)

#define A4(FTYP,EPS,SUF) \
{ \
    FTYP p, nice; \
    int i, ie; \
    const FTYP set[] = {1.0, 2.0, 5.0}; \
    extern FTYP gl_power ## SUF (FTYP, int); \
 \
    ie = (int) (FTYP) floor((FTYP) log10((FTYP) d)); \
    nice = p = gl_power ## SUF (10.,ie); \
 \
    for (i=1; nice < d; i++) { \
        if (i >= 3) { \
            i=0; \
            p *= 10.; \
        } \
 \
        nice = set[i]*p; \
    } \
 \
    return nice; \
}

/*
	Finds what power of 10 would best serve for labeling the axes.
*/
#define A5(FTYP,SUF) \
FTYP gl_power ## SUF (FTYP f, int ie)
#define A6(FTYP,EPS,SUF) \
{ \
    FTYP p; \
 \
    if (ie < 0) { \
        f = 1./f; \
        ie = -ie; \
    } \
 \
    p = (ie & 1)? f: 1.; \
 \
    for (ie >>= 1; ie > 0; ie >>=1) { \
        f *= f; \
        if (ie & 1) p *= f; \
    } \
 \
    return p; \
}

A1(float, )
A2(float,FLT_EPSILON, )
A3(float, )
A4(float,FLT_EPSILON, )
A5(float, )
A6(float,FLT_EPSILON, )

A1(double,_g)
A2(double,DBL_EPSILON,_g)
A3(double,_g)
A4(double,DBL_EPSILON,_g)
A5(double,_g)
A6(double,DBL_EPSILON,_g)

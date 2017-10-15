/*
* This routine will fecth the min,and max values  and pad. 
*/
/* 
3-6-90  fixed padding problem
        now do calculations on normalized data
*/
#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_padint ## SUF (struct coordinfo ## SUF  *coordinate)
#else
#define A1(FTYP,SUF) \
int gl_padint ## SUF (coordinate) \
struct coordinfo ## SUF *coordinate;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
FTYP           mid1, mid2, dev1, dev2; \
int            pad, npad; \
    if (coordinate->pad)  \
    { \
        gl_massage ## SUF (&coordinate->min1, &coordinate->max1, &mid1, &dev1) ; \
        if (coordinate->fmin1) \
        { \
        if (coordinate->npad) \
        coordinate->min1 = 1.04 * coordinate->min1; \
        } \
        else \
        coordinate->min1 = 1.04 * coordinate->min1; \
        if (coordinate->fmax1) \
        { \
        if (coordinate->npad) \
        coordinate->max1 = 1.04 * coordinate->max1; \
        } \
        else \
        coordinate->max1 = 1.04 * coordinate->max1; \
        gl_invmassage ## SUF (&coordinate->min1, &coordinate->max1,  mid1, dev1) ; \
        gl_massage ## SUF (&coordinate->min2, &coordinate->max2, &mid2, &dev2) ; \
        if (coordinate->fmin2) \
        { \
        if (coordinate->npad) \
        coordinate->min2 = 1.04 * coordinate->min2; \
        } \
        else \
        coordinate->min2 = 1.04 * coordinate->min2; \
        if (coordinate->fmax2) \
        { \
        if (coordinate->npad) \
        coordinate->max2 = 1.04 * coordinate->max2; \
        } \
        else \
        coordinate->max2 = 1.04 * coordinate->max2; \
        gl_invmassage ## SUF (&coordinate->min2, &coordinate->max2, mid2, dev2) ; \
    } \
    if (coordinate->min1 == 0 && coordinate->max1 == 0) \
    { \
    coordinate->min1 = -1.; \
    coordinate->max1 = 1.; \
    } \
    if (coordinate->min2 == 0 && coordinate->max2 == 0) \
    { \
    coordinate->min2 = -1.; \
    coordinate->max2 = 1.; \
    } \
    if (coordinate->min1 == coordinate->max1) \
    { \
        coordinate->max1 = coordinate->max1 * 1.04; \
        coordinate->min1 = coordinate->min1 * .96; \
    } \
    if (coordinate->min2 == coordinate->max2) \
    { \
        coordinate->max2 = coordinate->max2 * 1.04; \
        coordinate->min2 = coordinate->min2 * .96; \
    } \
 \
	return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

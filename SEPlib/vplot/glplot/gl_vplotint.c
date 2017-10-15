/*
*
*This routine initializes the device 
*
*/
#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_vplotint ## SUF (struct plotposition ## SUF *position, \
             struct coordinfo ## SUF *coord, \
	     struct axisinfo ## SUF *axis1,struct axisinfo ## SUF *axis2)
#else
#define A1(FTYP,SUF) \
int gl_vplotint ## SUF (position, coord, axis1, axis2) \
struct plotposition ## SUF *position; \
struct coordinfo ## SUF *coord; \
struct axisinfo ## SUF *axis1; \
struct axisinfo ## SUF *axis2;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
    FTYP scale1, scale2, orig1, orig2, uorig1, uorig2; \
    vp_style ## SUF  (STANDARD); \
    if (coord->min2 == coord->max2) \
	seperr ("min2 is equal to max2 change one of them\n"); \
    if (coord->min1 == coord->max1) \
	seperr ("min1 is equal to max1 change one of them\n"); \
    if (position->xll != position->xur) \
    { \
	axis1->inch = position->xur - position->xll; \
	orig1 = position->xll + (position->xur - position->xll) / 2.0; \
    } \
    else \
	orig1 = (position->screenwd) / 2 ; \
    if (position->yll != position->yur) \
    { \
	axis2->inch = position->yur - position->yll; \
	orig2 = position->yll + (position->yur - position->yll) / 2.0; \
    } \
    else \
	orig2 = (position->screenht) / 2 ; \
    scale1 = axis1->inch / (coord->max1 - coord->min1); \
    scale2 = axis2->inch / (coord->max2 - coord->min2); \
    uorig2 = (coord->min2 + coord->max2) / 2; \
    uorig1 = (coord->min1 + coord->max1) / 2; \
    vp_scale ## SUF  (scale1, scale2); \
    vp_orig ## SUF  (orig1, orig2); \
    vp_uorig ## SUF  (uorig1, uorig2); \
    vp_uclip ## SUF  (coord->min1, coord->min2, coord->max1, coord->max2);  \
	return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

/*
 * Copyright 1987 the Board of Trustees of the Leland Stanford Junior
 * University. Official permission to use this software is included in
 * the documentation. It authorizes you to use this file for any
 * non-commercial purpose, provided that this copyright notice is not
 * removed and that any modifications made to this file are commented
 * and dated in the style of my example below.
 */

/*
 *
 *  source file:   ./lvplot/vp_circle.c
 *
 * Stewart A. Levin (SEP), 19 Sep, 2014
 *      Initial version
 */

#include "../include/vplot.h"
#include <math.h>

#ifdef FORTRAN

#define X	*x
#define Y	*y
#define R	*r

#else

#define X	x
#define Y	y
#define R	r

#endif

#define A1(NAM,FTYP) \
int NAM (FTYP X,FTYP Y,FTYP R)
#ifndef UUU
#define B2(SUF) \
	vp_move ## SUF (next_x, next_x);
#else /* UUU */
#define B2(SUF) \
	vp_umove ## SUF (next_x, Y);
#endif /* UUU */
#ifndef UUU
#define B3(SUF) \
	vp_draw ## SUF (next_x, next_y);
#else /* UUU */
#define B3(SUF) \
	vp_udraw ## SUF (next_x, next_y);
#endif /* UUU */
/*
 * plot a circle around (x,y) with radius r
 */
#define A2(FTYP,SUF) \
{ \
FTYP           next_x, next_y; \
double         angle, dangle; \
 \
    dangle=1.0/(R*RPERIN); \
    angle=0.0; \
    next_x = X+R; next_y = Y; \
    /* move to initial point on circle */ \
    B2(SUF) \
 \
    while (angle < 2.0*M_PI) \
    { \
        next_x = (FTYP) (X+R*cos(angle)); \
        next_y = (FTYP) (Y+R*sin(angle)); \
	angle += dangle; \
        B3(SUF) \
    } \
    return (vp_fixpc ## SUF()); \
}
#ifdef FORTRAN

#ifndef UUU
A1(vpcircle_,float)
A2(float, )
A1(vpcircleg_,double)
A2(double,_g)
#else /* UUU */
A1(vpucircle_,float)
A2(float, )
A1(vpucircleg_,double)
A2(double,_g)
#endif /* UUU */

#else

#ifndef UUU
A1(vp_circle,float)
A2(float, )
A1(vp_circle_g,double)
A2(double,_g)
#else /* UUU */
A1(vp_ucircle,float)
A2(float, )
A1(vp_ucircle_g,double)
A2(double,_g)
#endif /* UUU */

#endif

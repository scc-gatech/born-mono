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
 *  source file:   ./lvplot/vp_arc.c
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
#define S	*s
#define E	*e

#else

#define X	x
#define Y	y
#define R	r
#define S	s
#define E	e

#endif

#define A1(NAM,FTYP) \
int NAM (FTYP X,FTYP Y,FTYP R, FTYP S, FTYP E)
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
 * plot an arc along a circle around (x,y) with radius r starting from
 *             angle s degrees and ending at angle e degrees.
 */
#define A2(FTYP,SUF) \
{ \
FTYP           next_x, next_y; \
double         sangle, eangle, dangle; \
 \
    dangle=1.0/(R*RPERIN); \
    if(S < E) { \
       sangle=S*M_PI/180.0; \
       eangle=E*M_PI/180.0; \
    } else { \
       eangle=S*M_PI/180.0; \
       sangle=E*M_PI/180.0; \
    } \
    next_x = (FTYP) (X+R*cos(sangle)); \
    next_y = (FTYP) (Y+R*sin(sangle)); \
    /* move to initial point on arc */ \
    B2(SUF) \
 \
    while (sangle < eangle) \
    { \
        next_x = (FTYP) (X+R*cos(sangle)); \
        next_y = (FTYP) (Y+R*sin(sangle)); \
        sangle += dangle; \
        B3(SUF) \
    } \
    return (vp_fixpc ## SUF()); \
}
#ifdef FORTRAN

#ifndef UUU
A1(vparc,float)
A2(float, )
A1(vparcg_,double)
A2(double,_g)
#else /* UUU */
A1(vpuarc_,float)
A2(float, )
A1(vpuarcg_,double)
A2(double,_g)
#endif /* UUU */

#else

#ifndef UUU
A1(vp_arc,float)
A2(float, )
A1(vp_arc_g,double)
A2(double,_g)
#else /* UUU */
A1(vp_uarc,float)
A2(float, )
A1(vp_uarc_g,double)
A2(double,_g)
#endif /* UUU */

#endif

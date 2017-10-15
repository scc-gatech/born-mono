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
 *  source file:   ./lvplot/vp_arrow.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * W. Bauske IBM 03-27-91
 *	commented out stuff on ends of #else and #endif for RS/6000
 * Stewart A. Levin (SEP), 18 Feb, 2012
 *      Added double precision _g/g versions
 */

#include "../include/vplot.h"
#include <math.h>
#define pio4 0.785398

#ifdef FORTRAN

#define X0	*x0
#define Y0	*y0
#define X	*x
#define Y	*y
#define R	*r

#else

#define X0	x0
#define Y0	y0
#define X	x
#define Y	y
#define R	r

#endif

#define A1(NAM,FTYP) \
int NAM (FTYP X0,FTYP Y0,FTYP X,FTYP Y,FTYP R)
#ifndef UUU
#define B2(SUF) \
	    vp_area ## SUF (xp, yp, 4, 0, 0, 0);
#else /* UUU */
#define B2(SUF) \
	    vp_uarea ## SUF (xp, yp, 4, 0, 0, 0);
#endif /* UUU */
#ifndef UUU
#define B3(SUF) \
	    vp_area ## SUF (xp, yp, 4, 0, 1, 1);
#else /* UUU */
#define B3(SUF) \
	    vp_uarea ## SUF (xp, yp, 4, 0, 1, 1);
#endif /* UUU */
#ifndef UUU
#define B4(SUF) \
	vp_move ## SUF (X0, Y0); \
	vp_draw ## SUF (X, Y);
#else /* UUU */
#define B4(SUF) \
	vp_umove ## SUF (X0, Y0); \
	vp_udraw ## SUF (X, Y);
#endif /* UUU */
#ifndef UUU
#define B5(SUF) \
	    vp_area ## SUF (xp, yp, 3, 0, 0, 0);
#else /* UUU */
#define B5(SUF) \
	    vp_uarea ## SUF (xp, yp, 3, 0, 0, 0);
#endif /* UUU */
#ifndef UUU
#define B6(SUF) \
	    vp_area ## SUF (xp, yp, 3, 0, 1, 1);
#else /* UUU */
#define B6(SUF) \
	    vp_uarea ## SUF (xp, yp, 3, 0, 1, 1);
#endif /* UUU */
/*
 * plot an arrow from (x0,y0) to (x,y) with arrow-size r
 */
#define A2(FTYP,SUF) \
{ \
FTYP           beta, alpha, xp[4], yp[4], rr; \
int             flag = 0; \
 \
    if (R < 0.) \
    { \
	rr = -(R); \
	flag = 1; \
    } \
    else \
	rr = R; \
 \
    if (X == X0 && Y == Y0) \
    { \
	xp[0] = X - rr / 3.0f; \
	yp[0] = Y - rr / 3.0f; \
	xp[1] = X - rr / 3.0f; \
	yp[1] = Y + rr / 3.0f; \
	xp[2] = X + rr / 3.0f; \
	yp[2] = Y + rr / 3.0f; \
	xp[3] = X + rr / 3.0f; \
	yp[3] = Y - rr / 3.0f; \
	if (flag) \
            B2(SUF) \
	else \
            B3(SUF) \
    } \
    else \
    { \
       B4(SUF) \
 \
	beta = (FTYP) atan2 (Y - Y0, X - X0); \
 \
	xp[0] = X; \
	yp[0] = Y; \
	alpha = (FTYP) (pio4 + beta); \
	xp[1] = (FTYP) (X - rr * cos (alpha)); \
	yp[1] = (FTYP) (Y - rr * sin (alpha)); \
	alpha = (FTYP) (pio4 - beta); \
	xp[2] = (FTYP) (X - rr * cos (alpha)); \
	yp[2] = (FTYP) (Y + rr * sin (alpha)); \
	if (flag) \
           B5(SUF) \
	else \
           B6(SUF) \
    } \
    return (vp_fixpc ## SUF()); \
}
#ifdef FORTRAN

#ifndef UUU
A1(vparrow_,float)
A2(float, )
A1(vparrowg_,double)
A2(double,_g)
#else /* UUU */
A1(vpuarrow_,float)
A2(float, )
A1(vpuarrowg_,double)
A2(double,_g)
#endif /* UUU */

#else

#ifndef UUU
A1(vp_arrow,float)
A2(float, )
A1(vp_arrow_g,double)
A2(double,_g)
#else /* UUU */
A1(vp_uarrow,float)
A2(float, )
A1(vp_uarrow_g,double)
A2(double,_g)
#endif /* UUU */

#endif

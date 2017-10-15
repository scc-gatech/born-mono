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
 *  source file:   ./lvplot/vp_draw.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * W. Bauske IBM 03-27-91
 *	Comment out stuff on ends of #else and #endif for RS/6000
 * Stewart A. Levin (SEP), 17 Feb 2012
 *      Added double precision _g/g versions
 */

#include <stdio.h>
#include "../include/vplot.h"
#include "vp_pc.h"

#ifdef FORTRAN

#ifndef UUU
#define DRAW vpdraw_
#else /* UUU */
#define UDRAW	vpudraw_
#endif /* UUU */

#define X *x
#define Y *y

#else

#ifndef UUU
#define DRAW vp_draw
#else /* UUU */
#define UDRAW	vp_udraw
#endif /* UUU */

#define X x
#define Y y

#endif

#define A1(NAM,FTYP) \
int NAM (FTYP X, FTYP Y)
#ifndef UUU
#define B2(FTYP,SUF) \
    vp_plot ## SUF (X, Y, 1);
#else /* UUU */
#define B2(FTYP,SUF) \
FTYP           xp, yp; \
    xp = vp_pc ## SUF._x0 + (X - vp_pc ## SUF._xu0) * vp_pc ## SUF._xscl; \
    yp = vp_pc ## SUF._y0 + (Y - vp_pc ## SUF._yu0) * vp_pc ## SUF._yscl; \
    vp_plot ## SUF (xp, yp, 1);
#endif /* UUU */
#define A2(FTYP,SUF) \
{ \
  B2(FTYP,SUF) \
  return (vp_fixpc ## SUF()); \
}

#ifdef FORTRAN

#ifdef UUU
A1(vpudraw_,float)
A2(float, )
A1(vpudrawg_,double)
A2(double,_g)
#else
A1(vpdraw_,float)
A2(float, )
A1(vpdrawg_,double)
A2(double,_g)
#endif

#else/* !FORTRAN */

#ifdef UUU
A1(vp_udraw,float)
A2(float, )
A1(vp_udraw_g,double)
A2(double,_g)
#else
A1(vp_draw,float)
A2(float, )
A1(vp_draw_g,double)
A2(double,_g)
#endif

#endif

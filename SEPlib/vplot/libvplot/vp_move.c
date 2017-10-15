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
 *  source file:   ./lvplot/vp_move.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * W. Bauske IBM 03-27-91
 *	Comment out stuff on ends of #else and #endif for RS/6000
 */

#include <stdio.h>
#include "../include/vplot.h"
#include "vp_pc.h"

#ifdef FORTRAN

#ifndef UUU
#define X *x
#define Y *y
#else /* UUU */
#define X		*x
#define Y		*y
#endif /* UUU */

#else

#ifndef UUU
#define X x
#define Y y
#else /* UUU */
#define X		x
#define Y		y
#endif /* UUU */

#endif

#define A1(NAM,FTYP) \
int NAM (FTYP X,FTYP  Y)
#ifndef UUU
#define B2(SUF,FTYP) \
    vp_plot ## SUF ((FTYP) X, (FTYP) Y, 0);
#else
#define B2(SUF,FTYP) \
FTYP           xp, yp; \
    xp =  (vp_pc ## SUF._x0 + (X - vp_pc ## SUF._xu0) * vp_pc ## SUF._xscl); \
    yp =  (vp_pc ## SUF._y0 + (Y - vp_pc ## SUF._yu0) * vp_pc ## SUF._yscl); \
    vp_plot ## SUF (xp, yp, 0);
#endif
#define A2(SUF,FTYP) \
{ \
 B2(SUF,FTYP) \
 return (vp_fixpc ## SUF()); \
}

#ifndef UUU
#ifdef FORTRAN
A1(vpmove_,float)
A2( ,float)
A1(vpmoveg_,double)
A2(_g,double)
#else
A1(vp_move,float)
A2( ,float)
A1(vp_move_g,double)
A2(_g,double)
#endif
#else /* UUU */
#ifdef FORTRAN
A1(vpumove_,float)
A2( ,float)
A1(vpumoveg_,double)
A2(_g,double)
#else
A1(vp_umove,float)
A2( ,float)
A1(vp_umove_g,double)
A2(_g,double)
#endif
#endif

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
 *  source file:   ./lvplot/vp_uplot.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Stewart A. Levin (SEP), 18 Feb 2012
 *      Added double precision _g/g versions
 */

#include <stdio.h>
#include "../include/vplot.h"
#include "vp_pc.h"

#ifdef FORTRAN

#define X		*x
#define Y		*y
#define DOWN	*down

#else

#define X		x
#define Y		y
#define DOWN	down

#endif

#define A1(NAM,FTYP,SUF) \
int NAM (FTYP X, FTYP Y,int  DOWN) \
{ \
FTYP           xp, yp; \
    xp = vp_pc ## SUF._x0 + (X - vp_pc ## SUF._xu0) * vp_pc ## SUF._xscl; \
    yp = vp_pc ## SUF._y0 + (Y - vp_pc ## SUF._yu0) * vp_pc ## SUF._yscl; \
    return vp_plot ## SUF (xp, yp, DOWN); \
}
#ifdef FORTRAN
A1(vpuplot_,float, )
A1(vpuplotg_,double,_g)
#else
A1(vp_uplot,float, )
A1(vp_uplot_g,double,_g)
#endif

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
 *  source file:   ./lvplot/vp_plot.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Joe Dellinger Jan 14 1988
 *	Do rounding (not truncation) before output.
 * Stewart A. Levin (SEP), December 6, 1993
 *      Moved initialization of vp_pc to separate file to aid in
 *      Sun shared library support.
 *  Bob Clapp - Added init function because not allowed to initialize
 *             with non-constant (stdout)
 * Stewart A. Levin (SEP), 17 Feb 2012
 *      Added double precision _g/g versions
 */

#ifndef FORTRAN

#include <math.h>
#include "vp_plot.h"

static double          p_fmod (float, float) ;
static double          p_fmod_g (double, double) ;

#define B1(NAM,FTYP,SUF) \
int NAM (FTYP x,FTYP y,int  down) \
{ \
FTYP           dx, dy, dist, dpos, xp, yp, tonext, cosine, sine; \
int             i; \
 \
 if(vp_pc ## SUF._pltout==0){ vp_plot_init ## SUF();} \
 \
    if (!down || !vp_pc ## SUF._dashon)	/* if move or no dashes */ \
    { \
	p_pout ((float) x, (float) y, down, vp_pc ## SUF._pltout);	/* output a move or draw */ \
	vp_pc ## SUF._xold = x; \
	vp_pc ## SUF._yold = y;		/* save old x and y */ \
	vp_pc ## SUF._dpos = 0.0f;		/* reset position in dashes */ \
	return (vp_fixpc ## SUF()); \
    } \
    dx = x - vp_pc ## SUF._xold; \
    dy = y - vp_pc ## SUF._yold;		/* change in x and y */ \
    dist = (FTYP) sqrt (dx * dx + dy * dy);	/* distance */ \
    if (dist <= 0.0f) \
	return (vp_fixpc ## SUF());			/* return if no change */ \
    cosine = dx / dist; \
    sine = dy / dist; \
    dpos = vp_pc ## SUF._dpos;		/* current position in dashes */ \
    vp_pc ## SUF._dpos = (FTYP) p_fmod ## SUF (dpos + dist, vp_pc ## SUF._ddef[3]);	/* next position in \
							 * dashes */ \
    for (i = 0; i < 4 && vp_pc ## SUF._ddef[i] <= dpos; i++);	/* index to dash def */ \
    xp = vp_pc ## SUF._xold; \
    yp = vp_pc ## SUF._yold;		/* initialize xp and yp */ \
    while (dist > 0.0f) \
    { \
	tonext = vp_pc ## SUF._ddef[i] - dpos;	/* dist to next gap or dash */ \
	if (tonext > dist) \
	    tonext = dist; \
	xp += tonext * cosine; \
	yp += tonext * sine; \
	p_pout ((float) xp, (float) yp, !(i % 2), vp_pc ## SUF._pltout); \
	dpos = vp_pc ## SUF._ddef[i];	/* new position */ \
	i = (i + 1) % 4;	/* i = 0,1,2, or 3 */ \
	if (i == 0) \
	    dpos = 0.0f;		/* back to start of dashes */ \
	dist -= tonext; \
    } \
    vp_pc ## SUF._xold = xp; \
    vp_pc ## SUF._yold = yp; \
    return (vp_fixpc ## SUF()); \
}\
int vp_plot_init ## SUF(void)\
{\
vp_pc ## SUF._pltout=stdout; \
return (vp_fixpc ## SUF()); \
}

void p_pout (float xp,float  yp,int  down, FILE *plt)
{
int             ix, iy;
    xp = (xp > VP_MAX) ? VP_MAX : xp;
    xp = (xp < -VP_MAX) ? -VP_MAX : xp;
    yp = (yp > VP_MAX) ? VP_MAX : yp;
    yp = (yp < -VP_MAX) ? -VP_MAX : yp;
    ix = ROUND (xp * RPERIN);
    iy = ROUND (yp * RPERIN);
    putc ((down ? VP_DRAW : VP_MOVE), plt);
    puth (ix, plt);
    puth (iy, plt);
	return;
}

static double p_fmod (float x,float  y)
{
    return (x - floor (x / y) * y);
}
static double p_fmod_g (double x,double  y)
{
    return (x - floor (x / y) * y);
}


B1(vp_plot,float, )
B1(vp_plot_g,double,_g)

#else

#include <stdio.h>
#include "../include/vplot.h"
#include "vp_pc.h"

void vpplot_ (float *x,float *y,int *down)
{
    vp_plot (*x, *y, *down);
}
void vpplotg_ (double *x, double *y, int *down)
{
    vp_plot_g (*x, *y, *down);
}
#endif

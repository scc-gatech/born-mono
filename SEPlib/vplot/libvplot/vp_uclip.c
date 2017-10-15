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
 *  source file:   ./lvplot/vp_uclip.c
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

#define XMIN	*xmin
#define YMIN	*ymin
#define XMAX	*xmax
#define YMAX	*ymax
#define XPMIN	&xpmin
#define YPMIN	&ypmin
#define XPMAX	&xpmax
#define YPMAX	&ypmax

#else

#define XMIN	xmin
#define YMIN	ymin
#define XMAX	xmax
#define YMAX	ymax
#define XPMIN	xpmin
#define YPMIN	ypmin
#define XPMAX	xpmax
#define YPMAX	ypmax

#endif

#define A1(NAM,FTYP,CLIP,SUF) \
int NAM (FTYP XMIN,FTYP  YMIN,FTYP  XMAX,FTYP  YMAX) \
{ \
float           xpmin, ypmin, xpmax, ypmax; \
    xpmin = (float) (vp_pc ## SUF._x0 + (XMIN - vp_pc ## SUF._xu0) * vp_pc ## SUF._xscl); \
    ypmin = (float) (vp_pc ## SUF._y0 + (YMIN - vp_pc ## SUF._yu0) * vp_pc ## SUF._yscl); \
    xpmax = (float) (vp_pc ## SUF._x0 + (XMAX - vp_pc ## SUF._xu0) * vp_pc ## SUF._xscl); \
    ypmax = (float) (vp_pc ## SUF._y0 + (YMAX - vp_pc ## SUF._yu0) * vp_pc ## SUF._yscl); \
    return CLIP (XPMIN, YPMIN, XPMAX, YPMAX); \
}

#ifdef FORTRAN
A1(vpuclip_,float,vpclip_, )
A1(vpuclipg_,double,vpclipg_,_g)
#else
A1(vp_uclip,float,vp_clip, )
A1(vp_uclip_g,double,vp_clip_g,_g)
#endif

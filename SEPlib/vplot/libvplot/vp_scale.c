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
 *  source file:   ./lvplot/vp_scale.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Stewart A. Levin (SEP), 17 Feb 2012
 *      Added double precision _g/g versions
 * Stewart A. Levin (SEP), 25 Feb 2015
 *      Transmit scale changes to pen filters so they can report interactive
 *      picks in user coordinates.
 */

#include <stdio.h>
#include "../include/vplot.h"
#include "vp_pc.h"

#ifdef FORTRAN

#define XSCL	*xscl
#define YSCL	*yscl

#else

#define XSCL	xscl
#define YSCL	yscl

#endif

#define A1(NAM,FTYP,SUF) \
int NAM (FTYP XSCL, FTYP YSCL) \
{ \
    int rc; \
    float ux, uy; \
    if (vp_pc ## SUF._pltout==0) { rc = vp_plot_init ## SUF(); } \
    else { rc= vp_fixpc ## SUF(); } \
    vp_pc ## SUF._xscl = XSCL; \
    vp_pc ## SUF._yscl = YSCL; \
    ux = (float) (vp_pc ## SUF._x0 - vp_pc ## SUF._xu0 * vp_pc ## SUF._xscl); \
    uy = (float) (vp_pc ## SUF._y0 - vp_pc ## SUF._yu0 * vp_pc ## SUF._yscl); \
    putc (VP_UORIGSCL, vp_pc ## SUF._pltout); \
    putf (ux, vp_pc ## SUF._pltout); \
    putf (uy, vp_pc ## SUF._pltout); \
    putf ((float) vp_pc ## SUF._xscl, vp_pc ## SUF._pltout); \
    putf ((float) vp_pc ## SUF._yscl, vp_pc ## SUF._pltout); \
    return (rc); \
}

#ifdef FORTRAN
A1(vpscale_,float, )
A1(vpscaleg_,double,_g)
#else
A1(vp_scale,float, )
A1(vp_scale_g,double,_g)
#endif

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
 *  source file:   ./lvplot/vp_stretch.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Stewart A. Levin (SEP), 19 Feb 2012
 *      Added double precision _g/g versions
 */

#include <stdio.h>
#include "../include/vplot.h"
#include "./vp_style.h"

#ifdef FORTRAN

#define XMIN	*xmin
#define YMIN	*ymin
#define XMAX	*xmax
#define YMAX	*ymax

#else

#define XMIN	xmin
#define YMIN	ymin
#define XMAX	xmax
#define YMAX	ymax

#endif


#define A1(NAM,FTYP) \
int NAM (FTYP XMIN,FTYP YMIN,FTYP  XMAX,FTYP YMAX)
#define A2(SUF) \
{ \
 \
    vp_uorig ## SUF (XMIN, YMIN); \
    vp_orig ## SUF (0.0f, 0.0f); \
 \
    if (cur_style == ROTATED) \
    { \
	vp_scale ## SUF (ROTATED_HEIGHT / (XMAX - XMIN), (ROTATED_HEIGHT / SCREEN_RATIO) / (YMAX - YMIN)); \
    } \
    else \
    { \
	vp_scale ## SUF ((STANDARD_HEIGHT / SCREEN_RATIO) / (XMAX - XMIN), STANDARD_HEIGHT / (YMAX - YMIN)); \
    } \
    return (vp_fixpc ## SUF()); \
}
#ifdef FORTRAN

A1(vpstretch_,float)
A2( )
A1(vpstretchg_,double)
A2(_g)
#else

A1(vp_stretch,float)
A2( )
A1(vp_stretch_g,double)
A2(_g)

#endif

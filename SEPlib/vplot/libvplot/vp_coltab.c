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
 *  source file:   ./lvplot/vp_coltab.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Stewart A. Levin (SEP), 17 Feb 2012
 *      Added double precision _g/g versions.
 */

#include <stdio.h>

#include "../include/vplot.h"
#undef RED
#undef BLUE
#undef GREEN
#include "params.h"
#include "round.h"

#include "vp_pc.h"

#ifdef FORTRAN

#define COLOR *color
#define RED   *red
#define GREEN *green
#define BLUE  *blue

#else

#define COLOR color
#define RED   red
#define GREEN green
#define BLUE  blue

#endif

#define A1(NAM) \
int NAM (int COLOR,float  RED,float  GREEN,float  BLUE)
#define A2(SUF) \
{ \
int             c, r, g, b; \
    c = COLOR; \
    r = ROUND (RED * MAX_GUN); \
    g = ROUND (GREEN * MAX_GUN); \
    b = ROUND (BLUE * MAX_GUN); \
/*		fprintf(stderr,"adding %d %f %f %f %d %d %d \n",COLOR,RED,GREEN,BLUE,r,g,b);*/ \
 \
    putc (VP_SET_COLOR_TABLE, vp_pc ## SUF._pltout); \
    puth (c, vp_pc ## SUF._pltout); \
    puth (r, vp_pc ## SUF._pltout); \
    puth (g, vp_pc ## SUF._pltout); \
    puth (b, vp_pc ## SUF._pltout); \
    return (vp_fixpc ## SUF()); \
}

#ifdef FORTRAN
A1(vpcoltab_)
A2( )
A1(vpcoltabg_)
A2(_g)
#else
A1(vp_coltab)
A2( )
A1(vp_coltab_g)
A2(_g)
#endif

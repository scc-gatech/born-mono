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
 *  source file:   ./lvplot/vp_hatchload.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Stewart A. Levin (SEP), 17 Feb 2012
 *      Added double precision _g/g versions
 */

#include <stdio.h>
#include "../include/vplot.h"
#include "vp_pc.h"

#ifdef FORTRAN

#define ANGLE   *angle
#define NUMHATCH    *numhatch
#define IHATCH  *ihatch

#else

#define ANGLE   angle
#define NUMHATCH    numhatch
#define IHATCH  ihatch

#endif

#define A1(NAM) \
int NAM (int ANGLE,int  NUMHATCH, int IHATCH, int *hatcharray)
#define A2(SUF) \
{ \
int             c; \
int             ii; \
 \
    putc (VP_PATLOAD, vp_pc ## SUF._pltout); \
    c = ANGLE; \
    puth (c, vp_pc ## SUF._pltout); \
    c = -1; \
    puth (c, vp_pc ## SUF._pltout); \
    c = NUMHATCH; \
    puth (c, vp_pc ## SUF._pltout); \
    c = IHATCH; \
    puth (c, vp_pc ## SUF._pltout); \
 \
    for (ii = 0; ii < 2 * 4 * NUMHATCH; ii++) \
    { \
	c = hatcharray[ii]; \
	if (ii % 4 > 1) \
	    c = (int) (c * RPERIN / HATCHPERIN); \
	puth (c, vp_pc ## SUF._pltout); \
    } \
    return (vp_fixpc ## SUF()); \
}
#ifdef FORTRAN
A1(vphatchload_)
A2( )
A1(vphatchloadg_)
A2(_g)
#else
A1(vp_hatchload)
A2( )
A1(vp_hatchload_g)
A2(_g)
#endif

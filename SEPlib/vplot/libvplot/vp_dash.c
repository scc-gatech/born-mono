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
 *  source file:   ./lvplot/vp_dash.c
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

#define DASH1	*dash1
#define GAP1	*gap1
#define DASH2	*dash2
#define GAP2	*gap2

#else

#define DASH1	dash1
#define GAP1	gap1
#define DASH2	dash2
#define GAP2	gap2

#endif

#define A1(NAM,FTYP) \
int NAM (FTYP DASH1, FTYP GAP1, FTYP DASH2,FTYP  GAP2)
#define A2(SUF) \
{ \
    if (DASH1 < 0. || GAP1 < 0. || DASH2 < 0. || GAP2 < 0.) \
    { \
	vp_pc ## SUF._dashon = 0; \
	return vp_fixpc ## SUF(); \
    } \
    vp_pc ## SUF._ddef[0] = DASH1; \
    vp_pc ## SUF._ddef[1] = vp_pc ## SUF._ddef[0] + GAP1; \
    vp_pc ## SUF._ddef[2] = vp_pc ## SUF._ddef[1] + DASH2; \
    vp_pc ## SUF._ddef[3] = vp_pc ## SUF._ddef[2] + GAP2; \
    if (vp_pc._ddef[3] <= 0.) \
    { \
	vp_pc._dashon = 0; \
	return vp_fixpc ## SUF(); \
    } \
    vp_pc ## SUF._dashon = 1; \
    vp_pc ## SUF._dpos = 0.0; \
    return vp_fixpc ## SUF(); \
}

#ifdef FORTRAN
A1(vpdash_,float)
A2( )
A1(vpdashg_,double)
A2(_g)
#else
A1(vp_dash,float)
A2( )
A1(vp_dash_g,double)
A2(_g)
#endif

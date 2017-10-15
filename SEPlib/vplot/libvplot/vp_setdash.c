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
 *  source file:   ./lvplot/vp_setdash.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Joe Dellinger Jan 14 1988
 *	Do rounding.
 * W. Bauske IBM 03-27-91
 *	Comment out stuff on ends of #else and #endif for RS/6000
 * Stewart A. Levin (SEP), 17 Feb 2012
 *      Added double precision _g/g versions
 */

#include <stdio.h>
#include "../include/vplot.h"
#include "round.h"
#include "vp_pc.h"

#ifdef FORTRAN

#define LP		*lp
#define DASHP dashp2
#define GAPP gapp2

#else

#define LP		lp
#define DASHP dashp
#define GAPP gapp

#endif

#define A1(NAM,FTYP) \
int NAM (FTYP *dashp,FTYP *gapp, int LP)
#ifdef FORTRAN
#define B2(FTYP) \
register FTYP *dashp2, *gapp2; \
    dashp2 = dashp; \
    gapp2 = gapp;
#else
#define B2(FTYP)  
#endif /* FORTRAN */
#define A2(FTYP,SUF) \
{ \
int             i; \
int             ix, iy; \
 \
B2(FTYP) \
 \
    putc (VP_SETDASH, vp_pc ## SUF._pltout); \
    puth (LP, vp_pc ## SUF._pltout); \
    for (i = 0; i < LP; i++) \
    { \
	ix = ROUND ((*DASHP) * RPERIN); \
	iy = ROUND ((*GAPP) * RPERIN); \
	DASHP++; \
	GAPP++; \
	puth (ix, vp_pc ## SUF._pltout); \
	puth (iy, vp_pc ## SUF._pltout); \
    } \
    return vp_fixpc ## SUF(); \
}

#ifdef FORTRAN

A1(vpsetdash_,float)
A2(float, )
A1(vpsetdashg_,double)
A2(double,_g)

#else

A1(vp_setdash,float)
A2(float, )
A1(vp_setdash_g,double)
A2(double,_g)

#endif

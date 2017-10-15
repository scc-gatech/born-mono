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
 *  source file:   ./lvplot/vp_pmark.c
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

#define NPTS	*npts
#define MTYPE	*mtype
#define MSIZE	*msize
#define XP xp2
#define YP yp2

#else

#define NPTS	npts
#define MTYPE	mtype
#define MSIZE	msize
#define XP xp
#define YP yp

#endif

#define A1(NAM,FTYP) \
int NAM (int NPTS,int  MTYPE, int MSIZE, FTYP *xp,FTYP *yp)
#ifdef FORTRAN
#define B2(FTYP) \
FTYP          *xp2, *yp2; \
 \
    xp2 = xp; \
    yp2 = yp;
#else
#define B2  
#endif /* FORTRAN */
#ifdef UUU			/* units are user units */
#define B3(SUF) \
    for (i = 0; i < NPTS; i++) \
    { \
	ix = ROUND ((vp_pc ## SUF._x0 + ((*XP) - vp_pc ## SUF._xu0) * vp_pc ## SUF._xscl) * RPERIN); \
	iy = ROUND ((vp_pc ## SUF._y0 + ((*YP) - vp_pc ## SUF._yu0) * vp_pc ## SUF._yscl) * RPERIN); \
	XP++; \
	YP++; \
	puth (ix, vp_pc ## SUF._pltout); \
	puth (iy, vp_pc ## SUF._pltout); \
    }
#else /* UUU */			/* units are inches */
#define B3(SUF) \
    for (i = 0; i < NPTS; i++) \
    { \
	ix = ROUND ((*XP) * RPERIN); \
	iy = ROUND ((*YP) * RPERIN); \
	XP++; \
	YP++; \
	puth (ix, vp_pc ## SUF._pltout); \
	puth (iy, vp_pc ## SUF._pltout); \
    }
#endif /* UUU */
#define A2(FTYP,SUF) \
{ \
int             i, ix, iy; \
 \
    B2(FTYP) \
 \
    putc (VP_PMARK, vp_pc ## SUF._pltout); \
    puth (NPTS, vp_pc ## SUF._pltout); \
    puth (MTYPE, vp_pc ## SUF._pltout); \
    puth (MSIZE, vp_pc ## SUF._pltout); \
 \
    B3(SUF) \
    return (vp_fixpc ## SUF()); \
}

#ifdef FORTRAN

#ifdef UUU
A1(vpupmark_,float)
A2(float, )
A1(vpupmarkg_,double)
A2(double,_g)
#else /* UUU */
A1(vppmark_,float)
A2(float, )
A1(vppmarkg_,double)
A2(double,_g)
#endif /* UUU */

#else

#ifdef UUU
A1(vp_upmark,float)
A2(float, )
A1(vp_upmark_g,double)
A2(double,_g)
#else /* UUU */
A1(vp_pmark,float)
A2(float, )
A1(vp_pmark_g,double)
A2(double,_g)
#endif /* UUU */

#endif

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
 *  source file:   ./lvplot/vp_area.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Joe Dellinger Jan 14 1988
 *	Do rounding.
 * W. Bauske IBM 03-27-91
 *	Fix the junk on the ends of #else and #endif for RS/6000
 * Stewart A. Levin 17 Feb 2012
 *      Added g/_g double precision versions
 */

#include <stdio.h>
#include "../include/vplot.h"
#include "round.h"
#include "vp_pc.h"

#ifdef FORTRAN

#define LP		*lp
#define FAT		*fat
#define XMASK	*xmask
#define YMASK	*ymask
#define XP xp2
#define YP yp2

#else

#define LP		lp
#define FAT		fat
#define XMASK	xmask
#define YMASK	ymask
#define XP xp
#define YP yp
   
#endif

#define A1(NAM,FTYP) \
int NAM (FTYP *xp, FTYP *yp,int  LP, int FAT,int  XMASK,int  YMASK)
#ifdef FORTRAN
#define B2(FTYP) \
register FTYP *xp2, *yp2; \
    xp2 = xp; \
    yp2 = yp;
#else
#define B2(FTYP)
#endif /* FORTRAN */
#ifndef UUU
#define B3(SUF) \
	ix = ROUND ((*XP) * RPERIN); \
	iy = ROUND ((*YP) * RPERIN);
#else /* UUU */
#define B3(SUF) \
	ix = ROUND ((vp_pc ## SUF._x0 + ((*XP) - vp_pc ## SUF._xu0) * vp_pc ## SUF._xscl) * RPERIN); \
	iy = ROUND ((vp_pc ## SUF._y0 + ((*YP) - vp_pc ## SUF._yu0) * vp_pc ## SUF._yscl) * RPERIN);
#endif /* UUU */
#define A2(FTYP,SUF) \
{ \
int             i; \
int             ix, iy; \
 \
B2(FTYP) \
 \
    putc (VP_OLDAREA, vp_pc ## SUF._pltout); \
    puth (LP, vp_pc ## SUF._pltout); \
    puth (FAT, vp_pc ## SUF._pltout); \
    puth (XMASK, vp_pc ## SUF._pltout); \
    puth (YMASK, vp_pc ## SUF._pltout); \
    for (i = 0; i < LP; i++) \
    { \
        B3(SUF) \
	XP++; \
	YP++; \
	puth (ix, vp_pc ## SUF._pltout); \
	puth (iy, vp_pc ## SUF._pltout); \
    } \
    return (vp_fixpc ## SUF()); \
}

#ifdef FORTRAN

#ifdef UUU
A1(vpuarea_,float)
A2(float, )
A1(vpuareag_,double)
A2(double,_g)
#else
A1(vparea_,float)
A2(float, )
A1(vpareag_,double)
A2(double,_g)
#endif

#else/* ! FORTRAN */

#ifdef UUU
A1(vp_uarea,float)
A2(float, )
A1(vp_uarea_g,double)
A2(double,_g)
#else
A1(vp_area,float)
A2(float, )
A1(vp_area_g,double)
A2(double,_g)
#endif

#endif

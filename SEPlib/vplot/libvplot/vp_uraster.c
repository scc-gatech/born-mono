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
 *  source file:   ./lvplot/vp_uraster.c
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

#define XLL		*xll
#define YLL		*yll
#define XPIX		*xpix
#define YPIX		*ypix
#define PPI		*ppi
#define	BIT		*bit
#define OFFSET		*offset
#define	BLAST		*blast
#define	ORIENT		*orient
#define	INVERT		*invert

#else

#define XLL		xll
#define YLL		yll
#define XPIX		xpix
#define YPIX		ypix
#define PPI		ppi
#define	BIT		bit
#define OFFSET		offset
#define	BLAST		blast
#define	ORIENT		orient
#define	INVERT		invert

#endif

#define A1(NAM,FTYP,RASTER,SUF) \
int NAM (unsigned char *array, int BLAST,int  BIT,int  OFFSET,int  XPIX,int  YPIX,FTYP  XLL,FTYP  YLL,FTYP  PPI,FTYP *xur,FTYP *yur, int ORIENT, int INVERT) \
{ \
FTYP           x1, y1, x2, y2; \
 \
    x1 = vp_pc ## SUF._x0 + (XLL - vp_pc ## SUF._xu0) * vp_pc ## SUF._xscl; \
    y1 = vp_pc ## SUF._y0 + (YLL - vp_pc ## SUF._yu0) * vp_pc ## SUF._yscl; \
 \
    if (PPI == 0.) \
    { \
	x2 = vp_pc ## SUF._x0 + (*xur - vp_pc ## SUF._xu0) * vp_pc ## SUF._xscl; \
	y2 = vp_pc ## SUF._y0 + (*yur - vp_pc ## SUF._yu0) * vp_pc ## SUF._yscl; \
    } \
 \
    RASTER (array, BLAST, BIT, OFFSET, XPIX, YPIX, x1, y1, PPI, &x2, &y2, ORIENT, INVERT); \
 \
    if (PPI != 0. && vp_pc ## SUF._xscl != 0. && vp_pc ## SUF._yscl != 0.) \
    { \
	*xur = (x2 - vp_pc ## SUF._x0) / vp_pc ## SUF._xscl + vp_pc ## SUF._xu0; \
	*yur = (y2 - vp_pc ## SUF._y0) / vp_pc ## SUF._yscl + vp_pc ## SUF._yu0; \
    } \
    return (vp_fixpc ## SUF()); \
}
#ifdef FORTRAN
A1(vpuraster_,float,vpraster_, )
A1(vpurasterg_,double,vprasterg_,_g)
#else
A1(vp_uraster,float,vp_raster, )
A1(vp_uraster_g,double,vp_raster_g,_g)
#endif

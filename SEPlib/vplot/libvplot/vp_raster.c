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
 *  source file:   ./lvplot/vp_raster.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Joe Dellinger Jan 14 1988
 *	Do rounding.
 * Joe Dellinger Jan 14 1990
 *	Can't change passed argument "orient" willy-nilly because
 *	when it comes from FORTRAN it's passed by pointer. Use
 *	"OFFSET" in code instead of "offset".
 * Stewart A. Levin (SEP), 17 Feb 2012
 *      Added double precision _g/g versions
 */

/* This program cannot be indented! */

#include <stdio.h>
#include "../include/vplot.h"
#include "round.h"
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

#define ARRAY(A,B)	array[(((INVERT)==0)?(A):(YPIX-1-(A)))*(XPIX)+(B)]

#define MAXREP	8
#define PUTHSIZE sizeof(int)

#define RASOUT(NUMPAT,NUMBYTE,BITFLAG,BYTES)	\
{\
    puth ((int) (NUMPAT), plotout);\
    puth ((int) (NUMBYTE), plotout);\
\
    if (BITFLAG)\
    {\
	for (nn = 0; nn + 7 < (NUMBYTE); nn += 8)\
	{\
	    obyte = (unsigned char) 0x00;\
\
	    obyte = (unsigned char) (\
		((*((BYTES) + nn + 0) != 0) << 7) |\
		((*((BYTES) + nn + 1) != 0) << 6) |\
		((*((BYTES) + nn + 2) != 0) << 5) |\
		((*((BYTES) + nn + 3) != 0) << 4) |\
		((*((BYTES) + nn + 4) != 0) << 3) |\
		((*((BYTES) + nn + 5) != 0) << 2) |\
		((*((BYTES) + nn + 6) != 0) << 1) |\
		((*((BYTES) + nn + 7) != 0) << 0));\
	    putc ((char) obyte, plotout);\
	}\
	if (nn < (NUMBYTE))\
	{\
	    obyte = (unsigned char) 0x00;\
	    for (nnn = 7; nn < (NUMBYTE); nn++, nnn--)\
	    {\
		obyte = (unsigned char) (obyte |((*((BYTES) + nn) != 0) << nnn));\
	    }\
	    putc ((char) obyte, plotout);\
	}\
    }\
    else\
    {\
	for (nn = 0; nn < (NUMBYTE); nn++)\
	{\
	    putc ((char) * ((BYTES) + nn), plotout);\
	}\
    }\
}

#define A1(NAM,FTYP) \
int NAM (unsigned char *array, int BLAST, int BIT, int OFFSET, int XPIX, int YPIX, FTYP XLL, FTYP YLL,FTYP  PPI,FTYP *xur,FTYP *yur, int ORIENT, int INVERT)
#define A2(FTYP,SUF) \
{ \
    int     mm, nn, nnn, ll, kk, jj, ii, count, ucount, bitbyte; \
    int     ix, iy; \
    unsigned char   obyte; \
    int	    orient_copy; \
    FILE    *plotout = vp_pc ## SUF._pltout; \
 \
    if (BIT) \
    { \
	bitbyte = 8; \
	putc (VP_BIT_RASTER, plotout); \
    } \
    else \
    { \
	bitbyte = 1; \
	putc (VP_BYTE_RASTER, plotout); \
    } \
 \
/* \
 * Necessary for FORTRAN version of the routine to work reliably. \
 */ \
    orient_copy = ORIENT; \
 \
    if (orient_copy >= 0) \
	orient_copy = orient_copy % 4; \
    else \
	orient_copy = ((orient_copy % 4) + 4) % 4; \
 \
    puth ((int) orient_copy, plotout); \
    puth ((int) OFFSET, plotout); \
    ix = ROUND (XLL * RPERIN); \
    iy = ROUND (YLL * RPERIN); \
    puth (ix, plotout); \
    puth (iy, plotout); \
    if (PPI > 0) \
    { \
	ix += ROUND (RPERIN * XPIX / PPI); \
	iy += ROUND (RPERIN * YPIX / PPI); \
	*xur = ix / (FTYP) RPERIN; \
	*yur = iy / (FTYP) RPERIN; \
    } \
    else \
    { \
	ix = ROUND (*xur * RPERIN); \
	iy = ROUND (*yur * RPERIN); \
    } \
    puth (ix, plotout); \
    puth (iy, plotout); \
 \
    puth ((int) XPIX, plotout); \
    puth ((int) YPIX, plotout); \
 \
    if (BLAST) \
    { \
/* Don't try to compact it at all */ \
	for (ii = 0; ii < YPIX; ii++) \
	{ \
	    puth ((int) 1, plotout); \
	    RASOUT (1, XPIX, BIT, &ARRAY (ii, 0)); \
	} \
    } \
    else \
    { \
/* Try to compact it */ \
	count = 1; \
	for (ii = 0; ii < YPIX; ii++) \
	{ \
	    for (jj = 0; jj < XPIX; jj++) \
	    { \
		if (ii == YPIX - 1 || ARRAY (ii, jj) != ARRAY (ii + 1, jj)) \
		{ \
/* Write this row out, it's not just a repeat of the next one */ \
 \
/* Write out how many times this line is to be repeated */ \
		    puth (count, plotout); \
 \
/* \
 * This entire section tries to efficiently represent ONE RASTER LINE \
 */ \
 \
/* \
 * Keep track of "unaccounted for" bytes between \
 * bytes containted in a pattern \
 */ \
		    ucount = 0; \
/* Loop through a raster line */ \
		    for (kk = 0; kk < XPIX; kk++) \
		    { \
/* Try different size patterns */ \
			for (ll = 1; ll <= MAXREP; ll++) \
			{ \
/* See how far this pattern size works */ \
			    for (mm = 0; mm <= XPIX - (kk + ll); mm++) \
			    { \
/* Pattern stopped working */ \
				if (mm == XPIX - (kk + ll) || \
					ARRAY (ii, kk + mm) != ARRAY (ii, kk + mm + ll)) \
				{ \
/* See how many repetitions we got */ \
				    mm = ll * (1 + (int) (mm / ll)); \
/* Is it worth it to use this? */ \
/* (PUTHSIZE is the number of bytes needed for a puth */ \
				    if (PUTHSIZE * 2 + ll <= 1 + (mm - 1) / bitbyte) \
				    { \
/* First take care of bytes not in a pattern */ \
					if (ucount > 0) \
					{ \
					    RASOUT (1, ucount, BIT, &ARRAY (ii, kk - ucount)); \
					    ucount = 0; \
					} \
/* Now take care of the bytes in the pattern */ \
					RASOUT ((int) mm / ll, ll, BIT, &ARRAY (ii, kk)); \
					kk += mm - 1; \
/* Stop looking, we already found what we wanted */ \
					goto compact_found; \
				    } \
/* This pattern didn't work. Stop looking at this size and try a longer one */ \
				    break; \
				} \
			    } \
			} \
/* No pattern including this byte, add it to the unaccounted for list */ \
			ucount++; \
		compact_found:  \
			continue; \
		    } \
/* Take care of any bytes left hanging on the end */ \
		    if (ucount > 0) \
		    { \
			RASOUT (1, ucount, BIT, &ARRAY (ii, kk - ucount)); \
		    } \
/* \
 * END of section that does ONE RASTER LINE \
 */ \
 \
 \
/* Reset counter */ \
		    count = 1; \
/* Exit the loop */ \
		    break; \
		} \
	    } \
/* If we didn't write it out, it will just be a repeat of the next one */ \
	    if (jj == XPIX) \
		count++; \
	} \
    } \
    return (vp_fixpc ## SUF()); \
}

#ifdef FORTRAN

A1(vpraster_,float)
A2(float, )
A1(vprasterg_,double)
A2(double,_g)

#else

A1(vp_raster,float)
A2(float, )
A1(vp_raster_g,double)
A2(double,_g)

#endif

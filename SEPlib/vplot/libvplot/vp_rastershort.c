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
\
    puth ((int) (NUMPAT), plotout);\
    puth ((int) (NUMBYTE), plotout);\
	for (nn = 0; nn < (NUMBYTE); nn++)\
	{\
     j=*(BYTES+nn);\
     k=j/256; j=j-k*256;\
	    putc ((char) k, plotout);\
	    putc ((char) j, plotout);\
	}\
}

#define A1(NAM,FTYP) \
int NAM (unsigned short *array, int BLAST, int BIT, int OFFSET, int XPIX, int YPIX, FTYP XLL, FTYP YLL,FTYP  PPI,FTYP *xur,FTYP *yur, int ORIENT, int INVERT)
#define A2(FTYP,SUF) \
{ \
    int     nn, ii; \
    int     ix, iy,j,k; \
    int	    orient_copy; \
    FILE   *plotout = vp_pc ## SUF._pltout; \
     \
 \
	putc (VP_SHORT_RASTER, plotout); \
 \
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
/* Don't try to compact it at all */ \
	for (ii = 0; ii < YPIX; ii++) \
	{ \
	    puth ((int) 1, plotout); \
	    RASOUT (1, XPIX, BIT, &ARRAY(ii,0)); \
 \
	} \
    return vp_fixpc ## SUF(); \
}
  


#ifdef FORTRAN

/*ARGSUSED*/
A1(vprastershort_,float)
A2(float, )
/*ARGSUSED*/
A1(vprastershortg_,double)
A2(double,_g)

#else

/*ARGSUSED*/
A1(vp_rastershort,float)
A2(float, )
/*ARGSUSED*/
A1(vp_rastershort_g,double)
A2(double,_g)

#endif

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
 *  source file:   ./lvplot/vp_clip.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Joe Dellinger Jan 14 1988
 *	Do rounding.
 * Stewart A. Levin (SEP), 17 Feb, 2012
 *      Added double precision _g/g versions
*/

#include <stdio.h>
#include "../include/vplot.h"
#include "round.h"
#include "vp_pc.h"

#ifdef FORTRAN

#define XMIN *xmin
#define YMIN *ymin
#define XMAX *xmax
#define YMAX *ymax

#else

#define XMIN xmin
#define YMIN ymin
#define XMAX xmax
#define YMAX ymax

#endif

#define A1(NAM,FTYP) \
int NAM (FTYP XMIN,FTYP YMIN,FTYP  XMAX,FTYP  YMAX)
#define A2(SUF) \
{ \
int             ix, iy; \
 \
    putc (VP_WINDOW, vp_pc ## SUF._pltout); \
    ix = ROUND (XMIN * RPERIN); \
    puth (ix, vp_pc ## SUF._pltout); \
    iy = ROUND (YMIN * RPERIN); \
    puth (iy, vp_pc ## SUF._pltout); \
    ix = ROUND (XMAX * RPERIN); \
    puth (ix, vp_pc ## SUF._pltout); \
    iy = ROUND (YMAX * RPERIN); \
    puth (iy, vp_pc ## SUF._pltout); \
    return (vp_fixpc ## SUF()); \
}

#ifdef FORTRAN
A1(vpclip_,float)
A2( )
A1(vpclipg_,double)
A2(_g)
#else
A1(vp_clip,float)
A2( )
A1(vp_clip_g,double)
A2(_g)
#endif

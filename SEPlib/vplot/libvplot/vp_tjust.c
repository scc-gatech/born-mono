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
 *  source file:   ./lvplot/vp_tjust.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 *
 * Joe Dellinger, Dec 7 1987
 *	If the person calls vp_tjust, write the command out.
 *	Don't bother trying to second guess them!!!
 *
 * Stewart A. Levin (SEP), 18 Feb 2012
 *      Added double precision _g/g versions
 */

#include <stdio.h>
#include "../include/vplot.h"
#include "vp_pc.h"

#ifdef FORTRAN

#define XJUST	*xjust
#define YJUST   *yjust

#else

#define XJUST	xjust
#define YJUST   yjust

#endif

#define A1(NAM,SUF) \
int NAM (int XJUST,int YJUST) \
{ \
    vp_pc ## SUF._xjust = XJUST; \
    vp_pc ## SUF._yjust = YJUST; \
 \
    putc (VP_TXALIGN, vp_pc ## SUF._pltout); \
    puth (vp_pc ## SUF._xjust, vp_pc ## SUF._pltout); \
    puth (vp_pc ## SUF._yjust, vp_pc ## SUF._pltout); \
 \
    return (vp_fixpc ## SUF()); \
}

#ifdef FORTRAN
A1(vptjust_, )
A1(vptjustg_,_g)
#else
A1(vp_tjust, )
A1(vp_tjust_g,_g)
#endif

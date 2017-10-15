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
 *  source file:   ./lvplot/vp_patload.c
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

#define PPI   *ppi
#define NX    *nx
#define NY    *ny
#define IPAT  *ipat

#else

#define PPI   ppi
#define NX    nx
#define NY    ny
#define IPAT  ipat

#endif
#define A1(NAM) \
int NAM (int PPI,int  NX,int  NY,int  IPAT,int  *colarray)
#define A2(SUF) \
{ \
int             c; \
int             ii; \
 \
    putc (VP_PATLOAD, vp_pc ## SUF._pltout); \
    c = PPI; \
    puth (c, vp_pc ## SUF._pltout); \
    c = NX; \
    puth (c, vp_pc ## SUF._pltout); \
    c = NY; \
    puth (c, vp_pc ## SUF._pltout); \
    c = IPAT; \
    puth (c, vp_pc ## SUF._pltout); \
 \
    for (ii = 0; ii < NX * NY; ii++) \
    { \
	c = colarray[ii]; \
	puth (c, vp_pc ## SUF._pltout); \
    } \
    return (vp_fixpc ## SUF()); \
}

#ifdef FORTRAN
A1(vppatload_)
A2( )
A1(vppatloadg_)
A2(_g)
#else
A1(vp_patload)
A2( )
A1(vp_patload_g)
A2(_g)
#endif

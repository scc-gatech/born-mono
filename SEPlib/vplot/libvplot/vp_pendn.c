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
 *  source file:   ./lvplot/vp_pendn.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * W. Bauske IBM 03-27-91
 *	Comment out stuff on ends of #else and #endif for RS/6000
 * Stewart A. Levin (SEP), 17 Feb 2012
 *      Added double precision _g/g versions
 */

#include "../include/vplot.h"
#include "vp_pc.h"

/* go to location (x,y) and then put the pen down */
#ifdef FORTRAN

#define X *x
#define Y *y

#else 

#define X x
#define Y y

#endif

#define A1(NAM,FTYP) \
int NAM (FTYP X, FTYP Y)
#ifndef UUU
#define B2(SUF) \
    vp_plot ((float) X, (float) Y, vp_pc ## SUF._pendown);
#else
#define B2(SUF) \
    vp_uplot ((float) X, (float) Y, vp_pc ## SUF._pendown);
#endif
#define A2(SUF) \
{ \
    B2(SUF) \
    vp_pc ## SUF._pendown = 1; \
    return (vp_fixpc ## SUF()); \
}

#ifdef FORTRAN

#ifndef UUU
A1(vppendn_,float)
A2( )
A1(vppendng_,double)
A2(_g)
#else /* UUU */
A1(vpupendn_,float)
A2( )
A1(vpupendng_,double)
A2(_g)
#endif /* UUU */

#else

#ifndef UUU
A1(vp_pendn,float)
A2( )
A1(vp_pendn_g,double)
A2(_g)
#else /* UUU */
A1(vp_upendn,float)
A2( )
A1(vp_upendn_g,double)
A2(_g)
#endif /* UUU */

#endif

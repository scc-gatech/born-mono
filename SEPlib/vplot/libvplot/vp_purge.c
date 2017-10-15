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
 *  source file:   ./lvplot/vp_purge.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Stewart A. Levin (SEP), 17 Feb 2012
 *      Added double precision _g/g versions
 */

#include "../include/vplot.h"
#include "vp_pc.h"

#define A1(NAM) \
int NAM (void)
#define A2(SUF) \
{ \
    putc (VP_PURGE, vp_pc ## SUF._pltout); \
    if (EOF == fflush (vp_pc ## SUF._pltout)) \
    { \
	fprintf (stderr, "\nvp_purge: unable to purge plot\n"); \
    } \
    return (vp_fixpc ## SUF()); \
}

#ifdef FORTRAN

A1(vppurge_)
A2( )
A1(vppurgeg_)
A2(_g)

#else

A1(vp_purge)
A2( )
A1(vp_purge_g)
A2(_g)

#endif

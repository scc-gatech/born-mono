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
 *  source file:   ./lvplot/vp_break.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Stewart A. Levin (SEP), 19 Feb 2012
 *      Added double precision _g/g versions
 */

#include <stdio.h>
#include "../include/vplot.h"
#include "vp_pc.h"

#define A1(NAM,SUF) \
int NAM (void) \
{ \
    putc (VP_BREAK, vp_pc ## SUF._pltout); \
    return (vp_fixpc ## SUF()); \
}
#ifdef FORTRAN

A1(vpbreak_, )
A1(vpbreakg_,_g)

#else

A1(vp_break, )
A1(vp_break_g,_g)
#endif

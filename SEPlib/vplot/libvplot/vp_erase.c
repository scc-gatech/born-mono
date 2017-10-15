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
 *  source file:   ./lvplot/vp_erase.c
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

#define ERASE vperase_

#else

#define ERASE vp_erase

#endif

#define A1(NAM) \
int NAM (void)
#define A2(SUF) \
{ \
    putc (VP_ERASE, vp_pc ## SUF._pltout); \
    return vp_fixpc ## SUF(); \
}

#ifdef FORTRAN
A1(vperase_)
A2( )
A1(vperaseg_)
A2(_g)
#else
A1(vp_erase)
A2( )
A1(vp_erase_g)
A2(_g)
#endif

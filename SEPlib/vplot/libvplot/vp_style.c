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
 *  source file:   ./lvplot/vp_style.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Stewart A. Levin (SEP), December 6, 1993
 *      Fixed duplicate cur_style definition
 * Stewart A. Levin (SEP), 18 Feb 2012
 *      Added double precision _g/g versions
 */

#include "../include/vplot.h"
#include "vp_style.h"

int cur_style = STYLE; /* default set in params.h */

#ifdef FORTRAN

#define ST		*st

#else

#define ST		st

#endif

#define A1(NAM) \
int NAM (int ST)
#define A2(SUF) \
{ \
 \
    cur_style = ST; \
    putc (VP_SETSTYLE, vp_pc ## SUF._pltout); \
    switch (ST) \
    { \
    case ROTATED: \
	putc ('r', vp_pc ## SUF._pltout); \
	break; \
    case ABSOLUTE: \
	putc ('a', vp_pc ## SUF._pltout); \
	break; \
    case STANDARD: \
    default: \
	putc ('s', vp_pc ## SUF._pltout); \
	break; \
    } \
    return (vp_fixpc ## SUF()); \
}
#ifdef FORTRAN
A1(vpstyle_)
A2( )
A1(vpstyleg_)
A2(_g)
#else
A1(vp_style)
A2( )
A1(vp_style_g)
A2(_g)
#endif

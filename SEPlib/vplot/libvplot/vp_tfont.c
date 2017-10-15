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
 *  source file:   ./lvplot/vp_tfont.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 *
 * Joe Dellinger, Dec 7 1987
 *	If they call vp_tfont, just write out the vplot command.
 *	Don't bother trying to second guess them! It's only
 *	asking for trouble.
 *
 * Stewart A. Levin (SEP), 18 Feb 2012
 *      Added double precision _g/g versions
 */

#include <stdio.h>
#include "../include/vplot.h"
#include "vp_pc.h"

#ifdef FORTRAN

#define FONT	*font
#define PREC   *prec
#define OVLY   *ovly

#else

#define FONT	font
#define PREC   prec
#define OVLY   ovly

#endif

#define A1(NAM,SUF) \
int NAM (int FONT, int PREC, int OVLY) \
{ \
    vp_pc ## SUF._font = FONT; \
    vp_pc ## SUF._prec = PREC; \
    vp_pc ## SUF._ovly = OVLY; \
 \
    putc (VP_TXFONTPREC, vp_pc ## SUF._pltout); \
    puth (vp_pc ## SUF._font, vp_pc ## SUF._pltout); \
    puth (vp_pc ## SUF._prec, vp_pc ## SUF._pltout); \
    puth (vp_pc ## SUF._ovly, vp_pc ## SUF._pltout); \
 \
    return (vp_fixpc ## SUF()); \
}
#ifdef FORTRAN
A1(vptfont_, )
A1(vptfontg_,_g)
#else
A1(vp_tfont, )
A1(vp_tfont_g,_g)
#endif

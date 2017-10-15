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
 *  source file:   ./lvplot/vp_endplt.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Stewart A. Levin (SEP), 17 Feb 2012
 *      Added double precision _g/g versions
 */

#include <sepConfig.h>
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include "../include/vplot.h"
#include "vp_pc.h"

#if defined(__stdc__) || defined(__STDC__)
#define ARG1  void
#else
#define ARG1 
#endif
#define A1(NAM) \
int NAM ( ARG1 )
#define A2(SUF) \
{ \
    if (fclose (vp_pc ## SUF._pltout) == EOF) \
    { \
	fprintf (stderr, "libvplot.endplot: cannot close plotfile\n"); \
	exit (-1); \
    } \
 \
    return (vp_fixpc ## SUF()); \
}

#ifdef FORTRAN
A1(vpendplot_)
A2( )
A1(vpendplotg_)
A2(_g)
#else
A1(vp_endplot)
A2( )
A1(vp_endplot_g)
A2(_g)
#endif

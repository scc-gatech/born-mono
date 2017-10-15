/*
 * Copyright 1993 the Board of Trustees of the Leland Stanford Junior
 * University. Official permission to use this software is included in
 * the documentation. It authorizes you to use this file for any
 * non-commercial purpose, provided that this copyright notice is not
 * removed and that any modifications made to this file are commented
 * and dated in the style of my example below.
 */

/*
 *
 *  source file:   ./lvplot/libvplot_sa.c
 *
 * Stewart A. Levin (SEP), December 6 1993
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Bob - Initializing stdout not allowed null +init function
 */

#include <stdio.h>
#include <math.h>
#include "../include/vplot.h"
#include "round.h"
#include "vp_pc.h"

#ifdef FORTRAN
#else
#define A1(SUF) \
PC ## SUF            vp_pc ## SUF  = \
{ \
 0.0, 0.0, \
 0.0, 0.0, \
 1.0, 1.0, \
 0.0, 0.0, \
 0.0, \
 0.0, 0.0, 0.0, 0.0, \
 0, \
 0, \
 0, \
 TH_NORMAL, TV_NORMAL, \
 NO_CHANGE, NO_CHANGE, NO_CHANGE, \
};

A1( )
A1(_g)
#endif

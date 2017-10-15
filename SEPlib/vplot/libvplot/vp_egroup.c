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
 *  source file:   ./lvplot/vp_egroup.c
 *
 * Joe Dellinger (SEP), Feb 27 1988
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 */

#include "../include/vplot.h"
#include "vp_pc.h"

#define A1(NAM) \
int NAM (void)
#define A2(SUF) \
{ \
    putc (VP_END_GROUP, vp_pc ## SUF._pltout); \
    return (vp_fixpc ## SUF()); \
}

#ifdef FORTRAN
A1(vpegroup_)
A2( )
A1(vpegroupg_)
A2(_g)
#else
A1(vp_egroup)
A2( )
A1(vp_egroup_g)
A2(_g)
#endif

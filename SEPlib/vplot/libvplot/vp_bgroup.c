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
 *  source file:   ./lvplot/vp_bgroup.c
 *
 * Joe Dellinger (SEP), Feb 27 1988
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * W. Bauske IBM 03-27-91
 *	Commented out stuff on #else and #endif for RS/6000
 * Stewart A. Levin (SEP), 17 Feb 2012
 *      Added _g/g double precision versions
 */

#include <stdio.h>
#include "../include/vplot.h"
#include "vp_pc.h"

#ifdef FORTRAN

#define BGROUP	vpwbgroup

#else

#define BGROUP	vp_bgroup

#endif

#ifdef FORTRAN
#define A1(NAM) \
int NAM (char *string, int *nchars)
#else
#define A1(NAM) \
int NAM (char *string)
#endif
#ifdef FORTRAN
#define B2 \
int length,i;
#else
#define B2  
#endif
#ifdef FORTRAN
#define B3(SUF) \
/* \
 * Try to still work even if they didn't specify nchars. \
 */ \
    length = 80; \
    if (nchars != NULL) \
    { \
	if (*nchars >= 0) \
	    length = *nchars; \
    } \
 \
    for (i = 0; i < length; i++) \
    { \
	if (*(string + i) == '\0') \
	    break; \
	putc (*(string + i), vp_pc ##SUF._pltout); \
    } \
    putc ('\0', vp_pc._pltout);
#else /* FORTRAN */
#define B3(SUF) \
    do \
    { \
	putc (*string, vp_pc ## SUF._pltout); \
    } \
    while (*string++);
#endif /* FORTRAN */
#define A2(SUF) \
{ \
 B2 \
 \
    putc (VP_BEGIN_GROUP, vp_pc ## SUF._pltout); \
 \
 B3(SUF) \
 return (vp_fixpc ## SUF()); \
}

#ifdef FORTRAN
A1(vpwbgroup_)
A2( )
A1(gvpwbgroup_)
A2(_g)
#else
A1(vp_bgroup)
A2( )
A1(vp_bgroup_g)
A2(_g)
#endif

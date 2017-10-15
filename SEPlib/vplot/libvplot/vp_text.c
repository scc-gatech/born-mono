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
 *  source file:   ./lvplot/vp_text.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 *
 * Joe Dellinger Oct 17 1987
 *	Made Fortran and C calling arguments match.
 *
 * Joe Dellinger Dec 7 1987
 *	Transferred handling of the text justification and font
 *	to the routines where you'd expect that to get handled,
 *	instead of only updating that when this routine is called.
 * W. Bauske IBM 03-27-91
 *	Comment out stuff on ends of #else and #endif of RS/6000
 * Stewart A. Levin (SEP), 18 Feb 2012
 *      Added double precision _g/g versions
 */

#include <stdio.h>
#include "../include/vplot.h"
#include "vp_pc.h"

#ifdef FORTRAN

#define X		*x
#define Y		*y
#define SIZE	*size
#define ORIENT	*orient

#else

#define X		x
#define Y		y
#define SIZE	size
#define ORIENT	orient

#endif

#ifdef FORTRAN
#define A1(NAM,FTYP) \
int NAM (FTYP X,FTYP Y,int  SIZE,int  ORIENT, char *string, int *nchars)
#else
#define A1(NAM,FTYP) \
int NAM (FTYP X,FTYP Y,int  SIZE,int  ORIENT, char *string)
#endif
#ifdef FORTRAN
#define B2 \
int             i, length;
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
	putc (*(string + i), vp_pc ## SUF._pltout); \
    } \
    putc ('\0', vp_pc ## SUF._pltout); 
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
    if (SIZE == 0) \
	return (vp_fixpc ## SUF()); \
 \
    p_pout ((float) X, (float) Y, 0, vp_pc ## SUF._pltout); \
    putc (VP_TEXT, vp_pc ## SUF._pltout); \
    puth (SIZE, vp_pc ## SUF._pltout); \
    puth (ORIENT, vp_pc ## SUF._pltout); \
 \
    B3(SUF) \
    return (vp_fixpc ## SUF()); \
}
#ifdef FORTRAN
A1(vpwtext_,float)
A2( )
A1(vpwtextg_,double)
A2(_g)
#else
A1(vp_text,float)
A2( )
A1(vp_text_g,double)
A2(_g)
#endif

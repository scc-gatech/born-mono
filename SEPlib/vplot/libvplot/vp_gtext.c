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
 *  source file:   ./lvplot/vp_gtext.c
 *
 * Joe Dellinger (SEP), Jan 14 1988
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * W. Bauske IBM 03-27-91
 *	Comment out stuff on ends of #else and #endif for RS/6000
 * Stewart A. Levin (SEP), 19 Feb 2012
 *      Added double precision _g/g versions
 *
 */

#include <stdio.h>
#include "../include/vplot.h"
#include "params.h"
#include "round.h"
#include "vp_pc.h"

#ifdef FORTRAN

#define X		*x
#define Y		*y
#define XPATH		*xpath
#define YPATH		*ypath
#define XUP		*xup
#define YUP		*yup

#else

#define X		x
#define Y		y
#define XPATH		xpath
#define YPATH		ypath
#define XUP		xup
#define YUP		yup

#endif

#ifdef FORTRAN
#define A1(NAM,FTYP) \
int NAM (FTYP X,FTYP Y,FTYP  XPATH,FTYP  YPATH,FTYP  XUP,FTYP  YUP,char *string, int *nchars)
#else
#define A1(NAM,FTYP) \
int NAM (FTYP X,FTYP Y,FTYP  XPATH,FTYP  YPATH,FTYP  XUP,FTYP  YUP,char *string)
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
#else
#define B3(SUF) \
    do \
    { \
	putc (*string, vp_pc ## SUF._pltout); \
    } \
    while (*string++);
#endif
#define A2(SUF) \
{ \
 B2 \
    p_pout ((float) X, (float) Y, 0, vp_pc ## SUF._pltout); \
    putc (VP_GTEXT, vp_pc ## SUF._pltout); \
    puth (ROUND (TEXTVECSCALE * XPATH * RPERIN), vp_pc ## SUF._pltout); \
    puth (ROUND (TEXTVECSCALE * YPATH * RPERIN), vp_pc ## SUF._pltout); \
    puth (ROUND (TEXTVECSCALE * XUP * RPERIN), vp_pc ## SUF._pltout); \
    puth (ROUND (TEXTVECSCALE * YUP * RPERIN), vp_pc ## SUF._pltout); \
 \
 B3(SUF) \
 return (vp_fixpc ## SUF()); \
}

#ifdef FORTRAN
A1(vpwgtext_,float)
A2( )
A1(vpwgtextg_,double)
A2(_g)
#else
A1(vp_gtext,float)
A2( )
A1(vp_gtext_g,double)
A2(_g)
#endif

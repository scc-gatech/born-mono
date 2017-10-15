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
 *  source file:   ./lvplot/vp_utext.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 *
 * Joe Dellinger, Oct 17, 1987
 *      Made Fortran and C calling arguments match.
 *
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
#define XP		&xp
#define YP		&yp

#else

#define X		x
#define Y		y
#define SIZE	size
#define ORIENT	orient
#define XP		xp
#define YP		yp

#endif

#ifdef FORTRAN
#define A1(UTEXT,FTYP) \
int UTEXT (FTYP X,FTYP Y,int SIZE,int  ORIENT,char *string,int *nchars)
#else
#define A1(UTEXT,FTYP) \
int UTEXT (FTYP X,FTYP Y,int SIZE,int ORIENT, char *string)
#endif
#ifdef FORTRAN
#define B2(TEXT) \
    TEXT (XP, YP, size, orient, string, nchars)
#else
#define B2(TEXT) \
    TEXT (XP, YP, size, orient, string);
#endif
#define A2(TEXT,FTYP,SUF) \
{ \
FTYP           xp, yp; \
    xp = vp_pc ## SUF._x0 + (X - vp_pc ## SUF._xu0) * vp_pc ## SUF._xscl; \
    yp = vp_pc ## SUF._y0 + (Y - vp_pc ## SUF._yu0) * vp_pc ## SUF._yscl; \
    return B2(TEXT); \
}
#ifdef FORTRAN
A1(vpwutext_,float)
A2(vpwtext_,float, )
A1(vpwutextg_,double)
A2(vpwtextg_,double,_g)
#else
A1(vp_utext,float)
A2(vp_text,float, )
A1(vp_utext_g,double)
A2(vp_text_g,double,_g)
#endif

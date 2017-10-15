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
 *  source file:   ./lvplot/vp_ugtext.c
 *
 * Joe Dellinger (SEP), Jan 14 1988
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Stewart A. Levin (SEP), 18 Feb 2012
 *      Added double precision _g/g versions
 */

#include <stdio.h>
#include "../include/vplot.h"
#include "vp_pc.h"

#ifdef FORTRAN

#define X		*x
#define Y		*y
#define XP		&xp
#define YP		&yp
#define XPATH		*xpath
#define YPATH		*ypath
#define XPATHP		&xpathp
#define YPATHP		&ypathp
#define XUP		*xup
#define YUP		*yup
#define XUPP		&xupp
#define YUPP		&yupp

#else

#define X		x
#define Y		y
#define XP		xp
#define YP		yp
#define XPATH		xpath
#define YPATH		ypath
#define XPATHP		xpathp
#define YPATHP		ypathp
#define XUP		xup
#define YUP		yup
#define XUPP		xupp
#define YUPP		yupp

#endif

#ifdef FORTRAN
#define A1(NAM,FTYP) \
int NAM (FTYP X,FTYP Y,FTYP XPATH,FTYP YPATH,FTYP XUP,FTYP YUP,char *string,int *nchars)
#else
#define A1(NAM,FTYP) \
int NAM (FTYP X,FTYP Y, FTYP XPATH,FTYP YPATH,FTYP XUP,FTYP YUP, char *string)
#endif
#ifdef FORTRAN
#define B2(TEXT) \
    TEXT (XP, YP, XPATHP, YPATHP, XUPP, YUPP, string, nchars)
#else
#define B2(TEXT) \
    TEXT (XP, YP, XPATHP, YPATHP, XUPP, YUPP, string)
#endif
#define A2(TEXT,FTYP,SUF) \
{ \
FTYP           xp, yp; \
FTYP           xpathp, ypathp; \
FTYP           xupp, yupp; \
 \
    xp = vp_pc ## SUF._x0 + (X - vp_pc ## SUF._xu0) * vp_pc ## SUF._xscl; \
    yp = vp_pc ## SUF._y0 + (Y - vp_pc ## SUF._yu0) * vp_pc ## SUF._yscl; \
    xpathp = XPATH * vp_pc ## SUF._xscl; \
    ypathp = YPATH * vp_pc ## SUF._yscl; \
    xupp = XUP * vp_pc ## SUF._xscl; \
    yupp = YUP * vp_pc ## SUF._yscl; \
 \
    return B2(TEXT);  \
}
#ifdef FORTRAN
A1(vpwugtext_,float)
A2(vpwgtext_,float, )
A1(vpwugtextg_,double)
A2(vpwgtextg_,double,_g)
#else
A1(vp_ugtext,float)
A2(vp_gtext,float, )
A1(vp_ugtext_g,double)
A2(vp_gtext_g,double,_g)
#endif

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
 *  source file:   ./lvplot/vp_orig.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * W. Bauske IBM 03-27-91
 *	Comment out stuff on ends of #else and #endif for RS/6000
 * Stewart A. Levin (SEP), 17 Feb 2012
 *      Added double precision _g/g versions
 * Stewart A. Levin (SEP), 25 Feb 2015
 *      Transmit changes in origin to pen filters so they can report
 *      interactive picks in user coordinates
 */

#include <stdio.h>
#include "../include/vplot.h"
#include "vp_pc.h"

#ifdef FORTRAN

#define X0		*x0
#define Y0		*y0

#else

#define X0		x0
#define Y0		y0

#endif

#define A1(NAM,FTYP) \
int NAM (FTYP X0,FTYP  Y0)
#ifndef UUU
#define B2(SUF) \
    vp_pc ## SUF._x0 = X0; \
    vp_pc ## SUF._y0 = Y0;
#else /* UUU */
#define B2(SUF) \
    vp_pc ## SUF._xu0 = X0; \
    vp_pc ## SUF._yu0 = Y0;
#endif /* UUU */
#define A2(SUF) \
{ \
  int rc; \
  float ux, uy; \
  if ( vp_pc ## SUF._pltout==0 ) { rc = vp_plot_init ## SUF(); } \
  else { rc=vp_fixpc ## SUF(); } \
  B2(SUF) \
  ux = (float) (vp_pc ## SUF._x0 - vp_pc ## SUF._xu0 * vp_pc ## SUF._xscl); \
  uy = (float) (vp_pc ## SUF._y0 - vp_pc ## SUF._yu0 * vp_pc ## SUF._yscl); \
  putc (VP_UORIGSCL, vp_pc ## SUF._pltout); \
  putf (ux,vp_pc ## SUF._pltout); \
  putf (uy,vp_pc ## SUF._pltout); \
  putf ((float) vp_pc ## SUF._xscl, vp_pc ## SUF._pltout); \
  putf ((float) vp_pc ## SUF._yscl, vp_pc ## SUF._pltout); \
  return (rc); \
}

#ifdef FORTRAN

#ifndef UUU
A1(vporig_,float)
A2( )
A1(vporigg_,double)
A2(_g)
#else /* UUU */
A1(vpuorig_,float)
A2( )
A1(vpuorigg_,double)
A2(_g)
#endif

#else

#ifndef UUU
A1(vp_orig,float)
A2( )
A1(vp_orig_g,double)
A2(_g)
#else /* UUU */
A1(vp_uorig,float)
A2( )
A1(vp_uorig_g,double)
A2(_g)
#endif

#endif

/*
 * Copyright 2012 the Board of Trustees of the Leland Stanford Junior
 * University. Official permission to use this software is included in
 * the documentation. It authorizes you to use this file for any
 * non-commercial purpose, provided that this copyright notice is not
 * removed and that any modifications made to this file are commented
 * and dated in the style of my example below.
 */

/*
 *
 *  source file:   ./lvplot/vp_fixpc.c
 *
 * Stewart A. Levin (SEP), 21 Feb 2012
 *      Support double precision _g/g versions by transferring
 *      information between vp_pc and vp_pc_g as appropriate.
 */

#include <stdio.h>
#include "../include/vplot.h"
#include "vp_pc.h"

#define A1(NAM) \
int NAM (void)
#define A2(SUFIN,SUFOUT) \
{ \
    if(((float) (vp_pc ## SUFIN._x0)) != ((float) (vp_pc ## SUFOUT._x0))) \
	vp_pc ## SUFOUT._x0 = (float) vp_pc ## SUFIN._x0; \
    if(((float) (vp_pc ## SUFIN._y0)) != ((float) (vp_pc ## SUFOUT._y0))) \
	vp_pc ## SUFOUT._y0 = (float) vp_pc ## SUFIN._y0; \
    if(((float) (vp_pc ## SUFIN._xu0)) != ((float) (vp_pc ## SUFOUT._xu0))) \
	vp_pc ## SUFOUT._xu0 = (float) vp_pc ## SUFIN._xu0; \
    if(((float) (vp_pc ## SUFIN._yu0)) != ((float) (vp_pc ## SUFOUT._yu0))) \
	vp_pc ## SUFOUT._yu0 = (float) vp_pc ## SUFIN._yu0; \
    if(((float) (vp_pc ## SUFIN._xscl)) != ((float) (vp_pc ## SUFOUT._xscl))) \
	vp_pc ## SUFOUT._xscl = (float) vp_pc ## SUFIN._xscl; \
    if(((float) (vp_pc ## SUFIN._yscl)) != ((float) (vp_pc ## SUFOUT._yscl))) \
	vp_pc ## SUFOUT._yscl = (float) vp_pc ## SUFIN._yscl; \
    if(((float) (vp_pc ## SUFIN._xold)) != ((float) (vp_pc ## SUFOUT._xold))) \
	vp_pc ## SUFOUT._xold = (float) vp_pc ## SUFIN._xold; \
    if(((float) (vp_pc ## SUFIN._yold)) != ((float) (vp_pc ## SUFOUT._yold))) \
	vp_pc ## SUFOUT._yold = (float) vp_pc ## SUFIN._yold; \
    if(((float) (vp_pc ## SUFIN._dpos)) != ((float) (vp_pc ## SUFOUT._dpos))) \
	vp_pc ## SUFOUT._dpos = (float) vp_pc ## SUFIN._dpos; \
    if(((float) (vp_pc ## SUFIN._ddef[0])) != ((float) (vp_pc ## SUFOUT._ddef[0]))) \
	vp_pc ## SUFOUT._ddef[0] = (float) vp_pc ## SUFIN._ddef[0]; \
    if(((float) (vp_pc ## SUFIN._ddef[1])) != ((float) (vp_pc ## SUFOUT._ddef[1]))) \
	vp_pc ## SUFOUT._ddef[1] = (float) vp_pc ## SUFIN._ddef[1]; \
    if(((float) (vp_pc ## SUFIN._ddef[2])) != ((float) (vp_pc ## SUFOUT._ddef[2]))) \
	vp_pc ## SUFOUT._ddef[2] = (float) vp_pc ## SUFIN._ddef[2]; \
    if(((float) (vp_pc ## SUFIN._ddef[3])) != ((float) (vp_pc ## SUFOUT._ddef[3]))) \
	vp_pc ## SUFOUT._ddef[3] = (float) vp_pc ## SUFIN._ddef[3]; \
    if(((int) (vp_pc ## SUFIN._dashon)) != ((int) (vp_pc ## SUFOUT._dashon))) \
	vp_pc ## SUFOUT._dashon = vp_pc ## SUFIN._dashon; \
    if(((int) (vp_pc ## SUFIN._pendown)) != ((int) (vp_pc ## SUFOUT._pendown))) \
	vp_pc ## SUFOUT._pendown = vp_pc ## SUFIN._pendown; \
    if(((FILE *) (vp_pc ## SUFIN._pltout)) != ((FILE *) (vp_pc ## SUFOUT._pltout))) \
	vp_pc ## SUFOUT._pltout = vp_pc ## SUFIN._pltout; \
    if(((int) (vp_pc ## SUFIN._xjust)) != ((int) (vp_pc ## SUFOUT._xjust))) \
	vp_pc ## SUFOUT._xjust = vp_pc ## SUFIN._xjust; \
    if(((int) (vp_pc ## SUFIN._yjust)) != ((int) (vp_pc ## SUFOUT._yjust))) \
	vp_pc ## SUFOUT._yjust = vp_pc ## SUFIN._yjust; \
    if(((int) (vp_pc ## SUFIN._font)) != ((int) (vp_pc ## SUFOUT._font))) \
	vp_pc ## SUFOUT._font = vp_pc ## SUFIN._font; \
    if(((int) (vp_pc ## SUFIN._prec)) != ((int) (vp_pc ## SUFOUT._prec))) \
	vp_pc ## SUFOUT._prec = vp_pc ## SUFIN._prec; \
    if(((int) (vp_pc ## SUFIN._ovly)) != ((int) (vp_pc ## SUFOUT._ovly))) \
	vp_pc ## SUFOUT._ovly = vp_pc ## SUFIN._ovly; \
    return (0); \
}

A1(vp_fixpc)
A2( ,_g)
A1(vp_fixpc_g)
A2(_g, )

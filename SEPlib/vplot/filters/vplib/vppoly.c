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
 *  source file:   ./filters/vplib/vppoly.c
 *
 * Joe Dellinger (SEP), Dec 19 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 */

#include <stdio.h>
#include "../../include/vplot.h"
#include "../include/extern.h"
#include "../include/pat.h"
#include "../include/enum.h"
#include "vp.h"

static float   *xp;
static float   *yp;
static int      vnpts;

void vpstartpoly (int npts)
{
    vpsetflag = NO;
    lost = YES;
    vnpts = 0;

    xp = (float *) malloc ((unsigned) (npts * sizeof (float)));
    yp = (float *) malloc ((unsigned) (npts * sizeof (float)));
}

void vpmidpoly (int x, int y)
{
    xp[vnpts] = (float) (x) / RPERIN;
    yp[vnpts] = (float) (y) / RPERIN;
    vnpts++;
}

/*ARGSUSED*/
void vpendpoly (int last)
{
    if (ipat == 0)
    {
	vp_area (xp, yp, vnpts, -1, pat[ipat].xdim_orig, pat[ipat].ydim_orig);
    }
    else
    {

	if (ipat - 1 != vpcolor)
	    vp_color (ipat - 1);

	vp_fill (xp, yp, vnpts);

	if (ipat - 1 != vpcolor)
	    vp_color (vpcolor);

    }

    free ((char *) xp);
    free ((char *) yp);
}

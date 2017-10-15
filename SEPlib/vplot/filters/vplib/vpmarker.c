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
 *  source file:   ./filters/vplib/vpmarker.c
 *
 * Joe Dellinger (SEP), Dec 19 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 */

#include <stdio.h>
#include "../../include/vplot.h"
#include "../include/round.h"
#include "../include/enum.h"
#include "../include/extern.h"
#include "vp.h"

void vpmarker (int npts, int type, int size, int *pvec)
{
float          *xp;
float          *yp;
int             ii;

    vpsetflag = NO;
    lost = YES;

    xp = (float *) malloc ((unsigned) (npts * sizeof (float)));
    yp = (float *) malloc ((unsigned) (npts * sizeof (float)));

    for (ii = 0; ii < npts; ii++)
    {
	xp[ii] = (float) pvec[2 * ii] / RPERIN;
	yp[ii] = (float) pvec[2 * ii + 1] / RPERIN;
    }

    size = ROUND (size * TXPERIN / RPERIN);

    vp_pmark (npts, type, size, xp, yp);

    free ((char *) xp);
    free ((char *) yp);

}

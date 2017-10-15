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
 *  source file:   ./filters/vplib/vplogvector.c
 *
 * Joe Dellinger (SEP), Jan 10 1988
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 */

#include <stdio.h>
#include "../include/extern.h"
#include "vp.h"

/*ARGSUSED4*/
void vplogvector (int x1, int y1, int x2, int y2, int nfat, int vpdashon)
{
    if (clip (&x1, &y1, &x2, &y2))
	return;

    if (x1 > vpxmax)
	vpxmax = x1;
    if (x1 < vpxmin)
	vpxmin = x1;

    if (y1 > vpymax)
	vpymax = y1;
    if (y1 < vpymin)
	vpymin = y1;

    if (x2 > vpxmax)
	vpxmax = x2;
    if (x2 < vpxmin)
	vpxmin = x2;

    if (y2 > vpymax)
	vpymax = y2;
    if (y2 < vpymin)
	vpymin = y2;
}

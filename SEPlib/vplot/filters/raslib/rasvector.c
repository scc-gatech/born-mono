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
 *  source file:   ./filters/raslib/rasvector.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Joe Dellinger (SEP), May 7 1989
 *	RGB version added.
 */
#include<sepConfig.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../include/extern.h"
#include "raspen.h"

/*
 * This code originally written by Jeff Thorson ("apenh"),
 * stolen by Joe Dellinger and somewhat modified for vplot standardization.
 */

#ifdef RGB
#ifndef BGR
extern int      color_table[NCOLOR][3], color_mult;
#define WRITEIT(A,B) \
Image3 (A, B, 0) = (unsigned char) color_table[rascolor * color_mult][0]; \
Image3 (A, B, 1) = (unsigned char) color_table[rascolor * color_mult][1]; \
Image3 (A, B, 2) = (unsigned char) color_table[rascolor * color_mult][2]
#define RASVECTOR rasvector3
#else
extern int      color_table[NCOLOR][3], color_mult;
#define WRITEIT(A,B) \
Image3 (A, B, 0) = (unsigned char) color_table[rascolor * color_mult][2]; \
Image3 (A, B, 1) = (unsigned char) color_table[rascolor * color_mult][1]; \
Image3 (A, B, 2) = (unsigned char) color_table[rascolor * color_mult][0]
#define RASVECTOR rasvector4
#endif
#else
#ifdef OR
#define WRITEIT(A,B) Image (A, B) = (unsigned char) (Image (A, B) | rascolor)
#define RASVECTOR rasvector2
#else
#define WRITEIT(A,B) Image (A, B) = (unsigned char) rascolor
#define RASVECTOR rasvector
#endif
#endif

void
RASVECTOR (int x1, int y1, int x2, int y2, int nfat, int dashon)
{
int             test, tmp, x, y;
double          slope, fx, fx3, fy, fy3;

/*
 * Vector rasterizes the line defined by the endpoints (x1,y1) and (x2,y2).
 * If 'nfat' is nonzero then draw parallel lines to fatten the line, by
 * recursive calls to vector.
 */

    if (nfat < 0)
	return;

    if (dashon)
    {
	dashvec (x1, y1, x2, y2, nfat, dashon);
	return;
    }

    if (nfat)
    {
	if (clip (&x1, &y1, &x2, &y2))
	    return;

	fatvec (x1, y1, x2, y2, nfat, dashon);
	return;
    }

    if (clip (&x1, &y1, &x2, &y2))
	return;

/* Beware checks out of bounds, since the coordinate system may have rotated */

    test = (abs (x2 - x1) >= abs (y2 - y1));

    if (test)
    {
	if (x1 == x2)
	{
	    /* Just a point */
	    WRITEIT (x1, y1);
	    return;
	}
	else
	if (x1 > x2)
	{
	    tmp = x1;
	    x1 = x2;
	    x2 = tmp;
	    tmp = y1;
	    y1 = y2;
	    y2 = tmp;
	}
	slope = (double) (y2 - y1) / (double) (x2 - x1);
	fy3 = y1;

	for (x = x1, fy = fy3; x < x2; x++, fy += slope)
	{
	    y = (int) (fy + .5f); /* OK rounding, since always positive */
	    WRITEIT (x, y);
	}
	WRITEIT (x2, y2);
	return;
    }
    else
    {
	/* y1 can't equal y2 here */
	if (y1 > y2)
	{
	    tmp = x1;
	    x1 = x2;
	    x2 = tmp;
	    tmp = y1;
	    y1 = y2;
	    y2 = tmp;
	}
	slope = (double) (x2 - x1) / (double) (y2 - y1);
	fx3 = x1;

	for (y = y1, fx = fx3; y < y2; y++, fx += slope)
	{
	    x = (int)(fx + .5f);
	    WRITEIT (x, y);
	}
	WRITEIT (x2, y2);
	return;
    }
}

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
 *  source file:   ./filters/utilities/greycorr.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Joe Dellinger March 28 1988
 *	Do invras here.
 */

/*
 * Utility to modify color tables for plotting grey rasters.
 */

#include <stdio.h>
#include "../include/extern.h"
#include "../include/round.h"

int
greycorr (int colornum)
{
float           newval;
int             outval;

    newval = (float) colornum;

    if (invras)
	newval = 255.0f - newval;

/* 
 * correction to simulate nonlinearity of graphics displays
 */
    if (greyc != 1.0f)
    {
	newval /= 255.0f;
	newval = (-2.0f + 2.0f * greyc) * newval * newval * newval + 3.0f * (1.0f - greyc) * newval * newval + greyc * newval;
	newval *= 255.0f;
	if (newval < 0.0f)
	    newval = 0.0f;
	if (newval > 255.0f)
	    newval = 255.0f;
    }

/*
 * correction for pixel overlap on hardcopy devices
 */
    if (pixc != 1.0f)
    {
	if (newval < pixc * 128.0f)
	{
	    newval /= pixc;
	}
	else
	{
	    newval = 128.0f + (newval - pixc * 128.0f) / (2.0f - pixc);
	}
	if (newval < 0.0f)
	    newval = 0.f;
	if (newval > 255.f)
	    newval = 255.f;
    }

    outval = ROUND (newval);

    return outval;
}

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
 *  source file:   ./filters/utilities/fatvec.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 */

/*
 * Utility routine to make fat vectors from several thin ones.
 * Should ONLY be called if nfat > 0 and dashon = 0
 *
 * Algorithm by Glenn Kroeger
 * Changes added by Joe Dellinger to make it more efficient when plotting
 */

#include <stdio.h>
#include <math.h>
#include "../include/extern.h"

/*ARGSUSED5*/
void fatvec (int x1, int y1, int x2, int y2, int nfat, int dashon)
{
register int    i;
register int    fplus, fminus;
static int      lastdir = 0;

    lastdir = 1 - lastdir;

    if (aspect_ratio != 1. && (y2 != y1 || x2 != x1) && nfat)
    {
nfat = (float) (nfat *sqrt (
	                    ((y2 - y1) * (y2 - y1) + ((x2 - x1) * (x2 - x1))
			     /               (aspect_ratio * aspect_ratio)) /
	                   ((y2 - y1) * (y2 - y1) + ((x2 - x1) * (x2 - x1)))
	));
    }

    fminus = (nfat / 2);
    fplus = (nfat + 1) / 2;

    if (x1 <= x2)
    {
	if (y2 > y1)
	{
	    if (x1 == x2)
	    {
		for (i = -fminus; i <= fplus; i++)
		    dev.vector (x1 + i, y1 - fminus, x2 + i, y2 + fplus, 0, 0);
	    }
	    else
	    {

		if (lastdir)
		{
		    for (i = (fminus + fplus); i > 0; i--)
		    {
			dev.vector (x1 - fminus + i, y1 - fminus, x2 + fplus,
				    y2 + fplus - i, 0, 0);
		    }
		    dev.vector (x1 - fminus, y1 - fminus, x2 + fplus, y2 + fplus, 0, 0);
		    for (i = 1; i < (fminus + fplus + 1); i++)
		    {
			dev.vector (x1 - fminus, y1 - fminus + i, x2 + fplus - i,
				    y2 + fplus, 0, 0);
		    }
		}
		else
		{
		    for (i = (fminus + fplus); i > 0; i--)
		    {
			dev.vector (x1 - fminus, y1 - fminus + i, x2 + fplus - i,
				    y2 + fplus, 0, 0);
		    }
		    dev.vector (x1 - fminus, y1 - fminus, x2 + fplus, y2 + fplus, 0, 0);
		    for (i = 1; i < (fminus + fplus + 1); i++)
		    {
			dev.vector (x1 - fminus + i, y1 - fminus, x2 + fplus,
				    y2 + fplus - i, 0, 0);
		    }
		}
	    }
	}
	else
	if (y2 == y1)
	{
	    for (i = -fminus; i <= fplus; i++)
		dev.vector (x1 - fminus, y1 + i, x2 + fplus, y2 + i, 0, 0);
	}
	else
	{
	    if (x1 == x2)
	    {
		for (i = -fminus; i <= fplus; i++)
		    dev.vector (x1 + i, y1 + fplus, x2 + i, y2 - fminus, 0, 0);
	    }
	    else
	    {

		if (lastdir)
		{
		    for (i = (fminus + fplus); i > 0; i--)
		    {
			dev.vector (x1 - fminus + i, y1 + fplus, x2 + fplus,
				    y2 - fminus + i, 0, 0);
		    }
		    dev.vector (x1 - fminus, y1 + fplus, x2 + fplus, y2 - fminus, 0, 0);
		    for (i = 1; i < (fminus + fplus + 1); i++)
		    {
			dev.vector (x1 - fminus, y1 + fplus - i, x2 + fplus - i,
				    y2 - fminus, 0, 0);
		    }
		}
		else
		{
		    for (i = (fminus + fplus); i > 0; i--)
		    {
			dev.vector (x1 - fminus, y1 + fplus - i, x2 + fplus - i,
				    y2 - fminus, 0, 0);
		    }
		    dev.vector (x1 - fminus, y1 + fplus, x2 + fplus, y2 - fminus, 0, 0);
		    for (i = 1; i < (fminus + fplus + 1); i++)
		    {
			dev.vector (x1 - fminus + i, y1 + fplus, x2 + fplus,
				    y2 - fminus + i, 0, 0);
		    }
		}
	    }
	}
    }
    else
    {
	if (y2 > y1)
	{
	    if (lastdir)
	    {
		for (i = (fminus + fplus); i > 0; i--)
		{
		    dev.vector (x1 + fplus, y1 - fminus + i, x2 - fminus + i, y2 + fplus, 0, 0);
		}
		dev.vector (x1 + fplus, y1 - fminus, x2 - fminus, y2 + fplus, 0, 0);
		for (i = 1; i < (fminus + fplus + 1); i++)
		{
		    dev.vector (x1 + fplus - i, y1 - fminus, x2 - fminus, y2 + fplus - i, 0, 0);
		}
	    }
	    else
	    {
		for (i = (fminus + fplus); i > 0; i--)
		{
		    dev.vector (x1 + fplus - i, y1 - fminus, x2 - fminus, y2 + fplus - i, 0, 0);
		}
		dev.vector (x1 + fplus, y1 - fminus, x2 - fminus, y2 + fplus, 0, 0);
		for (i = 1; i < (fminus + fplus + 1); i++)
		{
		    dev.vector (x1 + fplus, y1 - fminus + i, x2 - fminus + i, y2 + fplus, 0, 0);
		}
	    }
	}
	else
	if (y2 == y1)
	{
	    for (i = -fminus; i <= fplus; i++)
		dev.vector (x1 + fplus, y1 + i, x2 - fminus, y2 + i, 0, 0);
	}
	else
	{
	    if (lastdir)
	    {
		for (i = (fminus + fplus); i > 0; i--)
		{
		    dev.vector (x1 + fplus, y1 + fplus - i, x2 - fminus + i, y2 - fminus, 0, 0);
		}
		dev.vector (x1 + fplus, y1 + fplus, x2 - fminus, y2 - fminus, 0, 0);
		for (i = 1; i < (fminus + fplus + 1); i++)
		{
		    dev.vector (x1 + fplus - i, y1 + fplus, x2 - fminus, y2 - fminus + i, 0, 0);
		}
	    }
	    else
	    {
		for (i = (fminus + fplus); i > 0; i--)
		{
		    dev.vector (x1 + fplus - i, y1 + fplus, x2 - fminus, y2 - fminus + i, 0, 0);
		}
		dev.vector (x1 + fplus, y1 + fplus, x2 - fminus, y2 - fminus, 0, 0);
		for (i = 1; i < (fminus + fplus + 1); i++)
		{
		    dev.vector (x1 + fplus, y1 + fplus - i, x2 - fminus + i, y2 - fminus, 0, 0);
		}
	    }
	}
    }
}

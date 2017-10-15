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
 *  source file:   ./filters/pslib/psplot.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Joe Dellinger (SEP), October 18 1988
 *	Stole imagplot.c to make psplot.c.
 * Joe Dellinger (SEP), October 19 1988
 *	Use "move/draw" method of sending paths.
 * Steve Cole (SEP), July 27 1989
 *      Added check on PATHLENGTH to avoid too many points/path.
 * Steve Cole (SEP), August 26 1990
 *      Removed check for staying at same point in addpath. Correct
 *	vplot behavior is to plot a point in this case, not ignore it.
 * Joe Dellinger (SOEST) June 23 1992
 *	The xold, yold variables in addpath were set but never used.
 * Joe Dellinger (Amoco) April 25 1996
 *	Use "s" instead of "stroke" to save a few bytes.
 *	Use "r" (short for "rlineto") when it uses less space than
 *	"d" (short for "lineto").
 * Joe Dellinger (Amoco) April 26 1996
 *	Jon has plots that consist of zillions of dots, so try to
 *	optimize that case a little bit better. ("x" is shorthand for "0 0 r")
 */

#include <stdio.h>
#include <string.h>
#include "../include/err.h"
#include "../include/enum.h"
#include "../include/extern.h"
#include "ps.h"

int             lost = 1;
static int      where = -1;
static int      ps_oldx = 0, ps_oldy = 0;

void psplot (int x, int y, int draw)
{
    if (draw == 0)
    {
	startpath ();
	lost = 0;
    }
    addpath (x, y);
}

void startpath (void)
{
    if (where > 0)
    {
	endpath ();
    }
    where = 0;
}

void addpath (int x, int y)
{
    /*
     * Just in case, allow lots of extra room
     */
char            stringd[80];
char            stringr[80];

    /* If where is -1 here it is a BUG! */
    if (where)
    {
	/*
	 * Important special case: a dot.
	 */
	if (x - ps_oldx == 0 && y - ps_oldy == 0)
	{
	    fprintf (pltout, "x\n");
	}
	else
	{
	    /*
	     * A bit brute force, but a sure way of using whichever is
	     * shorter: absolute or relative draws.
	     */
	    sprintf (stringd, "%d %d d\n", x, y);
	    sprintf (stringr, "%d %d r\n", x - ps_oldx, y - ps_oldy);

	    if (strlen (stringd) <= strlen (stringr))
		fprintf (pltout, "%s", stringd);
	    else
		fprintf (pltout, "%s", stringr);
            fflush(pltout);
	}
    }
    else
	fprintf (pltout, "%d %d m\n", x, y);

    ps_oldx = x;
    ps_oldy = y;

    where++;
    if (where == PATHLENGTH - 1)
    {
	/* path reached maximum postscript pathlength; restart path */
	endpath ();
    }
}

void endpath (void)
{
    if (where > 0)
    {
	fprintf (pltout, "s\n");
	where = -1;
    }
    lost = 1;
}

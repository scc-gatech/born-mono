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
 *  source file:   ./filters/pslib/psvector.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Steve Cole (SEP), March 24 1988
 *      Corrected computation of linewidth.
 * Steve Cole (SEP), March 31 1988
 *	Added line dashing logic.
 * Joe Dellinger (SEP) October 17 1988
 *	Stole imagvector.c to create psvector.c that does things with
 *	paths.
 * Joe Dellinger (SEP), October 19 1988
 *	Fixed bugs, use "move/draw" method of sending paths.
 * Aritomo Shinozaki (Loomis Laboratory of Physics), February 25 1989
 *	The code which generates dash pattern lines was incorrect.
 * Joe Dellinger (SOEST) June 23 1992
 *	Added dumb_fat option
 * Joe Dellinger (SOEST) October 15 1992
 *	No need for extra space at start of "setlinewidth" command.
 *	If the dash pattern has been completely lost (because of a
 *	grestore) call ps_set_dash to recreate it.
 */

#include <stdio.h>
#include "../include/extern.h"
#include "../include/enum.h"
#include "ps.h"
#define NEW_FAT		(nfat != ps_last_fat)

int             ps_last_fat = -1;

void psvector (int x1, int y1, int x2, int y2, int nfat, int dashon)
{
static int      xlst, ylst;

/* Negative fatness? Skip it. */
    if (nfat < 0)
	return;

/* Clipped away? Skip it. */
    if (clip (&x1, &y1, &x2, &y2))
	return;

/*
 * ALAS, some postscript devices cannot be trusted to do their own
 * fattening. So we provide an option to do it all in the vplot
 * driver.
 */
    if (dumb_fat)
    {
	/* Have to dash FIRST */
	if (dashon)
	{
	    dashvec (x1, y1, x2, y2, nfat, dashon);
	    return;
	}

	/* Then fatten the resulting line segments SECOND */
	if (nfat)		/* Recursively calls itself to make fat lines */
	{
	    fatvec (x1, y1, x2, y2, nfat, dashon);
	    return;
	}

	/*
	 * (Unfortunately this means that if we do fattening in software we
	 * also have to do dashing in software.)
	 */
    }

/*
 *--------------------------------------------------------------------------
 * If dumb_fat == YES, then if we get to this point we must be
 * dealing with thin undashed lines. Just let the default code handle it;
 * it'll keep track of everything properly and won't try to do
 * anything because it will never need to be done.
 *--------------------------------------------------------------------------
 */

/*
 * set the fatness for the path
 */
    if (NEW_FAT)
    {
	endpath ();
	fprintf (pltout, "%d setlinewidth\n", nfat);
	ps_last_fat = nfat;
    }

/*
 * Make sure line dashing style is set correctly. The dashing pattern
 * was set in psattr, but it may or may not be turned on because
 * vplot can draw solid lines at any time without warning. Also,
 * if a new clipping window has been defined the dash pattern will have
 * been completely forgotten. So we have five cases to deal with:
 *
 * dashon=0 and !ps_dash_pattern_set -> thin line is default; just draw
 * dashon>0 and  ps_dash_pattern_set -> ready to draw dashed; just draw
 * dashon=0 and  ps_dash_pattern_set -> save the dash pattern
 *				        set solid line
 *				        ps_dash_pattern_set = NO
 *				        draw
 * dashon>0 and !ps_dash_pattern_set ->
 *				if (ps_dash_pattern_exists) then
 *					reload saved dash pattern
 *				        ps_dash_pattern_set = YES
 *				        draw
 *				else
 *					recreate lost pattern (ps_set_dash)
 *				        (sets ps_dash_pattern_set = YES)
 *				        (sets ps_dash_pattern_exists = YES)
 *					draw
 *				endif
 */
    if (dashon == 0 && ps_dash_pattern_set)
    {
	endpath ();
	fprintf (pltout, "currentdash\n");
	fprintf (pltout, "/vplotoffset exch def\n");
	fprintf (pltout, "/vplotdash exch def\n");
	fprintf (pltout, "[] 0 setdash\n");
	ps_dash_pattern_set = NO;
    }
    else if (dashon != 0 && !ps_dash_pattern_set)
    {
	if (ps_dash_pattern_exists)
	{
	    endpath ();
	    fprintf (pltout, "vplotdash vplotoffset setdash\n");
	    ps_dash_pattern_set = YES;
	}
	else
	{
	    endpath ();
	    /* This will set ps_dash_pattern_set and ps_dash_pattern_exists */
	    ps_set_dash (dashon);
	}
    }

    if ((x1 != xlst) || (y1 != ylst) || lost == 1)
    {
	/* Make sure it is a move, not a draw */
	psplot (x1, y1, 0);
    }
    psplot (x2, y2, 1);
    xlst = x2;
    ylst = y2;
}

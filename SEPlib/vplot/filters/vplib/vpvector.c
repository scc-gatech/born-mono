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
 *  source file:   ./filters/vplib/vpvector.c
 *
 * Joe Dellinger (SEP), Dec 19 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 */

#include <stdio.h>
#include "../../include/vplot.h"
#include "../include/extern.h"
#include "../include/enum.h"
#include "../include/attrcom.h"
#include "../include/round.h"
#include "vp.h"
#define MOVE 0
#define DRAW 1

void vpvector (int x1, int y1, int x2, int y2, int nfat, int vpdashon)
{
static int      xlst, ylst;
int             d1, d2;

    if (nfat < 0)
	return;

    /*
     * Important special case: Zero-length vector at the end of what you've
     * already plotted. Don't need to do anything. 
     */
    if (x1 == x2 && y1 == y2 && !lost && x1 == xlst && y1 == ylst)
    {
	return;
    }

/*
 * As stated in the documentation, dev.vector must be
 * ready to accept changes in fatness and linestyle without
 * warning at any time.
 */

    if (nfat != fat)
    {
	vp_fat (ROUND ((float) nfat * FATPERIN / RPERIN));
	lost = YES;
    }

    if (vpdashon != dashon)
    {
	dev.attributes (NEW_DASH, vpdashon, 0, 0, 0);
    }

    /*
     * Minimize movement of "pen" Don't turn around dashed lines, since order
     * of drawing matters. 
     */
    if (!lost && !vpdashon)
    {
	d1 = abs (x1 - xlst) + abs (y1 - ylst);
	d2 = abs (x2 - xlst) + abs (y2 - ylst);
	if (d2 < d1)
	{
	    d1 = x1;
	    d2 = y1;
	    x1 = x2;
	    y1 = y2;
	    x2 = d1;
	    y2 = d2;
	}
    }

    if ((x1 != xlst) || (y1 != ylst) || lost)
    {
	/* Make sure it is a move, not a draw */
	dev.plot (x1, y1, MOVE);
    }
    dev.plot (x2, y2, DRAW);
    xlst = x2;
    ylst = y2;

/*
 * Restore fat and dash stuff if we changed it.
 */
    if (nfat != fat)
    {
	vp_fat (ROUND ((float) fat * FATPERIN / RPERIN));
	lost = YES;
    }

    if (vpdashon != dashon)
    {
	dev.attributes (NEW_DASH, dashon, 0, 0, 0);
    }
/*
 * Above can be inefficient, but that's a rare case and it's hard
 * to get around. (Very hard.) This works!
 */
}

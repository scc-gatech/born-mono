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
 *  source file:   ./filters/pslib/pstext.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Steve Cole (SEP), September 16 1987
 *      Font 0 is now used if an undefined font is requested.
 * Steve Cole (SEP), March 23 1988
 *      For vplot fonts, added return after gentext call.
 * Steve Cole (SEP), April 2 1988
 *      Changed scaling to agree with other routines.
 * Steve Cole (SEP), June 20 1988
 *      Removed unused array "instruction".
 * Joe Dellinger (SOEST), March 1 1993
 *	Pspen hardware fonts were coming out too small due to a bug
 *	in this routine that used to not matter. This whole routine is
 *	full of bugs, though, and should be re-written from scratch!
 *	For example it should NOT attempt to use yscale and xscale itself.
 *	That is certainly WRONG! I took that part out; better to have
 *	text always come out square than have it scaled wrongly.
 *	(Talking to postscript in terms of "orient" and "size" is pointless
 *	and awkward in any case.) The "DIFFERENT_COORDINATES" stuff was not
 *	working and not worth saving, so I made it always "true".
 */

#include	<math.h>
#include	<stdio.h>
#include	"../../include/vplot.h"
#include	"../include/extern.h"
#include	"../include/enum.h"
#include	"../include/params.h"
#include	"../include/round.h"
#include	"./ps.h"
#include	"./psfonts.h"

static double   path_orient_dx, path_orient_dy;
static double   up_orient_dx, up_orient_dy;

void pstext (char *string, float pathx, float pathy, float upx, float upy)
{
double          fpathx, fpathy, fupx, fupy;
double          up, path;
int             size, orient;
double          yfact, xfact;
static int      last_size = 0, last_font;

    endpath ();

    if (*string == '\0')
	return;

    if (txfont < 100)
    {
	gentext (string, pathx, pathy, upx, upy);
	return;
    }
    else if (txfont > psmaxfont)
    {
	txfont = default_ps_font;
    }

/*
 * Set the inital parameters
 */
    fpathx = (double) pathx;
    fpathy = (double) pathy;
    fupx = (double) upx;
    fupy = (double) upy;

    path = sqrt ((double) (fpathx * fpathx + fpathy * fpathy));
    up = sqrt ((double) (fupx * fupx + fupy * fupy));

    path_orient_dx = fpathx / path;
    path_orient_dy = fpathy / path;
    up_orient_dx = fupx / up;
    up_orient_dy = fupy / up;

/*
 * Postscript manual says height of "700 units" for the default
 * "1000 unit high" font is a typical "cap height". Vplot scales
 * by cap height, so we have to rescale by (1000./700.).
 * But I find that 570 is a better number!
 */
    size = ROUND (path * 1000. / 570.);
    orient = ROUND (acos (path_orient_dx+0.*path_orient_dy+0.*up_orient_dx+0.*up_orient_dy) * 180.0 / 3.141592654);
    if (pathy < 0)
	orient *= -1;

/*
 *   Set the font and size
 */
    if ((size != last_size) || (txfont != last_font))
    {
	fprintf (pltout, "/%s findfont %d scalefont setfont\n",
		 psfonts[txfont - 100], size);
	last_size = size;
	last_font = txfont;
    }

    /*
     * SAVE THE CURRENT GRAPHICS STATE, ROTATE AND/OR SCALE THE COORDINATE
     * SYSTEM BEFORE MOVING
     */
    fprintf (pltout, "gsave\n");

    fprintf (pltout, "%d  %d translate\n", xold, yold);
    fprintf (pltout, "%d rotate\n", orient);


/*
 *  SET THE PROPER ALIGNMENT FROM THE CALCULATED LENGTH OF THE
 *  TEXT STRING
 */
    fprintf (pltout, "(%s) stringwidth pop\n", string);

    switch (txalign.ver)
    {
    case TV_TOP:
	yfact = -0.81 * (float) size;	/* was -1.0 */
	break;
    case TV_CAP:
	yfact = -0.654 * (float) size;	/* was -0.8 */
	break;
    case TV_SYMBOL:		/* CHECK THIS!!! */
    case TV_HALF:
	yfact = -0.327 * (float) size;	/* was -0.45 */
	break;
    case TV_BOTTOM:
	yfact = .1666666667 * (float) size;	/* was - */
	break;
    case TV_BASE:
	yfact = 0.0;
	break;
    default:
	break;
    }

    switch (txalign.hor)
    {
    case TH_RIGHT:
	xfact = -1.;
	break;
	break;
    case TH_NORMAL:
    case TH_LEFT:
	xfact = 0.;
	break;
    case TH_CENTER:
    case TH_SYMBOL:
	xfact = -.5;
	break;
    default:
	break;
    }
    fprintf (pltout, "%.2f mul\n", xfact);
    fprintf (pltout, "%.2f m\n", yfact);

    fprintf (pltout, "(%s) show\n", string);

    fprintf (pltout, "grestore\n");
}

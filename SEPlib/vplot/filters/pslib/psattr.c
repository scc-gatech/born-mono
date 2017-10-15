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
 *  source file:   ./filters/pslib/psattr.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Steve Cole (SEP), March 27 1988
 *      Routine no longer modifies the current vplot color. It modifies
 *      postscript's current color definition only. This is the way
 *      it's supposed to be done.
 * Steve Cole (SEP), March 29 1988
 *      Clipping (the SET_WINDOW case) is now supported.
 * Steve Cole (SEP), March 31 1988
 *      Dashed lines (NEW_DASH) is now supported.
 * Steve Cole (SEP), June 20 1988
 *      Dashon was incorrectly used as a local variable here. Cleaned
 *      up the clipping code some.
 * Steve Cole (SEP), August 19 1988
 *      Added newpath after setting of clipping window. Otherwise when
 *      a stroke command came along, the clipping path would be drawn in.
 * Joe Dellinger (SEP), October 19 1988
 *	Added "grey" option.
 * Steve Cole (SEP), November 1 1991
 *      Replaced grey option with proper color support. Colors are
 *      shaded in grey (like the grey option) by default in PostScript
 *      on greyscale devices.
 * Joe Dellinger (SOEST), October 15 1992
 *	After setting a new clipping window the global plot parameters
 *	line width, dash pattern, and color were being lost. (Polygon
 *	fill pattern is apparently not lost.)
 * Joe Dellinger (SOEST), October 16 1992
 *	Postscript allows through everything STRICTLY INSIDE a clipping
 *	path; the path itself is not included. Thus we have to extend the
 *	postscript clipping box by one minimal coordinate unit in each
 *	direction, because vplot assumes the edges of the clipping window
 *	ARE included.
 * Steve Cole (SEP), November 30 1992
 *      Added color raster support.
 * Joe Dellinger (Amoco), April 24 1996
 *	Wrap the stuff that needs to be done after a grestore up into
 *	a subroutine so it can be called from other routines too.
 * Joe Dellinger (Amoco), March 20 1997
 *	Don't do a grestore without a preceding gsave to go with it.
 * Joe Dellinger (BP Amoco), Oct 5 1999
 *	pspen color=y force=y should just give you the colors you ask for,
 *	period. pspen color=y force=n should give you their complements,
 *	period. Don't try to get excessively tricky flipping some colors
 *	around and not others. Some of the color versus monochrome logic
 *	was entangled.
 */

#include <stdio.h>
#include "./../include/attrcom.h"
#include "./../include/intcom.h"
#include "./../include/params.h"
#include "./../include/extern.h"
#include "./../include/enum.h"
#include "ps.h"

/* Is a dash pattern currently in effect? */
int             ps_dash_pattern_set = NO;
/* Is there a dash pattern saved in postscript's memory? */
int             ps_dash_pattern_exists = NO;
float           psscale;

/* black, blue, red, magenta, green, cyan, yellow, white, ... */
int             red[32768] = {0, 0, 255, 255, 0, 0, 255, 255};
int             green[32768] = {0, 0, 0, 0, 255, 255, 255, 255};
int             blue[32768] = {0, 255, 0, 255, 0, 255, 0, 255};

int             ps_grey_ras[32768];

float           ps_curcolor = 0.;	/* Only used for mono */
static float    new_ps_curcolor;	/* Only used for mono */
static int      ps_curcolor_no = 7;
static int      new_ps_curcolor_no;
static int      ps_curcolor_set = NO;

int             ps_done_clipping_gsave = NO;

int psattributes (int command, int value, int v1, int v2, int v3)
{
int             xmin, ymin, xmax, ymax;

/*
 * Are we in the middle of a move - draw - draw - draw ... sequence?
 * If so, wrap it up into a path and draw it, so we can move on and do
 * whatever needs to be done here!
 */
    endpath ();

    switch (command)
    {
    case SET_COLOR:
	ps_set_color (value);
	break;

    case SET_COLOR_TABLE:
/*
 * Note this will never be called if monochrome.
 */
	red[value] = v1;
	green[value] = v2;
	blue[value] = v3;

#ifdef PSDEBUG
	fprintf (stderr, "Color %d is now %d %d %d\n", value, v1, v2, v3);
#endif

/*
 * Used for greyscale raster
 */
	ps_grey_map (value);

/*
 * Check to see if the color they've just redefined is the one we're
 * currently using!
 */
	if (ps_curcolor_no == value)
	{
	    ps_curcolor_set = NO;
	    ps_set_color (ps_curcolor_no);
	}
	break;

/*
 * It is important that the clipping code begins with a grestore
 * and does a gsave just before setting the clipping path.
 * In PostScript, the clipping path is defined relative to the
 * current clipping path. So if you clipped with some path, then
 * tried to clip with a path that didn't overlap with the original
 * clipping region, you would get nothing. So we gsave before setting
 * the clipping window, then grestore just before re-setting it.
 * The first time through, there is no saved graphics state to restore.
 * Note: we have to check for an "inside out" clipping window
 * (xmin > xmax or ymin > ymax) here, because PostScript will not
 * catch this case.
 */
    case SET_WINDOW:
/*
 * We have to bump the box out by one unit in each direction, because
 * postscript doesn't include the boundary, but vplot does. At least, so says
 * the postscript reference manual. HOWEVER, the SPARC version of postscript
 * doesn't seem to actually do that: it includes the boundary at the
 * upper end, but doesn't at the lower end!!!! Under SPARC's interpretation
 * the clipping windows vary depending on the ORIENTATION of the same
 * postscript plot on the physical page, so I'm going to do it "the right way"
 * here. This allows an extra pixel width of stuff on each axis to sneak
 * through when plotting on the SPARC, unfortunately, but this seems the
 * best compromise to me. -- Joe D.
 */
	xmin = value - 1;
	ymin = v1 - 1;
	xmax = v2 + 1;
	ymax = v3 + 1;

/*
 * Inside-out clipping window test. We use (value,v1) and (v2,v3) instead
 * of (xmin,ymin) and (xmax,ymax), because we need to do the test according
 * to vplot's idea of how clipping windows work, not Postscript's.
 */
	if (value > v2)
	    xmin = xmax;
	if (v1 > v3)
	    ymin = ymax;

	if (ps_done_clipping_gsave == YES)
	    fprintf (pltout, "grestore\n");

	fprintf (pltout, "/clippingpath\n");
	fprintf (pltout, " {newpath\n");
	fprintf (pltout, " %d %d m\n", xmin, ymin);
	fprintf (pltout, " %d %d d\n", xmax, ymin);
	fprintf (pltout, " %d %d d\n", xmax, ymax);
	fprintf (pltout, " %d %d d\n", xmin, ymax);
	fprintf (pltout, " closepath} def\n");

	fprintf (pltout, "gsave clippingpath eoclip\n");
	ps_done_clipping_gsave = YES;

	fprintf (pltout, "newpath\n");

	/*
	 * The grestore caused postscript to forget most of the current
	 * state. Set it all back up again.
	 */
	ps_fixup_after_grestore ();

	/* Global plot parameters fixed; we can exit now. */
	break;

    case NEW_DASH:
	ps_set_dash (value);
	break;

    default:
	break;
    }

    return DOVPLOT_CONT;
}


void ps_fixup_after_grestore (void)
{
/*
 * Alas, when we did the "gerestore" above ALL GLOBAL PLOT SETTINGS WERE LOST!
 * So go back and tidy up. The current fatness is easy; we just need to let
 * psvector know that the current value is the default (0) again. As for the
 * dash pattern, the variable ps_dash_pattern_exists tells psvector it's been
 * lost and the routine ps_set_dash provides a way for psvector to recreate it
 * when needed. The current color has to be reset right away.
 */

    /* The current fatness reverts to 0 */
    ps_last_fat = 0;

    /* No dash pattern is in effect */
    ps_dash_pattern_set = NO;
    /* No dash pattern remains in memory */
    ps_dash_pattern_exists = NO;

    /*
     * The current color got reset to the default. We could try to be tricky
     * and not reset the color if the default is OK, but it's safer and
     * simpler to force the color to be explicitly reset right away.
     */
    ps_curcolor_set = NO;
    ps_set_color (ps_curcolor_no);
}

void ps_grey_map (int coltab)
{
int             grey;

    /*
     * Calculate the grey level that goes with this color.
     */
    grey = (blue[coltab] * 1 + red[coltab] * 2 + green[coltab] * 4 + 6) / 7;

    /*
     * For raster, apply appropriate gamma corrections and invert as
     * necessary.
     */
    ps_grey_ras[coltab] = greycorr (grey);
}

void ps_set_color (int value)
{
    new_ps_curcolor_no = value;


#ifdef PSDEBUG
    fprintf (stderr, "Set? %d, Change Color to %d from %d\n",
	     ps_curcolor_set, new_ps_curcolor_no, ps_curcolor_no);
#endif


    if (mono)
    {
	if (new_ps_curcolor_no > 0)
	    new_ps_curcolor = PS_BLACK;
	else
	    new_ps_curcolor = PS_WHITE;

	if (new_ps_curcolor != ps_curcolor || !ps_curcolor_set)
	{
	    fprintf (pltout, "%.2g setgray\n", new_ps_curcolor);

	    ps_curcolor = new_ps_curcolor;
	    ps_curcolor_no = new_ps_curcolor_no;
	    ps_curcolor_set = YES;
	}
    }
    else
    {
	if (new_ps_curcolor_no != ps_curcolor_no || !ps_curcolor_set)
	{
	    if (force_color)
	    {
#ifdef PSDEBUG
		fprintf (stderr, "%.2g %.2g %.2g setrgbcolor\n", red[new_ps_curcolor_no] / 255., green[new_ps_curcolor_no] / 255., blue[new_ps_curcolor_no] / 255.);
#endif
		fprintf (pltout, "%.2g %.2g %.2g setrgbcolor\n", red[new_ps_curcolor_no] / 255., green[new_ps_curcolor_no] / 255., blue[new_ps_curcolor_no] / 255.);
	    }
	    else
	    {
#ifdef PSDEBUG
		fprintf (stderr, "%.2g %.2g %.2g setrgbcolor\n", 1. - red[new_ps_curcolor_no] / 255., 1. - green[new_ps_curcolor_no] / 255., 1. - blue[new_ps_curcolor_no] / 255.);
#endif
		fprintf (pltout, "%.2g %.2g %.2g setrgbcolor\n", 1. - red[new_ps_curcolor_no] / 255., 1. - green[new_ps_curcolor_no] / 255., 1. - blue[new_ps_curcolor_no] / 255.);
	    }

	    ps_curcolor_no = new_ps_curcolor_no;
	    ps_curcolor_set = YES;
	}
    }
}

void ps_set_dash (int value)
{
/*
 * A local variable named dashon. This routine has no business resetting
 * the global variable of the same name.
 */
int             dashon;
int             ii;

    dashon = value;

/*
 * No need to erase the pattern; psvector knows not to use it if dashon==0.
 */
    if (dashon == 0)
	return;

    fprintf (pltout, "[");
    for (ii = 0; ii < dashon * 2; ii++)
    {
	fprintf (pltout, "%.2f ", dashes[ii] * PSPERIN / psscale);
    }
    fprintf (pltout, "] 0 setdash\n");

    /* A dash pattern is now in postscript's memory. */
    ps_dash_pattern_exists = YES;
    /* And it is currently active, too. */
    ps_dash_pattern_set = YES;
}

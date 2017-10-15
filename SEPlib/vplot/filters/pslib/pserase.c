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
 *  source file:   ./filters/pslib/pserase.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Steve Cole (SEP), March 29 1988
 *      Added plot label.
 * Steve Cole (SEP), March 31 1988
 *      Added support for ncopies and hold command line options.
 * Dave Nichols (SEP), May 22 1990
 *	Added a grestore at the end of the page to match the gsave at the start
 *	Modified output for tex=y to be compatible with /psfig macros in TeX.
 * Joe Dellinger (SOEST), Oct 15 1992
 *	Added a grestore/gsave pair just before printing the label
 *	to turn off any clipping window which may be in effect.
 * Joe Dellinger (AMOCO), April 24 1996
 *	Paper size was not set on pages after the first.
 *	Line join type was not set if tex=y, causing "fanged S syndrome".
 *	The variable "rotate" should not have been used here.
 *	Fatness (and some other variables) needed to be reset at the
 *	beginning of each new page.
 * Joe Dellinger (AMOCO), March 20 1997
 *	If we did a gsave for a clipping window, then do a grestore
 *	here so gsaves and grestores are properly paired.
 */

#include	<stdio.h>
#include	"ps.h"
#include	"../include/erasecom.h"
#include	"../include/enum.h"
#include	"../include/extern.h"

/*
 * Location and height of label and surrounding box in ps coordinates
 */
#define TEXT_YPOS	100
#define TEXT_HEIGHT	50
#define TEXT_PAD	18

float           psscale;

void pserase (int command)
{
char            full_label[100];
static int      page_count = 1;

    endpath ();

    switch (command)
    {
    case ERASE_MIDDLE:
    case ERASE_END:
/*
 * put on label if desired
 */
	if (label[0] != '\0')
	{
	    if (page_count == 1 && command == ERASE_END)
	    {
		sprintf (full_label, "%s", label);
	    }
	    else
	    {
		sprintf (full_label, "%s : Page %d.", label, page_count);
	    }
	    /*
	     * Do a grestore so any clipping window currently in force won't
	     * clip away the plot label. Also do a gsave, because by
	     * convention the plot ends with a grestore.
	     */
	    fprintf (pltout, "grestore gsave\n");
	    fprintf (pltout, "/labelshow\n");
	    fprintf (pltout, " {dup stringwidth pop\n");
	    fprintf (pltout, "  dup dup %d exch sub\n", dev_xmax - 50);
	    fprintf (pltout, "  newpath %d m\n", TEXT_YPOS);
	    fprintf (pltout, "  gsave 0 %d rlineto\n", -TEXT_PAD);
	    fprintf (pltout, "  0 rlineto\n");
	    fprintf (pltout, "  0 %d rlineto\n", TEXT_HEIGHT + TEXT_PAD);
	    fprintf (pltout, "  neg 0 rlineto\n");
	    fprintf (pltout, "  closepath 1 setgray fill grestore\n");
	    fprintf (pltout, "  gsave 0 setgray\n");
	    fprintf (pltout, "  show grestore} def\n");
	    fprintf (pltout, "/Helvetica-BoldOblique findfont %d scalefont setfont\n", TEXT_HEIGHT);
	    fprintf (pltout, "(%s) labelshow\n", full_label);
	}
	if (tex == NO)
	{
	    if (ncopies_document != 1)
	    {
		fprintf (pltout, "/#copies %d def\n", ncopies_document);
	    }
	    if (hold == YES)
	    {
		fprintf (pltout, "statusdict begin\n");
		fprintf (pltout, "/manualfeed true def\n");
		fprintf (pltout, "/manualfeedtimeout 300 def\n");
		fprintf (pltout, "end\n");
	    }
	}

        if (ps_done_clipping_gsave == YES)
            fprintf (pltout, "grestore\n");
        ps_done_clipping_gsave = NO;

	fprintf (pltout, "grestore\n");
	fprintf (pltout, "showpage\n");

	if (command == ERASE_MIDDLE)
	{
	    if (tex == NO)
	    {
		fprintf (pltout, "initgraphics 1 setlinecap 1 setlinejoin\n");
		fprintf (pltout, "%d rotate", 90);
		fprintf (pltout, " 0 %.2f translate %.2f %.2f scale gsave\n",
			 -ps_ypapersize * PSPERIN, psscale, psscale);
	    }
	    else
	    {
		fprintf (pltout, " %.2f %.2f scale gsave\n", psscale, psscale);
		fprintf (pltout, " 1 setlinecap 1 setlinejoin\n");
	    }

	    /*
	     * Postscript has forgetten most of the current state. Set it all
	     * back up again.
	     */
	    ps_fixup_after_grestore ();
	}
	page_count++;
	break;
    case ERASE_START:
    default:
	break;
    }
}

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
 *  source file:   ./filters/xtlib/xtattr.c
 *
 * Steve Cole (SEP), August 4 1991
 *      Inserted this sample edit history entry.
 * Dave Nicols (SEP), February 14 1992
 *      Rewrote using new xtFrame interface that only plots one
 *      frame at a time.
 * Dave Nichols (SEP), April 30 1992
 *      Final version of the new xtpen, added message window.
 * Dave Nichols (SEP), May 13 1992
 *      Changed mono mode to use pixels returned by BlackPixel & WhitePixel
 * Joe Dellinger (SOEST), Oct 16 1992
 *	Vplot includes the boundary of the clipping window in what is shown,
 *	so the width of the clipping window is (max - min + 1).
 * Dave Nichols (SEP) Nov 12 1993
 *	Recognise BREAK_EGROUP as special case and set xt_after_break
 *  	This makes breaks work (if break=b is specified)
 * Dave Nichols (SEP) Dec 14 1993
 *	Use color manipulation routines in xtcolors.c.
 */

#include "xtpen.h"
#include "../include/extern.h"
#include "../include/intcom.h"
#include "../include/attrcom.h"

XRectangle clip_rect;
int clip_xorigin,clip_yorigin;

int xtattributes (int command, int v, int v1, int v2, int v3)
{

    switch (command)
    {
    case SET_COLOR:
	xt_set_color( v );
	break;

    case SET_COLOR_TABLE:
        xt_set_color_table( v, v1, v2, v3 );
	break;

    case SET_WINDOW:

	clip_rect.x = (short) (XCORD( v  ));
	clip_rect.y = (short) (YCORD( v3 ));
	clip_rect.width = (short) (XCORD( v2 ) - XCORD( v ) + 1);
	clip_rect.height = (short) (YCORD( v1 ) - YCORD( v3 ) + 1);
	clip_xorigin = XCORD(dev_xmin); clip_yorigin =  YCORD(dev_ymax);
	XSetClipRectangles( pen_display, pen_gc, clip_xorigin, clip_yorigin,
			 &clip_rect, 1, Unsorted );
	break;
	
    case BEGIN_GROUP:
	break;
    case END_GROUP:
	if( v == 0 ) {
	    /* add the group name to the message window (if it exists ) */
            /*   addText( group_name );*/
            /*   addText( "\n" );*/
	    switch( v1 ) {
		case UNSPECIFIED_EGROUP:
		case USER_EGROUP:
		case ERASE_EGROUP:
		case EOF_ERASE_EGROUP:
		    xt_endframe = YES;
		    break;
		case BREAK_EGROUP:
		    xt_after_break = 1;
		    xt_endframe = YES;
		    break;
		case IGNORE_EGROUP:
		case EOF_IGNORE_EGROUP:
		    xt_endframe = NO; /* eof but not the end of a frame */
		    break;
	    }
	}
		
	break;
    }
    return DOVPLOT_CONT;
}


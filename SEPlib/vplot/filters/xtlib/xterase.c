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
 *  source file:   ./filters/xtlib/xterase.c
 *
 * Steve Cole (SEP), August 4 1991
 *      Inserted this sample edit history entry.
 * Dave Nichols (SEP), April 30 1992
 *      Final version of the new xtpen, added message window
 * Dave Nichols (SEP), Nov 12 1993
 *      Ignore bogus ERASE_MIDDLE after a break.
 */

#include "xtpen.h"
#include "../include/extern.h"
#include "../include/attrcom.h"
#include "../include/erasecom.h"

int xt_after_break = 0;

void xterase (int command)
{

    /* ignore erase middle after a break */
    if( xt_after_break && command == ERASE_MIDDLE ){
	xt_after_break = 0;
	return;
    }

    xt_after_erase = 2;

    switch (command)
    {
    case ERASE_START:
    case ERASE_MIDDLE:
	if( have_pixmap ){
	    clear_pixmap();
	}else{
	    /* clear window */
	      Dimension width, height;
	      Position left, top;

	      XtVaGetValues(pen_picture,XtNwidth,&width,XtNheight,&height,
                XtNx,&left,XtNy,&top,NULL);

	      XSetForeground(pen_display, pen_gc, map[0] );
    	      XFillRectangle(pen_display, pen_drawable, pen_gc,
             			   0,0,width,height);
              XSetForeground(pen_display, pen_gc, color );
	}
	break;
    }
}

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
 *  source file:   ./filters/xtlib/xtplot.c
 *
 * Steve Cole (SEP), August 4 1991
 *      Inserted this sample edit history entry.
 */

#include "xtpen.h"

int             oldx = 0, oldy = 0;

void xtplot (int x, int y, int draw)
{


    if (draw)
    {
	XDrawLine (pen_display, pen_drawable, pen_gc, XCORD (oldx), YCORD (oldy), XCORD (x), YCORD (y));
    }
    oldx = x;
    oldy = y;
}

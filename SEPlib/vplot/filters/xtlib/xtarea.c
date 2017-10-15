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
 *  source file:   ./filters/xtlib/xtarea.c
 *
 * Steve Cole (SEP), August 4 1991
 *      Inserted this sample edit history entry.
 * Dave Nichols (SEP), April 30 1992
 *      Final version of the new xtpen, added message window.
 */

#include "xtpen.h"
#include "../include/vertex.h"
#define MAXVERT 1000
XPoint vlist[MAXVERT];

void
xtarea (
    int             npts,
    struct vertex  *head
       )
{
int             i;

    /* translate data structures */
    for (i = 0; i < npts && i < MAXVERT; i++)
    {
	vlist[i].x = (short) XCORD (head->x);
	vlist[i].y = (short) YCORD (head->y);
	head = head->next;
    }
    XDrawLines(pen_display, pen_drawable, pen_gc, vlist, npts, CoordModeOrigin);
    XFillPolygon(pen_display, pen_drawable, pen_gc, vlist, npts, Complex, CoordModeOrigin);
}

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
 *  source file:   ./filters/genlib/genhatch.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Joe Dellinger, Feb 16 1988
 *	Make number of arguments to dev.attributes consistent.
 * W. Bauske 03-26-91
 *	Remove re-declare of system alloc routine for RS/6000
*  Bob Clapp
 *     Switch malloc,stdlib to site def

 */

#include <sepConfig.h>
#include <stdio.h>
#include <math.h>
#include "../include/enum.h"
#include "../include/vertex.h"
#include "../include/params.h"
#include "../include/extern.h"
#include "../include/attrcom.h"

static void poly_rot (float angle, int *x, int *y);

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#else
extern char    *malloc ();
#endif

#if defined(__stdc__) || defined(__STDC__)
void
genhatch (int npts, int numhatch, float angle, int *hafat, int *hacol, int *haoff, int *hasiz, struct vertex *head)
#else
genhatch (npts, numhatch, angle, hafat, hacol, haoff, hasiz, head)
    int             npts, numhatch;
    struct vertex  *head;
    float           angle;
    int            *hafat, *hacol, *haoff, *hasiz;
#endif
{
register int    x, y, i;
int             xstr, xend, ystr, yend, y1, y2, x1, x2, start;
int             xa[4], ya[4], xwmin_r, xwmax_r, ywmin_r, ywmax_r;
int             skip, which_time;
int             ncross;
int             vminx, vmaxx, vminy, vmaxy;
struct vertex  *xhead, *yhead, *v;
int            *crosses;
int             cur_color_save;

    cur_color_save = cur_color;

    v = head;
    for (i = 0; i < npts; i++)
    {
	poly_rot (angle, &v->x, &v->y);
	v++;
    }

    /*
     * allocate storage for scan line cross points 
     */
    crosses = (int *) malloc ((unsigned) npts * sizeof (int));

    /*
     * double link the vertices. (head) is set to the node with the maximum
     * x-value so that intersect() will not eliminate 'head' while casting
     * off vertices. 
     */
    vminx = head->x;
    vmaxx = head->x;
    vminy = head->y;
    vmaxy = head->y;
    xhead = head;
    yhead = head;

    v = head;
    for (i = 0; i < npts; i++)
    {
	if (v->x > vmaxx)
	{
	    vmaxx = v->x;
	    xhead = v;
	}
	if (v->y > vmaxy)
	{
	    vmaxy = v->y;
	    yhead = v;
	}
	if (v->x < vminx)
	    vminx = v->x;
	if (v->y < vminy)
	    vminy = v->y;
	v++;
    }

/*
 * Find a new window which contains the old, rotated window
 */
    xwmin_r = xa[0] = xa[1] = xwmin;
    xwmax_r = xa[2] = xa[3] = xwmax;
    ywmin_r = ya[0] = ya[3] = ywmin;
    ywmax_r = ya[1] = ya[2] = ywmax;
    for (i = 0; i < 4; i++)
    {
	poly_rot (angle, &xa[i], &ya[i]);
	if (xwmin_r > xa[i])
	    xwmin_r = xa[i];
	if (ywmin_r > ya[i])
	    ywmin_r = ya[i];
	if (xwmax_r < xa[i])
	    xwmax_r = xa[i];
	if (ywmax_r < ya[i])
	    ywmax_r = ya[i];
    }

    if (vmaxx > xwmax_r)
	vmaxx = xwmax_r;
    if (vminx < xwmin_r)
	vminx = xwmin_r;
    if (vmaxy > ywmax_r)
	vmaxy = ywmax_r;
    if (vminy < ywmin_r)
	vminy = ywmin_r;

    /* stretch polygon in y-direction */
    v = yhead;
    do
    {
	v->y = 2 * (v->y) + 1;
	v = v->next;
    } while (v != yhead);

    for (which_time = numhatch; which_time < 2 * numhatch; which_time++)
    {
	if (hasiz[which_time] > 0)
	{
	    if (cur_color != hacol[which_time] || need_devcolor)
	    {
		cur_color = hacol[which_time];
		dev.attributes (SET_COLOR, cur_color, 0, 0, 0);
		need_devcolor = NO;
	    }

	    skip = hasiz[which_time];
	    start = haoff[which_time] + skip * (int) (vminy / skip);
	    for (y = start; y <= vmaxy; y += skip)
	    {
		ncross = intersect (2 * y, crosses, yhead, 1);
		sort (crosses, ncross);
		for (i = 0; i < ncross; i += 2)
		{
		    xstr = crosses[i];
		    xend = crosses[i + 1];
		    y1 = y2 = y;
		    poly_rot (-angle, &xstr, &y1);
		    poly_rot (-angle, &xend, &y2);
		    dev.vector (xstr, y1, xend, y2, hafat[which_time], 0);
		}
	    }
	}
    }
    /* shrink in y */
    v = yhead;
    do
    {
	v->y = ((v->y - 1) / 2);
	v = v->next;
    } while (v != yhead);

    /*
     * expand in x 
     */
    v = xhead;
    do
    {
	v->x = 2 * v->x + 1;
	v = v->next;
    } while (v != xhead);

    for (which_time = 0; which_time < numhatch; which_time++)
    {
	if (hasiz[which_time] > 1)
	{
	    if (cur_color != hacol[which_time] || need_devcolor)
	    {
		cur_color = hacol[which_time];
		dev.attributes (SET_COLOR, cur_color, 0, 0, 0);
		need_devcolor = NO;
	    }

	    skip = hasiz[which_time];
	    start = haoff[which_time] + skip * (int) (vminx / skip);
	    for (x = start; x <= vmaxx; x += skip)
	    {
		ncross = intersect (2 * x, crosses, xhead, 0);
		sort (crosses, ncross);
		for (i = 0; i < ncross; i += 2)
		{
		    ystr = crosses[i];
		    yend = crosses[i + 1];
		    x1 = x2 = x;
		    poly_rot (-angle, &x1, &ystr);
		    poly_rot (-angle, &x2, &yend);
		    dev.vector (x1, ystr, x2, yend, hafat[which_time], 0);
		}
	    }

	}
    }
    /*
     * shrink in x 
     */
    v = xhead;
    do
    {
	v->x = ((v->x - 1) / 2);
	v = v->next;
    } while (v != xhead);

    free ((char *) crosses);

    if (cur_color != cur_color_save)
    {
	cur_color = cur_color_save;
	need_devcolor = YES;
    }
}

#if defined(__stdc__) || defined(__STDC__)
void poly_rot (float angle, int *x, int *y)
#else
poly_rot (angle, x, y)
    float           angle;
    int            *x, *y;
#endif
{
int             temp;
    temp = (int) ((((*x) * cos(angle)) - ((*y) * sin (angle))) + .5);
    *y = (int) ((((*x) * sin (angle)) + ((*y) * cos (angle))) + .5);
    *x = temp;
}

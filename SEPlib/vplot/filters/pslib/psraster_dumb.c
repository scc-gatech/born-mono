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
 *  source file:   ./filters/pslib/psraster.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Steve Cole (SEP), September 11 1987
 *	Wrote dumb version of psraster.c.
 * Steve Cole (SEP), February 10 1988
 *	Rewrote using readhexstring instead of creating procedure for image.
 *      This avoids possible size problems.
 * Joe Dellinger, Feb 16 1988
 *	Make order and type of arguments same for both kinds of raster, to
 *	avoid problems with machines that have to have consistency of
 *	arguments.
 * Steve Cole (SEP), March 28 1988
 *      Cleaned up bit-handling code. Fixed bugs in handling of orient
 *      parameter and documented that code.
 * Steve Cole (SEP), March 31 1988
 *	Corrected polarity for an input 8 bits/pixel raster.
 * Joe Dellinger (SEP), October 19 1988
 *	Added "harddither" option.
 * Stew Levin (Mobil), May 8, 1996
 *	Changed mask0 definition to make source portable to machines
 *	that don't handle bit strings properly.
 */

#define mask0	((unsigned char) (((unsigned int) 1)  << 7))
#include <stdio.h>
#include "ps.h"
#include "../include/err.h"
#include "../include/extern.h"

/*ARGSUSED7*/
void dumb_psraster (int count, int out_of, int xpos, int ypos, int length, int orient, unsigned short *raster, int dummy1, int dummy2, int byte2)
{
int             i;
unsigned char   mask, outchar;
int             bitcount;
int             xshift, yshift;
int             xscale, yscale;
int             rangle;


/*   if(byte2==1)*/
/*   ERR(FATAL,"Asked for two byte option with dumb_psraster (unimplemented) \n");*/


    endpath ();

    if (count == 0)
    {
/*
 * Comments to explain the code for different orientations:
 * The PostScript command image outputs a raster with its lower left corner
 * at the origin, assuming that the raster is filled from (given the
 * transformation matrix we use below) left to right and top to bottom.
 * Depending on orient, this is not the way that the input raster is
 * filled. For orient 1 it is filled from top to bottom and right to
 * left, for 2 it is from right to left and bottom to top, and for 3
 * it is from left to right and bottom to top. We must apply a transformation
 * that changes the input raster so it appears to be filled in the orient
 * 0 manner. This is just a simple rotation. For orient=(0,1,2,3) the
 * rotation angle (measured clockwise) is (0,270,180,90) degrees.
 * This rotation is done about the origin.
 * To understand the postioning of the origin, for a given orientation,
 * start with a picture of what the raster looks like after the rotation.
 * The frist point of the first raster line (xpos,ypos) is at the upper
 * left. Now "back out" the rotation. The starting point is now at the
 * (upper left,upper right,lower right,lower left) corner for orient
 * (0,1,2,3). But we don't want the rotation to be performed about this
 * first point - we want it to be performed about the first point of the
 * LAST line of raster (sketch this on paper, then it is easy to see why).
 * So we want the origin to be at the beginning of the last line of raster.
 * Starting from the beginning of the first line, the shift needed to get
 * there is (0,-out_of) for orient 0, (-out_of,0) for orient 1,
 * (0,out_of) for orient 2, and (out_of,0) for orient 3. After this
 * translation and rotation, the scaling necessary to get the right
 * size raster is pretty obvious.
 *
 * Don't try to understand these comments unless you need to modify the
 * code! If you do need to modify the code, I think that these comments 
 * will help, even if it takes a few minutes to digest them.
 * A final note: the way to do all this neatly is to incorporate the
 * shifting, scaling, and rotating described here directly into the
 * transform matrix for image. But as long as this code works maybe
 * it's not worth the effort.
 * -S. Cole
 */

	if (orient == 0)
	{
	    xshift = xpos;
	    yshift = ypos - out_of;
	    xscale = length;
	    yscale = out_of;
	    rangle = 0;
	}
	else
	if (orient == 1)
	{
	    xshift = xpos - out_of;
	    yshift = ypos;
	    xscale = out_of;
	    yscale = length;
	    rangle = 270;
	}
	else
	if (orient == 2)
	{
	    xshift = xpos;
	    yshift = ypos + out_of;
	    xscale = length;
	    yscale = out_of;
	    rangle = 180;
	}
	else
	if (orient == 3)
	{
	    xshift = xpos + out_of;
	    yshift = ypos;
	    xscale = out_of;
	    yscale = length;
	    rangle = 90;
	}

	fprintf (pltout, "/picstr %d string def\n", (length + 7) / 8);

	fprintf (pltout, "gsave %d %d translate %d %d scale %d rotate\n", xshift, yshift, xscale, yscale, rangle);

	fprintf (pltout, "/raster {%d %d %d [ %d 0 0 %d 0 %d ] {currentfile picstr readhexstring pop} image} def\n", length, out_of, 1, length, -out_of, out_of);

	fprintf (pltout, "raster\n");
    }

    mask = mask0;
    outchar = 0;
    bitcount = 0;

    for (i = 0; i < length; i++)
    {
	if (raster[i] == 0)
	    outchar = (unsigned char) (outchar|mask);
	mask >>= 1;
	bitcount++;
	if (bitcount == 8)
	{
	    fprintf (pltout, "%2.2x", outchar);
	    mask = mask0;
	    outchar = 0;
	    bitcount = 0;
	}
    }
    if (bitcount != 0)
	fprintf (pltout, "%2.2x", outchar);

    if (count != out_of - 1)
	return;

    fprintf (pltout, "\n");
    fprintf (pltout, "grestore\n");

}

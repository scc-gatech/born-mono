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
 *	Wrote smart version of psraster.c.
 * Steve Cole (SEP), February 10 1988
 *      Rewrote using readhexstring instead of creating procedure for image.
 *      This avoids possible size problems.
 *	Fixed orientation problems.
 * Steve Cole (SEP), March 28 1988
 *      Simplified the handling of the raster. In this routine we want
 *      always to output bytes and let PostScript do its own dithering.
 * Steve Cole (SEP), March 31 1988
 *      Corrected polarity of output raster.
 * Joe Dellinger (SEP), October 19 1988
 *	Added "harddither" option.
 * Steve Cole (SEP), November 30 1992
 *	Added color raster support.
 * Dave Nichols (SEP), April 7 1993
 *	Made grey rasters split their lines (like color ones do)
 * Joe Dellinger, David Lumley, 10-03-94
 * 	Check external variable ras_allgrey to decide if only gray raster
 *	is needed when color is requested by the pspen color=y option.
 */

#include <stdio.h>
#include "ps.h"
#include "../include/err.h"
#include "../include/extern.h"


/*ARGSUSED9*/
void smart_psraster (int xpix, int ypix, int xmin, int ymin, int xmax, int ymax, unsigned short *raster_block, int orient, int dither_it, int byte2)
{
int             i,j;
int             xshift, yshift;
int             xscale, yscale;
int             rangle;


/*   if(byte2==1)*/
/*   ERR(FATAL,"Asked for two byte option with smart_psraster (unimplemented) \n");*/


    endpath ();

    xscale = xmax - xmin;
    yscale = ymax - ymin;

    if (orient == 0)
    {
	xshift = xmin;
	yshift = ymin;
	rangle = 0;
    }
    else
    if (orient == 1)
    {
	xshift = xmin;
	yshift = ymax;
	rangle = 270;
    }
    else
    if (orient == 2)
    {
	xshift = xmax;
	yshift = ymax;
	rangle = 180;
    }
    else
    if (orient == 3)
    {
	xshift = xmax;
	yshift = ymin;
	rangle = 90;
    }

    fprintf (pltout, "gsave /picstr %d string def\n", xpix);

    fprintf (pltout, "%d %d translate %d %d scale %d rotate\n", xshift, yshift, xscale, yscale, rangle);

		fflush(pltout);
    if ( mono || ras_allgrey ) {
    fprintf (pltout, "/raster {%d %d 8 [ %d 0 0 %d 0 %d ] {currentfile picstr readhexstring pop} image} def\n", xpix, ypix, xpix, -ypix, ypix);

    fprintf (pltout, "raster\n");

    if (dither_it)
    {
	for (j = 0; j < xpix*ypix; j+=80)
	{
	   	for (i=j; (i<j+80 && i<xpix*ypix); i++)
	   	{
	    fprintf (pltout, "%2.2x", 255 - (int) raster_block[i]);
	   	}
    	   	fprintf (pltout, "\n");
	}
    }
    else
    {
	for (j = 0; j < xpix*ypix; j+=80)
	{
	   	for (i=j; (i<j+80 && i<xpix*ypix); i++)
	   	{
	    fprintf (pltout,"%2.2x", 255 - ps_grey_ras[(int) raster_block[i]]);
	   	}
    	   	fprintf (pltout, "\n");
	}
    }
    } else {
    fprintf (pltout, "/colraster {%d %d 8 [ %d 0 0 %d 0 %d ] {currentfile picstr readhexstring pop } false 3 colorimage} def\n", xpix, ypix, xpix, -ypix, ypix);

    fprintf (pltout, "colraster\n");

	for (j = 0; j < xpix*ypix; j+=80)
	{
		for (i=j; (i<j+80 && i<xpix*ypix); i++)
		{
	    	fprintf (pltout, "%2.2x%2.2x%2.2x", red[(int) raster_block[i]],green[(int) raster_block[i]],blue[(int) raster_block[i]]);
		}
    		fprintf (pltout, "\n");
	}
    }

    fprintf (pltout, "grestore\n");
}

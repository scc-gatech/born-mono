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
 *  source file:   ./filters/vplib/vpdovplot.c
 *
 * Joe Dellinger (SEP), Jan 9 1988
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Steve Cole (SEP), Feb 2 1989
 *	Added 'vpfit' option (xsize and ysize) to vppen.
 * Dave Nichols (SEP), May 22 1990
 *	Change format of vpstat printout, I put some more spaces in 
 *      so that I can use awk on the output.
 * Joe Dellinger (SEP), August 28 1990
 *	Make vppen scale fatmult along with everything else. Scale
 *	by whichever dimension is less expanded or more compressed.
 *	Take the spaces out of the vpstat printout unless "stat=L".
 *	The spaces made the stupid output > 80 characters!!!
 * Joe Dellinger (AMOCO), Nov 22 1994
 *	Added total framecount info to "stat" output.
 */

#include <stdio.h>
#include "../../include/vplot.h"
#include "../include/enum.h"
#include "../include/extern.h"
#include "../include/params.h"
#include "../include/err.h"
#include "vp.h"

int             vpxmax, vpxmin, vpymax, vpymin;
static int      vpxmaxs, vpxmins, vpymaxs, vpymins;

int vp_do_dovplot (
    int             nn,
    FILE          **inpltin,
    char            innames[][MAXFLEN + 1]
                   )
{
int             ii;
int             save_wantras;
int             save_shade;
char            string[80];
static int      it_got_clipped;
float           hh, ww;
float           rescale_x, rescale_y;
char            format_string[80];

/*	fprintf(stderr,"dfjsdka \n");*/

    if (nn == 0)
	return 0;

/*
 * If they want statistics, make one "dummy" pass through first
 * before you really do it.
 * The align and fit options need these statistics to do their work.
 */
/*	fprintf(stderr,"boop \n");*/
    if (vpalign || vpfit)
    {
	/*
	 * Turn on automatic processing 
	 */
	smart_clip = NO;
	smart_raster = NO;

	/*
	 * Just outline polygons and raster with vectors 
	 */
	save_wantras = wantras;
	save_shade = shade;

	wantras = NO;
	shade = NO;

	/*
	 * Turn off any actual output 
	 */
	dev.reset = nullreset;
	dev.message = vplogmessage;
	message = dev.message;
	dev.erase = vpderase;
	dev.close = nullclose;
	dev.vector = vplogvector;
	dev.marker = genmarker;
	dev.text = gentext;
	dev.area = nullarea;
	dev.raster = nullraster;
	dev.point = genpoint;
	dev.attributes = nullattributes;

/*	fprintf(stderr,"dfjsdk \n");*/

/*
 * Now do the trial pass
 */

	vpxmaxs = dev_xmin;
	vpxmins = dev_xmax;
	vpymaxs = dev_ymin;
	vpymins = dev_ymax;

	it_got_clipped = NO;

	if (vpstat)
	{
	    if (vpstat == YES)
	    {
		strcpy (format_string,
		"%17s: h=%6.2f w=%6.2f; x=(%6.2f,%6.2f) y=(%6.2f,%6.2f)\n");
	    }
	    else
	    {
		strcpy (format_string,
			"%17s: h= %6.2f w= %6.2f ;  x=( %6.2f , %6.2f ) y=( %6.2f , %6.2f ) ");
	    }
	}
/*	fprintf(stderr,"dfjsd2 \n");*/

	for (ii = 0; ii < nn; ii++)
	{
	    vpxmax = dev_xmin;
	    vpxmin = dev_xmax;
	    vpymax = dev_ymin;
	    vpymin = dev_ymax;

	    pltin = inpltin[ii];
/*	fprintf(stderr,"dfjsd3 %s \n",inpltin[ii]);*/
	    strcpy (pltname, innames[ii]);
	    dovplot ();
	    rewind (pltin);

	    if (vpxmaxs < vpxmax)
		vpxmaxs = vpxmax;
	    if (vpymaxs < vpymax)
		vpymaxs = vpymax;
	    if (vpxmins > vpxmin)
		vpxmins = vpxmin;
	    if (vpymins > vpymin)
		vpymins = vpymin;

/*	fprintf(stderr,"d4444 %s \n",inpltin[ii]);*/
/*
 * For vpstat=y, write out the statistics. Also note if any
 * parts of the plot got clipped.
 */
	    if (vpstat)
	    {
		hh = (float) (vpymax - vpymin) / RPERIN;
		ww = (float) (vpxmax - vpxmin) / RPERIN;

		if (hh < 0. || ww < 0.)
		{
		    fprintf (pltout,
			     "%17s: clipped away. ",
			     innames[ii]);
		}
		else
		{
		    fprintf (pltout, format_string,
			     innames[ii],
			     hh, ww,
			     (float) vpxmin / RPERIN,
			     (float) vpxmax / RPERIN,
			     (float) vpymin / RPERIN,
			     (float) vpymax / RPERIN);
		}

		if (vpxmax == dev_xmin || vpxmax == dev_xmax ||
		    vpxmin == dev_xmax || vpxmin == dev_xmin ||
		    vpymax == dev_ymin || vpymax == dev_ymax ||
		    vpymin == dev_ymax || vpymin == dev_ymin)
		{
		    fprintf (pltout, "*\n");
		    it_got_clipped = YES;
		}
		else
		{
		    fprintf (pltout, "\n");
		}
	    }
	}
/*	fprintf(stderr,"d5444 %s \n",inpltin[ii]);*/

	if (vpstat && nn > 1)
	{
	    sprintf (string, "All %d", nn);
	    fprintf (pltout, format_string,
		     string,
		     (float) (vpymaxs - vpymins) / RPERIN,
		     (float) (vpxmaxs - vpxmins) / RPERIN,
		     (float) vpxmins / RPERIN,
		     (float) vpxmaxs / RPERIN,
		     (float) vpymins / RPERIN,
		     (float) vpymaxs / RPERIN);
	}

	if (vpstat)
	{
	    if (vpframecount == 0)
	        fprintf(pltout,
		"            Total %d plot frame.\n", vpframecount + 1);
	    else
	        fprintf(pltout,
		"            Total %d plot frames.\n", vpframecount + 1);

	    if (it_got_clipped)
	    {
		if (vpbig)
		{
		    fprintf (pltout,
			 "\nA * indicates a plot that has been clipped.\n");
		    fprintf (pltout,
			     "Remember rotated style or relative size plots go to the top\n");
		    fprintf (pltout,
			     "of the \"screen\", which is infinitely far away if big=y.\n");
		}
		else
		{
		    fprintf (pltout,
		    "\nA * indicates a plot that has been clipped at the\n");
		    fprintf (pltout,
		     "virtual screen boundaries. You may not want this.\n");
		    fprintf (pltout,
		    "This clipping can be disabled by the big=y option.\n");
		}
	    }

	    for (ii = 0; ii < nn; ii++)
	    {
		pltin = inpltin[ii];
		fclose (pltin);
	    }
/*
 * Statistics get changed by re-aligning anyways,
 * So might as well just exit if we're doing vpstat=y.
 */
	    return 0;
	}

/*
 * Compute scale factors for vpfit option.
 * These are passed to dovplot as default scales.
 * The user may have specified both xsize and ysize, or just one.
 * Specifying one but not both is a signal that vppen should choose
 * the other scale factor to preserve the plot's aspect ratio.
 */
	if (vpfit)
	{
	    if (xsize == 0.)
	    {
		xsize = ysize * (vpxmaxs - vpxmins) / (vpymaxs - vpymins);
	    }
	    if (ysize == 0.)
	    {
		ysize = xsize * (vpymaxs - vpymins) / (vpxmaxs - vpxmins);
	    }

	    rescale_x = xsize * RPERIN / (vpxmaxs - vpxmins);
	    vpxmaxs *= rescale_x;
	    vpxmins *= rescale_x;
	    default_xscale *= rescale_x;

	    rescale_y = ysize * RPERIN / (vpymaxs - vpymins);
	    vpymaxs *= rescale_y;
	    vpymins *= rescale_y;
	    default_yscale *= rescale_y;

/*
 * Scale fatness according to whichever dimension is more compressed
 */
	    if (rescale_x > rescale_y)
		fatmult_orig *= rescale_y;
	    else
		fatmult_orig *= rescale_x;

	}
/*
 * Compute shifts for vpalign and vpfit options.
 * These are passed to dovplot as the default shifts.
 */
	switch (vpaligns[0])
	    /*
	     * horizontal 
	     */
	{
	case 'l':
	    default_hshift += (0 - vpxmins);
	    break;
	case 'r':
	    default_hshift += (0 - vpxmaxs);
	    break;
	case 'c':
	    default_hshift += (0 - ((vpxmaxs + vpxmins) / 2));
	    break;
	case 'u':
	    break;
	default:
	    ERR (WARN, name, "Unknown left-right alignment type %c.",
		 vpaligns[0]);
	    break;
	}


	switch (vpaligns[1])
	    /*
	     * vertical 
	     */
	{
	case 'b':
	    default_vshift += (0 - vpymins);
	    break;
	case 't':
	    default_vshift += (0 - vpymaxs);
	    break;
	case 'c':
	    default_vshift += (0 - ((vpymaxs + vpymins) / 2));
	    break;
	case 'u':
	    break;
	default:
	    ERR (WARN, name, "Unknown top-bottom alignment type %c.",
		 vpaligns[1]);
	    break;
	}

	style = default_style;

	reset_parameters ();

	/*
	 * Lie to dovplot, tell it to start from scratch again 
	 */
	first_time = YES;

	/*
	 * Undo the damage from the first pass 
	 */
	wantras = save_wantras;
	shade = save_shade;

	dev.reset = vpreset;
	dev.message = vpmessage;
	message = dev.message;
	dev.erase = vperase;
	dev.close = vpclose;
	dev.vector = vpvector;
	dev.marker = vpmarker;
	dev.text = vptext;
	dev.area = genarea;
	dev.raster = vpraster;
	dev.point = genpoint;
	dev.attributes = vpattributes;
    }
/*	fprintf(stderr,"coop \n");*/

/*
 *********************************************************************
 * "Real" pass
 *********************************************************************
 */

    if (vpdumb)
    {
	dev.message = genmessage;
	dev.vector = genvector;
	dev.marker = genmarker;
	dev.text = gentext;
	dev.area = vecarea;
	dev.raster = genraster;
	smart_clip = NO;
	smart_raster = NO;
    }
    else
    {
	smart_clip = YES;
	smart_raster = YES;
    }
/*	fprintf(stderr,"doop \n");*/

/* Second (or first) pass */
    for (ii = 0; ii < nn; ii++)
    {
	pltin = inpltin[ii];
	strcpy (pltname, innames[ii]);
/*	fprintf(stderr,"ee \n");*/
	dovplot ();
/*	fprintf(stderr,"ff \n");*/
	fclose (pltin);
    }

    return 0; /*DOVPLOT_CONT*/
}

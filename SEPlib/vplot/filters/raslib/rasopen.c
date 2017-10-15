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
 *  source file:   ./filters/raslib/rasopen.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Joe Dellinger (SEP), Sept 25 1988
 *	"ppi" undocumented, and read as an integer and not a float!
 * Joe Dellinger (SEP), May 7 1989
 *	Cleaned up to make raslib work with Tektronix color plotter.
 *	Incorporated portions of Stew Levin's color plotter code.
 * Joe Dellinger (SEP), August 5, 1989
 *	Use the variable "default_out" to remember the ORIGINAL
 *	value of isatty(fileno(pltout)).
 * Chuck Karish, 5 August 1989
 *	Non-SEP, colfile defaults to empty string instead of "colfile".
 * Joe Dellinger, 17 Jan 1990
 *	ARG!!! I'm changing the default behavior back so it matches
 *	the documentation. I'm also fixing the bug introduced by the
 *	change so that "colfile" was not even settable from the command
 *	line.
 * Dave Nichols, 10 July 1992
 *	Added support for ppm output, (ppmpen, Ppmpen).
 */

#include<sepConfig.h>

#include <stdlib.h>
#include <stdio.h>
#include "../include/enum.h"
#include "../include/extern.h"
#include "../include/err.h"
#include "../include/params.h"
#if defined(__stdc__) || defined(__STDC__)
#include <string.h>
#else
#include <strings.h>
#endif
#ifdef SEP
#include <ctype.h>
#endif
#define DEFAULT_OUT	isatty(fileno(pltout))
#include "raspen.h"

/*#include <ppm.h>*/
#include "ppm.h"

unsigned char  *image;
int		color_mult;
int             rasor = 0;
char            colfile[60];
int             esize = 1;

int             grf_format = NO;
int             ppm_format = NO;
int             white = NO;
float           o_pixels_per_inch, o_aspect_ratio;
int             default_out = YES;

void rasopen (void)
{
#ifdef SEP
extern int      headfd;
char            headname[30], fname[80];
char            path[50];
char           *front, *tail, *ptr;
#endif
char            newpath[60];
MIXED		vartemp;

    txfont = DEFAULT_HARDCOPY_FONT;
    txprec = DEFAULT_HARDCOPY_PREC;

    allowecho = YES;

    if (
	strcmp (callname, "tcprpen") == 0 ||
	strcmp (callname, "Tcprpen") == 0 ||
	strcmp (wstype, "tcpr") == 0
     )
    {
	grf_format = YES;
	vartemp.i = &white;
	getpar ("white", "1", vartemp);
    }

    if ( 
	strcmp (callname, "ppmpen" ) == 0 ||    
	strcmp (callname, "Ppmpen" ) == 0 ||    
	strcmp (wstype, "ppm") == 0
     )
    {
	ppm_format = YES;
	ppm_init( &sepxargc, sepxargv );
    }


    if (grf_format)
    {
/*
 * physical device parameters for Tektronix color printer 4693D
 */
	o_pixels_per_inch = pixels_per_inch = 300.0f;
	dev_xmax = 8.53f * pixels_per_inch;
	dev_ymax = 8.13f * pixels_per_inch;
	dev_xmin = 0.0f;
	dev_ymin = 0.0f;
	o_aspect_ratio = aspect_ratio = 1.0f;
/*
 * Best compromise between resolution and speed
 */
	pixels_per_inch = 150.0f;

    }else if (ppm_format)
    {
/*
 * physical device parameters for ppm format output
 */
	o_pixels_per_inch = pixels_per_inch = 100.0f;
	dev_xmax = 10.0f * pixels_per_inch;
	dev_ymax = 7.5f * pixels_per_inch;
	dev_xmin = 0.0f;
	dev_ymin = 0.0f;
	o_aspect_ratio = aspect_ratio = 1.0f;
    }
    else
    {
/*
 * physical device parameters for RasterTek
 */
	o_pixels_per_inch = pixels_per_inch = 100.0f;
	dev_xmax = 1024.0f;
	dev_ymax = 768.0f;
	dev_xmin = 0.0f;
	dev_ymin = 0.0f;
	o_aspect_ratio = aspect_ratio = 1.0f;
    }

/*
 * device capabilities
 */
    need_end_erase = YES;
    buffer_output = YES;
    smart_clip = NO;

    if (grf_format)
    {
        vartemp.i = &esize;
	getpar ("esize", "d", vartemp);
	if (esize == 3)
	{
	    dev.vector = rasvector4;
	}
	else
	if (esize != 1)
	    ERR (FATAL, name, "Esize must be 1 or 3!\n");

	color_mult = 1;
	num_col = NCOLOR;
    }
    else if ( ppm_format )
    {
	/* always use esize 3 */
	esize=3;
	color_mult =1;
	num_col = NCOLOR;
	dev.vector = rasvector3;

    }
    else
    {
        vartemp.i = &esize;
	getpar ("esize", "d", vartemp);
	if (esize == 1)
	{
	    color_mult = 2;
            vartemp.i = &color_mult;
	    getpar ("colormult", "d", vartemp);
	    num_col = NCOLOR / color_mult;

            vartemp.i = &rasor;
	    getpar ("or", "1", vartemp);
	    if (rasor)
	    {
		dev.vector = rasvector2;
	    }
	}
	else
	if (esize == 3)
	{
	    color_mult = 1;
	    num_col = NCOLOR;
	    dev.vector = rasvector3;
	}
	else
	    ERR (FATAL, name, "Esize must be 1 or 3!\n");
    }


    vartemp.f = &aspect_ratio;
    getpar ("aspect", "f", vartemp);
    vartemp.f = &pixels_per_inch;
    getpar ("ppi", "f", vartemp);
    dev_xmax *= pixels_per_inch / o_pixels_per_inch;
    dev_ymax *= (o_aspect_ratio / aspect_ratio) *
     (pixels_per_inch / o_pixels_per_inch);
    vartemp.i = &dev_xmax;
    getpar ("n1", "d", vartemp);
    vartemp.i = &dev_ymax;
    getpar ("n2", "d", vartemp);
#ifdef SEP
    if (esize == 1)
    {
	Puthead ("\n\n# Raspen: VPLOT graphics via Movie,\n");
	Puthead ("#\tor any other byte-deep raster device.\n\n");
    }
    else
    {
	Puthead ("\n\n# Raspen: VPLOT graphics via RGB byte triples.\n\n");
    }
    Puthead ("\taspect_ratio=%f\n", aspect_ratio);
    Puthead ("\tppi=%f\n", pixels_per_inch);
    Puthead ("\tesize=%d\n", esize);
    Puthead ("\tn1=%d\n", dev_xmax);
    Puthead ("\tn2=%d\n", dev_ymax);
#endif


    /*
     * Allocate space for image 
     */
    if ((image = (unsigned char *) malloc (dev_xmax * dev_ymax * esize)) == NULL)
    {
	ERR (FATAL, name, "Can't allocate space for raster image\n");
    }

    default_out = DEFAULT_OUT;

    if (!grf_format && default_out)
    {
#ifdef SEP
	datapath (path);
/* Code stolen from output.c to get a reasonable raster file name. */
	if (0 < findnm (headfd, headname, sizeof (headname)))
	{
	    /* modify slightly */
	    strcpy (fname, "");
	    front = strrchr (headname, '/');
	    if (front == ((char *) NULL))
		front = headname;
	    else
		front++;
	    if ((*front) == 'H')
		strcat (fname, ++front);
	    else
	    {
		tail = strrchr (front, '.');
		if (tail == ((char *) NULL))
		    strcat (fname, front);
		else
		{
		    for (ptr = tail + 1; *ptr; ptr++)
			if (!isupper (*ptr))
			    break;
		    if (!(*ptr))/* drop suffix if all caps */
			*tail = '\0';
		    (void) strcat (fname, front);
		}
	    }
	    (void) strcat (fname, ".raster");
	}
	else
	{
	    strcpy (fname, "raster");
	}

	sprintf (newpath, "%s%s", path, fname);
	Puthead ("\tin=%s\n", newpath);
#else
	sprintf (newpath, "%s", "raster_file");
#endif
	pltout = fopen (newpath, "w");
	if (pltout == NULL)
	    ERR (FATAL, name, "can't open file %s\n", newpath);
    }

    if (!grf_format &&  !ppm_format  && esize == 1)
    {
	strcpy (colfile, "colfile");
        vartemp.s = &(colfile[0]);
	getpar ("colfile", "s", vartemp);
#ifdef SEP
	Puthead ("\tcolfile=%s\n", colfile);
	Puthead ("\tcolor=T\n");
	if (color_mult == 1)
	    Puthead ("\tumask=255\n");
#endif
    }
}

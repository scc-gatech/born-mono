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
 *  source file:   ./filters/vplib/vpopen.c
 *
 * Joe Dellinger (SEP), Dec 19 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Joe Dellinger (SEP), Dec 10 1988
 *	Added "outN" option.
 * Steve Cole (SEP), Apr 15 1989
 *	Added vpfit option (xsize and ysize) to vppen.
 * Joe Dellinger (SEP), May 11 1989
 *	Don't dump raw vplot to people's screens.
 * Joe Dellinger (SEP), Aug 28 1990
 *	Added stat=L option.
 * Joe Dellinger (AMOCO), Nov 19 1994
 *	Have vpopen_name return a value indicating whether the output
 *	stream has been redirected.
 */

#include <stdio.h>
#include "../../include/vplot.h"
#include "../include/enum.h"
#include "../include/extern.h"
#include "../include/err.h"
#include "../include/params.h"
#include "vp.h"

int             vpbig = NO; /* bug workaround for now --- Stew */
int             vpdumb = NO;
int             vpstat = NO;
int             vpfit = NO;
float           xsize = 0., ysize = 0.;
int             vpalign = NO;
int             vpstyle = YES;
int             vpblast = YES;
int             vpbit = 0;
char            vpaligns[80] = "uu";
int             vparray[2] = {0, 0};
int             vpasize[2] = {0, 0};
int             vpframe = -1;

int             vpsetflag;

int             vpnobuf = NO;

void vpopen (void)
{
float           atemp[2];
char            vpstat_string[10];
int		ii;
MIXED		vartemp;

    first_time = YES;


/*
 * Reset the saved global color table information array.
 */
    for (ii=0; ii < VPPEN_NUM_COL; ii++)
    {
	vpscoltabinfo[ii][ISITSET] = NO;
	vpscoltabinfo[ii][1] = 0;
	vpscoltabinfo[ii][2] = 0;
	vpscoltabinfo[ii][3] = 0;
    }

/*
 * Special options
 */
    vartemp.i = &vpnobuf;
    getpar ("nobuf", "1", vartemp);
    vartemp.i = &vpdumb;
    getpar ("dumb", "1", vartemp);
    vartemp.i = &vpblast;
    getpar ("blast", "1", vartemp);
    vartemp.i = &vpbit;
    getpar ("bit", "d", vartemp);
    vartemp.i = &vpframe;
    getpar ("grid", "d", vartemp);

/*
 * stat=L means to insert extra spaces into stat output.
 */
    vartemp.s = &(vpstat_string[0]);
    if (getpar ("stat", "s", vartemp))
    {
	if (vpstat_string[0] == 'y' || vpstat_string[0] == 'Y' ||
	    vpstat_string[0] == '1')
	    vpstat = YES;
	else
	if (vpstat_string[0] == 'l' || vpstat_string[0] == 'L')
	    vpstat = 2;
	else
	    vpstat = NO;
    }

    vartemp.s = &(vpaligns[0]);
    getpar ("align", "s", vartemp);
    vartemp.f = &xsize;
    getpar ("xsize", "f", vartemp);
    vartemp.f = &ysize;
    getpar ("ysize", "f", vartemp);
    if (xsize != 0. || ysize != 0.)
	vpfit = YES;

    if (vpstat || strcmp (vpaligns, "uu") != 0 || vpfit)
    {
	vpalign = YES;
	allow_pipe = NO;
    }

    vartemp.i = &(vparray[0]);
    getpar ("gridnum", "d", vartemp);

    if (vparray[1] == 0)
	vparray[1] = vparray[0];

    if (vparray[0] != 0)
    {
	vpbig = NO;
	vpstyle = NO;
    }

    vartemp.i = &vpbig;
    getpar ("big", "1", vartemp);
    vartemp.i = &vpstyle;
    getpar ("vpstyle", "1", vartemp);

    if (vparray[0] != 0)
    {
	if (vpbig || vpalign)
	    ERR (FATAL, name, "Incompatible option with gridnum");
	atemp[0] = (float) (STANDARD_HEIGHT / SCREEN_RATIO) / vparray[0];
	atemp[1] = (float) (STANDARD_HEIGHT) / vparray[1];

        vartemp.f = &(atemp[0]);
	getpar ("gridsize", "f", vartemp);

	vpasize[0] = atemp[0] * RPERIN;
	vpasize[1] = atemp[1] * RPERIN;
    }


/*
 * We want to go through the input files ourselves
 */

    genreader = vp_do_dovplot;

/*
 * device capabilities
 */

    if (vpbig)
    {
	dev_xmax = VP_MAX * RPERIN;
	dev_ymax = VP_MAX * RPERIN * SCREEN_RATIO;
	dev_xmin = -dev_xmax;
	dev_ymin = -dev_ymax;
	default_hshift = -dev_xmin;
	default_vshift = -dev_ymin;
    }
    else
    {
	dev_xmax = STANDARD_HEIGHT * RPERIN / SCREEN_RATIO;
	dev_ymax = STANDARD_HEIGHT * RPERIN;
	dev_xmin = 0;
	dev_ymin = 0;
	default_hshift = 0;
	default_vshift = 0;
    }

    pixels_per_inch = RPERIN;
    aspect_ratio = 1.;
    num_col = VPPEN_NUM_COL;
    if (vparray[0] == 0)
	size = ABSOLUTE;


/*
 * Since font gets hard-wired in after first pass,
 * make it a nice one if they don't specify it.
 */
    txfont = DEFAULT_HARDCOPY_FONT;
    txprec = DEFAULT_HARDCOPY_PREC;

/*
 * Make vplib routines more useful to be included in other programs
 * besides just vppen
 */
    dev.open = vpopen;
    dev.reset = vpreset;
    dev.message = vpmessage;
    dev.erase = vperase;
    dev.close = vpclose;
    dev.vector = vpvector;
    dev.marker = vpmarker;
    dev.text = vptext;
    dev.area = genarea;
    dev.raster = vpraster;
    dev.point = genpoint;
    dev.attributes = vpattributes;
    dev.getpoint = nullgetpoint;
    dev.interact = nullinteract;
    dev.plot = vpplot;
    dev.startpoly = vpstartpoly;
    dev.midpoly = vpmidpoly;
    dev.endpoly = vpendpoly;

/*
 * To keep messages from being lost
 */
    message = dev.message;

/*
 * Open up the "device", if we're ever going to use libvplot at all.
 */
    if (!vpstat)
    {
/*
 * The very first time we don't care whether vpopen_name opened an
 * output file or not; we'd have to initialize everything in any case!
 */
	(void) vpopen_name (0);
    }

    allowecho = YES;
    cachepipe = YES;

}

int vpopen_name (int num)
{
char            string[BUFSIZ];
char            outname[BUFSIZ];
static FILE    *vp_pltout = NULL;
static int      gotwhich = 0;
int		newout;
MIXED		vartemp;


/*
 * If last time we opened a file for output, then this time
 * we'll also want to re-write the color table.
 */
    if (gotwhich > 0)
    {
	newout = 1;
    }
    else
    {
	newout = 0;
    }

    gotwhich = 0;

    vartemp.s = &(string[0]);
    if (getpar ("outN+", "s", vartemp))
    {
	gotwhich = 2;
    }
    else if (getpar ("outN", "s", vartemp))
    {
	gotwhich = 1;
    }

    if (gotwhich)
    {
	sprintf (outname, string, num);
    }

    sprintf (string, "out%d+", num);
    vartemp.s = &(outname[0]);
    if (getpar (string, "s", vartemp))
    {
	gotwhich = 2;
    }
    else
    {
	sprintf (string, "out%d", num);
	if (getpar (string, "s", vartemp))
	    gotwhich = 1;
    }

    if (gotwhich)
    {
	if (vp_pltout != (FILE *) NULL)
	    fclose (vp_pltout);

	if (gotwhich == 2)
	    vp_pltout = fopen (outname, "a");
	else
	    vp_pltout = fopen (outname, "w");

	if (vp_pltout == (FILE *) NULL)
	{
	    ERR (WARN, name, "Can't open %s", outname);
	    vp_check_filep (pltout);
	    vp_pltout = (FILE *) NULL;
	}
	else
	{
	    vp_check_filep (vp_pltout);
/*
 * If we're opening a new file for output, then
 * we'll need to re-write the color table at the start of it.
 */
	    newout = 1;
	}
    }
    else
    {
	vp_check_filep (pltout);
	vp_pltout = (FILE *) NULL;
    }

    return newout;
}

void vp_check_filep (FILE *plot)
{
    if(vpnobuf) {
        (void) setvbuf(plot, NULL, _IONBF, 0);
    } else {
        if (isatty (fileno (plot)))
 	  ERR (FATAL, name,
	     "Dumping binary data to your terminal is unhealthy.");
    }

    vp_filep (plot);
}

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
 *  source file:   ./filters/pslib/psopen.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Steve Cole (SEP), September 15 1987
 *	Added smart_raster, mono, dither, pixc, and greyc to attributes.
 * Steve Cole (SEP), January 10 1988
 *      Vplot pen font is now default font.
 * Steve Cole (SEP), March 23 1988
 *      Default font is now vplot default hardcopy font.
 * Steve Cole (SEP), March 29 1988
 *      Added an intial gsave so that the first clipping window call
 *      will not destroy the rotation and translation done here when
 *      it tries to back out the previous clipping call.
 *	Added need_end_erase for plotting label.
 *      Corrected definition of limits of device space.
 * Steve Cole (SEP), March 31 1988
 *      The vplot to PostScript units conversion factor is computed here
 *	instead of being hardwired in many different places.
 *      Scaling is done here so that all coordinates output by pspen
 *      can be integers.
 * Joe Dellinger (SEP), October 19 1988
 *	Added "grey" and "harddither" options, fixed bugs, use "move/draw"
 *	method of sending paths.
 * Joe Dellinger (SEP), January 19 1989
 *	This is a "page type" hardcopy device;
 *	size=relative makes more sense.
 * Joe Dellinger (SEP), April 17 1990
 *	Remove pointless calloc call; "getlogin" provides its own allocation.
 * Dave Nichols (SEP), May 22 1990
 *	Changed output for tex=y to be compatible with /psfig macros in TeX.
 * Dave Nichols (SEP), Aug 14 1990
 *	Changed size of page to be maximum possible and force size=ABSOLUTE
 *	for tex=y to prevent clipping of plots off the page.
 * Steve Cole (SEP), Nov 1 1991
 *	Added color support.
 * Dave Nichols (SEP) May 2 1992
 *      Allow VPLOTSPOOLDIR environment variable to override PEN_SPOOL
 * Joe Dellinger (SOEST) Oct 15 1992
 *	The Apple LaserWriter page limits are a little more generous
 *	than the SPARC printer allows.
 * Steve Cole (SEP), Nov 30 1992
 *      Added color raster support.
 *	Added limits for Tektronix color postscript printer. These are
 *      used if pspen is invoked by the alias tcprpen, or if
 *      wstype=tcpr.
 * Ray Abma (SEP) 3 Aug 1993
 *      changed getdate to dateget to avoid conflict with HP time.h
 * David Lumley (SEP) March 7 1994
 *	added translate call so tcpr printable area doesn't get clipped
 * Joe Dellinger (AMOCO) March 22 1995
 *	Added the ability to specify a page size from the command line.
 *	Added the ability to set the default page size either by the
 *	define DEFAULT_PAPER_SIZE at compile time or by setting an
 *	environmental variable of the same name.
 *	Corrected the bug (I hope?) that caused plots to sometimes be
 *	lost if pspen was run in background from a shell after the user
 *	logged out. Added more detail to error messages.
 *	Added "oyo" option to wstype.
 * Joe Dellinger (AMOCO) April 24 1996
 *	Set line join type when tex=y too.
 *	The variable "rotate" was always zero because it hadn't been
 *	properly set yet and shouldn't have been used here.
 * Joe Dellinger (AMOCO) April 25 1996
 *	Add "s" and "r" as shorthands for "stroke" and "rlineto", respectively.
 * Joe Dellinger (AMOCO) April 26 1996
 *	Add "x" as shorthand for "0 0 rlineto" (a dot).
 * Joe Dellinger (BP Amoco) Oct 5, 1999
 *	Set the actual colors when calling psattributes (changes made
 *	there to fix a bug required a change here).
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../../include/vplot.h"
#include "../include/err.h"
#include "../include/enum.h"
#include "../include/extern.h"
#include "../include/params.h"
#include "../include/attrcom.h"
#include "ps.h"
#include <pwd.h>

#if defined(__stdc__) || defined(__STDC__)
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#else
extern char    *getenv ();
#endif

#ifdef SEP
#define GETPAR getch
extern int getch(const char *, const char *, MIXED);
#else
#define GETPAR getpar
extern int getpar( char *,  char *, MIXED);
#endif


#ifndef DEFAULT_PAPER_SIZE
#define DEFAULT_PAPER_SIZE	"letter"
#endif

#ifndef DEFAULT_PAPER_UNITS
#define DEFAULT_PAPER_UNITS	'i'
#endif


int             file_created = NO;
int             tex = NO;
int             ncopies_document = 1;
char            label[100];
char            ps_paper[80];

char            mapfile[100] = "default";
char            scratch_file[100];
int             hold;

int             default_ps_font = DEFAULT_HARDCOPY_FONT;
float           psscale;

int             ps_color = NO;
int             dumb_fat = NO;
int             force_color = NO;

float           ps_xlength, ps_ylength;
float           ps_xmin, ps_xmax, ps_ymin, ps_ymax;
float           ps_xborder, ps_yborder;
int             ps_set_papersize = NO;
float           ps_ypapersize;
char            psprintertype[80] = "default";

#if defined(__stdc__) || defined(__STDC__)
void psopen (void)
#else
psopen ()
#endif
{
char           *user_name;
#if defined(__stdc__) || defined(__STDC__)
#include <unistd.h>
#else
char           *getlogin ();
#endif
char            date[50];
int             hard_dither = YES;
int             i;
char            units;
float           unitscale;
struct stat     statbuf;
int             creatafile;
int             yesget;
char           *paperstring;
MIXED		vartemp;

/*
 * A list of different printer types recognized
 */
    if ((strcmp (callname, "oyopen") == 0) ||
	(strcmp (callname, "Oyopen") == 0) ||
	(strcmp (wstype, "oyo") == 0))
    {
	strcpy (psprintertype, "oyo");
    }
    else if ((strcmp (callname, "tcprpen") == 0) ||
	     (strcmp (callname, "Tcprpen") == 0) ||
	     (strcmp (wstype, "tcpr") == 0))
    {
	strcpy (psprintertype, "tcpr");
    }
    else
    {
/* Default */
	strcpy (psprintertype, "default");
    }


/*
 * physical device parameters
 */

/*
 * pixc and greyc are used even if smart_raster = YES because
 * PostScript's built in dithering does not compensate for the
 * effects that pixc and greyc correct for.
 */

    if ((strcmp (callname, "oyopen") == 0) ||
	(strcmp (callname, "Oyopen") == 0) ||
	(strcmp (wstype, "oyo") == 0))
    {
/*
 * OYO electrostatic plotters need a VERY different pixc and greyc
 */
	pixc = 1.5f;
	greyc = .1f;

/*
 * Our main OYO happens to be 300 dpi
 */
	pixels_per_inch = 300.0f;

/*
 * Change this if for example you want the oyo-s to use bigger sheets
 * of paper by default than the usual HP printer engines.
 */
	strcpy (ps_paper, DEFAULT_PAPER_SIZE);
    }
    else
    {
/* For HP printer engines, the usual default */

	pixc = 0.6f;
	greyc = -0.5f;

	pixels_per_inch = DEFAULT_PIXELS_PER_INCH;

	strcpy (ps_paper, DEFAULT_PAPER_SIZE);
    }


/*
 * For now, elsewhere in the code we just ignore aspect_ratio,
 * implicitly assuming it will always be 1.
 */
    aspect_ratio = 1.0f;


/*
 * Allow override of pixels_per_inch from the command line. (If we used
 * GETPAR here it would also search the incoming history file... I think
 * it's safer to require them to be on the command line.)
 */
    vartemp.f = &pixels_per_inch;
    getpar ("ppi", "f", vartemp);


/*
 * based on the number of pixels per inch and PostScript's units
 * of length, we compute a PostScript scale factor here that will allow us to
 * send vplot coordinates (integers) directly to PostScript.
 */
    psscale = PSPERIN / ((float) pixels_per_inch);


/*
 * American paper sizes
 *
 * letter: 8.5 by 11 inch page; imageable region of 8.0 by 10.92 inches.
 * legal: 8.5 by 13 inch page; imageable region of 6.72 by 13 inches.
 * (these limits are imposed by the apple laserwriter).
 */


/*
 * Default units for custom paper sizes.
 */
    units = DEFAULT_PAPER_UNITS;

/*
 * Environment variable can override default paper size...
 */
    if ((paperstring = getenv ("DEFAULT_PAPER_SIZE")) != NULL)
	strcpy (ps_paper, paperstring);

/*
 * but GETPAR has the very last say.
 */
    vartemp.s = &(ps_paper[0]);
    if (GETPAR ("paper", "s", vartemp))
	paperstring = NULL;

    if (strcmp (ps_paper, "letter") == 0)
    {
	if ((strcmp (callname, "tcprpen") == 0) ||
	    (strcmp (callname, "Tcprpen") == 0) ||
	    (strcmp (wstype, "tcpr") == 0))
	{
/*
 * Empirically determined plotting limits for the Tek color plotter with
 * letter-size paper
 */
	    dev_xmin = 1.23333f * pixels_per_inch + .5f;
	    dev_ymin = .18333f * pixels_per_inch + .5f;
	    dev_xmax = 9.76666f * pixels_per_inch + .5f;
	    dev_ymax = 8.31333f * pixels_per_inch + .5f;
/*
 * ps_ypapersize controls an overall plot shift. Normally it will be the
 * same as the height (Y) dimension of the paper, but may be different in
 * case the plotter doesn't put (0,0) at the corner of the paper.
 */
	    ps_ypapersize = 7.66666f;
	}
	else
	{
/* Empirically determined limits for SPARC printers with letter-size paper */
	    dev_xmin = .19f * pixels_per_inch + .5f;
	    dev_ymin = .26666f * pixels_per_inch + .5f;
	    dev_xmax = 10.88333f * pixels_per_inch + .5f;
	    dev_ymax = 8.23333f * pixels_per_inch + .5f;
	    ps_ypapersize = 8.5f;
	}
    }
    else if (strcmp (ps_paper, "legal") == 0)
    {
/* Empirically determined limits for SPARC printers with legal-size paper */
	dev_xmin = .50333f * pixels_per_inch + .5f;
	dev_ymin = .89f * pixels_per_inch + .5f;
	dev_xmax = 13.49666f * pixels_per_inch + .5f;
	dev_ymax = 7.61f * pixels_per_inch + .5f;
	ps_ypapersize = 8.5f;
    }
    else if (strcmp (ps_paper, "a5") == 0 || strcmp (ps_paper, "A5") == 0)
    {
	/*
	 * For our European friends, A5 paper
	 * 
	 * For now, use (DEFAULT_BORDER * CMPERIN) as the best guess for a
	 * nonimageable area at the edges of the paper all around.
	 */

	dev_xmin = (DEFAULT_BORDER * CMPERIN) * (1.0f / CMPERIN) * pixels_per_inch + .5f;
	dev_ymin = (DEFAULT_BORDER * CMPERIN) * (1.0f / CMPERIN) * pixels_per_inch + .5f;
	dev_xmax = (21.0f - (DEFAULT_BORDER * CMPERIN)) * (1.0f / CMPERIN) * pixels_per_inch + .5f;
	dev_ymax = (14.85f - (DEFAULT_BORDER * CMPERIN)) * (1.0f / CMPERIN) * pixels_per_inch + .5f;
	ps_ypapersize = 14.85f * (1.0f / CMPERIN);
    }
    else if (strcmp (ps_paper, "a4") == 0 || strcmp (ps_paper, "A4") == 0)
    {
	/*
	 * For our European friends, A4 paper
	 * 
	 * For now, use (DEFAULT_BORDER * CMPERIN) as the best guess for a
	 * nonimageable area at the edges of the paper all around.
	 */

	dev_xmin = (DEFAULT_BORDER * CMPERIN) * (1.0f / CMPERIN) * pixels_per_inch + .5f;
	dev_ymin = (DEFAULT_BORDER * CMPERIN) * (1.0f / CMPERIN) * pixels_per_inch + .5f;
	dev_xmax = (29.7f - (DEFAULT_BORDER * CMPERIN)) * (1.0f / CMPERIN) * pixels_per_inch + .5f;
	dev_ymax = (21.0f - (DEFAULT_BORDER * CMPERIN)) * (1.0f / CMPERIN) * pixels_per_inch + .5f;
	ps_ypapersize = 21.0f * (1.0f / CMPERIN);
    }
    else if (strcmp (ps_paper, "a3") == 0 || strcmp (ps_paper, "A3") == 0)
    {
	/*
	 * For our European friends, A3 paper
	 * 
	 * For now, use (DEFAULT_BORDER * CMPERIN) as the best guess for a
	 * nonimageable area at the edges of the paper all around.
	 */

	dev_xmin = (DEFAULT_BORDER * CMPERIN) * (1.0f / CMPERIN) * pixels_per_inch + .5f;
	dev_ymin = (DEFAULT_BORDER * CMPERIN) * (1.0f / CMPERIN) * pixels_per_inch + .5f;
	dev_xmax = (42.0f - (DEFAULT_BORDER * CMPERIN)) * (1.0f / CMPERIN) * pixels_per_inch + .5f;
	dev_ymax = (29.7f - (DEFAULT_BORDER * CMPERIN)) * (1.0f / CMPERIN) * pixels_per_inch + .5f;
	ps_ypapersize = 29.7f * (1.0f / CMPERIN);
    }
    else
    {
	ps_xborder = -1.0f;
	ps_yborder = -1.0f;

	if (sscanf (ps_paper, "%fx%f%c,%f,%f",
		    &ps_ylength, &ps_xlength,
		    &units,
		    &ps_yborder, &ps_xborder) >= 2)
	{
	    if (units == 'i')
	    {
		/* Inches */
		unitscale = 1.0f;
	    }
	    else if (units == 'c')
	    {
		/* Centimeters */
		unitscale = 1.0f / CMPERIN;
	    }
	    else
	    {
		ERR (FATAL, name,
		     "Sorry, don't recognize units of type \"%c\"!", units);
	    }

	    /*
	     * Use a default border of DEFAULT_BORDER inches if they didn't
	     * specify it.
	     */
	    if (ps_xborder < 0.0f)
		ps_xborder = DEFAULT_BORDER / unitscale;

	    if (ps_yborder < 0.0f)
		ps_yborder = DEFAULT_BORDER / unitscale;


	    if (ps_xlength <= 0.0f)
	    {
		if (ps_ylength <= 0.0f)
		{
		    ERR (FATAL, name,
		    "Height and width of paper are both negative or zero!");
		}
		else
		    ps_xlength = 2.0f * ps_xborder + (ps_ylength - 2.0f * ps_yborder) / SCREEN_RATIO;
	    }
	    else if (ps_ylength <= 0.0f)
	    {
		ps_ylength = 2.0f * ps_yborder + (ps_xlength - 2.0f * ps_xborder) * SCREEN_RATIO;
	    }


	    ps_xlength *= unitscale;
	    ps_ylength *= unitscale;
	    ps_xborder *= unitscale;
	    ps_yborder *= unitscale;

	    ps_xmin = ps_xborder;
	    ps_ymin = ps_yborder;
	    ps_xmax = (ps_xlength - ps_xborder);
	    ps_ymax = (ps_ylength - ps_yborder);
	}
	else if (sscanf (ps_paper, "%f,%f,%fx%f,%f,%f%c",
			 &ps_ymin,
			 &ps_ymax,
			 &ps_ylength,
			 &ps_xmin,
			 &ps_xmax,
			 &ps_xlength,
			 &units) >= 6)
	{
	    if (units == 'i')
	    {
		/* Inches */
		unitscale = 1.0f;
	    }
	    else if (units == 'c')
	    {
		/* Centimeters */
		unitscale = 1.0f / CMPERIN;
	    }
	    else
	    {
		ERR (FATAL, name,
		     "Sorry, don't recognize units of type \"%c\"!", units);
	    }

	    ps_xmin *= unitscale;
	    ps_ymin *= unitscale;
	    ps_xmax *= unitscale;
	    ps_ymax *= unitscale;
	    ps_xlength *= unitscale;
	    ps_ylength *= unitscale;
	}
	else
	{
	    if (paperstring != NULL)
	    {
		ERR (COMMENT, name,
		     "Is your environmental variable \"$DEFAULT_PAPER_SIZE\" correct?");
	    }

	    ERR (FATAL, name,
		 "Don't recognize requested paper type \"%s\"!", ps_paper);
	}


	dev_xmin = ps_xmin * pixels_per_inch + .5f;
	dev_ymin = ps_ymin * pixels_per_inch + .5f;
	dev_xmax = ps_xmax * pixels_per_inch + .5f;
	dev_ymax = ps_ymax * pixels_per_inch + .5f;
	ps_ypapersize = ps_ylength;

	if (dev_xmin >= dev_xmax || dev_ymin >= dev_ymax)
	{
	    ERR (FATAL, name,
		 "Custom paper size has no plotting area.");
	}

	ps_set_papersize = YES;
    }


/*
 * device capabilities
 */
    vartemp.i = &dumb_fat;
    getpar ("dumbfat", "1", vartemp);
    vartemp.i = &ps_color;
    getpar ("grey gray color", "1", vartemp);
    vartemp.i = &force_color;
    getpar ("force", "1", vartemp);

    if (ps_color ||
	(strcmp (callname, "tcprpen") == 0) ||
	(strcmp (callname, "Tcprpen") == 0) ||
	(strcmp (wstype, "tcpr") == 0))
    {
	mono = NO;
	num_col = 16384;
    }
    else
    {
	mono = YES;
	num_col = 0;
    }
    vartemp.i = &hard_dither;
    getpar ("harddither", "1", vartemp);

    if (hard_dither)
    {
	smart_raster = YES;
	dev.raster = smart_psraster;
    }
    else
    {
	smart_raster = NO;
	dev.raster = dumb_psraster;
    }

    need_end_erase = YES;
    buffer_output = YES;
    smart_clip = NO;

    dither = 3;
    txfont = default_ps_font;
    txprec = DEFAULT_HARDCOPY_PREC;

    vartemp.s = &(label[0]);
    if (!GETPAR ("label", "s", vartemp))
    {
	if ((user_name = getlogin ()) == NULL)
	    user_name = getpwuid (getuid ())->pw_name;
	dateget (date);
	sprintf (label, "%s, %s", user_name, date);
    }
    else
    {
	if ((strcmp (label, "no") == 0) || (strcmp (label, "n") == 0)
	    || (strcmp (label, " ") == 0) || (strcmp (label, "") == 0))
	    label[0] = '\0';
    }


/*
 *   Get the output file for the device
 */

    if (isatty (fileno (pltout)))
    {
/*
 * If it's a tty, then we certainly don't want to write to it!
 */
	creatafile = YES;
    }
    else
    {
/*
 *   If it's not a tty, they MAY be running pspen in a script
 *   and have logged out. In that case they want us to spool the plot,
 *   even though the output may not be a tty. (It may be /dev/null.)
 */
	if (fstat (fileno (pltout), &statbuf) != 0)
	{
	    ERR (WARN, name,
		 "Can't stat plot output! Trying to create a spool file instead.");

	    creatafile = YES;
	}
	else
	{
	    if (S_ISREG (statbuf.st_mode) || S_ISSOCK (statbuf.st_mode) ||
		S_ISFIFO (statbuf.st_mode))
	    {
		creatafile = NO;
	    }
	    else
	    {
		creatafile = YES;
	    }
	}
    }


    if (creatafile)
    {
char           *spooldirnm;

	file_created = YES;

	if ((spooldirnm = getenv ("VPLOTSPOOLDIR")) != NULL)
	{
	    sprintf (scratch_file, "%s%s", spooldirnm, "/PSPEN_XXXXXX");
	}
	else
	{
	    sprintf (scratch_file, "%s%s", PEN_SPOOL, "/PSPEN_XXXXXX");
	}

	if(-1 == mkstemp (scratch_file)) {
            perror(scratch_file);
        }
	if ((pltout = fopen (scratch_file, "w")) == NULL)
	{
	    if (spooldirnm != NULL)
	    {
		ERR (COMMENT, name,
		     "Check permissions on directory \"$VPLOTSPOOLDIR\" = \"%s\".",
		     spooldirnm);
	    }
	    else
	    {
		ERR (COMMENT, name,
		   "Perhaps you need to do \"setenv VPLOTSPOOLDIR /tmp\"?");
	    }

	    ERR (FATAL, name, "Could not open scratch file \"%s\"!",
		 scratch_file);
	}
    }

    vartemp.i = &yesget;
    if (GETPAR ("strip tex", "1", vartemp))
    {
	if (yesget)
	    tex = YES;
    }

    if (tex == YES)
    {
	/*
	 * allow as big a plot as we can and make sure our plot is in
	 * absolute coordinates. This prevents any clipping of the image. If
	 * tex=y is specified this plot will be scaled by TeX to fit whatever
	 * size we want, so don't clip it to page boundaries.
	 */
	dev_xmax = VP_MAX * RPERIN;
	dev_ymax = VP_MAX * RPERIN * SCREEN_RATIO;
	dev_xmin = 0;
	dev_ymin = 0;
	size = ABSOLUTE;
	label[0] = '\0';
    }

    vartemp.i = &yesget;
    if (GETPAR ("hold", "1", vartemp))
    {
	if (yesget)
	    hold = YES;
	else
	    hold = NO;
    }

    vartemp.i = &ncopies_document;
    if (!GETPAR ("ncopies copies", "i", vartemp))
    {
	ncopies_document = 1;
    }

/*
 * Initialize the PostScript file
 */
    if (tex == NO)
    {
	fprintf (pltout, "%%!\n");

	fprintf (pltout, "initgraphics 1 setlinecap 1 setlinejoin\n");
	fprintf (pltout, "%d rotate", 90);
	fprintf (pltout, " 0 %.2f translate %.2f %.2f scale gsave\n",
		 -ps_ypapersize * PSPERIN, psscale, psscale);
    }
    else
    {
	/*
	 * changed to work with psfig macros in TeX. Take the rotation and
	 * translation out. Now all we need is a shell that puts the
	 * BoundingBox stuff on the front.
	 */

	fprintf (pltout, " \n");
	fprintf (pltout, "%% Start of pspen output\n");
	fprintf (pltout, " \n");
	fprintf (pltout, " %.2f %.2f scale gsave\n", psscale, psscale);
	/*
	 * Make sure postscript uses the correct line join type to avoid
	 * "fanged S syndrome", though!
	 */
	fprintf (pltout, " 1 setlinecap 1 setlinejoin\n");
    }

    /* in case colorimage command is not known by this device */
    fprintf (pltout, " \n");
    fprintf (pltout, "systemdict /colorimage known not { \n");
    fprintf (pltout, "  /colortograyscale { \n");
    fprintf (pltout, "    dup /rgbdata exch store \n");
    fprintf (pltout, "    length 3 idiv \n");
    fprintf (pltout, "    /npixls exch store \n");
    fprintf (pltout, "    /indx 0 store \n");
    fprintf (pltout, "    /pixls npixls string store \n");
    fprintf (pltout, "    0 1 npixls -1 add { \n");
    fprintf (pltout, "      pixls exch \n");
    fprintf (pltout, "      rgbdata indx get .3 mul \n");
    fprintf (pltout, "      rgbdata indx 1 add get .59 mul add \n");
    fprintf (pltout, "      rgbdata indx 2 add get .11 mul add \n");
    fprintf (pltout, "      cvi \n");
    fprintf (pltout, "  put \n");
    fprintf (pltout, "      /indx indx 3 add store \n");
    fprintf (pltout, "    } for \n");
    fprintf (pltout, "    pixls \n");
    fprintf (pltout, "  } bind def \n");
    fprintf (pltout, "  /mergeprocs { \n");
    fprintf (pltout, "    dup length \n");
    fprintf (pltout, "    3 -1 roll \n");
    fprintf (pltout, "    dup \n");
    fprintf (pltout, "    length \n");
    fprintf (pltout, "    dup \n");
    fprintf (pltout, "    5 1 roll \n");
    fprintf (pltout, "    3 -1 roll \n");
    fprintf (pltout, "    add \n");
    fprintf (pltout, "    array cvx \n");
    fprintf (pltout, "    dup \n");
    fprintf (pltout, "    3 -1 roll \n");
    fprintf (pltout, "    0 exch \n");
    fprintf (pltout, "    putinterval \n");
    fprintf (pltout, "    dup \n");
    fprintf (pltout, "    4 2 roll \n");
    fprintf (pltout, "    putinterval \n");
    fprintf (pltout, "  } bind def \n");
    fprintf (pltout, "  /colorimage { \n");
    fprintf (pltout, "     pop \n");
    fprintf (pltout, "     pop \n");
    fprintf (pltout, "     {colortograyscale} \n");
    fprintf (pltout, "     mergeprocs \n");
    fprintf (pltout, "     image \n");
    fprintf (pltout, "  } bind def \n");
    fprintf (pltout, "} if \n");
    fprintf (pltout, " \n");

    /* Some abbreviations to make the files more compact */
    fprintf (pltout, "/m { moveto } bind def\n");
    fprintf (pltout, "/d { lineto } bind def\n");
    fprintf (pltout, "/r { rlineto } bind def\n");
    fprintf (pltout, "/x { 0 0 rlineto } bind def\n");
    fprintf (pltout, "/s { stroke } bind def\n");
    fprintf (pltout, "/gr { grestore } bind def\n");
    fprintf (pltout, "/np { newpath } bind def\n");
    fprintf (pltout, "/cf { closepath eofill } bind def\n");
    fprintf (pltout, " \n");

    if (!mono)
    {
	/*
	 * Begin by setting standard 7 colors. (The monochrome equivalent
	 * takes place in "psreset.c" for the monochrome case.)
	 */
	for (i = 0; i < 8; i++)
	{
	    psattributes (SET_COLOR_TABLE, i, red[i], green[i], blue[i]);
	}
	psattributes (SET_COLOR, 7, 0, 0, 0);
    }

    fflush (pltout);

    epause = 0;
    endpause = NO;
/*
 * This turns out not to be a good default for this hardcopy device,
 * since people think of it more as a screen-type device.
 *
 *  size = ABSOLUTE;
 */
}


/*
 * get the date
 */
#if defined(__stdc__) || defined(__STDC__)
#include <time.h>
#endif
#include <sys/time.h>
#if defined(__stdc__) || defined(__STDC__)
void dateget (char *date)
#else
dateget (date)
    char           *date;
#endif
{
time_t          clock;
#if defined(__stdc__) || defined(__STDC__)
#else
time_t          time ();
struct tm      *localtime ();
char           *asctime ();
#endif

    clock = time ((time_t *) NULL);
    sprintf (date, "%.16s", asctime (localtime (&clock)));
}

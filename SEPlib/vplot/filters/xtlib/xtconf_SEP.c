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
 *  source file:   ./filters/xtlib/xtconf.c
 * 
 * Steve Cole (SEP), August 4 1991
 *      Inserted this sample edit history entry.
 */

#include "xtpen.h"

/* declarations */
char            name[] = "Sxtpen";
#include "xtdoc.h"

/* device routine table */

struct device   dev = {
 /* control routines */
		       xtopen,		/* open */
		       nullreset,	/* reset */
		       xtmessage,	/* message */
		       xterase,		/* erase */
		       xtclose,		/* close */
 /* high level output */
		       genvector,	/* vector */
		       genmarker,	/* marker */
		       gentext,		/* text */
		       xtarea,		/* area */
		       xtraster,	/* raster */
		       xtpoint,		/* point */
		       xtattributes,	/* attributes */
 /* input */
		       nullgetpoint,	/* getpoint */
		       xtinteract,	/* interact */
 /* low level output */
		       xtplot,		/* plot */
		       nullstartpoly,	/* startpoly */
		       nullmidpoly,	/* midpoly */
		       nullendpoly	/* endpoly */
};

int             lost;

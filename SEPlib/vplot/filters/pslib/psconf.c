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
 *  source file:   ./filters/pslib/psconf.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Steve Cole (SEP), August 26 1987
 *	Replaced genraster with psraster.
 * Steve Cole (SEP), March 25 1988
 *      Added psarea as it is now available.
 */

/*
 * Apple Laser Writer Configuration
 */
#include <stdio.h>
#include "../include/enum.h"
#include "../include/extern.h"

/*
 * mandatory declarations and initializations
 */
#ifdef SEP
char            name[] = "Pspen";
#else
char            name[] = "pspen";
#endif
#include "psdoc.h"

/*
 * device routine table
 */

struct device   dev = {

 /* control routines */
		       psopen,	/* open */
		       psreset,	/* reset */
		       genmessage,	/* message */
		       pserase,	/* erase */
		       psclose,	/* close */

 /* high level output */
		       psvector,	/* vector */
		       genmarker,	/* marker */
		       pstext,	/* text */
		       psarea,	/* area */
		       smart_psraster,	/* raster */
		       genpoint,	/* point */
		       psattributes,	/* attributes */

 /* input */
		       nullgetpoint,	/* getpoint */
		       nullinteract,	/* interact */

 /* low level output */
		       psplot,	/* plot */
		       nullstartpoly,	/* startpoly */
		       nullmidpoly,	/* midpoly */
		       nullendpoly,	/* endpoly */
};

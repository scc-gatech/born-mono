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
 *  source file:   ./filters/raslib/rasconf.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 */

/*
 * Keyword: vplot raster movie pen
 */
#include<sepConfig.h>
#include <stdio.h>
#include "../include/enum.h"
#include "../include/extern.h"
#include "raspen.h"

/*
 * mandatory declarations and initializations
 */
#ifdef SEP
char            name[] = "Raspen";
#else
char            name[] = "raspen";
#endif
#include "rasdoc.h"

/*
 * device routine table
 */

struct device   dev =
{

 /* control routines */
 rasopen,		/* open */
 rasreset,		/* reset */
 genmessage,		/* message */
 raserase,		/* erase */
 rasclose,		/* close */

 /* high level output */
 rasvector,		/* vector */
 genmarker,		/* marker */
 gentext,		/* text */
 genpatarea,		/* area */
 genraster,		/* raster */
 genpoint,		/* point */
 rasattr,		/* attributes */

 /* input */
 nullgetpoint,		/* getpoint */
 nullinteract,		/* interact */

 /* low level output */
 nullplot,		/* plot */
 nullstartpoly,		/* startpoly */
 nullmidpoly,		/* midpoly */
 nullendpoly		/* endpoly */
};

int lost;

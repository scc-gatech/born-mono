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
 *  source file:   ./filters/vplib/vpconf.c
 *
 * Joe Dellinger (SEP), Dec 19 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 */

/*
 * A generic filter --- both inputs and outputs vplot.
 * Keyword: pchain pen vplot
 */
#include <stdio.h>
#include "../include/extern.h"

/*
 * mandatory declarations
 */
char            name[] = "Svppen";
#include "vpdoc.h"

/*
 * device routine table
 */

struct device   dev =
{
 /* control routines */
 vpopen,		/* open */
 vpreset,		/* reset */
 vpmessage,		/* message */
 vperase,		/* erase */
 vpclose,		/* close */

 /* high level output */
 vpvector,		/* vector */
 vpmarker,		/* marker */
 vptext,		/* text */
 genarea,		/* area */
 vpraster,		/* raster */
 genpoint,		/* point */
 vpattributes,		/* attributes */

 /* input */
 nullgetpoint,		/* getpoint */
 nullinteract,		/* interact */

 /* low level output */
 vpplot,		/* plot */
 vpstartpoly,		/* startpoly */
 vpmidpoly,		/* midpoly */
 vpendpoly		/* endpoly */
};

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
 *  source file:   ./filters/genlib/gen_do_dovplot.c
 *
 * Joe Dellinger (SEP), Feb 24 1988
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 */

#include <sepConfig.h>
#include <stdio.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#include "../include/params.h"
#include "../include/extern.h"

/*
 * Call dovplot, cycling through the input plot files.
 */

int
#if defined(__stdc__) || defined(__STDC__)
gen_do_dovplot (int nn, FILE **inpltin, char innames[][MAXFLEN + 1])
#else
gen_do_dovplot (nn, inpltin, innames)
    int             nn;
    FILE          **inpltin;
    char            innames[][MAXFLEN + 1];
#endif
{
int             ii;

    for (ii = 0; ii < nn; ii++)
    {
/*
 * Set things up for dovplot.
 * The "-5" is to leave room for dovplot to add a little onto the
 * end of pltname if it wants.
 */
	pltin = inpltin[ii];
	strncpy (pltname, innames[ii], MAXFLEN - 5);
/*	fprintf(stderr,"before %s \n",innames[ii]);*/
	dovplot ();
/*	fprintf(stderr,"2efore %s \n",innames[ii]);*/
	fclose (pltin);
    }
    return 0;
}

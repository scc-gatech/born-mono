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
 *  source file:   ./filters/raslib/rasclose.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Joe Dellinger (SEP), May 7, 1989
 *	RGB option added.
 * Chuck Karish, August 5, 1989
 * 	For non-SEP users, don't try to open colfile unless it has been
 *	specified!
 */

#include<sepConfig.h>
#include <stdio.h>
#include "../include/closestat.h"
#include "../include/err.h"
#include "../include/extern.h"
#include "../include/params.h"
#include "raspen.h"

void rasclose (int status)
{
int             value;
FILE           *colout;

    switch (status)
    {
    case CLOSE_NORMAL:
#ifdef SEP
	Puthead ("Color table:\n");
	for (value = 0; value < NCOLOR; value++)
	{
	    if (color_table[value][0] != -1)
	    {
		Puthead ("%d\t\t%d\t%d\t%d\n", value,
			 color_table[value][0], color_table[value][1], color_table[value][2]);
	    }
	}
	Puthead ("\n");
#endif

	if (!grf_format && esize == 1 && colfile[0])
	{
	    colout = fopen (colfile, "w");
	    if (colout == NULL)
		ERR (WARN, name, "can't open colfile %s\n", colfile);
	    else
	    {
		for (value = 0; value < NCOLOR; value++)
		{
		    if (color_table[value][0] != -1)
		    {
			fprintf (colout, "%d\t\t%f\t%f\t%f\n", value,
				 (float) color_table[value][0] / MAX_GUN,
				 (float) color_table[value][1] / MAX_GUN,
				 (float) color_table[value][2] / MAX_GUN);
		    }
		}
	    }
	}
	break;
    case CLOSE_FLUSH:
	if (!grf_format)
	    fflush (pltout);
	break;
    default:
	break;
    }
}

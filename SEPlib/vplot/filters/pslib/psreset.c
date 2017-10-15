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
 *  source file:   ./filters/pslib/psreset.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Joe Dellinger (SEP), October 19 1988
 *	Added "grey" option.
 * Joe Dellinger (BP Amoco) October 5 1999
 *	Grey-level stuff should only be done for mono=y case.
 */

#include <stdio.h>
#include "../include/attrcom.h"
#include "../include/extern.h"
#include "ps.h"


void psreset (void)
{
int             ii;

    if (mono)
    {
	for (ii = 0; ii < 8; ii++)
	{
	    ps_grey_map (ii);
	}

	psattributes (SET_COLOR, 7, 0, 0, 0);
    }
}

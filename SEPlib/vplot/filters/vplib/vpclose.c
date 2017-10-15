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
 *  source file:   ./filters/vplib/vpclose.c
 *
 * Joe Dellinger (SEP), Dec 19 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 */

#include "../../include/vplot.h"
#include "../include/closestat.h"
#include "../include/extern.h"
#include "vp.h"

void vpclose (int status)
{
    switch (status)
    {
    case CLOSE_DONE:
/*	vp_purge();*/
/*	vp_erase();*/
	vp_endplot ();
	break;
    }
}

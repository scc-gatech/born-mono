/*
 * Copyright 1987 the Board of Trustees of the Leland Stanford Junior
 * University. Official permission to use this software is included in the
 * documentation. It authorizes you to use this file for any non-commercial
 * purpose, provided that this copyright notice is not removed and that any
 * modifications made to this file are commented and dated in the style of my
 * example below. 
 */

/* 
 * source file:   ./filters/xtlib/xtinteract.c 
 *
 * Steve Cole (SEP), August 4 1991
 *      Inserted this sample edit history entry.
 * Dave Nichols (SEP), April 30 1992
 *      Final version of the new xtpen, added message window
 */

#include <stdio.h>
#include "xtpen.h"
#include "../include/intcom.h"
#include "../include/attrcom.h"
#include "../include/extern.h"
#include <sys/ioctl.h>

int	skipit = NO;

/*ARGSUSED1*/
int xtinteract (
    int             what,
    FILE           *controltty,
    char           *string
               )
{
 
    switch (what)
    {
    case INT_PAUSE:
	if (skipit){
	    skipit = NO;
	    break;   /* skip the pause that occurs after a restart */
	}
    case INT_USER_PAUSE:
	skipit = NO;

	/*
 	 * Pauses caused by the "break" command too hard to handle!
 	 */
        if (group_number != 0){
            return DOVPLOT_CONT;
	}

	return DOVPLOT_EXIT; /* exit dovplot routine at end of frame */


    case INT_GET_STRING:
	/* Not supported */
	break;
    default:
	break;
    }

    return DOVPLOT_CONT;

}

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
 *  source file:   ./filters/genlib/geninteract.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Joe Dellinger Feb 22 1988
 *	Created INT_PAUSE.
 * Joe Dellinger May 20 1989
 *	controltty may be NULL, meaning "/dev/tty" can't be read from.
 *	Handle this case gracefully.
 * Joe Dellinger April 2 1992
 *	Return 0 so dovplot won't get confused and exit accidentally due
 *	to a junk return value happening to be DOVPLOT_EXIT.
 */

/*
 * For most devices, this can be the interact routine.
 * This routine will work if the terminal control during plotting
 * is the same place as standard input.
 */

/*
 * Get a string from the user.
 */

#include <stdio.h>
#include "../include/extern.h"
#include "../include/intcom.h"
#include "../include/attrcom.h"
#include "../include/err.h"

int geninteract (
    int             what,
    FILE           *controltty,
    char           *string
                )
{
    switch (what)
    {
    case INT_PAUSE:
    case INT_F_PAUSE:
    case INT_GET_STRING:
	if (controltty == NULL)
	{
	    ERR (WARN, name, "Sorry, can't read string from terminal.");
	}
	else
	{
	    if(NULL == fgets (string, 79, controltty))
            {
	        ERR (WARN, name, "Sorry, problem reading string from terminal.");
            }
	}
	break;
    default:
	break;
    }
    return DOVPLOT_CONT;
}

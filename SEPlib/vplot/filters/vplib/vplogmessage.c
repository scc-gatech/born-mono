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
 *  source file:   ./filters/vplib/vplogmessage.c
 *
 * Joe Dellinger (SEP), Jan 10 1988
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 */

#include <stdio.h>
#include "../include/enum.h"
#include "../include/mesgcom.h"
#include "../include/extern.h"
#include "vp.h"

static int      saveitlog;

void vplogmessage (int command, char string[])
{
    switch (command)
    {
    case MESG_READY:
	saveitlog = YES;
	break;
    case MESG_MESSAGE:
	saveitlog = NO;
	break;
    case MESG_TEXT:
	if (saveitlog)
	    fprintf (stderr, "%s", string);
	break;
    default:
	break;
    }
}

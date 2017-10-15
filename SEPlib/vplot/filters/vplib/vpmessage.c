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
 *  source file:   ./filters/vplib/vpmessage.c
 *
 * Joe Dellinger (SEP), Dec 19 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 */

#include <stdio.h>
#include "../../include/vplot.h"
#include "../include/enum.h"
#include "../include/extern.h"
#include "../include/mesgcom.h"
#include "vp.h"

static int      saveit;
static char     savestring[80 * 24];

void vpmessage (int command, char string[])
{
    switch (command)
    {
    case MESG_READY:
	saveit = NO;
	break;
    case MESG_MESSAGE:
	saveit = YES;
	strcpy (savestring, "");
	break;
    case MESG_DONE:
	if (saveit && !vpdumb)
	{
	    vp_message (savestring);
	}
	break;
    case MESG_TEXT:

	if (saveit)
	{
	    if (strcmp (string, CRLF) != 0)
		(void) strcat (savestring, string);
	}
	else
	{
	    fprintf (stderr, "%s", string);
	}

	break;
    default:
	break;
    }
}

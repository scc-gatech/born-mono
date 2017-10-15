/*
 *
 *  source file:   ./filters/loclib/doc.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Steve Cole (SEP), June 30 1989
 *      Added xargfix call to fix up external variables.
 */

/*
 * Special version to make SEP selfdoc the same way glenn does.
 */
#include <stdio.h>
#include <stdlib.h>
#if defined(__stdc__) || defined(__STDC__)
#include <string.h>
#else
#include <strings.h>
#endif
#include "../include/extern.h"

int doc (char *sourcefile)
{
int             ii;

#ifdef XARGFIX
	extern void xargfix();
	xargfix();
#endif
 
    if ((sepxargc == 1 || (sepxargc == 2 && !strncmp ("wstype=", sepxargv[1], 7)))
	&& !redin ())
    {
	for (ii = 0; ii < doclength; ii++)
	    printf ("%s\n", documentation[ii]);
	exit (EXIT_SUCCESS);
    }
    return 0;
}

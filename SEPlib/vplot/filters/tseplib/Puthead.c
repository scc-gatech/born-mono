/*
 *
 *  source file:   ./filters/loclib/Puthead.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 *
 * Joe Dellinger Dec 19 1987
 *	Added check for hclose_done; Don't write to header if we've
 *	closed the history part.
 */

/*
 *	The standard version of puthead will barf if the header
 *	is closed or if headstream has been changed from the last call.
 *	We need it to be tolerant of these inconsistencies.
 */

#include    <stdio.h>
#include    <stdarg.h>
#include  "../include/extern.h"

/*
 *	put a line on the header.
 *	the arguments are identical to printf conventions, except 
 *		there is a maximum of nine
 */

void Puthead (char *format, ... )
{
  va_list ap;
  extern int get_hclose_done(void);
  extern FILE *get_headstream(void);
    if (get_headstream() == NULL || get_hclose_done())
	return;
    va_start(ap,format);
    vfprintf(get_headstream(),format,ap);
    va_end(ap);
    fflush (get_headstream());
}

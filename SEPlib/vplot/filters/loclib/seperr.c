/*
 *
 *  source file:   ./filters/loclib/seperr.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Joe Dellinger Feb 16 1988
 *	Use VARARGS and vsprintf if they are available.
 * Steve Cole (SEP), June 30, 1988
 *      Added xargfix call to fix up external variables.
 * Phil Farrell, Stanford Earth Sciences, July 11, 1989
 *	Change "ultrix" define flag to "ULTRIX" to match other routines.
 * Dave Nichols, (SEP), Feb 21 1991
 * 	Made perror match its prototype on the convex.
 */

/*
 *	error abortion subroutine
 *
 *	a is a printf format string, while b-h are optional arguments
 *	When the subroutine is called, the program will stop after
 *	printing its message.
 *	Example:	seperr("Cannot divide %f by %f\n", x, y);
 *
 *      perror() is wiped out by including a dummy version of it here.
 *	this is because perror has the audacity to print directly to
 *	stderr, which we can't allow. this special version of err replaces
 *	the normal seplib version and forces seplib to go through ERR,
 *	the vplot filter subroutine for error messages that takes special
 *	care not to try to print messages in the middle of plotting.
 */
#include <sepConfig.h>
#include <stdio.h>
#include "../include/err.h"
#include "../include/extern.h"

#if defined (SUN) || defined (ULTRIX) || defined (LINUX)
#if defined (SUN) || defined (ULTRIX)
#include <varargs.h>
/*VARARGS1*/
int seperr (va_alist)
va_dcl
#endif
#if defined (LINUX)
#include <stdarg.h>
int seperr (char *format, ...)
#endif
{
va_list         apdum;
char            string[120];
#if defined (SUN) || defined (ULTRIX)
char           *format;
    va_start (apdum);
    format = va_arg (apdum, char *);
#endif
#if defined (LINUX)
    va_start (apdum, format);
#endif
    vsprintf (string, format, apdum);
    va_end (apdum);

    ERR (FATAL, name, "sep: %s", string);
/*
 * Force the linker to link in my version of perror
 */
#ifndef ultrix
    perror (string);
#endif
    return 0;
}
#else
/*VARARGS1*/
int seperr (a, b, c, d, e, f, g, h)
    char           *a, *b, *c, *d, *e, *f, *g, *h;
{
extern char   **sepxargv;
char            string[120];

    sprintf (string, a, b, c, d, e, f, g, h);
    ERR (FATAL, name, "sep: %s", string);
/*
 * Force the linker to link in my version of perror
 * 
 * This gets a multiply defined under ultrix so just give it a miss
 */

#ifndef ultrix
    perror (string);
#endif
    return 0;
}
#endif

#ifndef ultrix

#if defined(HAVE_STDLIB_H)
/*ARGSUSED*/
void perror( const char* junk)
#else
perror (junk)
char *junk;
#endif
{
}
#endif

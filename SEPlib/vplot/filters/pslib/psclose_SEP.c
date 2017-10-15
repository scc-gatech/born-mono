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
 *  source file:   ./filters/pslib/psclose.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Steve Cole (SEP), March 29 1988
 *	Moved showpage to pserase.
 * Joe Dellinger (SOEST), Feb 18 1992
 *	Added option for "PSPRINTER" environment variable.
 * Steve Cole (SEP), October 22 1992
 *	Added color printer support.
 * Joe Dellinger (AMOCO), March 22 1995
 *      Added custom paper size support.
 *      Added comment for Amoco users so they can see where their plot
 *      is being spooled. If you don't like that, #define NOCOMMENTS
 * Hector Urdaneta (SEP), April 26 1996
 *      Added ifdef SGI64, define NOCOMMENTS, endif 
 *      Not compiling in the SGI
 */

/* Routine to finish up */


#include <stdio.h>
#if defined(__stdc__) || defined(__STDC__)
#include <string.h>
#else
#include <strings.h>
#endif
#if defined(__stdc__) || defined(__STDC__)
#include <stdlib.h>
#include <unistd.h>
#else
extern char    *getenv ();
#endif

#include "../include/closestat.h"
#include "../include/enum.h"
#include "../include/extern.h"
#include "../include/err.h"
#include "ps.h"

#ifdef SEP
#define GETPAR getch
extern int getch(const char *, const char *, MIXED);
#else
#define GETPAR getpar
extern int getpar(const char *, const char *, MIXED);
#endif

void psclose (int status)
{
char            system_call[120];
char            printer[40];
char           *stringptr;
int             ecode;
MIXED		vartemp;

    endpath ();

    switch (status)
    {
    case CLOSE_NORMAL:
	/*
	 * if(tex == YES) fprintf(pltout, "%c", POP);
	 */

#ifndef NOCOMMENTS
	if ((strcmp (psprintertype, "default") != 0)
	    || (pixels_per_inch != DEFAULT_PIXELS_PER_INCH))
	{
	    ERR (COMMENT, name,
		 "Printer is of type \"%s\", with %g pixels per inch.",
		 psprintertype, pixels_per_inch);
	}
#endif

	/*
	 * If we created a temporary file, spool it.
	 */
	if (file_created)
	{
	    fclose (pltout);

	    if ((stringptr = getenv ("PSPRINTER")) != NULL)
		strcpy (printer, stringptr);
	    else if (mono)
		strcpy (printer, "postscript");
	    else
		strcpy (printer, "colorps");
	    vartemp.s = &(printer[0]);
	    GETPAR ("printer", "s", vartemp);

	    if (ps_set_papersize)
	    {
		sprintf (system_call,
			 "lpr -r -s -P%s -CX%.2f,Y%.2f %s",
			 printer, ps_ylength, ps_xlength, scratch_file);
	    }
	    else
	    {
		sprintf (system_call,
			 "lpr -r -s -P%s %s", printer, scratch_file);
	
	    }

#ifndef NOCOMMENTS
	    if (ps_set_papersize)
	    {
		ERR (COMMENT, name,
		     "Spooling plot using command \"%s\".", system_call);
	    }
	    else
	    {
		ERR (COMMENT, name,
		     "Spooling plot to printer \"%s\".", printer);
	    }
#endif
	    ecode = system (system_call);

	    /*
	     * Shift 8 bits over; what's left contains the exit code from lpr
	     */
	    if ((ecode & 0xFF) != 0)
	    {
		ERR (WARN, name,
		"Signal stopped or killed system call \"%s\".", system_call);
		ERR (WARN, name,
		     "Output postscript file may have been left behind in \"%s\".",
		     scratch_file);
	    }
	    else
	    {
		ecode = (ecode > 8);
		if (ecode != 0)
		{
		    ERR (WARN, name,
			 "Exit code %d from lpr. Is \"%s\" a valid printer?",
			 ecode, printer);
		    if (stringptr == NULL)
		    {
			ERR (COMMENT, name,
			     "Perhaps you need to do \"setenv PSPRINTER printer_name\"?");
		    }
		    else
		    {
			ERR (COMMENT, name,
			     "Is your environment variable $PSPRINTER = \"%s\" correct?",
			     printer);
		    }
		    ERR (WARN, name,
			 "The output postscript file may have been left behind in \"%s\".",
			 scratch_file);
		}
	    }
	}
	file_created = NO;
	break;
    case CLOSE_ERROR:
    case CLOSE_NOTHING:
    case CLOSE_INTERRUPT:
	break;
    case CLOSE_DONE:
	/*
	 * If we created a temporary file, remove it
	 */
	if (file_created)
	    unlink (scratch_file);
	break;
    case CLOSE_FLUSH:
	fflush (pltout);
	break;
    case CLOSE_PAUSE:
	break;
    default:			/* not meant for us, ignore */
	break;
    }
}

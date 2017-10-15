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
 *  source file:   ./filters/proc_vplot.c
 *
 * Joe Dellinger (SEP), Feb 19 1988
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 *
 * Joe Dellinger Feb 22 1988
 *	Created INT_PAUSE to be separate from INT_GET_STRING.
 * Joe Dellinger Feb 27 1988
 *	Interact option turns of endpausing.
 * W. Bauske IBM 03-26-91 
 *	Apply SysV fixes for RS/6000
 * Stew Levin MOBIL 2-27-95
 *	Solaris mods
 & Bob Clapp
 *  Changed signals from BSD to POSIX1 for LINUX
 */
#include <sepConfig.h>

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<math.h>
#define		GETPAR	getpar


#if defined(HAVE_TERMIOS_H)
#include	<termios.h>
#else
#if defined(HAVE_TERMIO_H)
#include	<termio.h>
#else /* USG */
#ifdef LINUX
#include	<bsd/sgtty.h>
#else
#include	<sys/ioctl.h>
#include	<sgtty.h>
#endif /* LINUX */
#endif /* HAVE_TERMIO_H */
#endif /* HAVE_TERMIOS_H */
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<ctype.h>
#include	<string.h>

#include	"../include/vplot.h"

#include	"./include/params.h"	/* for machine dependencies */
#include	"./include/enum.h"
#include	"./include/err.h"
#include	"./include/attrcom.h"
#include	"./include/intcom.h"
#include	"./include/mesgcom.h"
#include	"./include/erasecom.h"
#include	"./include/closestat.h"
#include	"./include/pat.h"
#include	"./include/vertex.h"
#include	"./include/round.h"
#include	"./include/extern.h"

/*
 * This routine is responsible for processing the input files,
 * and performing the necessary pausing, etc, that may be needed
 * at the end before exiting.
 */

void proc_vplot (void)
{
int             ii;
char            string[MAXFLEN + 1];
MIXED		vartemp;
char		dummystr[] = " ";

/*
 * Finally, shove all the plot files off to be done!
 */

/*	fprintf(stderr,"i am in process plot \n");*/
    if (!buffer_input)
    {
	for (ii = 0; ii < infileno; ii++)
	{
/*	fprintf(stderr,"2 am in process plot \n");*/
	    setbuf (pltinarray[ii], (char *) NULL);
/*	fprintf(stderr,"3 am in process plot \n");*/
	}
    }
/*	fprintf(stderr,"3b am in process plot \n");*/

    (*genreader) (infileno, pltinarray, pltinname);

/*	fprintf(stderr,"4 am in process plot \n");*/
/*
 * Normally, *genreader will be gen_do_dovplot, found in genlib
 */

    if (ever_called)
    {
	dev.close (CLOSE_FLUSH);
	if (epause > 0)
	{
	    usleep ((unsigned int) (1000000.0*epause));
	}
	if (need_end_erase)
	{
	    dev.erase (ERASE_END);
	}
	/*
	 * Inquire point back from device. Skip endpause stuff if we do
	 * interact, Since that's a pause in itself. 
	 */
	if (interact[0] != '\0')
	{
	    getapoint ();
	}
	else
	{

/*
 * Pause at the end if the user specifically asks us to on the command line,
 * even if we don't think we should because we think it's a file.
 */
	    vartemp.s = &(string[0]);
	    if (epause <= 0 &&
		(out_isatty || getpar ("endpause", "s", vartemp))
		&& endpause)
	    {
		dev.close (CLOSE_PAUSE);
		dev.interact (INT_F_PAUSE, controltty, string);
	    }
	}
	message (MESG_ON,dummystr);
	dev.close (CLOSE_NORMAL);
	dev.close (CLOSE_DONE);
	nplots++;
    }
    else
    {
	dev.close (CLOSE_NOTHING);
	ERR (COMMENT, name, "No input?");
	dev.close (CLOSE_DONE);
    }

    /*
     * Done, let them see what they are doing again 
     */
     /*
    if (!allowecho)
    {
#if defined(HAVE_TERMIO_H) || defined(HAVE_TERMIOS_H)
	if (ioctl (pltoutfd, TCSETAW, &tty_clean_state) == -1)
	{
		ERR (FATAL, name, "Bad ioctl call!");
	}
#else 
	ioctl (pltoutfd, TIOCLSET, (char *) (&tty_clean_local_mode));
	ioctl (pltoutfd, TIOCSETN, (char *) (&tty_clean_state));
#endif 
    }
    */
}

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
 *  source file:   ./filters/pslib/psarea.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Steve Cole (SEP), March 25 1988
 *      Wrote psarea.
 * Steve Cole (SEP), March 30 1988
 *      Routine now checks for "all black" and "all white" patterns and
 *      does the fill for these using setgray rather than messing with the
 *      halftone screen.
 *      Also, when the halftone screen has been altered, the routine now
 *      saves that state and avoids having to do all the work again if the
 *      same pattern is used to fill another area.
 * Steve Cole (SEP), March 31 1988
 *      Corrected orientation of pattern. Corrected scaling bug in
 *      call to setpattern.
 * Steve Cole (SEP), October 19 1988
 *      For mono=n case added a gsave to go with the grestore.
 * Joe Dellinger (Amoco), April 25 1996
 *	Use relative moves when they save space.
 */

#define mask0 ((unsigned char) (((unsigned int) 1) << 7))
#include <stdio.h>
#include <string.h>
#include "ps.h"
#include "../include/vertex.h"
#include "../include/extern.h"
#include "../include/err.h"
#include "../include/params.h"
#include "../include/pat.h"


void psarea (
    int             npts,
    struct vertex  *verlist
            )
{
unsigned char   mask, outchar;
int             nx, ny, nxold, nyold, i, ix, iy;
int            *bptr;
struct vertex  *v;
static int      ever_called = 0;
static int      error_size = 0;
static int      error_square = 0;
int             all_black, all_white;
static int      last_pattern = 0;

int             ps_oldx = 0, ps_oldy = 0;
char            stringd[80];
char            stringr[80];

    endpath ();

    if (mono)
    {
/*
 * check for "all black" or "all white" patterns. It is a waste of time
 * to do these using the halftone screen method used below, since a
 * simple "setgray" call will work just as well.
 */
	nx = pat[ipat].xdim;
	ny = pat[ipat].ydim;
	bptr = pat[ipat].patbits;
	all_black = 1;
	all_white = 1;
	for (iy = 0; iy < ny; iy++)
	{
	    for (ix = 0; ix < nx; ix++)
	    {
		if (bptr[iy * nx + ix] != 0)
		    all_white = 0;
		if (bptr[iy * nx + ix] == 0)
		    all_black = 0;
	    }
	}
/*
 * This section of this routine is borrowed from the PostScript Language
 * Cookbook put out by Adobe Systems Incorporated, program 15 "Filling an Area
 * with a Pattern". I have commented the code to indicate the purpose
 * of each section and how the sections interact, but the Cookbook includes
 * more detailed comments that anyone attempting to modify this code
 * would certainly find useful.
 *
 * setuserscreendict allocates space for the routine setuserscreen.
 * ever_called is used to ensure that we only do all this initialization
 * once.
 * we don't need to do this initialization if the pattern is all black
 * or all white, because we use setgray rather than setscreen for those.
 */
	if (all_black == 0 && all_white == 0)
	{
	    if (ever_called == 0)
	    {
		fprintf (pltout, "/setuserscreendict 22 dict def\n");
		fprintf (pltout, "setuserscreendict begin\n");
		fprintf (pltout, " /tempctm matrix def\n");
		fprintf (pltout, " /temprot matrix def\n");
		fprintf (pltout, " /tempscale matrix def\n");
		fprintf (pltout, " /concatprocs\n");
		fprintf (pltout, " {/proc2 exch cvlit def\n");
		fprintf (pltout, "  /proc1 exch cvlit def\n");
		fprintf (pltout, "  /newproc proc1 length proc2 length add\n");
		fprintf (pltout, "   array def\n");
		fprintf (pltout, "  newproc 0 proc1 putinterval\n");
		fprintf (pltout, "  newproc proc1 length proc2 putinterval\n");
		fprintf (pltout, "  newproc cvx\n");
		fprintf (pltout, " } def\n");
		fprintf (pltout, "/resmatrix matrix def\n");
		fprintf (pltout, "/findresolution\n");
		fprintf (pltout, " {72 0 resmatrix defaultmatrix dtransform\n");
		fprintf (pltout, "  /yres exch def /xres exch def\n");
		fprintf (pltout, "  xres dup mul yres dup mul add sqrt\n");
		fprintf (pltout, " } def\n");
		fprintf (pltout, "end\n");
/*
 * setuserscreen takes the desired halftone cell size and angle in the current
 * user space from the procedure "setpattern" (defined below) and converts
 * them to device space values. Then it calls the postscript procedure
 * "setscreen" to re-define the halftone screen.
 */
		fprintf (pltout, "/setuserscreen\n");
		fprintf (pltout, " {setuserscreendict begin\n");
		fprintf (pltout, "  /spotfunction exch def\n");
		fprintf (pltout, "  /screenangle exch def\n");
		fprintf (pltout, "  /cellsize exch def\n");
		fprintf (pltout, "  /m tempctm currentmatrix def\n");
		fprintf (pltout, "  /rm screenangle temprot rotate def\n");
		fprintf (pltout, "  /sm cellsize dup tempscale scale def\n");
		fprintf (pltout, "  sm rm m m concatmatrix m concatmatrix pop\n");
		fprintf (pltout, "  1 0 m dtransform /y1 exch def /x1 exch def\n");
		fprintf (pltout, "  /veclength x1 dup mul y1 dup mul add sqrt def\n");
		fprintf (pltout, "  /frequency findresolution veclength div def\n");
		fprintf (pltout, "  /newscreenangle y1 x1 atan def\n");
		fprintf (pltout, "  m 2 get m 1 get mul m 0 get m 3 get mul sub\n");
		fprintf (pltout, "   0 gt\n");
		fprintf (pltout, "  {{neg} /spotfunction load concatprocs\n");
		fprintf (pltout, "    /spotfunction exch def\n");
		fprintf (pltout, "  } if\n");
		fprintf (pltout, "  frequency newscreenangle /spotfunction load\n");
		fprintf (pltout, "   setscreen\n");
		fprintf (pltout, " end\n");
		fprintf (pltout, " } def\n");
/*
 * setpatterndict allocates space for the procedure setpattern.
 * bitpatternspotfunction is an entry in this dictionary that defines
 * the pattern. setpattern will load this and pass it to setuserscreen.
 */
		fprintf (pltout, "/setpatterndict 18 dict def\n");
		fprintf (pltout, "setpatterndict begin\n");
		fprintf (pltout, " /bitison\n");
		fprintf (pltout, "  {/ybit exch def /xbit exch def\n");
		fprintf (pltout, "   /bytevalue bstring ybit bwidth mul xbit 8 idiv\n");
		fprintf (pltout, "    add get def\n");
		fprintf (pltout, "   /mask 1 7 xbit 8 mod sub bitshift def\n");
		fprintf (pltout, "   bytevalue mask and 0 ne\n");
		fprintf (pltout, "  } def\n");
		fprintf (pltout, "end\n");
		fprintf (pltout, "/bitpatternspotfunction\n");
		fprintf (pltout, " {setpatterndict begin\n");
		fprintf (pltout, " /y exch def /x exch def\n");
		fprintf (pltout, " /xindex x 1 add 2 div bpside mul cvi def\n");
		fprintf (pltout, " /yindex y 1 add 2 div bpside mul cvi def\n");
		fprintf (pltout, " xindex yindex bitison\n");
		fprintf (pltout, "  {/onbits onbits 1 add def 1}\n");
		fprintf (pltout, "  {/offbits offbits 1 add def 0}\n");
		fprintf (pltout, "  ifelse\n");
		fprintf (pltout, " end\n");
		fprintf (pltout, "} def\n");
/*
 * setpattern sets up the halftone screen given the parameters passed
 * from showpattern and sends this definition to setuserscreen.
 */
		fprintf (pltout, "/setpattern\n");
		fprintf (pltout, " {setpatterndict begin\n");
		fprintf (pltout, "  /cellsz exch def\n");
		fprintf (pltout, "  /angle exch def\n");
		fprintf (pltout, "  /bwidth exch def\n");
		fprintf (pltout, "  /bpside exch def\n");
		fprintf (pltout, "  /bstring exch def\n");
		fprintf (pltout, "  /onbits 0 def /offbits 0 def\n");
		fprintf (pltout, "  cellsz angle /bitpatternspotfunction load\n");
		fprintf (pltout, "   setuserscreen\n");
		fprintf (pltout, "  {} settransfer\n");
		fprintf (pltout, "  offbits offbits onbits add div setgray\n");
		fprintf (pltout, " end\n");
		fprintf (pltout, "} def\n");
		ever_called = 1;
	    }
/*
 * The following code uses the definitions made above to set the
 * halftone screen to represent the user's requested pattern.
 * If the previous area fill was also done with this pattern, however,
 * the necessary transfer function has been saved in the postscript
 * procedure "vplottransfer", and we can just re-load it.
 */
	    if (last_pattern == ipat && ipat != 0)
	    {
		fprintf (pltout, "vplottransfer settransfer\n");
	    }
	    else
	    {
/*
 * Here's where the vplot work begins.
 * Define the fill pattern. It must be square and it must be a
 * multiple of 16 bits wide.
 */
		nxold = pat[ipat].xdim;
		nx = nxold;
		if (nx % 16 != 0)
		{
		    nx = ((nx + 15) / 16) * 16;
		    if (error_size == 0)
		    {
			ERR (WARN, name, "Pspen pattern must be a multiple of 16 bits wide, so partially replicating");
			error_size = 1;
		    }
		}
		nyold = pat[ipat].ydim;
		ny = nyold;
		if (ny % 16 != 0)
		{
		    ny = ((ny + 15) / 16) * 16;
		    if (error_size == 0)
		    {
			ERR (WARN, name, "Pspen pattern must be a multiple of 16 bits wide, so partially replicating");
			error_size = 1;
		    }
		}
		if (nx != ny)
		{
		    if (error_square == 0)
		    {
			ERR (WARN, name, "Pspen pattern must be square, so partially replicating");
			error_square = 1;
		    }
		    if (nx > ny)
			ny = nx;
		    if (ny > nx)
			nx = ny;
		}
		bptr = pat[ipat].patbits;
		fprintf (pltout, "/pattern <");
		mask = mask0;
		outchar = 0;
		for (iy = 0; iy < ny; iy++)
		{
		    for (ix = 0; ix < nx; ix++)
		    {
			if (bptr[(nyold - 1 - (iy % nyold)) * nxold + (ix % nxold)] != 0)
			{
			    outchar = (unsigned char) (outchar|mask);
			}
			if (mask == 1)
			{
			    fprintf (pltout, "%2.2x", outchar);
			    mask = mask0;
			    outchar = 0;
			}
			else
			{
			    mask >>= 1;
			}
		    }
		}

		fprintf (pltout, "> def\n");
/*
 * Save the graphics state (because we're going to destroy the halftone
 * screen, etc.) Construct the polygon and fill it by calling setpattern.
 * Then restore the graphics state.
 * arguments to setpattern:
 * number of bits on a side of the pattern
 * number of bytes on a side of the pattern
 * size of the pattern in PostScript units.
 */
		fprintf (pltout, "gsave\n");
		fprintf (pltout, "pattern %d %d 0 %d setpattern\n", nx, nx / 8, nx);
/*
 * Save the transfer function (this is the modified halftone screen)
 * so that if another area is filled with the same pattern we can avoid
 * all the work of computing the transfer function.
 */
		fprintf (pltout, "/vplottransfer currenttransfer def\n");
	    }
	}
/*
 * For all-black or all-white patterns, we simply use setgray.
 */
	else
	{
	    fprintf (pltout, "gsave\n");
	    if (all_white == 1 && ps_curcolor != PS_WHITE)
		fprintf (pltout, "1 setgray\n");
	    if (all_black == 1 && ps_curcolor != PS_BLACK)
		fprintf (pltout, "0 setgray\n");
	}
	fprintf (pltout, "np\n");

	v = verlist;
	fprintf (pltout, "%d %d m\n", v->x, v->y);

	ps_oldx = v->x;
	ps_oldy = v->y;
	v++;

	for (i = 1; i < npts; i++)
	{
	    /*
	     * A bit brute force, but a sure way of using whichever is
	     * shorter: absolute or relative draws.
	     */
	    sprintf (stringd, "%d %d d\n", v->x, v->y);
	    sprintf (stringr, "%d %d r\n", v->x - ps_oldx, v->y - ps_oldy);

	    if (strlen (stringd) <= strlen (stringr))
		fprintf (pltout, "%s", stringd);
	    else
		fprintf (pltout, "%s", stringr);

	    ps_oldx = v->x;
	    ps_oldy = v->y;
	    v++;
	}
	fprintf (pltout, "cf\n");
	fprintf (pltout, "gr\n");
    }
    else
    {
	fprintf (pltout, "gsave\n");
	fprintf (pltout, "np\n");

	v = verlist;
	fprintf (pltout, "%d %d m\n", v->x, v->y);

	ps_oldx = v->x;
	ps_oldy = v->y;
	v++;

	for (i = 1; i < npts; i++)
	{
	    /*
	     * A bit brute force, but a sure way of using whichever is
	     * shorter: absolute or relative draws.
	     */
	    sprintf (stringd, "%d %d d\n", v->x, v->y);
	    sprintf (stringr, "%d %d r\n", v->x - ps_oldx, v->y - ps_oldy);

	    if (strlen (stringd) <= strlen (stringr))
		fprintf (pltout, "%s", stringd);
	    else
		fprintf (pltout, "%s", stringr);

	    ps_oldx = v->x;
	    ps_oldy = v->y;
	    v++;
	}
	fprintf (pltout, "cf\n");
	fprintf (pltout, "gr\n");
    }

}

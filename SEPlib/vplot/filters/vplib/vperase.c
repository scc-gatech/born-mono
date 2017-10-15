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
 *  source file:   ./filters/vplib/vperase.c
 *
 * Joe Dellinger (SEP), Dec 19 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Joe Dellinger Feb 25 1988
 *	Put out a setstyle command after erases.
 * Joe Dellinger Dec 10 1988
 *	Added "outN" option.
 * Joe Dellinger (AMOCO) Nov 19 1994
 *	Explicitly re-write out all used color table entries
 *	if the output has been redirected. This has the effect
 *	of making the "outN" option actually useful for something!
 * Joe Dellinger (AMOCO) Nov 22 1994
 *	Write out the number of frames in the "stat" case.
 */

#include <stdio.h>
#include "../../include/vplot.h"
#include "../include/attrcom.h"
#include "../include/erasecom.h"
#include "../include/enum.h"
#include "../include/err.h"
#include "../include/extern.h"
#include "../include/round.h"
#include "vp.h"

int      vpframecount = -1;

void vperase (int command)
{
int newout, ii;

    if (vparray[0] == 0)
    {
	switch (command)
	{
	case ERASE_START:
	    vpframecount = 0;
	    break;
	case ERASE_MIDDLE:
	    vpframecount++;
	    newout = vpopen_name (vpframecount);
	    vp_erase ();
	    if (!vpdumb && vpstyle)
	    {
		vp_style (ABSOLUTE);
	    }
	    lost = YES;
	    vpsetflag = NO;

	    if (!vpdumb && newout)
	    {
/*
 * If this is a new output file, then explicitly set the entire
 * color table to its current state.
 */
		vpsetcoltabanyway = YES;
		for (ii=0; ii < VPPEN_NUM_COL; ii++)
		{
		    if (vpscoltabinfo[ii][ISITSET])
		    {
			vpattributes (SET_COLOR_TABLE, ii,
			    vpscoltabinfo[ii][1],
			    vpscoltabinfo[ii][2],
			    vpscoltabinfo[ii][3]);
		    }
		}
		vpsetcoltabanyway = NO;

		lost = YES;
		vpsetflag = NO;
            }

	    break;
	case ERASE_BREAK:
	    vp_break ();
	    if (!vpdumb && vpstyle)
	    {
		vp_style (ABSOLUTE);
	    }
	    lost = YES;
	    vpsetflag = NO;
	    break;
	default:
	    break;
	}
    }
    else
    {
	switch (command)
	{
	case ERASE_START:
	    vpframecount = 0;
	    dev_ymin = STANDARD_HEIGHT * RPERIN;
	case ERASE_MIDDLE:
	    if (vpframecount < 0)
		ERR (FATAL, name, "Must have initial erase with gridnum");
	    if ((vpframecount % vparray[0]) == 0)
	    {
		dev_xmin = 0;
		dev_ymin -= vpasize[1];
	    }
	    else
	    {
		dev_xmin += vpasize[0];
	    }
	    dev_xmax = dev_xmin + vpasize[0];
	    dev_ymax = dev_ymin + vpasize[1];

	    if (command == ERASE_MIDDLE)
		vp_break ();

	    lost = YES;
	    vpsetflag = NO;
	    reset_parameters ();
	    vpframecount++;

	    if (vpframe >= 0)
	    {
		vp_color (WHITE);
		vp_fat (vpframe);

		vp_move ((float) dev_xmin / RPERIN, (float) dev_ymin / RPERIN);
		vp_draw ((float) dev_xmax / RPERIN, (float) dev_ymin / RPERIN);
		vp_draw ((float) dev_xmax / RPERIN, (float) dev_ymax / RPERIN);
		vp_draw ((float) dev_xmin / RPERIN, (float) dev_ymax / RPERIN);
		vp_draw ((float) dev_xmin / RPERIN, (float) dev_ymin / RPERIN);

		vp_color (vpcolor);
		vp_fat (ROUND (vpfat * FATPERIN / RPERIN));
	    }
	    break;
	case ERASE_BREAK:
	    break;
	default:
	    break;
	}
    }
}


/*
 * Dummy erase command; does nothing but count frames.
 */

void vpderase (int command)
{
	switch (command)
	{
	case ERASE_START:
	    vpframecount = 0;
	    break;
	case ERASE_MIDDLE:
	    vpframecount++;
	    break;
	case ERASE_BREAK:
	    break;
	default:
	    break;
	}
}

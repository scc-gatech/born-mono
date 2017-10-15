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
 *  source file:   ./filters/vplib/vpraster.c
 *
 * Joe Dellinger (SEP), Dec 19 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 */

#include <stdio.h>
#include "../../include/vplot.h"
#include "../include/enum.h"
#include "../include/err.h"
#include "../include/extern.h"
#include "vp.h"

/*ARGSUSED8*/
void
vpraster (int xpix, int ypix, int xmin, int ymin, int xmax, int ymax, unsigned short *raster_block, int orient, int dither_it, int byte2)
{
float           temp1, temp2;


/*fprintf(stderr,"IN VP RASTER \n");*/
/*   if(byte2==1)*/
/*   ERR(FATAL,"Asked for two byte option with vpraster (unimplemented) \n");*/


    vpsetflag = NO;
    lost = YES;

    temp1 = (float) xmax / RPERIN;
    temp2 = (float) ymax / RPERIN;

    vp_rastershort (raster_block, vpblast, (vpbit > 0), vpbit, xpix, ypix,
	       (float) xmin / RPERIN, (float) ymin / RPERIN,
	       0., &temp1, &temp2,
	       orient, NO);
}

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
 *  source file:   ./filters/raslib/rasattr.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Stewart A. Levin (MRDC/DRL), September 8, 1988
 *      Return DOVPLOT_CONT on exit.
 */

/*
 * control graphics attributes
 */
#include <sepConfig.h>
#include <stdio.h>
#include "../include/attrcom.h"
#include "../include/extern.h"
#include "raspen.h"

int             color_table[NCOLOR][3], rascolor;

int rasattr (int command, int value, int v1, int v2, int v3)
{
    switch (command)
    {
    case SET_COLOR:
	rascolor = color_mult * value;
/*fprintf(ntderr,"setting color %d  \n",rascolor);*/
	break;
    case SET_COLOR_TABLE:
	color_table[value * color_mult][0] = v1;
	color_table[value * color_mult][1] = v2;
	color_table[value * color_mult][2] = v3;
  if(value==0) zap_change(color_table[0]);
/*fprintf(stderr,"setting color %d %d %d \n",v1,v2,v3);*/
	break;
    default:
	break;
    }

    return (DOVPLOT_CONT);
}

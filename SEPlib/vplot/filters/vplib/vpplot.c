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
 *  source file:   ./filters/vplib/vpplot.c
 *
 * Joe Dellinger (SEP), Dec 19 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 */

#include "../../include/vplot.h"
#include "../include/enum.h"
#include "../include/extern.h"
#include "vp.h"

int             lost = YES;

void vpplot (int x, int y, int draw)
{

    vpsetflag = NO;

    if (draw)
	vp_draw ((float) x / RPERIN, (float) y / RPERIN);
    else
	vp_move ((float) x / RPERIN, (float) y / RPERIN);

    lost = NO;
}

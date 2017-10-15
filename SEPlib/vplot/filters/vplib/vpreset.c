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
 *  source file:   ./filters/vplib/vpreset.c
 *
 * Joe Dellinger (SEP), Dec 19 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Joe Dellinger Feb 25 1988
 *	Do erases FIRST! And only THEN the setstyle.
 */

#include <stdio.h>
#include "../../include/vplot.h"
#include "../include/extern.h"
#include "../include/attrcom.h"
#include "../include/enum.h"
#include "../include/round.h"
#include "vp.h"

void vpreset (void)
{
/*
 * Reset everything we can think of.
 * Ignore initial erases, and instead look at the command line
 * value of "erase" to decide whether to have an initial erase
 * or not.
 */

/*
 * vpsetflag is used to squeeze out redundant attribute-setting commands.
 */
    vpsetflag = NO;

    if (erase & FORCE_INITIAL)
	vp_erase ();

    if (!vpdumb && vpstyle)
    {
	vp_style (ABSOLUTE);
    }

    if (!vpdumb)
    {
	dev.attributes (SET_WINDOW, dev_xmin, dev_ymin, dev_xmax, dev_ymax);
	dev.attributes (SET_COLOR, WHITE, 0, 0, 0);
	dev.attributes (NEW_FAT, 0, 0, 0, 0);
	dev.attributes (NEW_DASH, 0, 0, 0, 0);
	dev.attributes (NEW_FONT, txfont, txprec, txovly, 0);
	dev.attributes (NEW_ALIGN, txalign.hor, txalign.ver, 0, 0);
	dev.attributes (NEW_OVERLAY, overlay, 0, 0, 0);
    }
}

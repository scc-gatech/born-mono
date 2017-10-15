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
 *  source file:   ./filters/vplib/vpattr.c
 *
 * Joe Dellinger (SEP), Dec 19 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Joe Dellinger (AMOCO), Nov 19 1994
 *	Keep track of what color table entries have been used.
 *	Don't bother to re-write out color table entries if
 *	doing so would have no effect.
 */

#include <stdio.h>
#include "../../include/vplot.h"
#include "../include/attrcom.h"
#include "../include/params.h"
#include "../include/extern.h"
#include "../include/round.h"
#include "../include/enum.h"
#include "../include/pat.h"
#include "vp.h"

int             vpcolor = WHITE;
int             vpfat = 0;

int		vpscoltabinfo[VPPEN_NUM_COL][4];
int		vpsetcoltabanyway = NO;

int
vpattributes (int command, int value, int v1, int v2, int v3)
{
static float    vpdash[MAXDASH];
static float    vpgap[MAXDASH];
static int      vpndash;
static int     *vpattrarray;
static int      vpxwmin, vpywmin, vpxwmax, vpywmax;
static int      vpfont1, vpfont2, vpfont3;
static int      vpjust1, vpjust2;
static int      vpcolt1, vpcolt2, vpcolt3, vpcolt4;
static int      vpovly;
int             ii, jj;

    lost = YES;

    if (!vpdumb)
    {
	switch (command)
	{
	case SET_COLOR:
	    if (vpsetflag & F_COL)
	    {
		if (vpcolor == value)
		    break;
	    }
	    vp_color (value);
	    vpcolor = value;
	    vpsetflag |= F_COL;
	    break;

	case SET_COLOR_TABLE:
	    if (vpsetflag & F_COLT)
	    {
		if (vpcolt1 == value &&
		    vpcolt2 == v1 &&
		    vpcolt3 == v2 &&
		    vpcolt4 == v3)
		    break;
	    }
/*
 * The only global attribute in vplot that stays set across
 * erases is the color table. If we're about to re-set this
 * color to the same thing it's already set to, then don't
 * bother! (Unless we've been told to do it anyway.)
 */
	    if (vpsetcoltabanyway ||
		vpscoltabinfo[value][ISITSET] != YES ||
	        vpscoltabinfo[value][1] != v1 ||
	        vpscoltabinfo[value][2] != v2 ||
	        vpscoltabinfo[value][3] != v3)
	    {
	        vp_coltab (value,
		       (float) v1 / (float) MAX_GUN,
		       (float) v2 / (float) MAX_GUN,
		       (float) v3 / (float) MAX_GUN);
/*
 * A new one! Save it.
 */
		vpscoltabinfo[value][ISITSET] = YES;
		vpscoltabinfo[value][1] = v1;
		vpscoltabinfo[value][2] = v2;
		vpscoltabinfo[value][3] = v3;
	    }

	    vpcolt1 = value;
	    vpcolt2 = v1;
	    vpcolt3 = v2;
	    vpcolt4 = v3;
	    vpsetflag |= F_COLT;

	    break;

	case SET_WINDOW:

	    if (vpsetflag & F_CLIP)
	    {
		if (value == vpxwmin &&
		    v1 == vpywmin &&
		    v2 == vpxwmax &&
		    v3 == vpywmax)
		    break;
	    }

	    vp_clip ((float) (value) / RPERIN, (float) (v1) / RPERIN,
		     (float) (v2) / RPERIN, (float) (v3) / RPERIN);
	    vpxwmin = value;
	    vpywmin = v1;
	    vpxwmax = v2;
	    vpywmax = v3;
	    vpsetflag |= F_CLIP;
	    break;

	case NEW_DASH:
	    if (vpsetflag & F_DASH)
	    {
		if (value == vpndash)
		{
		    jj = YES;
		    for (ii = 0; ii < value; ii++)
		    {
			if (vpdash[ii] != dashes[2 * ii] ||
			    vpgap[ii] != dashes[2 * ii + 1])
			    jj = NO;
		    }
		    if (jj)
			break;
		}
	    }

	    for (ii = 0; ii < value; ii++)
	    {
		vpdash[ii] = dashes[2 * ii];
		vpgap[ii] = dashes[2 * ii + 1];
	    }
	    vp_setdash (vpdash, vpgap, value);
	    vpndash = value;
	    vpsetflag |= F_DASH;
	    break;

	case NEW_PAT:
	    vpattrarray = (int *) malloc ((unsigned)
			(pat[value].xdim * pat[value].ydim * sizeof (int)));

	    if (vpattrarray != NULL)
	    {
		for (ii = 0; ii < pat[value].xdim * pat[value].ydim; ii++)
		    vpattrarray[ii] = pat[value].patbits[ii];

		vp_patload ((int) RPERIN,
			    pat[value].xdim, pat[value].ydim,
			    value - 1, vpattrarray);

		free ((char *) vpattrarray);
	    }
	    break;

	case NEW_FONT:
	    if (value == -1)
		value = vpfont1;
	    if (v1 == -1)
		v1 = vpfont2;
	    if (v2 == -1)
		v2 = vpfont3;

	    if (vpsetflag & F_FONT)
	    {
		if (vpfont1 == value &&
		    vpfont2 == v1 &&
		    vpfont3 == v2)
		    break;
	    }

	    vp_tfont (value, v1, v2);
	    vpfont1 = value;
	    vpfont2 = v1;
	    vpfont3 = v2;
	    vpsetflag |= F_FONT;
	    break;

	case NEW_OVERLAY:
	    if (vpsetflag & F_OVLY)
	    {
		if (vpovly == value)
		    break;
	    }
/*
 * Another libvplot command that doesn't exist but should.
 * XXXXXX
 *		vp_overlay(value);
 */
	    vpsetflag |= F_OVLY;
	    vpovly = value;
	    break;

	case NEW_ALIGN:
	    if (vpsetflag & F_JUST)
	    {
		if (vpjust1 == value &&
		    vpjust2 == v1)
		    break;
	    }
	    vp_tjust (value, v1);
	    vpjust1 = value;
	    vpjust2 = v1;
	    vpsetflag |= F_JUST;
	    break;

	case NEW_FAT:
	    if (vpsetflag & F_FAT)
	    {
		if (vpfat == value)
		    break;
	    }

	    vp_fat (ROUND (value * FATPERIN / RPERIN));
	    vpfat = value;
	    vpsetflag |= F_FAT;
	    break;

	case BEGIN_GROUP:
	    if (value > 0)
		vp_bgroup (group_name);
	    break;

	case END_GROUP:
	    if (value > 0)
		vp_egroup ();
	    break;

	default:
	    break;
	}
    }
    else
    {
	switch (command)
	{
	case SET_COLOR:
	    if (vpsetflag & F_COL)
	    {
		if (vpcolor == value)
		    break;
	    }
	    vp_color (value);
	    vpcolor = value;
	    vpsetflag |= F_COL;
	    break;

	default:
	    break;
	}
    }

    return DOVPLOT_CONT;
}

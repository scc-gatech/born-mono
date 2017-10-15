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
 *  source file:   ./filters/vplib/vptext.c
 *
 * Joe Dellinger (SEP), Dec 19 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Joe Dellinger Jan 15 1988
 *	Use the VP_GTEXT vplot command, instead of struggling trying
 *	to make do with the archaic VP_TEXT.
 */

#include	<stdio.h>
#include	<math.h>
#include	"../../include/vplot.h"
#include	"../include/extern.h"
#include	"../include/enum.h"
#include	"../include/params.h"
#include	"../include/round.h"
#include	"./vp.h"

void vptext (char *string, float pathx, float pathy, float upx, float upy)
{
void             (*savevector) (int,int,int,int,int,int);
int             (*saveattributes) (int,int,int,int,int);
void             (*savearea) (int,struct vertex *);

    vpsetflag = NO;
    lost = YES;

    if (*string == '\0')
	return;

/*	fprintf(stderr,"before 1 \n");*/
    vp_gtext ((float) xold / RPERIN, (float) yold / RPERIN,
	      pathx / RPERIN, pathy / RPERIN,
	      upx / RPERIN, upy / RPERIN,
	      string);
/*	fprintf(stderr,"before 2 \n");*/

/*
 *   Now reset the pen position to the end of the text.
 *   Do a dummy run through (if this indeed a gentext font)
 */
    if (txfont < NUMGENFONT)
    {
	savevector = dev.vector;
	saveattributes = dev.attributes;
	savearea = dev.area;

/*
 *   Disconnect everything except error messages
 */
	dev.vector = nullvector;
	dev.attributes = nullattributes;
	dev.area = nullarea;

	gentext (string, pathx, pathy, upx, upy);

	dev.vector = savevector;
	dev.attributes = saveattributes;
	dev.area = savearea;

/*
 * Jon note that this shows you how to find the size of the text.
 */
/*	fprintf(stderr,"before 3 \n");*/
	vp_move ((float) xold / RPERIN, (float) yold / RPERIN);
    }
/*	fprintf(stderr,"before 3 \n");*/
}

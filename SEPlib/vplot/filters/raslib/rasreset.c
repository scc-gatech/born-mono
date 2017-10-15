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
 *  source file:   ./filters/raslib/rasreset.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 */
#include<stdio.h>
#include<sepConfig.h>


#include "raspen.h"
#include "../include/params.h"
#include "../include/extern.h"

void rasreset (void)
{
int             value;
    zap ();
    for (value = 0; value < NCOLOR; value++)
    {
/*why should i bother resetting this in 99% cases-RGC*/
/*	color_table[value][0] = -1;*/
    }
    for (value = 0; value < 8; value++)
    {
	color_table[value * color_mult][0] = MAX_GUN * ((value & 2) / 2);
	color_table[value * color_mult][1] = MAX_GUN * ((value & 4) / 4);
	color_table[value * color_mult][2] = MAX_GUN * ((value & 1) / 1);
    }
	color_table[0 * color_mult][0] =256;
	color_table[0 * color_mult][1] =0;
	color_table[0 * color_mult][2] =0;
}

/*
 * Zero image
 */
void zap (void)
{
unsigned char  *p;
    for (p = image; p < &image[dev_xmax * dev_ymax * esize]; p++)
	*p = 0;
}
void zap_change(int *col)
{
unsigned char  *p;
  if(esize==3){
    for (p = image; p < &image[dev_xmax * dev_ymax*esize];){
	*p++ =(unsigned char) col[0];
	*p++ =(unsigned char)col[1];
	*p++ =(unsigned char)col[2];
    }
  }
}

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
 *  source file:   ./lvplot/geth.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 */

#ifndef FORTRAN
#include <stdio.h>
#include "../include/extern.h"

short geth (register FILE *iop)
{
union
{
    unsigned short  w;
    short           s;
}               r;

    r.w = (unsigned short) getc (iop);
    r.w = (unsigned short) (r.w + (getc (iop) << 8));

#ifdef CRAY
    if( r.s > 0x8000 ) r.s = r.s - 0x10000;
#endif

    if (feof (iop))
	return (EOF);
    return ( r.s );
}

float getf (register FILE *iop)
{
 union
 {
    int i;
    float f;
 } r;

   r.i = getc(iop) & 255;
   r.i |= (getc(iop) & 255) << 8;
   r.i |= (getc(iop) & 255) << 16;
   r.i |= (getc(iop) & 255) << 24;

   return (r.f);
}
#endif

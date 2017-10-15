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
 *  source file:   ./lvplot/puth.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 */

#ifndef FORTRAN
#include <stdio.h>
#include "../include/extern.h"

int puth (register int w, register FILE *iop)
{
register int    j;

    j = w & 255;		/* low order byte of halfword value */
    w >>= 8;
    putc ((char) j, iop);
/*    putc ( j, iop);*/
    j = w & 255;		/* high order byte of halfword value */
    putc ((char) j, iop);
/*    putc ( j, iop);*/
    return (ferror (iop));
}

int putf (float w, FILE *iop)
{
    union {
      int i;
      float f;
    } conv;
    int j;

    conv.f = w;
    j = conv.i & 255;
    conv.i >>= 8;
    putc ((char) j, iop);
    j = conv.i & 255;
    conv.i >>= 8;
    putc ((char) j, iop);
    j = conv.i & 255;
    conv.i >>= 8;
    putc ((char) j, iop);
    j = conv.i & 255;
    putc ((char) j, iop);

    return (ferror (iop));
}

#endif

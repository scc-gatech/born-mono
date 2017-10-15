/*
 *
 *  source file:   ./filters/loclib/geth.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Martin Karrenbach 11-3-92   ifdef cray
 */

#include <stdio.h>
#include "../include/extern.h"

short
geth(register FILE *iop)
{
        union {
	    unsigned short w;
	    short s;
	    } r;

	r.w = (unsigned short) (getc(iop));
	r.w = (unsigned short) (r.w + ((unsigned short) (getc(iop) << 8)));
#ifdef CRAY
	if( r.s > 0x8000 ) r.s = r.s - 0x10000;
#endif
	if (feof(iop))
		return(EOF);
	return(r.s);
}

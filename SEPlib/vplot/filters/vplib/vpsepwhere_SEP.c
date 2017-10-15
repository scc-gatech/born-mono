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
 *  source file:   ./filters/vplib/vpsepwhere.c
 *
 * Joe Dellinger (SEP), Feb 12 1988
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Stewart A. Levin (Mobil), Feb 19, 1993
 *      Removed erroneous ; after function body.
 */

#include "../include/extern.h"
#ifdef SEP
/*
 * Override the default places Pen wants to send "out" and "head".
 * Change them back to the normal SEP defaults.
 */
char            sepoutwhere[] = "";
char            sepheadwhere[] = "";

/* Make linker happy that it found it. */
void sepwhere (void)
{
return;
}
#endif

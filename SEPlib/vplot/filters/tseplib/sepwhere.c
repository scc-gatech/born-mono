/*
 *
 *  source file:   ./filters/loclib/sepwhere.c
 *
 * Joe Dellinger (SEP), Feb 12 1988
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Stewart A. Levin (Mobil) Feb 19, 1993
 *      Removed erroneous ; after function body.
 */

#include "../include/extern.h"
/*
 * If for some reason the default places for the output and header to
 * go aren't suitable, link in your own version of this routine
 * ahead of this one.
 */
char            sepoutwhere[] = "/dev/tty";
char            sepheadwhere[] = "/dev/null";

/* Make linker happy that it found it. */
void sepwhere (void)
{
}

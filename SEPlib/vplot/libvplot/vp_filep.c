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
 *  source file:   ./lvplot/vp_filep.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Stewart A. Levin (SEP), 17 Feb 2012
 *      Added double precision _g/g versions
 */

#include <stdio.h>
#include "../include/vplot.h"
#include "vp_pc.h"

#ifndef FORTRAN
int vp_filep (FILE *filepntr)
{
    vp_pc._pltout = filepntr;
    return (vp_fixpc());
}
int vp_filep_g (FILE *filepntr)
{
    vp_pc_g._pltout = filepntr;
    return (vp_fixpc_g());
}
#else
int vpfilep_ (int *filepntr)
{
    return vp_filep (fdopen (*filepntr, "w"));
}
int vpfilepg_ (int *filepntr) {
    return vp_filep_g (fdopen (*filepntr, "w"));
}
#endif

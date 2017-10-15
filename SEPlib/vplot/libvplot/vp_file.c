/*
 * Copyright 1987 the Board of Trustees of the Leland Stanford Junior
 * University. Official permission to use this software is included in
 * the documentation. It authorizes you to use this file for any
 * non-commercial purpose, provided that this copyright notice is not
 * removed and that any modifications made to this file are commented
 * and dated in the style of my example below.
 */

/*
 *  source file:   ./lvplot/vp_file.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 *
 * Steve Cole (SEP), October 27 1987
 *	Null character is added to filename when called from fortran
 *      to prevent allocation errors.
 * W. Bauske IBM 03-27-91
 *	Commented out stuff on ends of #else and #endif for RS/6000
 * Stewart A. Levin (SEP), 17 Feb 2012
 *      Added double precision _g/g versions
 */

#include <sepConfig.h>
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include "../include/vplot.h"
#include "vp_pc.h"

#ifdef FORTRAN
#define A1(NAM) \
int NAM (char *filename, int *nchars)
#else
#define A1(NAM) \
int NAM (char *filename)
#endif
#if !(defined(__stdc__) || defined(__STDC__))
#define B2 \
FILE           *fopen (); \
char           *malloc ();
#else
#define B2  
#endif
#ifdef FORTRAN
#define A2(SUF) \
{ \
FILE           *temp; \
B2 \
 \
char           *tempfilename; \
int             ncharsp1, i; \
    ncharsp1 = *(nchars) + 1; \
    tempfilename = (char *) malloc (ncharsp1); \
    tempfilename[ncharsp1 - 1] = '\0'; \
    for (i = 0; i < *(nchars); i++) \
    { \
	tempfilename[i] = filename[i]; \
    } \
    temp = fopen (tempfilename, "w"); \
    if (temp == NULL) \
    { \
	fprintf (stderr, "libvplot.vpfile: cannot create %s\n", tempfilename); \
	exit (-1); \
    } \
    vp_pc ## SUF._pltout = temp; \
    return (vp_fixpc ## SUF()); \
}
#else /* FORTRAN */
#define A2(SUF) \
{ \
FILE           *temp; \
B2 \
    temp = fopen (filename, "w"); \
    if (temp == NULL) \
    { \
	fprintf (stderr, "libvplot.vpfile: cannot create %s\n", filename); \
	exit (-1); \
    } \
    vp_pc ## SUF._pltout = temp; \
    return (0); \
}
#endif

#ifdef FORTRAN
A1(vpfile_)
A2( )
A1(vpfileg_)
A2(_g)
#else
A1(vp_file)
A2( )
A1(vp_file_g)
A2(_g)
#endif

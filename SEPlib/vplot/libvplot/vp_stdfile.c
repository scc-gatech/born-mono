/*
 * Copyright 1990 the Board of Trustees of the Leland Stanford Junior
 * University. Official permission to use this software is included in
 * the documentation. It authorizes you to use this file for any
 * non-commercial purpose, provided that this copyright notice is not
 * removed and that any modifications made to this file are commented
 * and dated in the style of my example below.
 */

/*

  source file:   ./lvplot/vp_stdfile.c

  Phil Farrell - Stanford Earth Sciences - 21 Aug 1990
      Support routine for vpunit.f.  Not directly called by user.
      Used to assign FILE pointer for standard input, output, or error
      to vp_pc structure for use by other vplot routines.  Fortran
      caller passes a pointer to a character string containing one of
      "stdin", "stdout", or "stderr" to indicate which stream is to
      be used.  This routine then puts the appropriate FILE pointer 
      into vp_pc.

   Stewart A. Levin (SEP), 18 Feb 2012
      Double precision _g/g versions

  Input Arguments:
	filename - pointer to character string
        filenamelen - pointer to long integer giving length of 
		character string.  As this string is created by Fortran,
		it does NOT have a terminating null (\0), which must be
		added here.
*/

#ifdef FORTRAN
#include <stdio.h>
#include "../include/vplot.h"
#include "vp_pc.h"

#define A1(NAM) \
int NAM (char *filename ,long *filenamelen)
#define A2(SUF) \
{ \
    char           *tempfilename; \
    char           *malloc (size_t); \
    int             totallen ,i; \
/* \
    Allocate a new temporary character string, copy the input string \
    there, and add the needed terminating null character. \
*/ \
    totallen = *(filenamelen) + 1; \
    tempfilename = (char *) malloc (totallen); \
    tempfilename[totallen - 1] = '\0'; \
    for (i = 0; i < *(filenamelen); i++) \
    { \
	tempfilename[i] = filename[i]; \
    } \
/* \
    Now, test the filename for "stdin", "stdout", or "stderr" - anything \
    else is a fatal error.  Assign the correct pre-defined FILE pointer \
    to vp_pc. \
*/ \
    if (strcmp(tempfilename,"stdin") == 0) \
	vp_pc ## SUF._pltout = stdin; \
    else if (strcmp(tempfilename,"stdout") == 0) \
	vp_pc ## SUF._pltout = stdout; \
    else if (strcmp(tempfilename,"stderr") == 0) \
	vp_pc ## SUF._pltout = stderr; \
    else { \
        fprintf(stderr ,"vpstdfile:  invalid argument, cannot determine file"); \
        exit (-1); \
    } \
    return vp_fixpc ## SUF(); \
}
A1(vpstdfile_)
A2( )
A1(vpstdfileg_)
A2(_g)
#endif

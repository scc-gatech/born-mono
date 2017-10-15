/*$

=head1 NAME

plas -    (PLot ASsembler) deformat vector plot commands

=head1 SYNOPSIS

plas < asciifile > binaryfile


=head1  DESCRIPTION

        Reads stdin; writes stdout.  Complement to 'pldb'.
        Converts vplot-language files in ascii (human-readable) form
        into the hybrid ascii/binary form that the 'pen' filters understand.

   A backslash at the end of a line of text allows multi-line text strings.


=head1 OPTIONS

=over 4

=item      C<-v>: Use vplot units (the default)

=item      C<-i>: Use inches

=item      C<-c>: Use centimeters

=back

  Note that these options apply to ALL geometric attributes,
  including line width and character height scale factors.

The default may be set within the file by having as the first line
C<#plas: X> where X is one of V, C, or I for Vplot, Cm, or Inches

=head1 SEE ALSO

L<vplot>,L<pen>,L<pldb>

=head1 CATEGORY

B<graphics/vplot>

=cut

*/
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
 *  source file:   ./util/plas.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 *
 * Martin Karrenbach 11-3-92  ifdef CRAY
 *
 * Stewart A. Levin (Mobil), Feb 15 1993
 *     Support for SGI (sys-v) terminal handling
 *
 * Stewart A. Levin (Mobil)  May 8, 1996
 *     Added LINUX terminal handling support == SGI
 */

char           *documentation[] =
{
 "",
 "",
 "NAME",
 "		plas 	(PLot ASsembler) -- deformat vector plot commands",
 "SYNOPSIS",
 "		plas < asciifile > binaryfile",
 "------------------------------------------------------------------------",
 "	Reads stdin; writes stdout.  Complement to 'pldb'.",
 "	Converts vplot-language files in ascii (human-readable) form",
 "	into the hybrid ascii/binary form that the 'pen' filters understand.",
 "",
 "   A backslash at the end of a line of text allows multi-line text strings.",
 "Options",
 "      -v: Use vplot units (the default)",
 "      -i: Use inches",
 "      -c: Use centimeters",
 "  Note that these options apply to ALL geometric attributes,",
 "  including line width and character height scale factors.",
 "",
 "The default may be set within the file by having as the first line",
 "#plas: X",
 "where X is one of V, C, or I for Vplot, Cm, or Inches",
 "SEE ALSO",
 "	manual entries for vplot, pen;  self-documentation for pldb."
};
int             doclength =
{
 sizeof documentation / sizeof documentation[0]
};

#include        <sepConfig.h>
#include	<stdio.h>
#ifdef HAVE_STRING_H
#include 	<string.h>
#endif
#ifdef HAVE_SYS_IOCTL_H
#include	<sys/ioctl.h>
#endif
#ifdef HAVE_TERMIOS_H
#include	<termios.h>
#else
#ifdef HAVE_SYS_TERMIOS_H
#include 	<sys/termios.h>
#else
#ifdef HAVE_SYS_TTOLD_H
#include        <sys/ttold.h>
#else
#ifdef HAVE_SGTTY_H
#include	<sgtty.h>
#endif
#endif
#endif
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_CTYPE_H
#include	<ctype.h>
#endif
#include	"../../include/vplot.h"
#include	"../../filters/include/params.h"
#include	"../../filters/include/round.h"

#define MAXLINE 24*84
#define NL  '\n'

#if defined(HAVE_TERMIOS_H) || defined(HAVE_SYS_TERMIOS_H)
struct termios  ttystat;
#else
struct sgttyb   ttystat;
#endif
#if !(defined(__stdc__) || defined(__STDC__))
FILE           *
fopen (), *fdopen ();
#endif

#if defined(__stdc__) || defined(__STDC__)
static void text (void);
#endif

#if defined(__stdc__) || defined(__STDC__)
int
main (int argc, char *argv[])
#else
main (argc, argv)
    int             argc;
    char           *argv[];
#endif
{
int             sepxargc;
char          **sepxargv;
char           *cptr;
char            c, a;
int             npat, ix, iy, iz, npts, mtype, orient, col_tab_no, maskx, masky, col;
int             nmul, nx, ny, ipat, i, j;
float           x, y, x1, y1, msize, size, fat, red, green, blue, off, rep;
float           xmin, ymin, xmax, ymax;
int             piped_in;
char            line[MAXLINE];
float           scale, txvecscale, txscale, fatscale, colscale, hscale;
float           xcor, ycor, xvplot, yvplot;
int             count;
int             xpix, ypix, num_pat, num_byte;
int             ibyte, byte, num_rep,tempi;
int             ras_offset, ras_orient;
int             line_count;

    npat = 0;

    /*
     * If no arguments, and not in a pipeline, self document 
     */
    piped_in = ioctl ((fileno (stdin)),
#if defined(HAVE_TERMIOS_H) || defined(HAVE_SYS_TERMIOS_H)
#ifndef TCGETA
#define TCGETA TIOCGETA
#endif
	TCGETA,
#else
	TIOCGETP,
#endif
	&ttystat);
    if (argc == 1 && !piped_in)
    {
	for (i = 0; i < doclength; i++)
	    printf ("%s\n", documentation[i]);
#if defined(__stdc__) || defined(__STDC__)
	return (0);
#else
	exit (0);
#endif
    }

/*
 * Default to vplot units
 */
    fatscale = 1.0f;
    txvecscale = 1.0f;
    txscale = 1.0f;
    scale = 1.0f;
    colscale = 1.0f;
    hscale = 1.0f;

    sepxargc = argc;
    sepxargv = argv;
    for (sepxargc--, sepxargv++; sepxargc; sepxargc--, sepxargv++)
    {
	cptr = *sepxargv;
	if (*cptr == '-')
	{
	    while (*(++cptr))
	    {
		switch (*cptr)
		{
		case 'v':
/*
 * Vplot units
 */
		    fatscale = 1.0f;
		    txvecscale = 1.0f;
		    txscale = 1.0f;
		    scale = 1.0f;
		    colscale = 1.0f;
		    hscale = 1.0f;
		    break;
		case 'c':
/*
 * Centimeters
 */
		    fatscale = FATPERIN / 2.54f;
		    txvecscale = TEXTVECSCALE;
		    txscale = TXPERIN / 2.54f;
		    scale = RPERIN / 2.54f;
		    colscale = MAX_GUN;
		    hscale = RPERIN / 2.54f;
		    break;
		case 'i':
/*
 * Inches
 */
		    fatscale = FATPERIN;
		    txvecscale = TEXTVECSCALE;
		    txscale = TXPERIN;
		    scale = RPERIN;
		    colscale = MAX_GUN;
		    hscale = RPERIN;
		    break;
		default:
		    break;
		}
	    }
	}
    }

/* main switch loop	*/

    line_count = 0;
    while (fgets (line, MAXLINE, stdin) != NULL)
    {
	c = line[0];
	line_count++;
	switch (c)
	{
	case '#':
	    if (line_count == 1)
	    {
		if (strncmp (line, "#plas: ", 7) == 0)
		{
		    switch (line[7])
		    {
		    case 'V':
		    case 'v':
			fatscale = 1.0f;
			txvecscale = 1.0f;
			txscale = 1.0f;
			scale = 1.0f;
			colscale = 1.0f;
			hscale = 1.0f;
			break;
		    case 'I':
		    case 'i':
			fatscale = FATPERIN;
			txvecscale = TEXTVECSCALE;
			txscale = TXPERIN;
			scale = RPERIN;
			colscale = MAX_GUN;
			hscale = RPERIN;
			break;
		    case 'C':
		    case 'c':
			fatscale = FATPERIN / 2.54f;
			txvecscale = TEXTVECSCALE;
			txscale = TXPERIN / 2.54f;
			scale = RPERIN / 2.54f;
			colscale = MAX_GUN;
			hscale = RPERIN / 2.54f;
			break;
		    }
		}
	    }
	    break;
	case VP_MESSAGE:
	    putc (c, stdout);
	    text ();
	    break;
	case VP_SETSTYLE:
	    sscanf (line, "%*c %c", &a);
	    putc (c, stdout);
	    putc (a, stdout);
	    break;
	case VP_ERASE:
	case VP_PURGE:
	case VP_BREAK:
	case VP_NOOP:
	    putc (c, stdout);
	    break;
	case VP_TXALIGN:
	    putc (c, stdout);
	    sscanf (line, "%*c %d %d", &ix, &iy);
	    puth (ix, stdout);
	    puth (iy, stdout);
	    break;
	case VP_UORIGSCL:
	    putc (c, stdout);
	    sscanf (line, "%*c %f %f %f %f", &x, &y, &x1, &y1);
	    putf (x, stdout);
	    putf (y, stdout);
	    putf (x1, stdout);
	    putf (y1, stdout);
	    break;
	case VP_ORIGIN:
	case VP_MOVE:
	case VP_DRAW:
	    putc (c, stdout);
	    sscanf (line, "%*c %f %f", &x, &y);
	    puth (ROUND (x * scale), stdout);
	    puth (ROUND (y * scale), stdout);
	    break;
	case VP_TXFONTPREC:
	    putc (c, stdout);
	    sscanf (line, "%*c %d %d %d", &ix, &iy, &iz);
	    puth (ix, stdout);
	    puth (iy, stdout);
	    puth (iz, stdout);
	    break;
	case VP_PLINE:
	case VP_SETDASH:
	    putc (c, stdout);
	    sscanf (line, "%*c %d", &npts);
	    puth (npts, stdout);
	    while (npts--)
	    {
		if(NULL == fgets (line, MAXLINE, stdin))
                {
                   perror("VP_PLINE|VP_SETDASH");
                   exit(-1);
                }
		sscanf (line, "%f %f", &x, &y);
		puth (ROUND (x * scale), stdout);
		puth (ROUND (y * scale), stdout);
	    }
	    break;
	case VP_PMARK:
	    putc (c, stdout);
	    sscanf (line, "%*c %d %d %f", &npts, &mtype, &msize);
	    puth (npts, stdout);
	    puth (mtype, stdout);
	    puth (ROUND (msize * txscale), stdout);
	    while (npts--)
	    {
		if(NULL == fgets (line, MAXLINE, stdin))
                {
                   perror("VP_PMARK");
                   exit(-1);
                }
		sscanf (line, "%f %f", &x, &y);
		puth (ROUND (x * scale), stdout);
		puth (ROUND (y * scale), stdout);
	    }
	    break;
	case VP_BEGIN_GROUP:
	    putc (c, stdout);
	    text ();
	    break;
	case VP_END_GROUP:
	    putc (c, stdout);
	    break;
	case VP_OLDTEXT:
	case VP_TEXT:
	    putc ((char) VP_TEXT, stdout);
	    sscanf (line, "%*c %f %d", &size, &orient);
	    puth (ROUND (size * txscale), stdout);
	    if (c == VP_OLDTEXT)
		orient *= 90;
	    puth (orient, stdout);
	    text ();
	    break;
	case VP_GTEXT:
	    putc (c, stdout);
	    sscanf (line, "%*c %f %f %f %f", &x, &y, &xcor, &ycor);
	    puth (ROUND (x * scale * txvecscale), stdout);
	    puth (ROUND (y * scale * txvecscale), stdout);
	    puth (ROUND (xcor * scale * txvecscale), stdout);
	    puth (ROUND (ycor * scale * txvecscale), stdout);
	    text ();
	    break;
	case VP_COLOR:
	case VP_OVERLAY:
	    putc (c, stdout);
	    sscanf (line, "%*c %d", &ix);
	    puth (ix, stdout);
	    break;
	case VP_SET_COLOR_TABLE:
	    putc (c, stdout);
	    sscanf (line, "%*c %d %f %f %f",
		    &col_tab_no, &red, &green, &blue);
	    puth (col_tab_no, stdout);
	    puth (ROUND (red * colscale), stdout);
	    puth (ROUND (green * colscale), stdout);
	    puth (ROUND (blue * colscale), stdout);
	    break;
	case VP_FAT:
	    putc (c, stdout);
	    sscanf (line, "%*c %f", &fat);
	    puth (ROUND (fat * fatscale), stdout);
	    break;
	case VP_WINDOW:
	    putc (c, stdout);
	    sscanf (line, "%*c %f %f %f %f", &xmin, &ymin, &xmax, &ymax);
	    puth (ROUND (scale * xmin), stdout);
	    puth (ROUND (scale * ymin), stdout);
	    puth (ROUND (scale * xmax), stdout);
	    puth (ROUND (scale * ymax), stdout);
	    break;
	case VP_OLDAREA:
	    putc (c, stdout);
	    sscanf (line, "%*c %d", &npts);
	    if(NULL == fgets (line, MAXLINE, stdin)) 
            {
               perror("VP_OLDAREA (1)");
               exit(-1);
            }
	    sscanf (line, "%f %d %d", &fat, &maskx, &masky);
	    puth (npts, stdout);
	    puth (ROUND (fat * fatscale), stdout);
	    puth (maskx, stdout);
	    puth (masky, stdout);
	    for (i = 0; i < npts; i++)
	    {
	        if(NULL == fgets (line, MAXLINE, stdin)) 
                {
                   perror("VP_OLDAREA (2)");
                   exit(-1);
                }
		sscanf (line, "%f %f", &x, &y);
		puth (ROUND (x * scale), stdout);
		puth (ROUND (y * scale), stdout);
	    }
	    break;
	case VP_AREA:
	    putc (c, stdout);
	    sscanf (line, "%*c %d", &npts);
	    puth (npts, stdout);
	    for (i = 0; i < npts; i++)
	    {
	        if(NULL == fgets (line, MAXLINE, stdin)) 
                {
                   perror("VP_AREA");
                   exit(-1);
                }
		sscanf (line, "%f %f", &x, &y);
		puth (ROUND (scale * x), stdout);
		puth (ROUND (scale * y), stdout);
	    }
	    break;
	case VP_PATLOAD:
	    npat++;
	    ipat = 0;
	    sscanf (line, "%*c %d %d %d %d", &nmul, &nx, &ny, &ipat);
	    putc (c, stdout);
	    puth (nmul, stdout);
	    puth (nx, stdout);
	    puth (ny, stdout);
	    if (ipat == 0)
	    {
		ipat = npat;
	    }
	    puth (ipat, stdout);

	    if (nx != -1)
	    {
		for (i = 0; i < nx; i++)
		{
char           *ptr;
	            if(NULL == fgets (line, MAXLINE, stdin)) 
                    {
                       perror("VP_PATLOAD (1)");
                       exit(-1);
                    }

		    for (j = 0, ptr = line; j < ny; j++, *ptr++)
		    {
			if (*ptr == '\0' || *ptr == NL)
			    fprintf (stderr, "null/nl");
			if (*ptr >= '0' && *ptr <= '9')
			{
			    ipat = (*ptr) - '0';
			}
			else
			{
			    ipat = (*ptr) - 'A' + 10;
			}
			puth (ipat, stdout);
		    }
		}
	    }
	    else
	    {
		for (i = 0; i < ny * 2; i++)
		{
	            if(NULL == fgets (line, MAXLINE, stdin)) 
                    {
                       perror("VP_PATLOAD (2)");
                       exit(-1);
                    }
		    sscanf (line, "%f %d %f %f", &fat, &col, &off, &rep);
		    puth (ROUND (fat * fatscale), stdout);
		    puth (col, stdout);
		    puth (ROUND (off * hscale), stdout);
		    puth (ROUND (rep * hscale), stdout);
		}
	    }
	    break;
	case VP_BYTE_RASTER:
	case VP_SHORT_RASTER:
	case VP_BIT_RASTER:
	    {

		sscanf (line, "%*c %d %d", &ras_orient, &ras_offset);
		putc (c, stdout);
		puth (ras_orient, stdout);
		puth (ras_offset, stdout);
	        if(NULL == fgets (line, MAXLINE, stdin)) 
                {
                   perror("VP_*_RASTER (1)");
                   exit(-1);
                }
		sscanf (line, "%f %f", &xcor, &ycor);
		puth (ROUND (xcor * scale), stdout);
		puth (ROUND (ycor * scale), stdout);
	        if(NULL == fgets (line, MAXLINE, stdin)) 
                {
                   perror("VP_*_RASTER (2)");
                   exit(-1);
                }
		sscanf (line, "%f %f", &xvplot, &yvplot);
		puth (ROUND (scale * xvplot), stdout);
		puth (ROUND (scale * yvplot), stdout);
	        if(NULL == fgets (line, MAXLINE, stdin)) 
                {
                   perror("VP_*_RASTER (3)");
                   exit(-1);
                }
		sscanf (line, "%d %d", &xpix, &ypix);
		puth (xpix, stdout);
		puth (ypix, stdout);

		for (j = 0; j < ypix;)
		{
		    count = 0;
	            if(NULL == fgets (line, MAXLINE, stdin)) 
                    {
                       perror("VP_*_RASTER (4)");
                       exit(-1);
                    }
		    count = 0;
		    sscanf (line, "%d", &num_rep);
		    if (num_rep < 1)
			fprintf (stderr, "Bad Raster repetition factor\n");
		    puth (num_rep, stdout);
	    more_line:   if(NULL == fgets (line, MAXLINE, stdin) )
                    {
                           perror("VP_*_RASTER (5)");
                           exit(-1);
                    }
		    sscanf (line, "%d %d", &num_pat, &num_byte);
		    puth (num_pat, stdout);
		    puth (num_byte, stdout);
		    for (i = 0; i < num_byte; i++)
		    {
	                if(NULL == fgets (line, MAXLINE, stdin)) 
                        {
                           perror("VP_*_RASTER (6)");
                           exit(-1);
                        }
			sscanf (line, "%d", &byte);
			if (c == VP_BYTE_RASTER)
			{
			    putc ((char) byte, stdout);
			}
			else if (c == VP_SHORT_RASTER)
			{
          tempi=byte/256;
			    putc ((char) tempi, stdout);
          tempi=byte-tempi*256;
			    putc ((char) tempi, stdout);
			}
			else
			{
			    if (i % 8 == 0)
				ibyte = 0;
			    ibyte |= ((byte != 0) << (7 - (i % 8)));
			    if (i % 8 == 7 || i == num_byte - 1)
				putc ((char) ibyte, stdout);
			}
		    }
		    count += num_byte * num_pat;
		    if (count < xpix)
			goto more_line;
		    j += num_rep;
		}
	    }
	    break;
	default:
	    ;			/* Treat unknown charactors as comments */
	}
    }
#if defined(__stdc__) || defined(__STDC__)
    return (0);
#else
    exit (0);
#endif
}

static
#if defined(__stdc__) || defined(__STDC__)
void
text (void)
#else
text ()
#endif
{
char            c;
    do
    {
	c = (char) getc (stdin);
	if (c == '\\')
	{
	    if ((c = (char) getc (stdin)) != '\n')
	    {
		putc ('\\', stdout);
	    }
	    else
		continue;
	}
	else
	if (c == '\n')
	    break;
	putc (c, stdout);
    } while (1);
    putc ('\0', stdout);
}

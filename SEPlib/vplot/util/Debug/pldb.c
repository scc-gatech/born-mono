/*$

=head1 NAME

pldb -- (PLot DeBugger) 

=head1 SYNOPSIS
               pldb < binaryfile > asciifile


=head1 DESCRIPTION

        Reads the standard input stream, writes standard out

        Input:  vplot metafile format (mixed ASCII and binary)
        Output: Human-readable representations of vplot vector plot
                commands

        plas returns pldb output to vplot format.
        Useful with pen output filters.

=head1 OPTIONS

=over 4


=item      C<-v>: use vplot units (default)

=item      C<-i>: use inches

=item      C<-c>: use centimeters

=back

=head1 COMMENTS

  Note that these options apply to ALL geometric attributes,
  including line width and character height.

=head1 SEE ALSO


L<vplot>,L<pen>,L<plas>

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
 *  source file:   ./util/pldb.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Martin Karrenbach 11-3-92  ifdef CRAY
 *
 * Stew Levin (Mobil), May 8, 1996
 *      Added LINUX terminal handling support == SGI
 */

char           *documentation[] =
{
 "",
 "",
 "NAME",
 "		 pldb -- (PLot DeBugger) ",
 "SYNOPSIS",
 "		pldb < binaryfile > asciifile",
 "-------------------------------------------------------------------------",
 "	Reads the standard input stream, writes standard out",
 "",
 "	Input: 	vplot metafile format (mixed ASCII and binary)",
 "	Output:	Human-readable representations of vplot vector plot",
 "		commands",
 "	plas returns pldb output to vplot format.",
 "	Useful with pen output filters.",
 "OPTIONS:",
 "      -v: use vplot units (default)",
 "      -i: use inches",
 "      -c: use centimeters",
 "  Note that these options apply to ALL geometric attributes,",
 "  including line width and character height.",
 "SEE ALSO",
 "	 manual entries for 'vplot' and 'pen'; selfdoc for 'plas'."
};
int             doclength =
{
 sizeof documentation / sizeof documentation[0]
};

/*
 *  Various portions of this program were written by
 *  Rob Clayton (longer ago)
 *  Jon Claerbout (Long long ago!),
 *  Jeff Thorson (1980-81?),  Michel Debiche (1982-84),
 *  Chuck Karish (1985),  and Joe Dellinger (1986-1987)
 *  of the Stanford University
 *  Departments of Geophysics and Geology.
 *
 *  Anybody want to clean this mess up?
 */

#include        <sepConfig.h>
#include	<stdio.h>
#ifdef HAVE_STDLIB_H
#include        <stdlib.h>
#endif
#if defined(HAVE_TERMIOS_H) || defined(__APPLE__)
#include        <termios.h>
#else
#if defined(HAVE_SYS_TERMIOS_H)
#include	<sys/termios.h>
#else
#if defined(CRAY)
#include        <sys/ttold.h>
#else
#ifdef HAVE_SGTTY_H
#include        <sgtty.h>
#endif
#endif
#endif
#endif
#ifdef HAVE_SYS_IOCTL_H
#include	<sys/ioctl.h>
#endif
#include	<ctype.h>
#include	"../../include/vplot.h"
#include	"../../filters/include/params.h"

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
static void
text (void);
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
char            a;
int             c;
int             ix, iy, iz, npts, mtype, orient, col_tab_no, xmask, ymask, col;
int             nmul, nx, ny, ipat, i, j, ii, key, pos, bit;
float           x, y, x1, y1, msize, size, fat, red, green, blue, off, rep;
float           xmin, ymin, xmax, ymax;
int             piped_in;
float           scale, txvecscale, txscale, fatscale, colscale, hscale;
float           xcor, ycor, xvplot, yvplot;
int             xpix, ypix, num_pat, num_byte;
int             ibyte, byte, num_rep;
int             ras_offset, ras_orient;
int             which_units,tempi;
int             retstat = 0;

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
 * Default to vplots
 */
    fatscale = 1.0f;
    txvecscale = 1.0f;
    txscale = 1.0f;
    scale = 1.0f;
    colscale = 1.0f;
    hscale = 1.0f;
    which_units = 'v';

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
		    which_units = 'v';
		    break;
		case 'c':
/*
 * Centimeters
 */
		    fatscale = 1.0f / (FATPERIN / 2.54f);
		    txvecscale = 1.0f / TEXTVECSCALE;
		    txscale = 1.0f / (TXPERIN / 2.54f);
		    scale = 1.0f / (RPERIN / 2.54f);
		    colscale = 1.0f / MAX_GUN;
		    hscale = 1.0f / (RPERIN / 2.54f);
		    which_units = 'c';
		    break;
		case 'i':
/*
 * Inches
 */
		    fatscale = 1.0f / FATPERIN;
		    txvecscale = 1.0f / TEXTVECSCALE;
		    txscale = 1.0f / TXPERIN;
		    scale = 1.0f / RPERIN;
		    colscale = 1.0f / MAX_GUN;
		    hscale = 1.0f / RPERIN;
		    which_units = 'i';
		    break;
		default:
		    break;
		}
	    }
	}
    }

    switch (which_units)
    {
    case 'v':
	printf ("#plas: Vplot units used in this file\n");
	break;
    case 'c':
	printf ("#plas: Centimeters used in this file\n");
	break;
    case 'i':
	printf ("#plas: Inches used in this file\n");
	break;
    }

    /* main switch statement */

    while ((c = (char) getc (stdin)) != EOF)
    {
	switch (c)
	{
	case VP_MESSAGE:
	    printf ("%c\n", c);
	    text ();
	    break;
	case VP_SETSTYLE:
	    a = (char) getc (stdin);
	    printf ("%c %c\n", c, a);
	    break;
	case VP_ERASE:
	case VP_BREAK:
	case VP_PURGE:
	case VP_NOOP:
	    printf ("%c\n", c);
	    break;
	case VP_ORIGIN:
	case VP_MOVE:
	case VP_DRAW:
	    x = scale * geth (stdin);
	    y = scale * geth (stdin);
	    printf ("%c %g %g\n", c, x, y);
	    break;
        case VP_UORIGSCL:
            x = getf(stdin);
            y = getf(stdin);
            x1 = getf(stdin);
            y1 = getf(stdin);
	    printf ("%c %g %g %g %g\n", c, x, y, x1, y1);
	    break;
	case VP_TXALIGN:
	    ix = geth (stdin);
	    iy = geth (stdin);
	    printf ("%c %d %d\n", c, ix, iy);
	    break;
	case VP_TXFONTPREC:
	    ix = geth (stdin);
	    iy = geth (stdin);
	    iz = geth (stdin);
	    printf ("%c %d %d %d\n", c, ix, iy, iz);
	    break;
	case VP_PLINE:
	case VP_SETDASH:
	    npts = geth (stdin);
	    printf ("%c %d\n", c, npts);
	    while (npts--)
	    {
		x = scale * geth (stdin);
		y = scale * geth (stdin);
		printf ("%g %g\n", x, y);
	    }
	    break;
	case VP_PMARK:
	    npts = geth (stdin);
	    mtype = geth (stdin);
	    msize = txscale * geth (stdin);
	    printf ("%c %d %d %g\n", c, npts, mtype, msize);
	    while (npts--)
	    {
		x = scale * geth (stdin);
		y = scale * geth (stdin);
		printf ("%g %g\n", x, y);
	    }
	    break;
	case VP_BEGIN_GROUP:
	    printf ("%c\n", c);
	    text ();
	    break;
	case VP_END_GROUP:
	    printf ("%c\n", c);
	    break;
	case VP_GTEXT:
	    x = scale * txvecscale * geth (stdin);
	    y = scale * txvecscale * geth (stdin);
	    xcor = scale * txvecscale * geth (stdin);
	    ycor = scale * txvecscale * geth (stdin);
	    printf ("%c %g %g %g %g\n", c, x, y, xcor, ycor);
	    text ();
	    break;
	case VP_OLDTEXT:
	    key = geth (stdin);
	    size = txscale * (key & 037);
	    orient = (key & 0140) >> 5;
	    printf ("%c %g %d\n", VP_TEXT, size, 90 * orient);
	    text ();
	    break;
	case VP_TEXT:
	    size = txscale * geth (stdin);
	    orient = geth (stdin);
	    printf ("%c %g %d\n", c, size, orient);
	    text ();
	    break;
	case VP_OVERLAY:
	case VP_COLOR:
	    ix = geth (stdin);
	    printf ("%c %d\n", c, ix);
	    break;
	case VP_SET_COLOR_TABLE:
	    col_tab_no = geth (stdin);
	    red = colscale * geth (stdin);
	    green = colscale * geth (stdin);
	    blue = colscale * geth (stdin);
	    printf ("%c %d %g %g %g\n", c, col_tab_no, red, green, blue);
	    break;
	case VP_FAT:
	    fat = fatscale * geth (stdin);
	    printf ("%c %g\n", c, fat);
	    break;
	case VP_WINDOW:
	    xmin = scale * geth (stdin);
	    ymin = scale * geth (stdin);
	    xmax = scale * geth (stdin);
	    ymax = scale * geth (stdin);
	    printf ("%c %g %g %g %g\n", c, xmin, ymin, xmax, ymax);
	    break;
	case VP_BIT_RASTER:
	case VP_BYTE_RASTER:
	case VP_SHORT_RASTER:
	    ras_orient = geth (stdin);
	    ras_offset = geth (stdin);
	    printf ("%c %d %d\n", c, ras_orient, ras_offset);
	    xcor = scale * geth (stdin);
	    ycor = scale * geth (stdin);
	    printf ("%g %g\n", xcor, ycor);
	    xvplot = scale * geth (stdin);
	    yvplot = scale * geth (stdin);
	    printf ("%g %g\n", xvplot, yvplot);
	    xpix = geth (stdin);
	    ypix = geth (stdin);
	    printf ("%d %d\n", xpix, ypix);
	    for (i = 0; i < ypix; i += num_rep)
	    {
		pos = 0;
		num_rep = geth (stdin);
		printf ("    %d  ------ lines %d to %d\n",
			num_rep, i, i + num_rep - 1);
	new_line:num_pat = geth (stdin);
		num_byte = geth (stdin);
		printf ("%d %d  - start byte %d in y_line %d\n",
			num_pat, num_byte, pos, i);
		if (num_pat < 0 || num_byte < 0)
		{
		    fprintf (stderr, "Erro1 in raster field\n");
#if defined(__stdc__) || defined(__STDC__)
		    return (1);
#else
		    exit (1);
#endif
		}
		pos += num_byte * num_pat;
		if (pos > xpix)
		{
		    fprintf (stderr, "Erro2 in raster field\n");
#if defined(__stdc__) || defined(__STDC__)
		    return (1);
#else
		    exit (1);
#endif
		}

		if (c == VP_BYTE_RASTER)
		{
		    for (ii = 0; ii < num_byte; ii++)
		    {
			byte = (int) getc (stdin);
			if (byte == EOF)
			{
			    fprintf (stderr, "Erro3 in raster field\n");
#if defined(__stdc__) || defined(__STDC__)
			    return (1);
#else
			    exit (1);
#endif
			}
			printf (" %d\n", byte);
		    }
		}
		else if (c == VP_SHORT_RASTER)
		{
		    for (ii = 0; ii < num_byte; ii++)
		    {
			byte = (int) getc (stdin);
      tempi=byte*256;
			byte = (int) getc (stdin);
      tempi=tempi+byte;
			if (byte == EOF)
			{
			    fprintf (stderr, "Erro3 in raster field\n");
#if defined(__stdc__) || defined(__STDC__)
			    return(1);
#else
			    exit (1);
#endif
			}
			printf (" %d\n", tempi);
		    }

    }
		else
		{
		    for (ii = 0; ii < num_byte; ii++)
		    {
			if (ii % 8 == 0)
			{
			    ibyte = (int) getc (stdin);
			}
			else
			{
			    ibyte <<= 1;
			}
			printf ("%d\n", ((ibyte >> 7) & 001));
		    }
		}
		if (pos < xpix)
		    goto new_line;
	    }
	    break;
	case VP_AREA:
	    {
		npts = geth (stdin);
		printf ("%c %d\n", c, npts);
		for (i = 0; i < npts; i++)
		{
		    x = scale * geth (stdin);
		    y = scale * geth (stdin);
		    printf ("%g %g\n", x, y);
		}
	    }
	    break;
	case VP_PATLOAD:
	    nmul = geth (stdin);
	    nx = geth (stdin);
	    ny = geth (stdin);
	    ipat = geth (stdin);
	    printf ("%c %d %d %d %d\n",
		    c, nmul, nx, ny, ipat);
	    if (nx == -1)
	    {
		for (i = 0; i < ny * 2; i++)
		{
		    fat = fatscale * geth (stdin);
		    col = geth (stdin);
		    off = hscale * geth (stdin);
		    rep = hscale * geth (stdin);
		    printf ("%g %d %g %g\n",
			    fat, col, off, rep);
		}
	    }
	    else
	    {
		for (i = 0; i < nx; i++)
		{
		    for (j = 0; j < ny; j++)
		    {
			bit = geth (stdin);
			if (bit >= 0 && bit <= 9)
			{
			    putc (bit + '0', stdout);
			}
			else
			{
			    putc (bit + 'A' - 10, stdout);
			}
		    }
		    putc ('\n', stdout);
		}
	    }
	    break;
	case VP_OLDAREA:
	    {
		npts = geth (stdin);
		printf ("%c %d\n", c, npts);
		fat = fatscale * geth (stdin);
		xmask = geth (stdin);
		ymask = geth (stdin);
		printf ("%g %d %d\n", fat, xmask, ymask);
		for (i = 0; i < npts; i++)
		{
		    x = scale * geth (stdin);
		    y = scale * geth (stdin);
		    printf ("%g %g\n", x, y);
		}
	    }
	    break;
	default:
	/* Yalei put this line here to make it work on Kana */
	    printf ("******unknown command %c %o\n", c, c);
	    retstat = 1;
            break;
	}
    }
#if defined(__stdc__) || defined(__STDC__)
    return (retstat);
#else
    exit (retstat);
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

    while ((c = (char) getc (stdin)))
	printf ("%c", c);
    printf ("\n");
}
/*  $Id: pldb.c,v 1.2 2004/07/08 18:15:33 bob Exp $ */

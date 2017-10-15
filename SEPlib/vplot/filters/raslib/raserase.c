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
 *  source file:   ./filters/raslib/raserase.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Joe Dellinger (SEP), May 7, 1989
 *	RGB option added. Stew Levin's Color tektronix code modified and
 *	incorporated.
 * Joe Dellinger (SEP), August 5, 1989
 *	Fixed bug that caused only first page of GRF format output to
 *	actually print.
 * Dave Nichols (SEP) May 2 1992
 *      Allow VPLOTSPOOLDIR environment variable to override PEN_SPOOL
 * Dave Nichols (SEP) July 10 1992
 *      Added ppm output.
 */

/*
 * Erase the graphics area
 */
#include<sepConfig.h>

#include	<stdio.h>
#include        <string.h>
#include        <unistd.h>
#include	"../include/erasecom.h"
#include	"../include/enum.h"
#include	"../include/err.h"
#include	"../include/extern.h"
#include	"../include/params.h"
#include "raspen.h"

#if defined(__stdc__) || defined(__STDC__)
#include <stdlib.h>
#else
extern char    *getenv ();
#endif

/*#include <ppm.h>*/
#include "ppm.h"

void raserase (int command)
{

    switch (command)
    {
    case ERASE_START:
    default:
	break;
    case ERASE_MIDDLE:
    case ERASE_END:
/*
 * Output raster file, and then CLEAR it out (erase)
 */
	ras_write ();
	zap ();
	break;
    case ERASE_BREAK:
/*
 * Output raster file, but don't CLEAR it!
 */
	ras_write ();
	break;
    }
}

void ras_write (void)
{
static int      file_created = NO;
static int      counter = 0;
char            scratch_file[120];
char            system_call[120];
int             value;

    counter++;

    if (!grf_format && ! ppm_format )
    {
	if (write (fileno (pltout), image, dev_xmax * dev_ymax * esize) != dev_xmax * dev_ymax * esize)
	{
	    ERR (FATAL, name, "Can't write raster image %d\n", counter);
	}
    }
    else if ( ppm_format )
    {
	static int called=0;
	pixel * pixrow;
	unsigned char r,g,b;
	unsigned char *ptr;
	register pixel *pP;
	int row, col;

        if( called ) {
		return;
	}
        called=1;
	
	ppm_writeppminit( pltout, dev_xmax, dev_ymax, (pixval)255, 0);
	pixrow = ppm_allocrow( dev_xmax );
	for ( row = 0, ptr=image;  row < dev_ymax; ++row )
        {
        for ( col = 0, pP = pixrow; col < dev_xmax; ++col, ++pP ){
	    r = *(ptr++);
	    g = *(ptr++);
	    b = *(ptr++);
/*fprintf(stderr,"check this dork %d %d %d \n",(int)r,(int)g,(int)b);*/
	    PPM_ASSIGN( *pP, r, g, b );
	}
/*			fprintf(stderr,"going in %d \n",PPM_GETR(pixrow[0]));*/
/*        bbb_writeppmrow( pltout, pixrow, dev_xmax, (pixval)255, 0 );*/
/*        dpm_writeppmrow( pltout, pixrow, dev_xmax, (pixval)255, 0 );*/
        ppm_writeppmrow( pltout, pixrow, dev_xmax, (pixval)255, 0 );
        }
	pm_close( pltout );
    }
    else
    {
	if (default_out)
	{
  	    char *spooldirnm;

	    file_created = YES;
	    if( (spooldirnm = getenv("VPLOTSPOOLDIR")) != NULL ){
               sprintf (scratch_file, "%s%s%d%s", spooldirnm, "/Tcpr_", counter, "_XXXXXX");
            }else{
               sprintf (scratch_file, "%s%s%d%s", PEN_SPOOL, "/Tcpr_", counter, "_XXXXXX");
            }

	    if(-1 == mkstemp (scratch_file))
            {
                ERR (WARN, name, "mkstemp() failure on vplot spool scratch file.\n");
            }
	    pltout = fopen (scratch_file, "w");
	    if (pltout == NULL)
	    {
		ERR (FATAL, name, "could not open scratch file %s!",
		     scratch_file);
	    }
	}
	else
	{
	    strcpy (scratch_file, "(pipe)");
	}

	/* build image attributes header block */
	fprintf (pltout, "source_platform\tvplot\n");
	fprintf (pltout, "source_application\traspen\n");
	fprintf (pltout, "image_type\tSCREEN_DATA\n");
	fprintf (pltout, "horizontal_resolution\t%d\n",
		 (int) pixels_per_inch);
	fprintf (pltout, "vertical_resolution\t%d\n",
		 (int) (pixels_per_inch / aspect_ratio));
	fprintf (pltout, "color_units\tRGB\n");
	fprintf (pltout, "number_of_scans\t%d\n", dev_ymax);
	fprintf (pltout, "scan_length\t%d\n", dev_xmax);
	fprintf (pltout, "scan_direction\tLEFT_TO_RIGHT\n");
	fprintf (pltout, "scan_order\tTOP_TO_BOTTOM\n");
	fprintf (pltout, "bits_per_value\t8\n");
	fprintf (pltout, "value_field_width\t8\n");
/*
 * The documentation seems to be inconsistent in describing whether
 * pixel_field_width should be 8 or 24 in the esize=3 case. 24 seems to
 * work.
 */
	if (esize == 1)
	    fprintf (pltout, "pixel_field_width\t8\n");
	else
	    fprintf (pltout, "pixel_field_width\t24\n");

	fprintf (pltout, "packing_level\tNO_PACKING\n");
	if (esize == 3)
	    fprintf (pltout, "map\t0\n");
	else
	{
	    fprintf (pltout, "map\t%d\n", NCOLOR);
	    for (value = 0; value < NCOLOR; value++)
	    {
		if (color_table[value][0] != -1)
		{
		    fprintf (pltout, "%d\t%d\t%d\n", color_table[value][0],
			     color_table[value][1], color_table[value][2]);
		}
		else
		{
		    fprintf (pltout, "%d\t%d\t%d\n", value, value, value);
		}
	    }
	}
	if (white)
	{
	    fprintf (pltout, "invert_bw\tINVERT\n");
	}
	else
	{
	    fprintf (pltout, "invert_bw\tPASS\n");
	}

	fprintf (pltout, "header_end\n");
	fflush (pltout);

	/* send image data */
	if (write (fileno (pltout), image, dev_xmax * dev_ymax * esize) != dev_xmax * dev_ymax * esize)
	{
	    ERR (FATAL, name, "Can't write raster image #%d to \"%s\".\n", counter, scratch_file);
	}
	fflush (pltout);

	if (file_created)
	{
	    fclose (pltout);
	    sprintf (system_call, "lpr -Ptcpr -r %s\n", scratch_file);
	    if (0 != system (system_call))
	    {
		/* else figure we'll need to delete scratch file ourselves */
		ERR (WARN, name, "Couldn't get lpr to cooperate\n");
		unlink (scratch_file);
	    }
	    file_created = NO;
	}
    }
}

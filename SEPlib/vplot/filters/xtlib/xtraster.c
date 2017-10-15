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
 *  source file:   ./filters/xtlib/xtraster.c
 *
 * Steve Cole (SEP), August 4 1991
 *      Inserted this sample edit history entry.
 * Stewart A. Levin (Mobil), Feb 19, 1993
 *      Cast data argument in XCreateImage to (char *)
 * Dave Nichols (Schlumberger) Oct 10 1998
 *      Add support for 12/15/16/24/32 bit visuals
 * Joe Dellinger (BP Amoco) Oct 4 1999
 *	Added missing "name" argument to ERR calls.
 * Dave Nichols (Schlumberger) Nov 16 1990
 *      Fix support for 12/15/16/24/32 bit visuals
 */

#include "xtpen.h"
#define	MASK0 0200

/*ARGSUSED7*/
void
xtraster (int count, int out_of, int xpos, int ypos,
          int length, int orient, unsigned short *raster,
          int dummy1, int dummy2,int byte2)
{
int             i, j, k;
XImage	       *pen_image;
unsigned short   *rp, *re;
unsigned char  *rp2, *rp3;
static unsigned char *raster2;
static unsigned char *raster3;
static unsigned short *raster4;
static int      xloc, yloc;
static int      offset, incr1, incr2;
static int      width, height, widthpad;
unsigned short *iref;
int		format,xoffset,bitmap_pad,bytes_per_line;
unsigned int	depth,xwidth,xheight;
unsigned char     *tip;
int              do32;
int junk[16384];
unsigned long   xcol;




  

 

/*
 * The following comments were written to go along with the
 * SunView vplot filter "sunpen". I've left them here in the hope
 * that even if they are not completely relevant they will help
 * others understand the code.
 * - S. Cole, 3 Aug 91
 *
 * Comments to help understand the code for different orientations:
 * XCORD(xpos), YCORD(ypos) are the coordinates of the first point in
 * the first line of raster passed to this routine. For the different
 * orientations:         this point is this corner of the raster:
 *      0			upper left
 *      1			upper right
 *      2			lower right
 *      3			lower left
 * The Sunview routines pw_rop and pr_rop want the coordinates of the
 * upper left corner. Keeping in mind the order in which vplot fills
 * the raster (for orient 0, this is TV-style: from left to right and
 * top to bottom; orient 1 is this rotated 90 degrees clockwise,
 * orient 2 180 degrees, orient 3 270 degrees) the computations of
 * xloc, yloc, width, and height should be clear.
 * The mapping of pixels in the input raster to the new rotated raster
 * is done in core, because there is no easy way to do it in Sunview
 * (that I could find). offset is the location where the first pixel
 * of the first row belongs on output (relative to the start of the
 * array), incr1 is the change in output location as you move from one
 * pixel to the next in the input raster line, and incr2 is the change
 * in output address as you move from one line to the next on input.
 * Together, offset incr1 and incr2 provide a simple way to determine
 * the output address from the input address for any pixel.
 * - S. Cole
 */
    if (count == 0)
    {
	if (orient == 0)
	{
	    xloc = XCORD (xpos);
	    yloc = YCORD (ypos);
	    width = length;
	    height = out_of;
	    if (xmono)
		widthpad = ((width + 15) / 16) * 16;
	    else
		widthpad = ((width + 1) / 2) * 2;
	    offset = 0;
	    incr1 = 1;
	    incr2 = widthpad;
	}
	else
	if (orient == 1)
	{
	    xloc = XCORD (xpos) - out_of + 1;
	    yloc = YCORD (ypos);
	    width = out_of;
	    height = length;
	    if (xmono)
		widthpad = ((width + 15) / 16) * 16;
	    else
		widthpad = ((width + 1) / 2) * 2;
	    offset = width - 1;
	    incr1 = widthpad;
	    incr2 = -1;
	}
	else
	if (orient == 2)
	{
	    xloc = XCORD (xpos) - length + 1;
	    yloc = YCORD (ypos) - out_of + 1;
	    width = length;
	    height = out_of;
	    if (xmono)
		widthpad = ((width + 15) / 16) * 16;
	    else
		widthpad = ((width + 1) / 2) * 2;
	    offset = widthpad * height - 1 - widthpad + width;
	    incr1 = -1;
	    incr2 = -widthpad;
	}
	else
	if (orient == 3)
	{
	    xloc = XCORD (xpos);
	    yloc = YCORD (ypos) - length + 1;
	    width = out_of;
	    height = length;
	    if (xmono)
		widthpad = ((width + 15) / 16) * 16;
	    else
		widthpad = ((width + 1) / 2) * 2;
	    offset = widthpad * (height - 1);
	    incr1 = -widthpad;
	    incr2 = 1;
	}
	/* raster2 = (unsigned char *) calloc ((widthpad / 2) * 	
					     visual_depth * height / 8, 2); */
	raster3 = (unsigned char *) calloc ((widthpad / 2) * height, 2);
	raster4 = (unsigned short *) calloc ((widthpad ) * height, sizeof(unsigned short));

    }

    if (xmono)		/* Just store in correct order AS 2/2/89 */
    {
	rp3 = raster3 + offset + count * incr2;
	re = raster + length;
	for (rp = raster; rp < re; rp++, rp3 += incr1)
	{
	    /* if( *rp )
		*rp3 = screen_black;
	    else
		*rp3 = screen_white; */
	    *rp3 = (unsigned char) (!!(*rp)); 
	}
    }
    else if( visual_depth == 8 ) 
    {
        /* convert color rasters to 8 bit pixels in raster3 */
	rp3 = raster3 + offset + count * incr2;
	re = raster + length;
	for (rp = raster; rp < re; rp++, rp3 += incr1)
	{
	   *rp3 = map[*rp];
        }

    } 
    else
    {
        /* just keep an interpolated raster of the color indicies in raster3 */
	rp3 = raster3 + offset + count * incr2;
  iref= raster4 + offset + count *incr2;
	re = raster + length;
	for (rp = raster; rp < re; rp++, iref += incr1)
	{
	    *iref =(unsigned short) *rp;
	}
}

    if (count != out_of - 1)
	return;

    if (xmono)
    {
	/* Convert raster3 to a bitmap AS 2/2/89 */
	raster2 = (unsigned char *) calloc ((widthpad / 2) * height / 8, 2);
	rp2 = raster2;
	rp3 = raster3;
	for (i = 0; i < height; i++)
	{
	    for (j = 0; j < widthpad / 8; j++, rp2++)
	    {
		for (k = 7; k >= 0; k--, rp3++)
		{
		    (*rp2) = (unsigned char) ((*rp2) | ((*rp3) << k));
		}
	    }
	}
    	depth = 1;
   	format = XYBitmap;
   	xoffset = 0;
    	bitmap_pad = 1;
    	bytes_per_line = widthpad/8;
    	pen_image = XCreateImage(pen_display,pen_visual,depth,format,xoffset,
		(char *) raster2,width,height,bitmap_pad,bytes_per_line);

    } else if( visual_depth == 8 ){
	/* its easy to make 8 bit images, fast */
    	depth = 8;
   	format = ZPixmap;
   	xoffset = 0;
	xwidth = width;
	xheight = height;
    	bitmap_pad = 8;
    	bytes_per_line = widthpad;
    	pen_image = XCreateImage(pen_display,pen_visual,depth,format,xoffset,
		(char *) raster3,xwidth,xheight,bitmap_pad,bytes_per_line);
	if (!pen_image) ERR(FATAL, name, "couldn't create XImage!");


      }else if( visual_depth == 16 ||
	       visual_depth==12 ||
	       visual_depth == 15 ){
	/* convert raster3 to a 12/15/16 bit image in raster2 */
         raster2 = (unsigned char *) calloc((size_t) (2*width*height),1);
	 depth = visual_depth;
	 format = ZPixmap;
	 xoffset = 0;
	 xwidth = width;
	 xheight = height;
	 bitmap_pad = 16;
	 bytes_per_line = 0;
	 pen_image = XCreateImage(pen_display,pen_visual,depth,format,xoffset,
				  (char *)raster2,xwidth,xheight,bitmap_pad,
				  bytes_per_line);
	 if (!pen_image) ERR(FATAL, name, "couldn't create XImage!");
	 
	 if (visual_depth == 12 && pen_image->bits_per_pixel != 16) {
	   ERR(FATAL, name,
		"Can't handle visual_depth==12 && bits_per_pixel != 16 ");
	 }

	 rp3 = raster3;
   iref = raster4;
	
	 if (pen_image->byte_order == MSBFirst) {
	   
           for (i=0, rp2=raster2; i<height; i++) {
	     iref = raster4 + i*widthpad;
	     for (j=0, tip=rp2; j<width; j++, iref++) {
	       xcol = map[*iref];
	       *tip++ = xcol >>8 & 0xff;
	       *tip++ = xcol & 0xff;
	     }
	     rp2 += pen_image->bytes_per_line;
	   }
	   
	 } else {  /* LSBFirst */
	   
	   for (i=0, rp2=raster2; i<height; i++) {
	     rp3 = raster3 + i*widthpad;
	     iref = raster4 + i*widthpad;
	     for (j=0, tip=rp2; j<width; j++, iref++) {
	       xcol = map[*iref];
	       *tip++ = xcol & 0xff;
	       *tip++ = xcol >>8 & 0xff;
	     }
	     rp2 += pen_image->bytes_per_line;
	   }
	 }
       

    } else if( visual_depth == 24 || visual_depth == 32 ) {
        /* convert raster3 to a 24/32 bit image in raster2 */
         raster2 = (unsigned char *) calloc((size_t) (4*width*height),1);
    	depth = visual_depth;
   	format = ZPixmap;
   	xoffset = 0;
	xwidth = width;
	xheight = height;
    	bitmap_pad = 32;
    	bytes_per_line = 0;
    	pen_image = XCreateImage(pen_display,pen_visual,depth,format,xoffset,
		(char *)raster2,xwidth,xheight,bitmap_pad,bytes_per_line);
        if (!pen_image) ERR(FATAL, name, "couldn't create XImage!");
 
        do32 = (pen_image->bits_per_pixel == 32);
 
    for(i=0; i < 16384; i++) junk[i]=0;
 
        if (pen_image->byte_order == MSBFirst) {
	  
           for (i=0, rp2=raster2; i<height; i++) {
	      rp3 = raster3 + i*widthpad;
	      iref = raster4 + i*widthpad;
              for (j=0, tip=rp2; j<width; j++, iref++) {
                 xcol = map[*iref];
                 junk[*iref]++;
                 if (do32) *tip++ = 0;
                 *tip++ = (xcol>>16) & 0xff;
                 *tip++ = (xcol>>8) & 0xff;
                 *tip++ =  xcol & 0xff;
               }
               rp2 += pen_image->bytes_per_line;
            }

        } else {  /* LSBFirst */

            for (i=0, rp2=raster2; i<height; i++) {
	      rp3 = raster3 + i*widthpad;
	      iref = raster4 + i*widthpad;
              for (j=0, tip=rp2; j<width; j++, iref++) {
                xcol = map[(int)*iref];
                 junk[*iref]++;
                *tip++ =  xcol & 0xff;
                *tip++ = (xcol>>8) & 0xff;
                *tip++ = (xcol>>16) & 0xff;
                if (do32) *tip++ = 0;
              }
              rp2 += pen_image->bytes_per_line;
            }
        }
/*     j=0;*/
/*     for(i=0; i <2200 ; i++){*/
/*      fprintf(stderr,"color %d used %d times \n",i,junk[i]);*/
/*      if(junk[i] >0) j++;*/
/*      }*/
/*      fprintf(stderr,"TOTAL COLORS USED %d \n",j);*/

    } else {
       
	/* this is a hard case, we will do it the slow way using XPutPixel */
    	depth = visual_depth;
   	format = ZPixmap;
   	xoffset = 0;
	xwidth = width;
	xheight = height;
    	/*bitmap_pad = 8;*/
    	bitmap_pad = (visual_depth+7/8)*8;
    	bytes_per_line = 0;
	raster2 = (unsigned char *) calloc ((widthpad / 2) * 	
					     bitmap_pad/8 * height, 2); 
    	pen_image = XCreateImage(pen_display,pen_visual,depth,format,xoffset,
		(char *)raster2,xwidth,xheight,bitmap_pad,bytes_per_line);
        if (!pen_image) ERR(FATAL,name,"couldn't create XImage!");
	for (i = 0; i < height; i++) {
	  rp = (unsigned short*)(raster3 + widthpad*i);
	  for (j = 0; j < width; j++) {
	    XPutPixel( pen_image, j, i, map[*rp++] );
	  }
        }
     }

    XPutImage(pen_display,pen_drawable,pen_gc,pen_image,
		0,0,xloc,yloc,xwidth,xheight);
    XDestroyImage(pen_image);

    /* free up remaining work space */
    if (xmono || visual_depth != 8 ) {
	free(raster3 );
    }
    raster2 = raster3 = 0;
}

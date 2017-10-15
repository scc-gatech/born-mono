
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
 *  source file:   ./filters/xtlib/xtcolors.c
 *
 * Dave Nichols (SEP), December 14 1993
 *      Inserted this sample edit history entry.
 */
/*
 * Joe Dellinger (AMOCO), June 9 1995
 *	This code had a check in the xt_set_color_table routine
 *	which disallowed setting colors 0 through 7 to black.
 *	This was to get around a bug elsewhere in the code
 *	that has been fixed, so the check (and the bug it
 *	created) can now safely be removed.
 * Dave Nichols (Schlumberger) Oct 10 1998
 *      Add support for true color visuals
 * Dave Nichols (Schlumberger) Nov 16 1999
 *      Restore colormaps all at once (for speed)
 */

#include "xtpen.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>


#ifndef SEP_MAP_SIZE
#define SEP_MAP_SIZE 65536
#endif
unsigned long    color = 0;	/* current color */

/* black, blue, red, purple, green, cyan, yellow, white */
unsigned char   red[SEP_MAP_SIZE] =   {0, 0,   255, 255, 0,   0,   255, 255};
unsigned char   green[SEP_MAP_SIZE] = {0, 0,   0,   0,   255, 255, 255, 255};
unsigned char   blue[SEP_MAP_SIZE] =  {0, 255, 0,   255, 0,   255, 0,   255};
#ifndef UO_SEP
#define U0_SEP (~((unsigned long) 0))
#endif
unsigned long  map[SEP_MAP_SIZE]  ;


/* routine to find a colormap with at most maxcol and at least mincol
 * available color cells in it
 * It returns the number of cells available and fills in the entries in
 * the array "map".
 */

int xtnumcol( int mincol, int maxcol )
{
    Colormap colormap;
    unsigned int ncol ;
    int sucess;
    int i;
    unsigned long planeMask;

   for(i=0; i < SEP_MAP_SIZE; i++) map[i]=U0_SEP;


    /* try to allocate these cells in the current colormap */
    ncol = maxcol;
    colormap = pen_colormap;
    do {
      sucess = XAllocColorCells(pen_display,colormap,
			0,&planeMask, 0,map,ncol ) ;
      if( !sucess ) ncol = ncol/2;

    } while( !sucess && ncol >=mincol );


    if( ! sucess ){
        /*-- It didn't work so ... construct our own XColormap 
	     We copy the old one so that  we still get black and pwhite
	     defined OK --*/
          ERR( COMMENT,name,"allocated new colormap");
	  colormap = XCopyColormapAndFree( pen_display,pen_colormap);
         /* try to allocate these cells in the current colormap */
         ncol = maxcol;
         do{
           sucess = XAllocColorCells(pen_display,colormap,
			0,&planeMask, 0,map,ncol ) ;
           if( !sucess ) ncol = ncol/2;

         } while( !sucess && ncol >=mincol );
    }


    if( !sucess ) 
      ERR( FATAL,name,
	  "unable to obtain minimum number of cells in any colormap");
  
    pen_colormap = colormap; 
    return ncol; 
}


void xt_set_color( int col )
{
   if (mono) {
       if (col){
   	color = map[1];
       }else{
	   color = map[0];
       }
   }else{
	   color = map[col];
   }
   XSetForeground(pen_display, pen_gc, color);

}

void xt_set_color_table( int col, int ired, int igreen, int iblue )
{
XColor           pen_color;

    if ( mono) return;

    red[col] = (unsigned char) ired;
    green[col] = (unsigned char) igreen;
    blue[col] = (unsigned char) iblue;

    /* put the color in the colormap, we specify the pixel value from the
       values in the table "map".
     */
    
    pen_color.red = (unsigned short) (red[col] << 8);
    pen_color.green = (unsigned short) (green[col] << 8);
    pen_color.blue = (unsigned short) (blue[col] << 8);
    pen_color.flags = DoRed | DoGreen | DoBlue;

    if( xtruecol ) {
	/* True-Color we get told the pixel */
	 XAllocColor(pen_display,pen_colormap,&pen_color);
	 map[col] = pen_color.pixel;
	
    }else{
	/* Pesudo-Color we suppy the pixel */
        pen_color.pixel = map[col];
	XStoreColor(pen_display,pen_colormap,&pen_color);
    }
	
    if(col == 0) { /* change of background color */
    	XSetWindowBackground( pen_display, pen_window, map[0]);
    }
}

/* save the current colormap in the structure associated with this frame */
void xt_save_colormap( xtFrame *frame)
{
int i;

    if( mono ) return;

    for( i=0; i < num_col ; i++ ){
     frame->cmap.map[i] = map[i];
     frame->cmap.red[i] = red[i];
     frame->cmap.green[i] = green[i];
     frame->cmap.blue[i] = blue[i];
    }

}

/* restore the saved  current colormap from this frame */
void xt_restore_colormap( xtFrame *frame)
{
XColor*           pen_color;
int i;

    if( mono ) return;

    pen_color = (XColor*)XtMalloc( num_col*sizeof(XColor) );

    for( i=0; i < num_col ; i++ ){
        map[i] = frame->cmap.map[i];
        red[i] = frame->cmap.red[i] ;
        green[i] = frame->cmap.green[i];
        blue[i] = frame->cmap.blue[i];
	if( ! xtruecol ) {
           pen_color[i].red = (unsigned short) (frame->cmap.red[i] << 8);
           pen_color[i].green = (unsigned short) (frame->cmap.green[i] << 8);
           pen_color[i].blue = (unsigned short) (frame->cmap.blue[i] << 8);
           pen_color[i].pixel = frame->cmap.map[i];
           pen_color[i].flags = DoRed | DoGreen | DoBlue;
	}
    }

    if( ! xtruecol ) {
       XStoreColors(pen_display,pen_colormap,pen_color, num_col);
    }
	
    XSetWindowBackground( pen_display, pen_window, map[0]);

    XtFree((void*) pen_color );
}

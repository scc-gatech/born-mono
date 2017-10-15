
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
 *  source file:   ./filters/xtlib/xtpixmap.c
 *
 * Steve Cole (SEP), August 4 1991
 *      Inserted this sample edit history entry.
 * Dave Nicols (SEP), February 14 1992
 *      Rewrote using new xtFrame interface that only plots one
 *      frame at a time.
 * Dave Nichols (SEP), April 30 1992
 *      Final version of the new xtpen, added message window
 * Dave Nichols (SEP), May 6 1992
 *      see_progress controls offscreen pixmap.
 */

#include "xtpen.h"

int pen_width = -1;
int pen_height = -1;
int have_pixmap;

/* draw a filled rectangle on the pixmap to clear it */
void clear_pixmap(void){
    XSetForeground(pen_display, pen_gc, map[0] );
    XFillRectangle(pen_display, pen_pixmap, pen_gc,
		0,0,pen_width,pen_height);
    XSetForeground(pen_display, pen_gc, color );
}

void remove_pixmap(void){
    if( pen_pixmap != (Pixmap)0 ) {
        XFreePixmap(pen_display,pen_pixmap);
	pen_pixmap = (Pixmap)0;
    }
}

/* this is the pixmap we draw onto, it should only be created when we
   first create a plot or when it is resized */

void  create_pixmap( int width, int height )
{

    /* Make sure we have a pixmap of the correct size */

    if( pen_width == width && pen_height== height ) { return; }

    remove_pixmap();

    xt_clear_images(frame_list);

    if(  see_progress ){ /* we don't want to draw to an offscreen pixmap */
    	pen_height = height; pen_width = width;
	have_pixmap = NO;
	pen_drawable = pen_window;
	return;
    }

    pen_pixmap = MyCreatePixmap(pen_display,pen_window,
                      width,height,
		      visual_depth );

    /* check if pixmap creation succeeded */
    if( pen_pixmap != (Pixmap)0 ) {
    	pen_height = height; pen_width = width;
    	clear_pixmap();
    	have_pixmap = YES;
    	pen_drawable = pen_pixmap;
    }else{
    	pen_height = height; pen_width = width;
	have_pixmap = NO;
	pen_drawable = pen_window;
    }

}

/* pixmap allocation with an error handler to detect failed allocation */

static int attachfailed;
 
/*ARGSUSED*/
static int MyHandler(Display *dpy, XErrorEvent *errorevent)
{
  attachfailed = 1;
  return(0);
}

Pixmap MyCreatePixmap(
Display *display,
Drawable drawable,
unsigned int width,
unsigned int height,
unsigned int depth
                     )
{
Pixmap pix;

XErrorHandler handler;
XSync(display,0);
attachfailed  = 0;

handler = XSetErrorHandler(MyHandler);      /* Start critical time */
pix = XCreatePixmap(display,drawable, width,height, depth );
XSync(display,0);
(void) XSetErrorHandler(handler);           /* Critical time over */

if( attachfailed ) return (Pixmap)0; 
else return pix;

}

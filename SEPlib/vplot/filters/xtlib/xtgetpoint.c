
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
 *  source file:   ./filters/xtlib/xtgetpoint.c
 *
 * Steve Cole (SEP), August 4 1991
 *      Inserted this sample edit history entry.
 */

#include "xtpen.h" 

#include <X11/cursorfont.h>

static Cursor crossCursor = (Cursor)0;
static Cursor pointerCursor = (Cursor)0;
static Cursor currentCursor = (Cursor)-1;

static void cross_cursor(void)
{

    if( crossCursor == (Cursor)0 ){
   	crossCursor =  XCreateFontCursor(pen_display,XC_cross);
    }
    XDefineCursor( pen_display, pen_window, crossCursor );
    currentCursor = crossCursor;
    XFlush(pen_display);
}
	
static void pointer_cursor(void)
{
    if( pointerCursor == (Cursor)0 ){
   	pointerCursor =  XCreateFontCursor(pen_display,XC_left_ptr);
    }
    XDefineCursor( pen_display, pen_window, pointerCursor );
    currentCursor = pointerCursor;
    XFlush(pen_display);
}

int xtgetpoint (int *x, int *y)
{
    XEvent event;

    if( currentCursor != crossCursor ) cross_cursor();
    XSync (pen_display, True);
    XSelectInput (pen_display, pen_window, ButtonPressMask);
    XNextEvent(pen_display,&event);
    XSelectInput (pen_display, pen_window, 0);
    if( event.xbutton.button == Button1 ){
        *x = XCORD (event.xbutton.x);
        *y = YCORD (event.xbutton.y);
        return (0);
    }else{
	pointer_cursor();
	return(1);
    }
}

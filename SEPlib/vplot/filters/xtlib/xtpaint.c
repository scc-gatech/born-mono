/*
 * Copyright 1987 the Board of Trustees of the Leland Stanford Junior
 * University. Official permission to use this software is included in the
 * documentation. It authorizes you to use this file for any non-commercial
 * purpose, provided that this copyright notice is not removed and that any
 * modifications made to this file are commented and dated in the style of my
 * example below. 
 */

/* 
 * source file:   ./filters/xtlib/xtpaint.c 
 *
 * Steve Cole (SEP), August 4 1991
 *      Inserted this sample edit history entry.
 *
 * Dave Nicols (SEP), February 14 1992
 *      Rewrote using new xtFrame interface that only plots one
 *      frame at a time.
 * Dave Nichols (SEP), April 30 1992
 *      Final version of the new xtpen, added message window
 * Dave Nichols (SEP), Nov 19 1993
 *      Added proper handling of repaints that involve breaks.
 *      Frame structure now contains info on how frame is terminated.
 */

#include	"xtpen.h"

#include	"../include/intcom.h"
#include	"../include/extern.h"

int             in_repaint = NO;

int xt_endframe = NO;
 
/* draw one frame from the file stream, epause is set to -1 to force
 * a USER_INT command at the end of the frame.
 * this will cause dovplot to exit at the end of the frame.
 */
void xt_draw_file( int start_file, long start_pos, int *end_file, long *end_pos , long *total_len )
/*int start_file;    index of start file in the array of input files */
/*long start_pos;    offset in the file to start reading the plot commands */
/*int *end_file;     index of the file we finished reading this frame */
/*int *end_pos;      position at which we ended reading this frame */
/*long *total_len;   the total length of vplot data for this frame */
{

    int save_pause;
    int file_num;
    long file_pos;

    file_num = start_file;
    file_pos = start_pos;

    save_pause = epause;
    epause = -1;
    xt_endframe=0;
    reset_parameters ();


    *total_len =0;

    do {

        pltin = inFiles[file_num] .stream;
        strcpy (pltname, inFiles[file_num].name);

	if( start_pos==0 && inFiles[file_num].new ){
            inFiles[file_num].new = NO;
    	    dovplot ();
	}else{
            inFiles[file_num].new = NO;

	    if( start_pos < -1 ){
	         /* start_pos will be < -1 for a pipe input */
		if( *end_pos < -1 ){
		  /* this is a replot request */
                  ERR (WARN, name, "sorry, unable to draw frame.");
	          xt_endframe = YES ;
		}else{
		  /* just do it! */
    	          dovplot ();
		}
	    }else{
	        /* this is for a file input */
                if ( !fseek (pltin, file_pos, 0)) {
    	            dovplot ();
                }else{
                    ERR (WARN, name, "sorry, unable to draw frame.");
	            xt_endframe = YES ;
                }
            }
	}

	/* find the ending position */
        *end_pos = ftell( inFiles[file_num].stream );

	if( *end_pos == -1 ){ 
	  /* when reading from a pipe */
	  *end_pos = start_pos - 2;
	  *total_len += 99999; /* fake it to make it save images */
        }else{
	  /* accumalate a length measure */
	  *total_len += *end_pos - file_pos;
	}
	

        if( feof(inFiles[file_num].stream) ){
	    *end_pos = -1;
	}

	/* now see if this is the end of a frame. If it isn't it means that
         * dovplot returned but the frame has not ended 
         * so we should keep plotting */
	if( ! xt_endframe ){
	    if( *end_pos == -1 ){
	        /* go on to the next file */
	        file_num++; file_pos = 0;
	    }else{
		/* keep plotting from this position */
		file_pos = *end_pos;
	    }

	    /* check to see if we have reached the end of all the files */
	    if( file_num == num_files ) {
		file_num--; xt_endframe = YES ;
	    }
	}

        skipit = YES; /* force skiping of pause at start of next dovplot */

    }while( ! xt_endframe );

    *end_file = file_num;


    epause = (float) save_pause;

}


void xt_draw_frame( xtFrame *frame )
{
int end_file;
long end_pos;
long total_len;



   if( frame->has_image ){
       xt_put_image( frame );
       end_pos = frame->end_pos;

   }else{
       if( frame->prev != 0 ){
	if( frame->prev->break_end ) xt_after_break = 1;
       }
       end_pos = frame->end_pos;
       xt_draw_file(frame->file_num, frame->file_position, &end_file, 
			&end_pos, &total_len);
       frame->total_len = total_len;
       frame->end_file = end_file;
       frame->end_pos = end_pos;
       if( xt_after_break ) frame->break_end = 1;
       if( want_images || greedy_pixmaps ) xt_store_image( frame );

   }

   xt_flush_display();

}

/* paint a file, get the window size, allocate a pixmap and call xt_draw_file */
/* save the current file info because this may not be the one we are redrawing*/
/* dovplot can be confused if we change the file pointers in midstream */

void xtredraw (void)
{

    static FILE     *save_pltin;
    static char     save_pltname[120];
    Dimension width, height;
    xtFrame* replot_frame;
 

    xt_size_n_scale(&width,&height);

    inactivate_buttons();
  
    /* save the current file info */
    /* we might be about to call dovplot to repaint the previous file */
    save_pltin = pltin;
    strcpy (save_pltname, pltname);

    /* to handle breaks correctly we need to backup redplot_frame
     * to the last frame that started with an erase (or the first frame)*/
    replot_frame = redraw_frame ;
    do{
	if( replot_frame->prev == 0 ) break;
	if( !replot_frame->prev->break_end ) break;
	replot_frame = replot_frame->prev;
    }while( replot_frame->frame_num != 0 );
	

    do{	
        xt_draw_frame( replot_frame );
	replot_frame = replot_frame->next;
    }while( replot_frame != redraw_frame->next );

    /* restore file info */
    strcpy (pltname, save_pltname);
    pltin = save_pltin;

    activate_buttons();

}


/*
 * repaint plot by copying in-memory pixmap to canvas
 */
/*ARGSUSED1*/
void
PenRepaint (Widget w, XEvent *event, String *params, Cardinal *num_params)
{
Dimension width, height;
Position left, top;
#ifdef notdef
XExposeEvent *exposeEv = (XExposeEvent *) event;
#endif


    XtVaGetValues(pen_picture,XtNwidth,&width,XtNheight,&height,
		XtNx,&left,XtNy,&top,NULL);


    if( have_pixmap && (width == pen_width )&& (height == pen_height)){
	/* lazy man's way! copy the whole pixmap */
        XSetClipMask( pen_display, pen_gc, None );
        XCopyArea(pen_display, pen_pixmap, pen_window, pen_gc, 
		  0, 0, width, height, 0, 0);
        XFlush(pen_display);
    }else{
        if( in_repaint ) return;
        in_repaint = YES;
    	xtredraw() ;
        in_repaint = NO;
    }

}

void xt_flush_display(void)
{
    if( have_pixmap ){
        /* copy the pixmap to the screen */
        XSetClipMask( pen_display, pen_gc, None );
        if( pen_pixmap != (Pixmap)0) XCopyArea(pen_display,pen_pixmap,
		        pen_window,pen_gc,
                        0,0,pen_width,pen_height,0 ,0);
    }
    XFlush(pen_display);
}

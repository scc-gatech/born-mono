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
 *  source file:   ./filters/xtlib/xt_dovplot.c
 *
 * Steve Cole (SEP), August 4 1991
 *      Inserted this sample edit history entry.
 *
 * Dave Nichols (SEP), February 14 1992
 *      Rewrote control structure, dovplot is now invoked only when
 *      needed and on a per-frame basis.
 * Dave Nichols (SEP), April 30 1992
 *      Final version of the new xtpen, added message window.
 * Dave Nichols (SEP), April 12 1993
 *      Moved xt_size_n_scale into this routine so that user_scales are
 *      set before it is called.
 * Dave Nichols (SEP), April 13 1993
 *      Use app_data to get defaults from resource database
 */

#include	"xtpen.h"
#include        "xtcommands.h"
#include        "../include/intcom.h"
#include        "../include/extern.h"

#ifdef SEP
#define GETPAR getch
extern int getch(const char *, const char *, MIXED);
#else
#define GETPAR getpar
extern int getpar(char *, char *, MIXED);
#endif

FileInfo       *inFiles;
int		num_files;

xtFrame *redraw_frame=0;
xtFrameList* frame_list; /* the list of frames to plot */

float fpause;

int
xt_dovplot (
    int             nn,
    FILE          **inpltin,
    char            innames[][MAXFLEN + 1]
           )
{
    int i;
    int draw_file, draw_pos;
    int next_file, next_pos;
    int what_next;
    int pause;
    xtFrame* draw_frame;
    xtFrame* next_frame;
    int doNEXT,doPREV,doRESTART,doQUIT,doRUN,doSTOP,doSIZE;
    int running;
    int going_forwards;
    Dimension width,height;
    MIXED vartemp;

    if (nn == 0)
	return 0;

    xt_size_n_scale( &width,&height);


    num_files = nn;

    going_forwards = YES;

    /* allocate space for array inFiles[] */
    inFiles = (FileInfo *) malloc (nn * sizeof (FileInfo));

    /* fill array inFiles */
    for (i = 0; i < nn; i++)
    {
	inFiles[i] .stream = inpltin[i];
	strcpy (inFiles[i] .name, innames[i]);
	inFiles[i] .new = YES;
    }

    /* ready to go! Generate an expose event */
    XFlush(pen_display);


    /* loop over plotting the files until the quit button is pressed */

    /* plot one frame at a time (forced by setting epause to -1 in
       xt_draw_frame, dovplot will return when a INT_PAUSE or
       INT_USER_PAUSE or end of file is found. 
       At this point we record the current frame and start a new frame 
       at the current position of the input stream */

    /* There are two modes for plotting frames:
     *  1) running mode  ( default mode if epause >= 0 )
     *  2) stopping mode ( default mode if epause < 0 ) stops after every frame
     */
    pause=1;
    vartemp.f = &epause;
    if( !GETPAR( "pause","f",vartemp) ){ epause=app_data.pause;pause=0;}

    running = ( epause >= 0.0f ); if( epause <0.0f ) epause=0.0f;

    fpause = (float)epause; /* initial pause is the user's epause */
    if(pause==0){
      vartemp.f = &fpause;
      if( !GETPAR( "fpause","f",vartemp) ){ 
        if( fpause == 0.0f ) fpause=.05f;
      }
    }
    set_delay_label(fpause);

    /* frame_list is a circular list of the frames so far */
    frame_list = xt_new_list( (xtFrameList*)0, (int*)0, 0 );

    /* start at the beginning of the first file */
    draw_file = 0; draw_pos = 0;
    draw_frame = xt_frame( frame_list, draw_file, draw_pos );

    /* Once this is set the Expose function actually does something */
    plotting_started=1;

    do{

	redraw_frame = draw_frame;

	xt_draw_frame( draw_frame ); /* draw the frame */

	set_frame_label( draw_frame->frame_num ); /*set the frame number label*/	/* keep it in sync every 10 frames */
	if( draw_frame->frame_num % 10 == 0 ){
            XSync(pen_display,0);
	}else{
            XFlush(pen_display);
	}


	if( draw_frame->end_pos == -1 ){  
	    /* end of file we must update next_file */
	    next_file = draw_frame->end_file + 1;
	    next_pos = 0;
	    if( next_file == nn ) {
		 next_file = -1; /* at the end */

	         /* check for case where we only have one frame */
	         if( frame_list->num_frame == 1 ) running=NO;
	    }

	}else{ /* still in the same file */
	    next_file = draw_frame->end_file;
	    next_pos = draw_frame->end_pos;
	}


    	/* do any pausing and/or button reading */

	if( running ){ 

	   
	    /* run in a loop, only the stop & quit buttons are valid */
	    doNEXT = doPREV = doRESTART  = doRUN = doSIZE = NO;
	    doQUIT = doSTOP = YES; 

	    what_next = xt_pause(doNEXT,doPREV,doRESTART,
					doQUIT,doRUN,doSTOP,doSIZE);
	     
            /* what to do if the timeout occurs */
	    if( what_next == TIME ){
	        switch( xt_run_mode ){
		    case XT_FORWARD:
		         what_next = NEXT;
			 break;
		    case XT_BACKWARD:
			 what_next = PREV;
			 break;
		    case XT_BOTH_WAYS:

	                if( draw_frame == xt_last_frame(frame_list) ){
			     going_forwards = NO;
			}

	                if( draw_frame == xt_first_frame(frame_list) ){
			     going_forwards = YES;
			}
 		
		        if( going_forwards ){
			    what_next = NEXT;
			}else{
			    what_next = PREV;
			}		    
		        break;
		}
	    }
	  
	}else{ 
	    /* pause for user interaction */
	    doRESTART = doQUIT = doRUN = doSIZE = YES;
            doPREV = (frame_list->num_frame != 1 );
	    doNEXT = !(frame_list->num_frame == 1 && next_file == -1 );
	    doSTOP = NO;
	    what_next =  xt_pause(doNEXT,doPREV,doRESTART,
	     				doQUIT,doRUN,doSTOP,doSIZE);
	 
	}


	/* act on the result of the pause */
	switch(what_next){
	  case QUIT:
	    draw_file = -1;
	    break;

	  case STOP:
	    running = NO;
	    break;

	  case RUN:
	    running = YES;
	    break;
	    
	  case NEXT:

	    /* see if the user typed a frame number */
	    if( xt_next_num != -1  && 
		(next_frame=xt_frame_num(frame_list,xt_next_num)) != 0  ){
		draw_frame = next_frame;
		draw_file = draw_frame->file_num;
		draw_pos = draw_frame->file_position;
		xt_next_num = -1;

	    }else{

		/* they didn't type anything or it was an invalid number */
	        if( (draw_frame == xt_last_frame(frame_list)) 
			&& next_file != -1 ){ 
		    /* this is currently our last frame,,
	             * but we are not at the end of the input.
		     * So get next frame to plot */
		    draw_file = next_file; draw_pos = next_pos;
		    draw_frame = xt_frame( frame_list, draw_file, draw_pos ); 

	        }else{
		    /* we already know what the next frame is */
		    draw_frame = draw_frame->next;
		    draw_file = draw_frame->file_num;
		    draw_pos = draw_frame->file_position;
	        }
	    }

	    break;
				
	  case PREV:
	    /* get previous frame to plot */
	    draw_frame = draw_frame->prev;
	    draw_file = draw_frame->file_num;
	    draw_pos = draw_frame->file_position;
	    break;

	  case CHANGED:
	    /* something changed in the plotting parameters
   	     * flush all cached images and replot the current frame */
	    xt_clear_images(frame_list);
	    xtredraw();
	    break;

	  case RESTART:
	    /* go back to the first frame */
	    draw_frame = xt_first_frame(frame_list); 
	    draw_file = draw_frame->file_num;
	    draw_pos = draw_frame->file_position;
	    break;
	}


    }while( draw_file != -1 );

    /* clean up */
    xt_clear_images(frame_list);
    remove_pixmap();
	
    return 0; /*DOVPLOT_CONT*/
}


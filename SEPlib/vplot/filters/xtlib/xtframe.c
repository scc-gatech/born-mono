
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
 *  source file:   ./filters/xtlib/xtframe.c
 *
 * Dave Nichols (SEP), February 12 1992
 *      Inserted this sample edit history entry.
 * Dave Nichols (SEP), April 30 1992
 *      Final version of the new xtpen, added message window
 * Dave Nichols (SEP), Nov 17 1993
 *      Frame structure now contains info on how frame is terminated.
 * Dave Nichols (SEP), Dec 14 1993
 *      Frame structure now contains colormap info.
 */
/*
 * Maintain a circular list of frame info
 *
 * The info includes the file_name, file_position, frame_number, and image.
 *
 * Given a redraw_file,redraw_position pair we can then return a valid image.
 */

#include "xtpen.h"


xtFrameList* xt_new_list(
xtFrameList* old_list,
int *frames,
int num_frame
                        )
{
   xtFrameList* new_list;

   new_list = (xtFrameList*) malloc(sizeof(xtFrameList));
   
   if( old_list == 0 ){
       new_list->start=0;
       new_list->end =0;
       new_list->parent=0;
       new_list->num_frame=0;
       
   }else{
       int i;
       for( i=0; i<num_frame; i++ ){
	   xt_add_frame( new_list, xt_frame_num( old_list, frames[i] ) );
       }
       new_list->parent = old_list;
   }
	
   return new_list;
}   
       

/* constructor for an new xtFrame object */       
xtFrame* xt_new_frame( int filenum, long filepos )
{
   xtFrame *newfr;

   newfr = (xtFrame*)malloc(sizeof(xtFrame));
   newfr->file_num = filenum;
   newfr->file_position = filepos;
   newfr->end_pos = 0;
   newfr->total_len = 0;
   newfr->frame_num = 0;
   newfr->break_end = 0;
   newfr->has_image = 0; 
   newfr->image = 0;
   newfr->pixmap = 0;
   newfr->next =  0;
   newfr->prev =  0;

   return newfr;
}

void xt_add_frame( xtFrameList *list, xtFrame *frame )
{

xtFrame *end; 
   
   frame->frame_num = (list->num_frame)++;

   /* list->start is the first fame in the circulary linked list
     and list->start->prev is the last */

   if( list->start == 0 ){
       list->start = frame;
       frame->prev = frame;
       frame->next = frame;
   }else{
       end = list->start->prev;
       end->next = frame;
       frame->prev = end;
       list->start->prev = frame;
       frame->next = list->start;
   }

}

xtFrame* xt_frame_num( xtFrameList *list, int num )
{
xtFrame *curr, *start, *found;

if( list==0 || list->start == 0 ) return (xtFrame*) 0;

found = (xtFrame*)0;
start = curr = list->start;

do{
  if( num == curr->frame_num ) found=curr;
  curr = curr->next;
}while( (found==(xtFrame*)0) && curr != start );

return found;
}
 
xtFrame* xt_find_frame(
xtFrameList* list,
int filenum,
long filepos
                      )
{
xtFrame *curr, *start;
xtFrame *found;

if( list==0 || list->start == 0 ) return (xtFrame*) 0;

found = (xtFrame*)0;
start = curr = list->start;

do{
  if( filenum == curr->file_num && filepos == curr->file_position) found=curr;
  curr = curr->next;
}while( (found==(xtFrame*)0 ) && curr != start );

return found;

}


xtFrame*  xt_frame(
xtFrameList* list,
int filenum,
long filepos
                  )
{
xtFrame *curr;
curr = xt_find_frame( list, filenum, filepos );

if( curr == 0 ) {
    curr = xt_new_frame( filenum, filepos );
    xt_add_frame( list, curr );
}

return ( curr );

}

void xt_store_image( xtFrame* fram)
{
    fram->has_image = 0;

/* get the image from the drawable and
 * copy it to either a Pixmap on the server or an XImage on the client
 */

    if( greedy_pixmaps ){

        if( fram->pixmap != (Pixmap)0 ) {
	   XFreePixmap(pen_display,fram->pixmap );
        }
        fram->pixmap = (Pixmap)0;

        fram->pixmap = MyCreatePixmap(pen_display,pen_drawable,
                        pen_width,pen_height,
                        visual_depth );

	if( fram->pixmap == (Pixmap)0 ){
	   /* allocation failed, give up on this method */
	   greedy_pixmaps = 0;
	}else{
           XSync( pen_display,0 );
	   XSetClipMask( pen_display, pen_gc, None );
           XCopyArea(pen_display,pen_drawable,fram->pixmap,pen_gc,
                        0,0,pen_width,pen_height,0 ,0);
           fram->has_image = 1;
	}

    }

    if( (!fram->has_image) && want_images ){

	/* We will save the image as an XImage in the client */

	/* saving the image in the client costs us a lot in
	   in data transmission so we try to make sure that the
           effort is worthwhile.

          The image size = pen_width * pen_height * visual_depth
	  The size of the plot data is frame->total_len.
	  If the plot is a lot smaller than the image size we will
          just replot it from scratch next time

	  This should probaly be related to an estimated connection speed
	  and the time taken to render the frame.
        */
	if( pen_width * pen_height * visual_depth < 50*fram->total_len ){
            XSync( pen_display,0 );
	    if( fram->image != 0 ){
		 XDestroyImage( fram->image );
	    }
	    fram->image=0;
            fram->image = XGetImage( pen_display, pen_drawable, 0,0,
			  pen_width, pen_height, AllPlanes, ZPixmap );
            fram->has_image = 1;
	}
    }
    xt_save_colormap( fram );

}

/* put the image back into the drawable */
void xt_put_image( xtFrame* fram)
{
  XImage* old_image;

  if( fram->pixmap != (Pixmap)0 ){
	XSetClipMask( pen_display, pen_gc, None );
        XCopyArea(pen_display,fram->pixmap,pen_drawable,pen_gc,
                        0,0,pen_width,pen_height,0 ,0);
  }else{
      old_image = fram->image;
      if( old_image != 0 ){
          /* put the old image onto the drawable */
          XPutImage( pen_display, pen_drawable, pen_gc, old_image, 0, 0, 0, 0,
                   old_image->width, old_image->height );
      }
  }
  xt_restore_colormap( fram );
}

void xt_clear_images( xtFrameList* list)
{
    xtFrame *curr;
    xtFrameList* currlist;
    
    currlist = list;
    if( list == 0 ) return;
    
    do{ 
	if( list->start != 0 ) {
    
	    curr = list->start;
	    do{
		/* free the image associated with this frame */
		if( curr->image != 0 )
		  XDestroyImage( curr->image ); curr->image=0;
		curr->image= 0;

		/* free the pixmap associated with this frame */
		if( curr->pixmap != (Pixmap)0 )
        	  XFreePixmap(pen_display,curr->pixmap);
		curr->pixmap = 0;

		/* we don't have a stored image for this frame */
		curr->has_image = 0;

		curr = curr->next;
	    }while( curr != list->start );
	}
	currlist = list->parent;
    }while( currlist != 0 );
}


xtFrame* xt_first_frame( xtFrameList* list)
{
    return list->start;
}

xtFrame* xt_last_frame( xtFrameList* list)
{
    return list->start->prev;
}

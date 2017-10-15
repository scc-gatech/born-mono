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
 *  source file:   ./filters/xtlib/xtclose.c
 *
 * Steve Cole (SEP), August 4 1991
 *      Inserted this sample edit history entry.
 * Dave Nicols (SEP), February 14 1992
 *      Rewrote using new xtFrame interface that only plots one
 *      frame at a time.
 * Dave Nichols (SEP), April 30 1992
 *      Final version of the new xtpen, added message window
 */

#include "xtpen.h"
#include "../include/closestat.h"
#include "../include/mesgcom.h"

int xt_after_erase = 0;

void xtclose (int status)
{


    switch (status)
    {
    case CLOSE_PAUSE:
	break;

    case CLOSE_NORMAL:
    case CLOSE_ERROR:
    case CLOSE_INTERRUPT:
	xt_clear_images(frame_list);
	remove_pixmap();
	break;

    case CLOSE_FLUSH:
	/*horrible hack to skip the first two flushes after erase */
	if( xt_after_erase ){
	    xt_after_erase--;
	}else{
            xt_flush_display(); 
	}
	break;
    }
}

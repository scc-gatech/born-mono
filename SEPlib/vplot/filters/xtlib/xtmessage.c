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
 *  source file:   ./filters/xtlib/xtmessage.c
 *
 * Steve Cole (SEP), August 4 1991
 *      Inserted this sample edit history entry.
 * Dave Nichols (SEP), April 30 1992
 *      Final version of the new xtpen, added message window
 */

#include "xtpen.h"
#include "../include/mesgcom.h"

#include <X11/Xaw/AsciiText.h>


#ifdef __STDC__
#include <stdlib.h>
#else
extern char* malloc();
extern char* realloc();
#endif


char            oldtext[100];

void xtaddText( char *str)
{
  
  if ( text_region ) {
    int len ;
    int rc;
    char *p;
    char *currentString;
    
    Arg argList[20];
    Cardinal args;
    
    XawTextPosition start;
    XawTextBlock tblk, empty_tblk;
    
    len = strlen(str);

    if ( len <= 0 )
      return;

    p = (char*)malloc( len+1 );
    strcpy( p, str );
    p[len] = '\0';
    
    tblk.firstPos = 0;
    tblk.length = len;
    tblk.ptr = p;
    tblk.format = FMT8BIT;
    
    /* empty text block for deletion */
    empty_tblk.firstPos = 0;
    empty_tblk.length = 0;
    empty_tblk.ptr = "";
    empty_tblk.format = FMT8BIT;

    args = 0;
    XtSetArg(argList[args], XtNstring, &currentString); args++;
    XtGetValues(text_region, argList, args);
    
    start = strlen( currentString );
    
    XawTextDisableRedisplay(text_region); 

    while ( start + len > TEXT_BUFFER_SIZE ) {
      
      /* find first line and delete it */
      
      char *eol = strchr( currentString, '\n');
      
      if ( eol != 0 && eol < &currentString[TEXT_BUFFER_SIZE] ) {
	XawTextReplace( text_region, 0, eol-currentString+1, &empty_tblk );
        start -= eol - currentString + 1;
      }
      else {
	XawTextReplace( text_region, 0, start, &empty_tblk );
	start = 0;
	break;
      }
    }

    rc= XawTextReplace( text_region, start, start, &tblk );
    if (rc != XawEditDone) {
        fprintf(stderr, "XawTextReplace(text, %ld, %ld, &textblock): %d\n",
                           (long) start, (long) start, rc);
    }
    XawTextSetInsertionPoint(text_region, start + len);
    XawTextEnableRedisplay(text_region);
    XawTextDisplay(text_region);

  }
}


void xtmessage (
    int             command,
    char           *text
          )
{
static int      eatit = NO;


    switch (command)
    {
    case MESG_TEXT:
	if (!eatit){
	    if( text_region && plotting_started ){
		xtaddText( text );
	    }else{
	        printf ("%s", text);
	    }
	}
	break;
    case MESG_HIGHLIGHT_ON:
	eatit = YES;
	break;
    case MESG_DONE:
	eatit = NO;
	if( !text_region && plotting_started ){
	    fflush (stdout);
        }
	break;
    }
}

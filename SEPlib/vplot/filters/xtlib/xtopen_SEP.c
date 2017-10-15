/*
 * Copyright 1987 the Board of Trustees of the Leland Stanford Junior
 * University. Official permission to use this software is included in
 * the documentation. It authorizes you to use this file for any
 * non-commercial purpose, provided that this copyright notice is not
 * removed and that any modifications made to this file are commented
 * and dated in the style of my eXample below.
 */

/*
 *
 *  source file:   ./filters/xtlib/xtopen.c
 *
 * Steve Cole (SEP), August 4 1991
 *      Inserted this sample edit history entry.
 * Joe Dellinger (SOEST), Feb 18 1992
 *	Made "DEFAULT_HARDCOPY_FONT" the default font.
 *	Made "break=ignore" the default.
 * Dave Nichols (SEP), April 30 1992
 *      Final version of the new xtpen, added message window
 * Dave Nichols (SEP), May 6 1992
 *      Added option want_text to control message window and
 *      see_progress to control the drawing to an offscreen pixmap.
 * Dave Nichols (SEP), May 13 1992
 *      Get and save what BlackPixel and WhitePixel are for this screen.
 * Stewart A. Levin (Mobil) Feb 19, 1993
 *      Removed erroneous ; after sepwhere function body.
 * Dave Nichols(SEP) April 12, 1993
 *      Moves xt_size_n_scale call into xt_dovplot.
 * Dave Nichols (SEP), April 13 1993
 *      Use app_data to get defaults from resource database
 * Dave Nichols (SEP), Dec 14 1993
 *      Colormap allocation now in xtcolors.c
 * Joe Dellinger (AMOCO), Oct 18 1994
 *      Allow overriding of "pixels_per_inch" from the command line.
 * Joe Dellinger (AMOCO), June 9 1995
 *	This code erroneously set colors 0 to 7 to all be BLACK.
 *	A check in the xtcolors code checked for colors 0 through 7
 *	being set to black and ignored all those erroneous calls.
 *	This had the side effect of also ignoring LEGITIMATE attempts
 *	to set any of colors 0 through 7 to black.
 * Dave Nichols (Schlumberger) Oct 10 1998
 *      Add support for 8/16/24 bit true color visuals
 * Joe Dellinger (BP Amoco), Oct 4 1999
 *	Round aspect_ratio to nearest percent. (On newer Sun workstations
 *	it comes out "1.002778" when it should be "1".)
 * Dave Nichols (Schlumberger) Nov 16 1999
 *      Change method for finding suitable visuals
 */
 
#include "xtpen.h"

#ifndef XT_SET_TRANSLATIONS
#define XT_SET_TRANSLATIONS
#endif

#include "xtcommands.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Shell.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Cardinals.h>
 
#include "../include/attrcom.h"
#include "../include/enum.h"
#include "../include/params.h"
#include "../include/extern.h"
 
#ifdef SEP
#define GETPAR getch
extern int getch(const char *, const char *, MIXED);
#else
#define GETPAR getpar
extern int getpar(char *, char *, MIXED);
#endif

Display		*pen_display;
GC		pen_gc;
Window		pen_window;
Pixmap		pen_pixmap;
Drawable	pen_drawable;
Visual		*pen_visual;
unsigned long	*pen_colors;
Colormap	pen_colormap;
Widget		xtpen, vpane, control_panel, pen_picture; 
Widget		text_region=(Widget)0;
XtAppContext	pen_context;
int		own_colormap;
int		x_num_col;
int		num_col_req = 0;
int		num_col_max = 16384;
int		num_col_min = 16;
int             xmono = NO;
int             xtruecol = NO;
int		screen_depth;
int		visual_depth;
long		screen_black, screen_white;
int             xt_stretchy;
int 		boxy,want_images,greedy_pixmaps,see_progress;
static char 	message_text[TEXT_BUFFER_SIZE];
int		plotting_started = 0;

String fallback_resources[] = {
	"*text.scrollVertical:            whenNeeded",
	"*text.autoFill:                  True",
	"*text.Wrap:                      word",
	"*text.height:                    50",
	"*Font:                           fixed",
	"*Foreground:                     black",
	"*Background:                     white",
	NULL,
};

static void choose_visual(Display*,int,int,XVisualInfo*);

void xtopen (void)
{
int 		dwidth,dheight,mwidth,mheight;
int             default_width,default_height;
int		want_buttons,want_labels,want_text,tellme_resolution;
Visual		*default_visual;
XVisualInfo	vinfo;
int		pen_screen;
Arg		args[20];
int             cnt,depth;
int		syncplot;
MIXED		valtemp;
int iarg;

#if  XtSpecificationRelease > 4
  int		xtargc;
  int           myxargc;
#else
Cardinal	xtargc;
Cardinal        myxargc;
#endif
char ** myxargv;

   /*
   * save the command line arguments
   */
  
  myxargc = sepxargc;
  myxargv = (char **) XtMalloc (myxargc * sizeof (char *));
  bcopy ((char *) sepxargv, (char *) myxargv, myxargc * sizeof (char *));
  
    
    /* device capabilities */
    txfont = DEFAULT_HARDCOPY_FONT;
    txprec = DEFAULT_HARDCOPY_PREC;
    brake = BREAK_IGNORE;
    endpause = YES;
    buffer_output = YES;
    smart_clip = NO;
    allowecho = YES;
    cachepipe = YES;
    syncplot = NO;

    /* see if user wants slow plotting */
    valtemp.i = &syncplot;
    getpar ("syncplot", "1", valtemp);
    if(syncplot == YES) _Xdebug = 1;

    /* initialize the first app (we will delete this one later ) */
    xtargc = sepxargc;
    xtpen = XtVaAppInitialize(
		&pen_context,
		"XTpen",
		NULL, ZERO,
		&xtargc, sepxargv,
		fallback_resources,
		NULL );
		
    sepxargc=xtargc;

    /* get app data from the resource database */
    xt_app_data( xtpen );

    /* get info about the screen */
    pen_display = XtDisplay(xtpen);
    pen_screen = DefaultScreen(pen_display);
    screen_black = BlackPixel( pen_display,pen_screen);
    screen_white = WhitePixel( pen_display,pen_screen);

    valtemp.i = &depth;
    if(0==getpar ("depth", "d", valtemp)) depth=app_data.vis_depth;
    /* Choose a visual */
    choose_visual( pen_display, pen_screen, depth, &vinfo );
		
    pen_visual = vinfo.visual;
    screen_depth = visual_depth;
/*    ERR( COMMENT,name,"visual depth = %d",visual_depth);*/


    /* figure out the screen size */
    dwidth = DisplayWidth(pen_display,pen_screen);
    dheight = DisplayHeight(pen_display,pen_screen);

    /* Height and Width of the screen in Millimeters */
    mwidth = DisplayWidthMM( pen_display,pen_screen);
    mheight = DisplayHeightMM( pen_display,pen_screen);
    pixels_per_inch = ((float)dwidth/(float)mwidth)*25.4f ;
    aspect_ratio = (((float)dheight/(float)mheight)*25.4f )/pixels_per_inch ;

    /*
     * Since X rounds values to integer millimeters, the aspect ratio
     * has some error in it. Push aspect ratio to nearest round percent.
     */
    aspect_ratio = ((int)((aspect_ratio * 100.0f) + .5f)) / 100.0f;

    tellme_resolution = NO;
    valtemp.i = &tellme_resolution;
    getpar ("x_screen_info", "1", valtemp);
    if (tellme_resolution != NO)
    {
	ERR(COMMENT, name,
	    "display width=%d, height=%d (pixels);  width=%d, height=%d (mm)",
	    dwidth, dheight, mwidth, mheight);
	ERR(COMMENT, name, "pixels_per_inch=%f,  aspect_ratio=%f",
	    pixels_per_inch, aspect_ratio);
    }

    /*
     * Allow override of pixels_per_inch and aspect_ratio from
     * command line. (If we used GETPAR here it would also search
     * the incoming history file... I think it's safer to require
     * them to be on the command line.)
     */
    valtemp.f = &aspect_ratio;
    getpar ("aspect", "f", valtemp);
    valtemp.f = &pixels_per_inch;
    getpar ("ppi", "f", valtemp);

 
    /* Take a default from the resource database */
    valtemp.i = &num_col_req;
    if ( !GETPAR("numcol","d",valtemp) ) num_col_req = app_data.num_col;

    /* colormap support */
    if( !mono ) x_num_col = DisplayCells(pen_display, pen_screen);
    if( x_num_col <= 2 ) mono=YES;
    if( x_num_col < num_col_req ) num_col_req = x_num_col;
    /* limit to 254 to get around tektronix bug */
    if( num_col_req > 254 ) num_col_req=254;

    /* if the user specifies the number of colors dont accept anything less */
    if( num_col_req != 0 ){
	num_col_min = num_col_max = num_col_req;
    }

    /* initialze argument list counter to zero */
    cnt=0;

   /* For the default visual set the initial colormap to be 
      the default colormap, otherwise aloocate your own */
   default_visual = DefaultVisual( pen_display, pen_screen);
   if( pen_visual->visualid  == default_visual->visualid ){
     pen_colormap = DefaultColormap(pen_display, pen_screen);
   }else{
     pen_colormap = XCreateColormap( pen_display, 
		DefaultRootWindow(pen_display) , pen_visual, AllocNone );
     XtSetArg (args[cnt], XtNvisual, pen_visual); ++cnt;
     XtSetArg (args[cnt], XtNdepth, visual_depth ); ++cnt;
   }

    if(  mono  ){
	num_col = x_num_col;
    }else{
	if( xtruecol ){
	    num_col = num_col_max;
        }else{
	    /* NB may change pen_colormap ! */
            num_col = xtnumcol( num_col_min, num_col_max );
	}
    }

    XtSetArg (args[cnt], XtNcolormap,pen_colormap ); ++cnt;
    /* end of colormap code */


    /* now destroy the widget we created and make a new one with our 
       chosen visual and colormap etc. */
    XtDestroyWidget( xtpen );
    
    /*
     * Now create the real toplevel widget.
     */
        
/*    XtSetArg (args[cnt], XtNargv, myxargv); ++cnt;*/
/*    XtSetArg (args[cnt], XtNargc, myxargc); ++cnt;*/
    iarg=0;
    while(iarg < myxargc){
      if(myxargv[iarg][0]=='-' && iarg < myxargc-1){
        XtSetArg(args[cnt],&(myxargv[iarg][1]),myxargv[iarg+1]);++cnt;
        iarg++;
      }
      iarg++;
    }
    



    xtpen = XtAppCreateShell ( "xtpen", "XTpen",
                                applicationShellWidgetClass,
                                pen_display, args, cnt);


    /* default size for the picture is 1/2 of screen height and width
     * this can be overridden by resources or the command line geometry spec 
     */
    default_width = dwidth*.5f;
    default_height = dheight*.5f;
  
    vpane = XtVaCreateManagedWidget("vpane",panedWidgetClass,
		xtpen, 
		XtNwidth, default_width,
        	NULL);

    valtemp.i = &want_buttons;
    if( !GETPAR("buttons","1",valtemp) ) want_buttons = app_data.buttons;
    valtemp.i = &want_labels;
    if( !GETPAR("labels","1",valtemp) ) want_labels = app_data.labels;
    valtemp.i = &want_text;
    if( !GETPAR("want_text","1",valtemp) ) want_text = app_data.textpane;

    valtemp.i = &xt_stretchy;
    if( !GETPAR("stretchy","1",valtemp) ) xt_stretchy=app_data.stretchy;

    valtemp.i = &boxy;
    if( !GETPAR("boxy","1",valtemp) ) boxy = 0;

    valtemp.i = &see_progress;
    if( !GETPAR("see_progress","1",valtemp) ) see_progress=0;
    valtemp.i = &want_images;
    if( !GETPAR("images","1",valtemp) ) want_images = app_data.images;
    valtemp.i = &greedy_pixmaps;
    if( !GETPAR("pixmaps","1",valtemp)) greedy_pixmaps=app_data.pixmaps;

    if( want_buttons || want_labels ){
        control_panel = XtCreateManagedWidget("control_panel",boxWidgetClass,
		vpane, NULL, ZERO);

        create_panel(control_panel,want_buttons,want_labels);
    }

    /* if they want to add a message to the window do it here */

    if( visual_depth != 8 ) want_text=0; /* BUG for 24 bit visuals */
    if( want_text ) {
	valtemp.s = &(message_text[0]);
        if( ! GETPAR("message","s",valtemp) ) strcpy(message_text,"");

        text_region = XtVaCreateManagedWidget("text",
		asciiTextWidgetClass,vpane,
		XtNstring, (XtArgVal)message_text,
		XtNlength, (XtArgVal)TEXT_BUFFER_SIZE, 
		XtNeditType, XawtextEdit,
		NULL );
    }

    pen_picture = XtVaCreateManagedWidget("pen_picture",
		widgetClass, vpane,
                XtNwidth, default_width,
	        XtNheight, default_height,
		XtNcolormap, pen_colormap,
		NULL);



    XtAppAddActions(pen_context,window_actions, XtNumber(window_actions));

    XtAugmentTranslations(pen_picture,XtParseTranslationTable(trans));

    XtRealizeWidget(xtpen);

    inactivate_buttons();

/* moved into xt_dovplot (should it be xt_reset ? )*/
/*    xt_size_n_scale( &width, &height);*/
  
    /* Pixmap is initially undefined, the drawable is initially the window */
    pen_pixmap = (Pixmap)0;
    have_pixmap = NO;
    pen_window = XtWindow(pen_picture);
    pen_drawable = pen_window;

/*
 * create and initialize gc
 */
    pen_gc = XCreateGC(pen_display,pen_window,0,NULL);
    XSetLineAttributes(pen_display,pen_gc, 0, LineSolid, CapButt, JoinRound);
    XSetFillRule(pen_display, pen_gc, WindingRule);
    XSetClipMask(pen_display, pen_gc, None );
 
    /* set standard colors */
    if( mono ){
	if( xmono ){
	    map[1] = screen_white;
	    map[0] = screen_black;
	}else{
	    map[7] = screen_white;
	    map[1] = screen_white;
	    map[0] = screen_black;
        }
    }else{
    	xtattributes (SET_COLOR_TABLE, 0, 0, 0, 0);
    	xtattributes (SET_COLOR_TABLE, 1, 0, 0, MAX_GUN);
    	xtattributes (SET_COLOR_TABLE, 2, MAX_GUN, 0, 0);
    	xtattributes (SET_COLOR_TABLE, 3, MAX_GUN, 0, MAX_GUN);
    	xtattributes (SET_COLOR_TABLE, 4, 0, MAX_GUN, 0);
    	xtattributes (SET_COLOR_TABLE, 5, 0, MAX_GUN, MAX_GUN);
    	xtattributes (SET_COLOR_TABLE, 6, MAX_GUN, MAX_GUN, 0);
    	xtattributes (SET_COLOR_TABLE, 7, MAX_GUN, MAX_GUN, MAX_GUN);
    }

    /* tell which entry in the colormap is the background color */
/*    XSetWindowBackground( pen_display, pen_window, map[0]);*/
    xtattributes (SET_COLOR, 7, 0, 0, 0);;

    /* xtpen controls the file stream itself */
    genreader = xt_dovplot;

    /* xtpen controls messages */
    message = dev.message;

    (void) atexit(cleanup_zooms);
}

/*
 *  make this version link ahead of the one in tseplib to define default
 *  output as /dev/null
 */
char            sepoutwhere[] = "/dev/null";
char            sepheadwhere[] = "/dev/null";

/* Make linker happy that it found it. */
void sepwhere (void)
{
}

void xt_size_n_scale( Dimension *width, Dimension *height)
{

    Position left, top;
    float win_ratio; 
    float new_stretch, new_xstretch,new_ystretch;
    static float old_xstretch = 1.;
    static float old_ystretch = 1.;

    /* now get the size of the actual picture and set dev_xmax etc. */

    XtVaGetValues(pen_picture,XtNwidth,width,XtNheight,height,
		XtNx,&left,XtNy,&top,NULL);
  
    /* set VPLOT plotting window */
    dev_xmin = left;
    dev_xmax = dev_xmin + *width - 1;
    dev_ymin = top;
    dev_ymax = dev_ymin + *height - 1;  

    /* attempt to create pixmap  to speed redraws */
    create_pixmap( *width, *height );

    if( xt_stretchy ){ 
	/* If the pen is in stretch mode we should scale the plots
	 * axis so that the plot fills the frame */
	
	win_ratio = (float)(*height) / (float)(*width);
	
	new_stretch = win_ratio/SCREEN_RATIO;

	if( new_stretch > 1.0f ){
	    new_ystretch = new_stretch; new_xstretch =1.0f;
	}else{
	    new_xstretch = 1.0f/new_stretch; new_ystretch =1.0f;
	}

    }else{
	/* go back to a rigid aspect ratio */
	new_xstretch=1.0f;
	new_ystretch=1.0f;
    }


    user_xscale = user_xscale * new_xstretch / old_xstretch;
    user_yscale = user_yscale * new_ystretch / old_ystretch;
	
    old_xstretch = new_xstretch; old_ystretch = new_ystretch;
  
}

static void choose_visual(
Display* display,
int screen,
int max_depth,
XVisualInfo* vinfo)
{

  /* try and figure out a good visual */
  
  /* try for 24bit true color */
   if(  max_depth >=24 &&
	XMatchVisualInfo(display,screen,24,TrueColor,vinfo) ){
     xmono=NO;
     xtruecol=YES;
     visual_depth = 24;
     
     /* try for 8 bit pseudo color */
   }else if( max_depth >=8 &&
	XMatchVisualInfo(display,screen,8,PseudoColor,vinfo) ){
     xmono=NO;
     xtruecol=NO;
     visual_depth = 8;
     x_num_col = 256;
     
     /* try for 16bit true color */
   }else if( max_depth >=16 &&
	XMatchVisualInfo(display,screen,16,TrueColor,vinfo) ){
     xmono=NO;
     xtruecol=YES;
     visual_depth = 16;
     
     /* try for 15 true color */
   }else if( max_depth >=15 &&
	XMatchVisualInfo(display,screen,15,TrueColor,vinfo) ){
     xmono=NO;
     xtruecol=YES;
     visual_depth = 15;
     
     /* try for 8 true color */
   }else if( max_depth >=8 &&
	XMatchVisualInfo(display,screen,8,TrueColor,vinfo) ){
     xmono=NO;
     xtruecol=YES;
     visual_depth = 8;
     
     /* try for 4 bit pseudo color */
   }else if( max_depth >=4 &&
	XMatchVisualInfo(display,screen,4,PseudoColor,vinfo) ){
     xmono=NO;
     num_col_min = 8;
     x_num_col = 8;
     visual_depth = 4;
     xtruecol=NO;
     
     /* if we can't get any of those try for a monochrome visual */
     /* try for 1 bit static grey */
   }else if( max_depth >=1 &&
	XMatchVisualInfo(display,screen,1,StaticGray,vinfo) ){
     xmono = YES;
     visual_depth = 1;
     xtruecol=NO;
     mono = YES;
     x_num_col = 2;
   }else{
     ERR(FATAL,name,"Could not obtain a suitable visual");
   }

}


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
 *  source file:   ./filters/xtlib/xtcommands.c
 *
 * Steve Cole (SEP), August 4 1991
 *      Inserted this sample edit history entry.
 *
 * Dave Nichols (SEP), February 14 1992
 *      Changed to new control structure, added new buttons
 *      for run, stop, prev.
 * Dave Nichols (SEP), April 30 1992
 *      Final version of the new xtpen, added message window
 * Dave Nichols (SEP), July 18 1992
 *      Added boxy mode for interact, it pops up a dialog to enter
 *	information for the label and it outputs a format suitable
 *	for the command line of Box
 */


#include "xtpen.h"
#include "../include/extern.h"

#include <X11/Xaw/Command.h>
#include <X11/Xaw/Label.h>

#include <X11/Xaw/Cardinals.h>
#include <X11/Shell.h>
#include <X11/cursorfont.h>

#include <sys/types.h>
#include <sys/param.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>

/* variable that control the existance of buttons and labels */
static int wantButtons = 0;
static int  xt_first_time = YES;
static int wantLabels = 0;
static FILE *outfp;


/* The buttons that go into the control panel */
Widget quit_button, next_button, prev_button,  restart_button;
Widget run_button, stop_button, stretch_button;
Widget mode_button, frame_label, delay_label;


/* static variables that are set when a button is pressed */
static int didNEXT = NO, didPREV = NO, didQUIT = NO, didTIME=NO;
static int didFRAM1 = NO,  didRUN = NO, didSTOP=NO , didCHANGE = NO;


/* Function active? flags. We can't use XtIsSensitive because we might
 * not have active buttons */
static int next_on=NO ,prev_on=NO ,quit_on=NO ,restart_on=NO ;
static int run_on=NO , stop_on=NO, size_on=NO;


/*ARGSUSED*/
void  dummy_proc (Widget w, XtPointer client_data, XtPointer call_data) { return; }
/*ARGSUSED1*/
void actionDummy(
Widget w, XEvent *ev, String *p, Cardinal *np
                ) { dummy_proc(w,NULL,NULL); }

/*ARGSUSED*/
void  next_proc(Widget w, XtPointer client_data, XtPointer call_data){  if( next_on == YES ) didNEXT = YES;    return; }
/*ARGSUSED1*/
void actionNext(
Widget w, XEvent *ev, String *p, Cardinal *np
                ) { next_proc(w,NULL,NULL); }

/*ARGSUSED*/
void  prev_proc(Widget w, XtPointer client_data, XtPointer call_data){  if( prev_on==YES ) didPREV = YES;    return; }
/*ARGSUSED1*/
void actionPrev(
Widget w, XEvent *ev, String *p, Cardinal *np
               ) { prev_proc(w,NULL,NULL); }

/*ARGSUSED*/
void quit_proc(Widget w, XtPointer client_data, XtPointer call_data){  if( quit_on == YES ) didQUIT = YES;    return; }

/*ARGSUSED1*/
void actionQuit(
Widget w, XEvent *ev, String *p, Cardinal *np
               ) 
{
 if( interact[0] != '\0' ) {
if (xt_first_time == YES){
        outfp = fopen (interact, "w");
        if (outfp == NULL) { 
            ERR (FATAL, name, "Can't open interact output file %s!", interact);
        }
        xt_first_time = NO;
   }
/*	 fprintf( outfp,"\n");*/
  fflush( outfp );
}
  quit_proc(w,NULL,NULL); 

}

/*ARGSUSED*/
void restart_proc(Widget w, XtPointer client_data, XtPointer call_data){  if( restart_on == YES) didFRAM1 = YES;    return; }
/*ARGSUSED1*/
void actionRestart(
Widget w, XEvent *ev, String *p, Cardinal *np
                  ) { restart_proc(w,NULL,NULL); }

/*ARGSUSED*/
void run_proc(Widget w, XtPointer client_data, XtPointer call_data){  if( run_on == YES ) didRUN = YES;    return; }
/*ARGSUSED1*/
void actionRun(
Widget w, XEvent *ev, String *p, Cardinal *np
              ) { run_proc(w,NULL,NULL); }

/*ARGSUSED*/
void stop_proc(Widget w, XtPointer client_data, XtPointer call_data){  if( stop_on == YES ) didSTOP = YES;    return; }
/*ARGSUSED1*/
void actionStop(
Widget w, XEvent *ev, String *p, Cardinal *np
               ) { stop_proc(w,NULL,NULL); }

static Arg rigidArgs[] = { { XtNlabel, (XtArgVal)"Rigid" } };
static Arg stretchArgs[] = { { XtNlabel, (XtArgVal)"Stretchy" } };

/*ARGSUSED*/
void stretch_proc(Widget w, XtPointer client_data, XtPointer call_data){ 
    xt_stretchy = !xt_stretchy;  /* toggle the "stretchy" attribute */

    if( wantButtons ){

	if( xt_stretchy ){
	    /* if we are now stretchy change the label to "rigid" */
	    XtSetValues( stretch_button, rigidArgs, XtNumber(rigidArgs) );
	}else{
	    /* if we are now rigid change the label to "stretchy" */  
	    XtSetValues( stretch_button, stretchArgs, XtNumber(rigidArgs) );
	}

    }
    didCHANGE = YES; return; 
}
/*ARGSUSED1*/
void actionStretch(
Widget w, XEvent *ev, String *p, Cardinal *np
                  ) { stretch_proc(w,NULL,NULL); }

/* spawns off a new xtpen with selected global clipping window */
/* given in vplot coordinates. All other current command line  */
/* parameters are preserved and/or reconstituted.              */

static int makezoompar(int vpxcenter,int vpycenter,
                       float x_ratio,float y_ratio, float nostretch_ratio,
		       char parfilename[])
{
  FILE *parfile;
  FILE *nullfile;
  int  ii;
  
  parfilename[0] = '\0';
  nullfile = fopen("/dev/null","r");
  if(nullfile != ((FILE *) NULL)) {
     parfile = tempcopy(nullfile,parfilename);
     (void) fclose(nullfile);
     if(parfile != ((FILE *) NULL)) {
        (void) fclose(parfile);
        parfile = fopen(parfilename,"w+");
     }
     if(parfile != ((FILE *) NULL)) {
        /* populate it with plot parameters */
	/* first the list of input vplot files */
        for(ii = 0; pltinname[ii] != ((char *) NULL) ; ++ii) {
           if(ii == 0) fprintf(parfile,"in=\"%s\"\n",pltinname[ii]);
           else {
             if(pltinname[ii][0] != '\0')
                 fprintf(parfile,"in%d=\"%s\"\n",ii,pltinname[ii]);
             else
                 break;
           }
        }
        fprintf(parfile,"numvplot=\"%d\"\n",ii-1);
        fprintf(parfile,"selfdoc=\"%d\"\n",0); /* we use in= for input */
        fprintf(parfile,"mono=\"%d\"\n",mono);
        fprintf(parfile,"wstype=\"%s\"\n",wstype);
        fprintf(parfile,"echo=\"%d\"\n",allowecho);
        fprintf(parfile,"endpause=\"%d\"\n",endpause);
        fprintf(parfile,"cachpipe=\"%d\"\n",cachepipe);
        fprintf(parfile,"shade=\"%d\"\n",shade);
        fprintf(parfile,"wantras=\"%d\"\n",wantras);
        fprintf(parfile,"overlay=\"%d\"\n",overlay);
        fprintf(parfile,"invras=\"%d\"\n",invras);
        fprintf(parfile,"txsquare=\"%d\"\n",no_stretch_text);
        fprintf(parfile,"colormask=\"%d,%d,%d,%d,%d\"\n",colormask[0],
		colormask[1],colormask[2],colormask[3],colormask[4]);
        fprintf(parfile,"redpow=\"%g\"\n",redpow);
        fprintf(parfile,"greenpow=\"%g\"\n",greenpow);
        fprintf(parfile,"bluepow=\"%g\"\n",bluepow);
        fprintf(parfile,"red=\"%g,%g,%g,%g\"\n",
                redmap[0],redmap[1],redmap[2],redmap[3]);
        fprintf(parfile,"green=\"%g,%g,%g,%g\"\n",
                greenmap[0],greenmap[1],greenmap[2],greenmap[3]);
        fprintf(parfile,"blue=\"%g,%g,%g,%g\"\n",
                bluemap[0],bluemap[1],bluemap[2],bluemap[3]);
        fprintf(parfile,"dither=\"%d\"\n",dither);
        fprintf(parfile,"greyc=\"%g\"\n",greyc);
        fprintf(parfile,"pixc=\"%g\"\n",pixc);
        fprintf(parfile,"txfont=\"%d\"\n",txfont);
        fprintf(parfile,"txprec=\"%d\"\n",txprec);
        fprintf(parfile,"txovly=\"%d\"\n",txovly);
        if(xcenterflag)fprintf(parfile,"xcenter=\"%d\"\n",xcenter);
        if(ycenterflag)fprintf(parfile,"ycenter=\"%d\"\n",ycenter);
        fprintf(parfile,"patternmult=\"%g\"\n",patternmult);
        fprintf(parfile,"interact=\"%s\"\n",interact);
        switch (interactcoords) {
        case 1: 
	    fprintf(parfile,"interactcoords=\"user\"\n");
            break;
        case 2:
	    fprintf(parfile,"interactcoords=\"vplot\"\n");
            break;
        default:
	    fprintf(parfile,"interactcoords=\"inches\"\n");
            break;
        }
        fprintf(parfile,"pause=\"%g\"\n",epause);
        switch (default_style) {
        case ROTATED:
           fprintf(parfile,"style=\"r\"\n");
           break;
        case OLD:
           fprintf(parfile,"style=\"o\"\n");
           break;
        case ABSOLUTE:
           fprintf(parfile,"style=\"a\"\n");
           break;
        case STANDARD:
        default:
	   if(style != NO_STYLE_YET) fprintf(parfile,"style=\"s\"\n");
	   break;
        }
        fprintf(parfile,"fatmult=\"%g\"\n",fatmult);
        fprintf(parfile,"fat=\"%d\"\n",fatbase);
        fprintf(parfile,"window=\"%d\"\n",window);
        fprintf(parfile,"frame=\"%d\"\n",framewindows);
        switch (erase) {
        case NO:
           fprintf(parfile,"erase=\"N\"\n");
           break;
        case FORCE_INITIAL:
           fprintf(parfile,"erase=\"O\"\n");
           break;
        case DO_LITERALS:
	   fprintf(parfile,"erase=\"L\"\n");
           break;
        default:
	   fprintf(parfile,"erase=\"Y\"\n");
	   break;
        }
        switch (brake) {
        case BREAK_BREAK:
           fprintf(parfile,"break=\"b\"\n");
           break;
        case BREAK_ERASE:
           fprintf(parfile,"break=\"e\"\n");
           break;
        default:
	   fprintf(parfile,"break=\"i\"\n");
	   break;
        }
        switch (user_size) {
        case ABSOLUTE:
           fprintf(parfile,"size=\"a\"\n");
           break;
        default:
	   fprintf(parfile,"size=\"r\"\n");
	   break;
        }
        fprintf(parfile,"rotate=\"%d\"\n",user_rotate);
        fprintf(parfile,"xshift=\"%g\"\n",user_hshift);
        fprintf(parfile,"yshift=\"%g\"\n",user_vshift);
        fprintf(parfile,"stretchy=\"%d\"\n",xt_stretchy);

        /* finally our zoom arguments */
        fprintf(parfile,"xcenter=\"%g\"\n",vpxcenter/RPERIN);
        fprintf(parfile,"ycenter=\"%g\"\n",vpycenter/RPERIN);
#ifdef notdef
        fprintf(parfile,"scale=\"%g\"\n",user_scale*nostretch_ratio);
#endif
        fprintf(parfile,"xscale=\"%g\"\n",user_xscale*x_ratio);
        fprintf(parfile,"yscale=\"%g\"\n",user_yscale*y_ratio);
        fprintf(parfile,"txscale=\"%g\"\n",user_txscale*nostretch_ratio);
        fprintf(parfile,"mkscale=\"%g\"\n",user_mkscale*nostretch_ratio);
        fprintf(parfile,"dashscale=\"%g\"\n",user_dashscale*nostretch_ratio);

        (void) fclose(parfile);
     } else { return EXIT_FAILURE; }
  } else { return EXIT_FAILURE; }
  return EXIT_SUCCESS;
}

static Boolean isShifted=False;/* hold shift key for sizing instead of moving */
static Cursor CrossHairCursor;
static Boolean imZoomingNow=False; /* don't report coordinates during zoom */
static Boolean zoomInitialized=False;

static struct zoom_cleanup {
  pid_t zoom_pid;
  char *parfile;
} *zoom_list = (struct zoom_cleanup *) NULL;

static size_t nzooms = 0;

void cleanup_zooms(void) {
  size_t i;
  int rc, wait_status;
  for(i=0; i<nzooms; ++i) {
     rc = waitpid(zoom_list[i].zoom_pid,&wait_status,WNOHANG);
     if(0 == rc) /* still running give it a moment to consume par= */ sleep(1);
     if(zoom_list[i].parfile != ((char *) NULL)) {
        unlink(zoom_list[i].parfile);
        zoom_list[i].parfile = (char *) NULL;
     }
  }
  free(zoom_list);
  zoom_list = (struct zoom_cleanup *) NULL;
}

typedef struct rect_struct {
  int x1, y1, x2, y2;
} Rectangle;

/* recipient is responsible for freeing return value when finished */
static Rectangle *make_rectangle(int x1, int y1, int x2, int y2)
{
  Rectangle *result = malloc(sizeof(*result));
  result->x1 = x1; result->x2 = x2;
  result->y1 = y1; result->y2 = y2;

  return result;
}

static void display_rectangle(Widget w, GC gc, Rectangle *rect)
{
   int x1, y1, x2, y2;
   unsigned int width, height;

   if(rect->x1 > rect->x2) {
      x1 = rect->x2;
      x2 = rect->x1;
   } else {
      x1 = rect->x1;
      x2 = rect->x2;
   }

   if(rect->y1 > rect->y2) {
      y1 = rect->y2;
      y2 = rect->y1;
   } else {
      y1 = rect->y1;
      y2 = rect->y2;
   }
   width  = (unsigned int) (x2-x1);
   height = (unsigned int) (y2-y1);

   (void) XDrawRectangle(XtDisplay(w), XtWindow(w), gc, x1, y1, width, height);
}

static struct drag_struct {
  GC xor_gc;
  void *object;
} drag_client;

/*ARGSUSED*/
static void press_event(Widget w, XtPointer client_data, XEvent *ev, Boolean *cont_dispatch)
{
  XButtonEvent *bev = (XButtonEvent *) ev;
  Rectangle *rect;
  if(imZoomingNow) {
  drag_client.object = (void *) make_rectangle(bev->x, bev->y, bev->x, bev->y);
  rect = (Rectangle *) (drag_client.object);
  display_rectangle(w,drag_client.xor_gc,rect);
  }
  *cont_dispatch = True;
}

/*ARGSUSED*/
static void motion_event(Widget w, XtPointer client_data, XEvent *ev, Boolean *cont_dispatch)
{
  XMotionEvent *bev = (XMotionEvent *) ev;
  Rectangle *rect;

  if(imZoomingNow) {
  rect = (Rectangle *) (drag_client.object);
  display_rectangle(w,drag_client.xor_gc,rect); /* erase old box */
  if(isShifted) { /* drag box */
     rect->x1 += (bev->x - rect->x2);
     rect->y1 += (bev->y - rect->y2);
  }
  rect->x2 = bev->x; rect->y2 = bev->y;
  display_rectangle(w,drag_client.xor_gc,rect); /* highlight new box */
  }
  *cont_dispatch = True;
  
}

/*ARGSUSED*/
static void shift_event(Widget w, XtPointer client_data, XEvent *ev, Boolean *cont_dispatch)
{
  XKeyEvent *kev = (XKeyEvent *) ev;
  KeyCode s_l, s_r;
  if(imZoomingNow) {
  s_l = XKeysymToKeycode(XtDisplay(w),XK_Shift_L);
  s_r = XKeysymToKeycode(XtDisplay(w),XK_Shift_R);
  if(kev->keycode == s_l || kev->keycode == s_r) {
     if(kev->type == KeyPress) isShifted = True;
     if(kev->type == KeyRelease) isShifted = False;
  }
  }
  *cont_dispatch = True;
}

/*ARGSUSED*/
static void release_event(Widget w, XtPointer client_data, XEvent *ev, Boolean *cont_dispatch)
{
  int vpxcenter, vpycenter;
  int vpxmin, vpymin;
  int vpxmax, vpymax;
  int rc;
  pid_t waitpid;
  char cmdbuf[BUFSIZ];
  char parfile[MAXPATHLEN+5];
  char *cptr;
  XButtonEvent *bev = (XButtonEvent *) ev;
  Rectangle *rect;
  int xdev_center, ydev_center;
  int temp1, temp2;
  float xscale_ratio, yscale_ratio, nostretchy_scale;

  if(imZoomingNow) {
  rect = (Rectangle *) (drag_client.object);
  display_rectangle(w,drag_client.xor_gc, rect);

  XUndefineCursor(XtDisplay(w),XtWindow(w));

  /* capture final box coordinates */
  if(isShifted) {
     rect->x1 += (bev->x - rect->x2);
     rect->y1 += (bev->y - rect->y2);
  }
  rect->x2 = bev->x;
  rect->y2 = bev->y;

  xdev_center = (rect->x1 + rect->x2)/2;
  ydev_center = (rect->y1 + rect->y2)/2;
  temp1 = abs(rect->x2-rect->x1); if(temp1 == 0) temp1 = 1;
  temp2 = abs(rect->y2-rect->y1); if(temp2 == 0) temp2 = 1;
  xscale_ratio = (float) (abs(dev_xmax-dev_xmin)/((float) temp1));
  yscale_ratio = (float) (abs(dev_ymax-dev_ymin)/((float) temp2));
  nostretchy_scale = (yscale_ratio < xscale_ratio) ? yscale_ratio : xscale_ratio;
  if(xt_stretchy == 0) {
    xscale_ratio = yscale_ratio = nostretchy_scale;
  }

  /* convert to vplot coordinates */
  devtovpxy(xdev_center,dev_ymax-ydev_center,&vpxcenter,&vpycenter);
  devtovpxy(dev_xmin,dev_ymin,&vpxmin,&vpymin);
  devtovpxy(dev_xmax,dev_ymax,&vpxmax,&vpymax);


  /* launch new xtpen */
  strcpy(parfile,"par=");
  if(EXIT_SUCCESS == makezoompar(vpxcenter,vpycenter,xscale_ratio,yscale_ratio,nostretchy_scale,parfile+strlen(parfile)))
   {
     strcpy(cmdbuf,orig_argv0); /* original pen command name */
     cptr = strrchr(cmdbuf,'/'); /* want to drop S before x if SEP */
     if(cptr == ((char *) NULL)) cptr = &(cmdbuf[0]);
     else cptr++;
     if(cptr[0] == 'S') strcpy(cptr,cptr+1);
     (void) fcntl(XConnectionNumber(XtDisplay(w)),F_SETFD,FD_CLOEXEC|fcntl(XConnectionNumber(XtDisplay(w)),F_GETFD));
     waitpid = /*v*/fork();
     if(waitpid == 0) {
#ifdef notdef
        freopen(pltinname[0],"r",stdin);
#endif
        if(((FILE *) NULL) == freopen("/dev/null","r",stdin)) {
            perror("zoom stdin reopen");
        }
        rc = execlp(cmdbuf,cmdbuf,parfile,NULL);
        if(rc == -1) perror("Zoom attempt failed: ");
        _exit(EXIT_FAILURE);
     }
     if(waitpid == -1) {
        perror("Zoom attempt failed: ");
     } else {
        /* save process ID and parfile for atexit cleanup */
        nzooms++;
        zoom_list = (struct zoom_cleanup *)
            realloc(zoom_list, nzooms*sizeof(struct zoom_cleanup));
        zoom_list[nzooms-1].zoom_pid = waitpid;
        zoom_list[nzooms-1].parfile = strdup(parfile+4);
     }
  }
  imZoomingNow=False;
  }

  *cont_dispatch = True;
}

/*ARGSUSED*/
void zoom_proc(Widget w, XtPointer client_data, XtPointer call_data) {
    XGCValues gcv;

   if(!zoomInitialized) {
   /* use rubberband box to define new plot window limits */
    gcv.function = GXxor;
    gcv.foreground = ~0;
    
    drag_client.xor_gc = XCreateGC(XtDisplay(w),XtWindow(w), 
                          GCFunction|GCForeground, &gcv);
    CrossHairCursor = XCreateFontCursor(XtDisplay(w), XC_crosshair);


    XtAddEventHandler(w,ButtonPressMask,False,press_event,&drag_client);
    XtAddEventHandler(w,ButtonMotionMask,False,motion_event,&drag_client);
    XtAddEventHandler(w,ButtonReleaseMask,False,release_event,&drag_client);
    XtAddEventHandler(w,KeyPressMask|KeyReleaseMask,False,shift_event,&drag_client);
    zoomInitialized=True;

    }
    XDefineCursor(XtDisplay(w),XtWindow(w),CrossHairCursor);
    imZoomingNow=True;
}

/*ARGSUSED1*/
void actionZoom(
Widget w, XEvent *ev, String *p, Cardinal *np
                ) {  zoom_proc(w,NULL,NULL); }

/* the timeout proc called in run mode by an application timer */
/*ARGSUSED*/
void timeout_proc(XtPointer client_data, XtIntervalId *id){    didTIME = YES; return; }

void set_delay_label(float delay)
{ 
   Arg arg[1];
   char text[32];

   if( wantLabels ){
      sprintf( text, "delay %4.2f ", delay );
      XtSetArg( arg[0], XtNlabel, text );
      XtSetValues( delay_label , arg, ONE );
   }
}

/* procs to set pause between frames, these are not currently set as
 * button callbacks but they may be used in a translation table 
 */
/*ARGSUSED*/
void slower_proc(Widget w, XtPointer client_data, XtPointer call_data){ 
   if( fpause==0.0f ) fpause =.25f; 
   else fpause = fpause*2.0f; 
   if( fpause >=10.0f ) fpause =9.99f;

   set_delay_label(fpause);
}
/*ARGSUSED1*/
void actionSlower(
Widget w, XEvent *ev, String *p, Cardinal *np
                 ) { slower_proc(w,NULL,NULL); }

/*ARGSUSED*/
void faster_proc(Widget w, XtPointer client_data, XtPointer call_data){ 
   fpause = fpause*0.5f; 
   if( fpause < .05f ) fpause =0.05f;
   set_delay_label(fpause);
}
/*ARGSUSED1*/
void actionFaster(
Widget w, XEvent *ev, String *p, Cardinal *np
                 ) { faster_proc(w,NULL,NULL); }


/* the number that is updated by user keystrokes */
static int inputNumber=0;

/*ARGSUSED*/
void actionNumber(
Widget w,
XEvent *event,
String *params,
Cardinal *num_params)
{

    if( params && *params){
        int digits = strlen(*params);
        int value = atoi(*params);
        if( inputNumber == -1 ) inputNumber =0;
        while( digits > 0 ){
	    inputNumber *= 10;
	    digits--;
        }
        inputNumber += value;
    }
}

/* this is the global variable that is examained to see what the user typed */
int xt_next_num = -1;

void numberReset(void){
    inputNumber = -1;
}
/*ARGSUSED*/
void actionNumReset(
Widget w, XEvent *ev, String *p, Cardinal *np
                   ) { numberReset(); }


/* set next frame number to the users number and then do a next frame */
void gotoFrame(void){
    xt_next_num = inputNumber ;
    if( next_on == YES ) didNEXT = YES;    return;
}
/*ARGSUSED*/
void actionGoto(
Widget w, XEvent *ev, String *p, Cardinal *np
               ) { gotoFrame(); }


/* toggle run mode between Forward, Backward, Both-ways */

/* this routine sets the mode label */
void set_mode_label(char* newlab)
{ 
   Arg arg[1];
   char text[32];

   if( wantLabels ){
      strncpy( text, newlab, 32 );
      XtSetArg( arg[0], XtNlabel, text );
      XtSetValues( mode_button , arg, ONE );
   }
}
/* this is the global variable that controls run mode */
/* the enumeration is defined in xtpen.h */
int xt_run_mode=XT_FORWARD;

/*ARGSUSED*/
void toggle_run_mode(Widget w, XtPointer client_data, XtPointer call_data){
  switch( xt_run_mode ) {
    case XT_FORWARD:
        xt_run_mode = XT_BACKWARD;
	set_mode_label("Backwards");
	break;
    case XT_BACKWARD:
        xt_run_mode = XT_BOTH_WAYS;
	set_mode_label("Both Ways");
	break;
    case XT_BOTH_WAYS:
        xt_run_mode = XT_FORWARD;
	set_mode_label("Forwards ");
	break;
  }
	    
}
/*ARGSUSED1*/
void actionRunMode(
Widget w, XEvent *ev, String *p, Cardinal *np
                  ) { toggle_run_mode(w,NULL,NULL); }


void set_frame_label(int frame_num)
{ 
   Arg arg[1];
   char text[10];

   if( wantLabels ){
       sprintf( text, " %3d ", frame_num );
       XtSetArg( arg[0], XtNlabel, text );
       XtSetValues( frame_label , arg, ONE );
   }
}

/* this action procedure prints the event location to the file "interact" */
/*ARGSUSED*/
void actionCoord(
Widget w,
XEvent *event,
String *params,
Cardinal *numparams)
{
int x,y;
float ux,uy;

   if( interact[0] == '\0' ) return;
   if( imZoomingNow) return;

   if (xt_first_time == YES){
        outfp = fopen (interact, "w");
        if (outfp == NULL) {
            ERR (FATAL, name, "Can't open interact output file %s!", interact);
        }
        xt_first_time = NO;
   }

   if( event->type == ButtonPress ){
       x = XCORD (event->xbutton.x);
       y = YCORD (event->xbutton.y);
#ifdef notdef
       devtovpxy ( x, y, &x, &y );
       if( boxy ){
           doPointPopup( outfp, (float) x/RPERIN, (float) y/RPERIN );
       }else{
           fprintf( outfp,"%f\t%f\n",(float) x/RPERIN,(float) y/RPERIN );
           fflush( outfp );
       }
#else
       if(interactcoords == 1) { /* user coordinates */
          devtovpuxy ( x, y, &ux, &uy );
       } else if(interactcoords == 2) { /* raw vplot */
         devtovpxy ( x, y, &x, &y );
         ux = (float) x; uy = (float) y;
       } else { /* plot inches */
         devtovpxy ( x, y, &x, &y );
         ux = x/RPERIN; uy = y/RPERIN;
       }
       if( boxy ){
           doPointPopup( outfp, ux, uy );
       } else {
           fprintf( outfp,"%f\t%f\n",ux, uy );
           fflush( outfp );
       }
#endif

   }
}

void create_panel(
Widget parent,
int want_buttons,
int want_labels)
{
    wantButtons = want_buttons;
    wantLabels = want_labels;

    if( wantButtons ){
  
        next_button = XtVaCreateManagedWidget("Next",commandWidgetClass,
		parent, NULL);
        XtAddCallback(next_button, XtNcallback, next_proc, NULL);

        prev_button = XtVaCreateManagedWidget("Prev",commandWidgetClass,
		parent,NULL);
        XtAddCallback(prev_button, XtNcallback, prev_proc, NULL);

        quit_button = XtVaCreateManagedWidget("Quit",commandWidgetClass,
		parent,NULL);
        XtAddCallback(quit_button, XtNcallback, quit_proc, NULL);

        restart_button = XtVaCreateManagedWidget("Restart",commandWidgetClass,
		parent,NULL);
        XtAddCallback(restart_button, XtNcallback, restart_proc, NULL);

        run_button = XtVaCreateManagedWidget("Run",commandWidgetClass,
		parent,NULL);
        XtAddCallback(run_button, XtNcallback, run_proc, NULL);
 
        stop_button = XtVaCreateManagedWidget("Stop",commandWidgetClass,
		parent,NULL);
        XtAddCallback(stop_button, XtNcallback, stop_proc, NULL);   

        stretch_button = XtVaCreateManagedWidget("Stretchy",commandWidgetClass,
		parent,NULL);
        XtAddCallback(stretch_button, XtNcallback, stretch_proc, NULL);   
 	if( xt_stretchy )
	    XtSetValues( stretch_button, rigidArgs, XtNumber(rigidArgs) );

        mode_button = XtVaCreateManagedWidget("Forwards ",commandWidgetClass,
		parent,NULL);
        XtAddCallback(mode_button, XtNcallback, toggle_run_mode, NULL);   

    }

    if( wantLabels ){
        frame_label = XtCreateManagedWidget("  0 ",labelWidgetClass,parent,
			NULL,ZERO);

        delay_label = XtCreateManagedWidget("delay      ",
		labelWidgetClass,parent, NULL,ZERO);

    }

    return;
}
 
void activate_buttons(void)
{
    if( wantButtons ){
    /* turn on the buttons, if required */
    XtSetSensitive( next_button, (Boolean)( next_on == YES ) );
    XtSetSensitive( prev_button, (Boolean)( prev_on == YES ) );
    XtSetSensitive( quit_button, (Boolean)( quit_on == YES ) );
    XtSetSensitive( restart_button, (Boolean)( restart_on == YES ) );
    XtSetSensitive( run_button,  (Boolean)( run_on == YES ) );
    XtSetSensitive( stop_button, (Boolean)( stop_on == YES ) );
    XtSetSensitive( stretch_button, (Boolean)( size_on == YES ) );
    }
 
return;
}

void inactivate_buttons(void)
{
    /* turn all the buttons off */

    if( wantButtons ){
    XtSetSensitive( next_button, False );
    XtSetSensitive( prev_button, False );
    XtSetSensitive( quit_button, False );
    XtSetSensitive( restart_button, False );
    XtSetSensitive( run_button,  False );
    XtSetSensitive( stop_button, False );
    XtSetSensitive( stretch_button, False );
    }
   
    return;

}

int xt_pause( int doNEXT,int doPREV,int doREST,int doQUIT,int doRUN,int doSTOP,int doXSIZE)
{

    int retval;
    XtIntervalId id;


    next_on=doNEXT; prev_on=doPREV; quit_on=doQUIT; restart_on=doREST;
    run_on=doRUN;stop_on=doSTOP; size_on=doXSIZE;

    if( wantButtons ){
        activate_buttons();
    }

    if( doSTOP == YES ){ 
	unsigned long mpause; /* pause time in milliseconds */
	/* add a timeout event , add two millisecs to prevent zero */
        mpause = fpause*1000. + 2;
	didTIME = NO;
        id = XtAppAddTimeOut( pen_context,mpause, timeout_proc, NULL );

	/* we are in a run loop process events until the timer expires */
	/* only the stop and quit buttons will be active */

	while( !didTIME && !didSTOP && !didQUIT && !didNEXT && !didPREV ){
	    XtAppProcessEvent(pen_context,XtIMXEvent|XtIMTimer);
	}
	XtRemoveTimeOut(id);

	didTIME = NO;
	if( didSTOP ){
	    didSTOP=NO;	    return STOP;
        } else if (didQUIT) {
	       return  QUIT; 
	}else{
	    return TIME;
	}

    }else{
	/* if not in a run loop wait for a user event */
	while (!didNEXT && !didPREV && !didQUIT && 
	       !didFRAM1 && !didRUN && !didSTOP &&!didCHANGE) {
	    XtAppProcessEvent(pen_context,XtIMXEvent);
	}
  
	if (didNEXT) {
	        didNEXT = NO;	retval = NEXT;
        } else if (didPREV) {
		didPREV = NO;    retval = PREV; 
        } else if (didRUN) {
		didRUN = NO;     retval= RUN; 
        } else if (didSTOP) {
		didSTOP = NO;    retval = STOP; 
        } else if (didQUIT) {
	       retval = QUIT; 
        } else if (didFRAM1) {
		didFRAM1 = NO;	 retval = RESTART;  
        } else if (didCHANGE) {
		didCHANGE = NO;	 retval = CHANGED;  
        }
	if( wantButtons ){
	   inactivate_buttons();
        }
	return retval;
    }

}

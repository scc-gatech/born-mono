#ifndef VP_XTPEN_H
#define VP_XTPEN_H
/* include files */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include "../../include/vplot.h"
#include "../include/params.h"
#include "../include/enum.h"
#include "../include/err.h"
#include "../include/extern.h"

/* define a structure to reference the input files */
typedef struct
{
	FILE	*stream;
	char	name[MAXFLEN+1];
	int	new;
} FileInfo;
 
/* global definitions */
#define	TBORDER 22
#define	BORDER 5

/* global variables */
extern Display *pen_display;
extern int screen_depth;
extern int visual_depth;
extern long screen_black, screen_white;
extern Visual *pen_visual;
extern Window pen_window;
extern GC pen_gc;
extern XtAppContext pen_context;
extern Widget xtpen, vpane, buttonbox, pen_picture, text_region;
extern unsigned long *pen_colors;
extern int xt_endframe;

/* picture scaling info */
extern int xt_stretchy;
extern int dev_xmin, dev_xmax;
extern int dev_ymin, dev_ymax;
extern void xt_size_n_scale();

/* picture scaling info */
extern int want_images,greedy_pixmaps,see_progress;

/* interact output format */
extern int boxy;

extern int num_col;
extern unsigned long color;
extern int x_num_col;
extern int xmono;
extern int xtruecol; 
extern int skipit;
extern int xt_paused;
extern int xt_after_erase;

extern FileInfo *inFiles;
extern int mouse_clicked,cursx,cursy;

extern Drawable pen_drawable; /* drawable, could be the window or a pixmap */

extern int in_repaint;        /* Are we in the repaint proc ? */
extern int num_files;

/* Button functions */
extern void create_buttons();
extern void activate_buttons();
extern void inactivate_buttons();
extern float epause;   /* user specified pause in seconds */
extern float fpause; /* pause between frames in seconds */

extern void doPointPopup(FILE *, float, float);

/* "button press" values returned by xt_pause  */
enum {NEXT,PREV,RESTART,QUIT,RUN,STOP,CHANGED,TIME};  

/* "run_mode" values set by the mode function in xtcommands*/
extern int xt_run_mode;
enum {XT_FORWARD,XT_BACKWARD,XT_BOTH_WAYS};  

#include "xtpixmap.h"

/* inline coordinate transforms;  X origin- upper left, pen origin- lower left */
#define	XCORD(x)	x
#define	YCORD(y)	(dev_ymax - y)
#define	IXCORD(x)	x
#define	IYCORD(y)	(dev_ymax - y)
 
extern Colormap pen_colormap;

/* pixel map for color manipulation */
#define SEP_MAP_SIZE 65536
extern unsigned long map[SEP_MAP_SIZE];
/* do we own the whole colormap ? */
extern int own_colormap;

/* plotting_started is set on first entry to xt_dovplot */
extern int plotting_started;

/* size of the message buffer */
#define TEXT_BUFFER_SIZE 4096

extern int xt_after_break;

/* functions for dealing with the list of frames  and getting old images */

#include "xtframe.h"


extern xtFrameList *frame_list; /* current list of frames to draw */
extern xtFrame *redraw_frame;  /* the frame to redraw not necessarily the same 
	     			 as the current frame  because pauses occur
				 at the  start of the next frame */

/* appplication data to be read from the resource database */
typedef struct {
        Boolean stretchy;
        Boolean pixmaps;
        Boolean images;
        Boolean buttons;
        Boolean labels;
        Boolean textpane;
        int num_col;
        int vis_depth;
        float pause;
} AppData, *AppDataPtr;

extern AppData app_data;

extern int xt_app_data();

extern void xtclose(int);
extern void xt_flush_display(void);
extern int xtgetpoint(int *,int *);
extern void xt_save_colormap( xtFrame *frame);
extern void xt_draw_file(int,long,int *,long *,long *);
extern void xt_draw_frame(xtFrame *);
extern void xtredraw (void);

extern void PenRepaint(Widget,XEvent *,String *,Cardinal *);
extern void PopupConfirm(Widget,XtPointer,XtPointer);
extern void actionPointPopupConfirm(Widget,XEvent *,String *,Cardinal *);
extern void PopupCancel(Widget,XtPointer,XtPointer);
extern void dummy_proc(Widget,XtPointer,XtPointer);
extern void actionDummy(Widget,XEvent *,String *,Cardinal *);
extern void next_proc(Widget,XtPointer,XtPointer);
extern void actionNext(Widget,XEvent *,String *,Cardinal *);
extern void prev_proc(Widget,XtPointer,XtPointer);
extern void actionPrev(Widget,XEvent *,String *,Cardinal *);
extern void quit_proc(Widget,XtPointer,XtPointer);
extern void actionQuit(Widget,XEvent *,String *,Cardinal *);
extern void restart_proc(Widget,XtPointer,XtPointer);
extern void actionRestart(Widget,XEvent *,String *,Cardinal *);
extern void run_proc(Widget,XtPointer,XtPointer);
extern void actionRun(Widget,XEvent *,String *,Cardinal *);
extern void stop_proc(Widget,XtPointer,XtPointer);
extern void actionStop(Widget,XEvent *,String *,Cardinal *);
extern void stretch_proc(Widget,XtPointer,XtPointer);
extern void actionStretch(Widget,XEvent *,String *,Cardinal *);
extern void zoom_proc(Widget,XtPointer,XtPointer);
extern void actionZoom(Widget,XEvent *,String *,Cardinal *);
extern void timeout_proc(XtPointer, XtIntervalId *);
extern void set_delay_label(float);
extern void slower_proc(Widget,XtPointer,XtPointer);
extern void actionSlower(Widget,XEvent *,String *,Cardinal *);
extern void faster_proc(Widget,XtPointer,XtPointer);
extern void actionFaster(Widget,XEvent *,String *,Cardinal *);
extern void actionNumber(Widget,XEvent *,String *,Cardinal *);
extern void actionPointPopupCancel(Widget,XEvent *,String *,Cardinal *);
extern void numberReset(void);
extern void actionNumReset(Widget,XEvent *,String *,Cardinal *);
extern void gotoFrame(void);
extern void actionGoto(Widget,XEvent *,String *,Cardinal *);
extern void set_mode_label(char *);
extern void set_frame_label(int);
extern void toggle_run_mode(Widget,XtPointer,XtPointer);
extern void actionRunMode(Widget,XEvent *,String *,Cardinal *);
extern void actionCoord(Widget,XEvent *,String *,Cardinal *);
extern void create_panel(Widget,int,int);
extern int xt_pause(int,int,int,int,int,int,int);
extern void cleanup_zooms(void);

#endif

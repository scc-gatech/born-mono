#ifndef UI_RICK
#define UI_RICK 342
/*
user interface definitions

Supplies control panel, message box, and drawing surface
Mediates interactive commands
Maintains consist state between panel controls
Uses Motif
*/

#include "rick.h"

/* constants */
#define UI_WIDE		600
#define UI_HITE		600
#define	COLORBAR_THICK	25
#define NAXIS	3
#define	PICK_VALUE	0
#define	PICK_LINE	1
#define	PICK_REGION	2
#define UI_FONT "-*-courier-bold-r-normal--20-*-*-*-*-*"
#define SMALL_Z_DIR 0
#define LARGE_Z_DIR 1
#define SMALL_X_DIR 2 
#define LARGE_X_DIR 3
#define SMALL_Y_DIR 4
#define LARGE_Y_DIR 5
#define CANVAS_REFRESH_WAIT 500
#define COLORBAR_REFRESH_WAIT 500

/* UI object */
typedef struct {
	/* state variables */
	int	first;		/* first time called */
	int	style;		/* current view choice */
	int	across;		/* array across */
	int	down;		/* array down */
	int	delta;		/* array delta */
	int	start;		/* array start */
	float	width;		/* initial width */
	float	height;		/* initial height */
	int	wide;		/* canvas dimensions */
	int	hite;
	int	hzoom;		/* allow horizontal windowing */
	int	vzoom;		/* allow vertical windowing */
	int	x1, y1;
	int	x2, y2;
	int	v1, v2;		/* data values */
	int	timer;		/* timer value */
	int	pickmode;		/* pick state NONE, LINES, REGIONS */
        string font;		/* font type */
  
  int canvasActive;          /** state of canvas repainting **/
  int canvasActivePrev;      /** previous record of canvasActive **/
  int canvasStable;          /** state of canvas stability (stillness) **/
  int colorbarActive;        /** state of colorbar repainting **/
  int colorbarActivePrev;    /** previous record of colorbarActive **/
  int colorbarStable;        /** state of colorbar stability (stillness) **/

  int canvasWide;            /** sample width of canvas **/
  int canvasHite;            /** sample height of canvas **/
  int canvasX;               /** sample upper left x of canvas **/
  int canvasY;               /** sample upper left y of canvas **/
  int colorbarWide;          /** sample width of colorbar **/
  int colorbarHite;          /** sample height of colorbar **/
  int colorbarX;             /** sample upper left x of colorbar **/
  int colorbarY;             /** sample upper left y of colorbar **/

  int zoomDragValid;         /** valid to drag zoom box **/
  int growDragValid;         /** valid to drag grow box **/
  int eraseDragValid;        /** valid to drag erase box **/
  int pikDragValid;          /** valid to drag pik line **/
  int subvolumeDragValid;    /** valid to drag subvolume box **/
  int frameDragValid;        /** valid to drag movie bounds line **/
  int colorbarDragValid;     /** valid to drag colorbar line **/
  int movieOn;               /** movie was on when sampled **/
  int pathOn;                /** path animation was on when sampled **/
  int historyOn;             /** history animation was on when sampled **/
  int messageOn;             /** delayed message is waiting print **/
  int undoAnimate;           /** in process of undo animation **/
  int redoAnimate;           /** in process of redo animation **/
  int pathForwardAnimate;    /** in process of path animation forward **/
  int pathBackwardAnimate;   /** in process of path animation backward **/
  Message statusMessage;     /** delayed message **/
  int menuOn;                /** menus are visible **/
  int menuWait;              /** disable redraw following menu toggle **/
  
	/* base widgets */
	XtAppContext context;
	Widget	application;
	Widget	main;
	Widget	base;
	Widget	message;

	/* menu widgets */
	Widget	menuBar;
	Widget	menu;
	Widget	menu1;
	Widget	shape_stretch;
	Widget	shape_true;
	Widget	shape_pixel;

	/* control widgets */
        Widget	control1;
        Widget  control2;
	Widget	movieSpeed;
        Widget  velocityParam;
	Widget	contrast;
	Widget	contrast0;
        Widget  movieOnOff;
        Widget  movieDir;

	/* canvas widgets */
	Widget	colorbar;
	Widget	canvas;
	Widget	shell;

	/* array dialog */
	Widget	a_shell;
	Widget	a_base;
	Widget	a_dir;
	int	dir;
	Widget	a_down;
	Widget	a_across;
	Widget	a_start;
	Widget	a_delta;
	Widget	a_end;

	/* size dialog widget */
	Widget	s_shell;
	Widget	s_base;
	Widget	s_mins[4];
	Widget	s_label[4];
	Widget	s_maxs[4];
	Widget	s_frames[4];
	Widget	s_dframes[4];
	Widget	s_sizes[4];
	Widget	s_minv[4];
	Widget	s_maxv[4];
	Widget	s_sizev[4];
	Widget	s_framev[4];
	Widget	s_dframev[4];
	Widget	s_slist[20];
	Widget	s_vlist[20];
	int	s_alist[20];
	int	s_nslider;
        Widget  p_shell;
        Widget  p_base;
        Widget  p_label;

	/* finish panel */
	Widget	d_shell;
	Widget	d_base;
	Widget	d_fat;

        /* text panel */
	Widget	text_shell;
	Widget	text_base;
	Widget	text_label;
	Widget	text_fat;
	Widget	text_rotate;
	Widget	text_scale;

	/*pik panel */
	Widget	pik_shell;
	Widget	pik_base;
	Widget	pik_text;
	Widget  pik_radius; 
	Widget  pik_npaths;
	Widget  pik_nwind;
	Widget  pik_runcor;
	Widget  pik_j;
        Widget  pik_search;
        Widget  pik_min_tol;
        Widget  pik_max_tol;
        Widget  pik_dtol;

	/* syze panel */
	Widget	z_shell;
	Widget	z_base;
	Widget	z_label[4];
	Widget	z_min[4];
	Widget	z_max[4];
	Widget	z_frame[4];
	Widget	z_pixels[4];
	/* label panel widgets */
	Widget	l_shell;
	Widget	l_base;
	Widget	l_title;
	Widget	l_label[4];
	Widget	l_first[4];
	Widget	l_delta[4];
	Widget	l_tic0[4];
	Widget	l_dtic[4];
	Widget	l_tic2[4];

	/*Orient Raise */
	Widget  o_shell;
	Widget  o_base;
	Widget  o_axis[5];
	Widget  o_pos[5];
	

	/* Cubeplot panel widgets */
	Widget	vplot_shell;
	Widget	vplot_base;
	Widget  vplot_xll;
	Widget  vplot_xur;
	Widget  vplot_yll;
	Widget  vplot_yur;


	/* transparent panel widgets */
	Widget	t_shell;
	Widget	t_base;
	/* fence control parameters */
	Widget	f_shell;
	Widget	f_base;
	/* help dialog widget */
	Widget	i_shell;
	Widget	i_base;
	Widget	i_text;
	} *UI;

/* API */
extern XID UIColorbarWindow (void);
extern int UIPikMove (Widget widget,XButtonEvent *event);

#endif

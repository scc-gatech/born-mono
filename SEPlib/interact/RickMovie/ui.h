#ifndef RICKMOVIE_UI_H
#define RICKMOVIE_UI_H
/*
user interface definitions

Supplies control panel, message box, and drawing surface
Mediates interactive commands
Maintains consist state between panel controls
Uses Motif
*/

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include "map.h"

/* constants */
#define UI_WIDE		600
#define UI_HITE		600
#define	COLORBAR_THICK	25
#define NAXIS	3
#define	PICK_VALUE	0
#define	PICK_LINE	1
#define	PICK_REGION	2
#define UI_FONT "-*-courier-bold-r-normal--20-*-*-*-*-*"

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
	Widget	control;
	Widget	speed;
	Widget	contrast;
	Widget	contrast0;
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

/* typed returns */
extern void UIMenuInit (Widget parent);
extern void UIStyleChoice (Widget widget,int item);
extern void UIColorChoice (Widget widget,int item);
extern int UIScreen(void);
extern XID UIBarWindow();
extern XID UIMainWindow(void);
extern void UIMessage (Message message);
extern void UISaveMessage (Message message);
extern XID UICanvasWindow (void);
extern void UIColorbarSize (int *wide,int *hite);
extern XID UIColorbarWindow (void);
extern int UIFirst (void);
extern XFontStruct* UIFont (int size);

extern void UIMarkChoice ( Widget widget, int item);
extern void UIBackgroundChoice ( Widget widget, int item);
extern void UINeighborhoodChoice (Widget widget,int item);
extern void UIStatusChoice (Widget widget,int item);
extern void UIOverlayChoice ( Widget widget, int item);
extern void UIHelpChoice (Widget widget,int item);
extern void UIHelpPrint (char *start,char *finish);
extern void UIMouseInfo (void);
extern void UIDumpFloats (void);
extern void UIDumpFloats2 ( Widget widget, XtPointer stuff, XmFileSelectionBoxCallbackStruct *cbs);
extern void UIDumpBytes (void);
extern void UIDumpBytes2 ( Widget widget, XtPointer stuff, XmFileSelectionBoxCallbackStruct *cbs);
extern void UISavePar (void);
extern void UISavePar2 ( Widget widget, XtPointer stuff, XmFileSelectionBoxCallbackStruct *cbs);
extern void UIQuit (void);
extern void UIOrient0 (void);
extern void UIDownDump (void);
extern void UIAcrossDump (void);
extern void UIDeepDump (void);
extern void UISwapFrontSide (void);
extern void UISwapSideTop (void);
extern void UISwapTopFront (void);
extern void UISwapFrontExtra (void);
extern void UISwapSideExtra (void);
extern void UISwapTopExtra (void);
extern void UIFlipDown (void);
extern void UIFlipAcross (void);
extern void UIFlipDeep (void);
extern void UISizeChoice ( Widget widget, int item);
extern void UIInterpolateToggle (Widget widget);
extern void UISize0 (void);
extern void UIScreen0 (void);
extern void UISubvolumeSmooth (void);
extern void UISmoothUndo (void);
extern void UIPickClear ( Widget widget, XButtonEvent *event);
extern void UIEditGrade (void);
extern void UIStatistics (void);
extern void UIStatistics (void);
extern void UIWakeup (void);
extern void UIPikWrite (void);
extern void UIPikWrite2 ( Widget widget, XtPointer stuff, XmFileSelectionBoxCallbackStruct *cbs);
extern void UIPikRead (void);
extern void UIPikRead2 ( Widget widget, XtPointer stuff, XmFileSelectionBoxCallbackStruct *cbs);
extern void UISaveFront (void);
extern void UISaveFront2 ( Widget widget, XtPointer stuff, XmFileSelectionBoxCallbackStruct *cbs);
extern void UISaveSide (void);
extern void UISaveSide2 ( Widget widget, XtPointer stuff, XmFileSelectionBoxCallbackStruct *cbs);
extern void UISaveTop (void);
extern void UISaveTop2 ( Widget widget, XtPointer stuff, XmFileSelectionBoxCallbackStruct *cbs);
extern void UISaveAcross (void);
extern void UISaveAcross2 ( Widget widget, XtPointer stuff, XmFileSelectionBoxCallbackStruct *cbs);
extern void UISaveDeep (void);
extern void UISaveDeep2 (Widget widget,XtPointer stuff, XmFileSelectionBoxCallbackStruct *cbs);
void UISaveDown (void);
extern void UISaveDown2 ( Widget widget, XtPointer stuff, XmFileSelectionBoxCallbackStruct *cbs);

extern int EditFrame ( Buffer data, Map map1, Map map2, Map map3, int frame, float dist[256]);
extern int EditBox ( Buffer data, Map map1, Map map2, Map map3, float dist[256]);
extern void EditStats (int n,float *dist,int *low,int *median,int *high);
extern int EditCube ( Buffer data, Map map1, Map map2, Map map3, float dist[256]);
extern void EditGrade (Buffer data,Map map1,Map map2,Map map3);
extern void EditUndo (Buffer data);

extern void UIPikQuery ( Widget widget, XButtonEvent *event);
extern void UIPikDelete ( Widget widget, XButtonEvent *event);
extern void UIPikMove (Widget widget, XButtonEvent *event);
extern void UISubvolumeStart ( Widget widget, XButtonEvent *event);
extern void UIPickInsert ( Widget widget, XButtonEvent *event);
extern void UIPikAdd ( Widget widget, XButtonEvent *event);
extern void UIFrameEnd ( Widget widget, XButtonEvent *event);
extern void UIFrameStart ( Widget widget, XButtonEvent *event);
extern void UIPick ( Widget widget, XButtonEvent *event);
extern void UISubvolumeEnd ( Widget widget, XButtonEvent *event);
extern void UIColorbarEnd ( Widget widget, XButtonEvent *event);
extern void UIZoomEnd ( Widget widget, XButtonEvent *event);
extern void UIFrameDrag ( Widget widget, XButtonEvent *event);
extern void UISubvolumeDrag ( Widget widget, XButtonEvent *event);
extern void UIColorbarDrag ( Widget widget, XButtonEvent *event);
extern void UIZoomDrag ( Widget widget, XButtonEvent *event);
extern void UIColorbarStart ( Widget widget, XButtonEvent *event);
extern void UIVZoomStart ( Widget widget, XButtonEvent *event);
extern void UIHZoomStart ( Widget widget, XButtonEvent *event);
extern void UIZoomStart ( Widget widget, XButtonEvent *event);
extern void UIDrawCanvas (Widget widget);
extern void UIDrawColorbar (Widget widget);
extern void UIControlInit1 (Widget parent);
extern void UIMovie (Widget widget,int item);
extern void UIDirection (Widget widget,int item);
extern void UIControlInit2 (Widget parent);
extern int UIGetToggle (Widget widget);
extern void UIToggleSet (Widget widget,int state);
extern void UISetSlider (Widget widget, float value);
extern int UIGetSlider (Widget widget);
extern void UISpeed ( Widget widget, XtPointer client, XmScaleCallbackStruct *data);
extern void UIContrast ( Widget widget, XtPointer client, XmScaleCallbackStruct *data);
extern void UIContrast0 ( Widget widget, XtPointer client, XmScaleCallbackStruct *data);
extern void UIResetContrast (void);
extern void UISizeRaise (void);
extern void UISizeInit (void);
extern void UISizeDraw (void);
extern void UISizeReset (void);
extern void UISizeInitial (void);
extern void UISizeClose (void);
extern void UISizeSlider (Widget widget);
extern void UISizeText (Widget widget);
extern void UIArrayInit (void);
extern void UIArrayDir (Widget widget,int item);
extern void UIArrayRaise (void);
extern void UIArrayReset (int dir);
extern void UIArrayDraw (void);
extern void UIArrayClose (void);
extern void UIArrayEndAdjust (void);
extern void UIArrayDeltaAdjust (void);
extern void UIArrayShape ( int n, int *across, int *down, int *delta);
extern void UILabelInit (void);
extern void UILabelRaise (void);
extern void UILabelReset (void);
extern void UILabelDraw (void);
extern void UILabelClose (void);
extern void UITranspInit (void);
extern void UITranspRaise (void);
extern void UITranspClose (void);
extern void UITranspLow ( Widget widget, XtPointer client, XmScaleCallbackStruct *data);
extern void UITranspHigh ( Widget widget, XtPointer client, XmScaleCallbackStruct *data);
extern void UITranspGradient ( Widget widget, XtPointer client, XmScaleCallbackStruct *data);
extern void UITranspRate (Widget widget, int item);
extern void UIFenceInit (void);
extern void UIFenceRaise (void);
extern void UIFenceClose (void);
extern void UIFenceFront (void);
extern void UIFenceSide (void);
extern void UIFenceTop (void);
extern void UIFenceOpacity ( Widget widget, XtPointer client, XmScaleCallbackStruct *data);
extern void UIInfoInit (void);
extern void UIInfo (char *text);
extern void UIInfoClose (void);
extern void UISyzeRaise (void);
extern void UISyzeInit (void);
extern void UISyzeReset (void);
extern void UISyzeClose (void);
extern void UISyzeInitial (void);
extern void UISyzeDraw (void);
extern void UICanvasSize (int *wide,int *hite);
extern void UIGradeUndo (void);
extern void UIWindowInit (Widget parent);
extern void UITimer ( int delay, XtTimerCallbackProc action);
extern void UIMain (void);
extern void UIInit (int argc, char **argv);
extern Display* UIDisplay (void);

#endif

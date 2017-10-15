#include <sepConfig.h>
#if defined (HAVE_MOTIF) || defined(HAVE_ATHENA)
/*
user interface: menu setup and callbacks
*/
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <Xm/FileSB.h>
#include <Xm/XmAll.h>
#include <stdio.h>
#include "main.h"
#include "axis.h"
#include "data.h"
#include "map.h"
#include "render.h"
#include "view.h"
#include "region.h"
#include "pick.h"
#include "plane.h"
#include "pik.h"
#include "movie.h"
#include "section.h"
#include "color.h"
#include "colorbar.h"
#include "draw.h"
#include "ui.h"

extern UI ui;
extern Message message;

/* setup menus; install callbacks */

/* fetch list of menu item widgets */
#define	CS	XmStringCreateSimple
#define LIST(menu)	XtVaGetValues (ui->menu,XmNchildren,&list,NULL)
/* install callback in push button menu item */
#define PBCALLBACK(item,callback)	{\
	XtAddCallback (list[item],XmNactivateCallback,(XtCallbackProc)callback,NULL);}
/* install callback in toggle menu item */
#define TGCALLBACK(item,callback)	{\
	XtAddCallback (list[item],XmNvalueChangedCallback,(XtCallbackProc)callback,NULL);}

void UIMenuInit (Widget parent)
	{
	int i, j, ibar=0;
	WidgetList list;
	int *cb[3];

	/* menubar */
	if (!ui) return;
	ui->menuBar = XmVaCreateSimpleMenuBar (parent, "menubar",
		XmVaCASCADEBUTTON, CS("Main"), NULL,
		XmVaCASCADEBUTTON, CS("View"), NULL,
		XmVaCASCADEBUTTON, CS("Orient"), NULL,
		XmVaCASCADEBUTTON, CS("Size"), NULL,
		XmVaCASCADEBUTTON, CS("Navigate"), NULL,
		XmVaCASCADEBUTTON, CS("Color"), NULL,
		XmVaCASCADEBUTTON, CS("Pick"), NULL,
		XmVaCASCADEBUTTON, CS("EditVol"), NULL,
		XmVaCASCADEBUTTON, CS("Sections"), NULL,
		XmVaCASCADEBUTTON, CS("Status"), NULL,
		XmVaCASCADEBUTTON, CS("Help"), NULL,
		NULL);
	/* main drop down menu */
	ui->menu = XmVaCreateSimplePulldownMenu (ui->menuBar,"main",ibar++,NULL,
		XmVaPUSHBUTTON, CS("Redraw"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Fix Picking"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Write vgrid file (floats) ..."), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Write vgrid file (bytes) ..."), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Write parameter restart file ..."), NULL, NULL, NULL,
/*		XmVaCASCADEBUTTON, CS("Debug"), NULL,*/
/*		XmVaSEPARATOR,*/
		XmVaPUSHBUTTON, CS("Quit"), NULL, NULL, NULL,
		NULL);
	LIST (menu);
	PBCALLBACK (0,ViewDrawAll);
	PBCALLBACK (1,UIWakeup);
	PBCALLBACK (2,UIDumpFloats);
	PBCALLBACK (3,UIDumpBytes);
	PBCALLBACK (4,UISavePar);
	PBCALLBACK (5,UIQuit);
	/* debug pull right menu */
/*
	ui->menu1 = XmVaCreateSimplePulldownMenu (ui->menu,"debug",4,NULL,
		XmVaTITLE, CS("ARRAY->FILE DUMPS:"),
		XmVaPUSHBUTTON, CS("Down Axis"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Across Axis"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Deep Axis"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Render Color Map"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Render Image"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Color Table"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Sub-volume Picks"), NULL, NULL, NULL,
		NULL);
	LIST (menu1);
	PBCALLBACK (1,UIDownDump);
	PBCALLBACK (2,UIAcrossDump);
	PBCALLBACK (3,UIDeepDump);
	PBCALLBACK (4,RenderMapDump);
	PBCALLBACK (5,RenderImageDump);
	PBCALLBACK (6,ColorTableDump);
	PBCALLBACK (7,RegionDump);
	PBCALLBACK (8,UIWakeup);
	/* view drop down menu */
	ui->menu = XmVaCreateSimplePulldownMenu (ui->menuBar,"view",ibar++,(XtCallbackProc)UIStyleChoice,
		XmVaRADIOBUTTON, CS("Front Face"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Side Face"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Top Face"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Three Faces"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Array ..."), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Cube"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Fence ..."), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Transparent Cube ..."), NULL, NULL, NULL,
		XmNradioAlwaysOne, True,
		XmNradioBehavior, True,
		NULL);
	LIST (menu);
	XtVaSetValues (list[VIEW_CUBE],XmNset,True,NULL);
	/* orientation drop down menu */
	ui->menu = XmVaCreateSimplePulldownMenu (ui->menuBar,"orient",ibar++,NULL,
		XmVaPUSHBUTTON, CS("Swap Side & Top Faces; Transpose"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Swap Front & Side Faces"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Swap Top & Front Faces"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Swap Deep & 4th Axes"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Swap Across & 4th Axes"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Swap Down & 4th Axes"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Flip Down Direction"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Flip Across Direction"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Flip Deep Direction"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Move Frames to Middle"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Move Frames to Corner"), NULL, NULL, NULL,
/*		XmVaSEPARATOR,*/
		XmVaPUSHBUTTON, CS("Initial Orientation"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Adjust Labels ..."), NULL, NULL, NULL,
/*		XmVaSEPARATOR,*/
		XmVaTITLE, CS("4th AXIS IS n4= AND HIDDEN"),
		NULL);
	LIST (menu);
	PBCALLBACK (0,UISwapSideTop);
	PBCALLBACK (1,UISwapFrontSide);
	PBCALLBACK (2,UISwapTopFront);
	PBCALLBACK (5,UISwapFrontExtra);
	PBCALLBACK (4,UISwapSideExtra);
	PBCALLBACK (3,UISwapTopExtra);
	PBCALLBACK (6,UIFlipDown);
	PBCALLBACK (7,UIFlipAcross);
	PBCALLBACK (8,UIFlipDeep);
	PBCALLBACK (9,ViewFramesMiddle);
	PBCALLBACK (10,ViewFramesOrigin);
	PBCALLBACK (11,UIOrient0);
	PBCALLBACK (12,UILabelRaise);
	ui->menu = XmVaCreateSimplePulldownMenu (ui->menuBar,"size",ibar++,(XtCallbackProc)UISizeChoice,
		XmVaRADIOBUTTON, CS("Even Cube"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("True Proportions"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Sample Per Pixel"), NULL, NULL, NULL,
/*		XmVaSEPARATOR,*/
		XmVaTOGGLEBUTTON, CS("Interpolate"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Fine Control ..."), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Initial Size"), NULL, NULL, NULL,
/*		XmVaSEPARATOR,*/
		XmVaTITLE, CS("LEFT MOUSE BOX ZOOMS"),
		XmVaTITLE, CS("+ 'h' ONLY HORZ"),
		XmVaTITLE, CS("+ 'v' ONLY VERT"),
		NULL);
	/* save interpolation state widget */
	LIST (menu);
	ui->shape_stretch = list[0];
	ui->shape_true = list[1];
	ui->shape_pixel = list[2];
	/* set shape toggle value */
	XtVaSetValues (list[0],XmNset,True,NULL);
	XtVaSetValues (list[4],XmNset,False,NULL);
	TGCALLBACK (3,UIInterpolateToggle);
	PBCALLBACK (4,UISyzeRaise);
	PBCALLBACK (5,UISize0);
	/* movie drop down menu */
	ui->menu = XmVaCreateSimplePulldownMenu (ui->menuBar,"movie",ibar++,NULL,
		XmVaPUSHBUTTON, CS("Movie to Boundaries"), NULL, NULL, NULL,
		XmVaTOGGLEBUTTON, CS("High Speed"), NULL, NULL, NULL,
/*		XmVaSEPARATOR,*/
		XmVaTITLE, CS ("MIDDLE MOUSE CLICK XFRAMES"),
		XmVaTITLE, CS ("MIDDLE MOUSE DRAG MOVIE BOUNDS"),
		XmNradioAlwaysOne, True,
		NULL);
	LIST (menu);
	PBCALLBACK (0,ViewMovieFullBounds);
	TGCALLBACK (1,MovieToggleCache);
	XtVaSetValues (list[1],XmNset,False,NULL);
	/* color drop down menu */
	ui->menu = XmVaCreateSimplePulldownMenu (ui->menuBar,"color",ibar++,(XtCallbackProc)UIColorChoice,
		XmVaRADIOBUTTON, CS("Gray"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Straw"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Flag"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Tiger"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Blue"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Rainbow"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("AVO"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Veldata"), NULL, NULL, NULL,
/*		XmVaSEPARATOR,*/
		XmVaCASCADEBUTTON, CS("Overlay"), NULL,
		XmVaCASCADEBUTTON, CS("Mark"), NULL,
		XmVaCASCADEBUTTON, CS("Background"), NULL,
/*		XmVaSEPARATOR,*/
		XmVaPUSHBUTTON, CS("Flip Polarity"), NULL, NULL, NULL,
		XmNradioBehavior, True,
		XmNradioAlwaysOne, True,
		NULL);
	LIST (menu);
	PBCALLBACK (11,RenderTogglePolarity);
	XtVaSetValues (list[0],XmNset,True,NULL);
	/* overlay color pull right menu */
	ui->menu1 = XmVaCreateSimplePulldownMenu (ui->menu,"overlay",8,(XtCallbackProc)UIOverlayChoice,
		XmVaRADIOBUTTON, CS("Red"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Green"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("White"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Black"), NULL, NULL, NULL,
		XmNradioBehavior, True,
		XmNradioAlwaysOne, True,
		NULL);
	LIST (menu1);
	XtVaSetValues (list[COLOR_OVERLAY],XmNset,True,NULL);
	/* background color pull right menu */
	ui->menu1 = XmVaCreateSimplePulldownMenu (ui->menu,"background",10,(XtCallbackProc)UIBackgroundChoice,
		XmVaRADIOBUTTON, CS("Redish"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Greenish"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Light Gray"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Dark Gray"), NULL, NULL, NULL,
		XmNradioBehavior, True,
		XmNradioAlwaysOne, True,
		NULL);
	LIST (menu1);
	XtVaSetValues (list[COLOR_BACKGROUND],XmNset,True,NULL);
	/* mark color pull right menu */
	ui->menu1 = XmVaCreateSimplePulldownMenu (ui->menu,"mark",9,(XtCallbackProc)UIMarkChoice,
		XmVaRADIOBUTTON, CS("Red"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Green"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("White"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Black"), NULL, NULL, NULL,
		XmNradioBehavior, True,
		XmNradioAlwaysOne, True,
		NULL);
	LIST (menu1);
	XtVaSetValues (list[COLOR_MARK],XmNset,True,NULL);
	/* pick pulldown menu */
	ui->menu = XmVaCreateSimplePulldownMenu (ui->menuBar,"picks",ibar++,NULL,
		XmVaPUSHBUTTON, CS("Show/Hide picks"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Fix Picking"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Write to file"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Read from file"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Increase range"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Decrease range"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Increase draw size"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Decrease draw size"), NULL, NULL, NULL,
/*		XmVaSEPARATOR,*/
		XmVaTITLE, CS("RIGHT MOUSE MAKES PICK"),
		XmVaTITLE, CS("+ 'a' KEY ADDS POINT"),
		XmVaTITLE, CS("+ 'm' KEY MOVES NEAREST POINT"),
		XmVaTITLE, CS("+ 'd' KEY DELETES NEAREST POINT"),
		XmVaTITLE, CS("+ 'q' or '?' KEY INQUIRES NEAREST POINT"),
		XmVaTITLE, CS("+ 's' KEY SELECTS SMOOTH REGION"),
		NULL);
	LIST (menu);
	PBCALLBACK (0,ViewTogglePick);
	PBCALLBACK (1,UIWakeup);
	PBCALLBACK (2,UIPikWrite);
	PBCALLBACK (3,UIPikRead);
	PBCALLBACK (4,PikIncreaseRange);
	PBCALLBACK (5,PikDecreaseRange);
	PBCALLBACK (6,PikIncreaseSize);
	PBCALLBACK (7,PikDecreaseSize);
	/* edit drop down menu */
	ui->menu = XmVaCreateSimplePulldownMenu (ui->menuBar,"edit",ibar++,NULL,
		XmVaPUSHBUTTON, CS("Clear Blob Pick"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Fill Blob With Edge Value"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Undo Blob Fill"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Interpolate Current Cube Edge Values Across Cube"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Undo Cube Edge Interpolation"), NULL, NULL, NULL,
/*		XmVaSEPARATOR,*/
		XmVaTITLE, CS("'s' KEY + RIGHT MOUSE DRAG PICKS SUBVOL"),
		XmVaTITLE, CS("SET SMOOTH VALUE FROM COLORBAR"),
		NULL);
	LIST (menu);
	PBCALLBACK (0,UIPickClear);
	PBCALLBACK (1,UISubvolumeSmooth);
	PBCALLBACK (2,UISmoothUndo);
	PBCALLBACK (3,UIEditGrade);
	PBCALLBACK (4,UIGradeUndo);
	/* status drop down menu */
	ui->menu = XmVaCreateSimplePulldownMenu (ui->menuBar,"sections",ibar++,NULL,
		XmVaCASCADEBUTTON, CS("On screen wiggle plot"), NULL,
		XmVaCASCADEBUTTON, CS("On screen contour plot"), NULL,
		XmVaCASCADEBUTTON, CS("On screen profile graph"), NULL,
		XmVaCASCADEBUTTON, CS("Print wiggle plot"), NULL,
		XmVaCASCADEBUTTON, CS("Print contour plot"), NULL,
		XmVaCASCADEBUTTON, CS("Print profile graph"), NULL,
		XmVaCASCADEBUTTON, CS("Save section in file"), NULL,
		XmVaCASCADEBUTTON, CS("Save profile in file"), NULL,
/*		XmVaSEPARATOR,*/
		XmVaTITLE, CS("CROSS HAIRS SET PLANES & PROFILES"),
		NULL);
	ui->menu1 = XmVaCreateSimplePulldownMenu (ui->menu,"wiggle",0,NULL,
		XmVaPUSHBUTTON, CS("Front"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Side"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Top"), NULL, NULL, NULL,
		NULL);
	LIST (menu1);
	PBCALLBACK (0,PlotFrontWiggle);
	PBCALLBACK (1,PlotSideWiggle);
	PBCALLBACK (2,PlotTopWiggle);
	ui->menu1 = XmVaCreateSimplePulldownMenu (ui->menu,"contour",1,NULL,
		XmVaPUSHBUTTON, CS("Front"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Side"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Top"), NULL, NULL, NULL,
		NULL);
	LIST (menu1);
	PBCALLBACK (0,PlotFrontContour);
	PBCALLBACK (1,PlotSideContour);
	PBCALLBACK (2,PlotTopContour);
	ui->menu1 = XmVaCreateSimplePulldownMenu (ui->menu,"profile",2,NULL,
		XmVaPUSHBUTTON, CS("Down"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Across"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Deep"), NULL, NULL, NULL,
		NULL);
	LIST (menu1);
	PBCALLBACK (0,PlotDownProfile);
	PBCALLBACK (1,PlotAcrossProfile);
	PBCALLBACK (2,PlotDeepProfile);
	ui->menu1 = XmVaCreateSimplePulldownMenu (ui->menu,"prwiggle",3,NULL,
		XmVaPUSHBUTTON, CS("Front"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Side"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Top"), NULL, NULL, NULL,
		NULL);
	LIST (menu1);
	PBCALLBACK (0,PrintFrontWiggle);
	PBCALLBACK (1,PrintSideWiggle);
	PBCALLBACK (2,PrintTopWiggle);
	ui->menu1 = XmVaCreateSimplePulldownMenu (ui->menu,"prcontour",4,NULL,
		XmVaPUSHBUTTON, CS("Front"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Side"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Top"), NULL, NULL, NULL,
		NULL);
	LIST (menu1);
	PBCALLBACK (0,PrintFrontContour);
	PBCALLBACK (1,PrintSideContour);
	PBCALLBACK (2,PrintTopContour);
	ui->menu1 = XmVaCreateSimplePulldownMenu (ui->menu,"prprofile",5,NULL,
		XmVaPUSHBUTTON, CS("Down"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Across"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Deep"), NULL, NULL, NULL,
		NULL);
	LIST (menu1);
	PBCALLBACK (0,PrintDownProfile);
	PBCALLBACK (1,PrintAcrossProfile);
	PBCALLBACK (2,PrintDeepProfile);
	ui->menu1 = XmVaCreateSimplePulldownMenu (ui->menu,"savecontour",6,NULL,
		XmVaPUSHBUTTON, CS("Front"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Side"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Top"), NULL, NULL, NULL,
		NULL);
	LIST (menu1);
	PBCALLBACK (0,UISaveFront);
	PBCALLBACK (1,UISaveSide);
	PBCALLBACK (2,UISaveTop);
	ui->menu1 = XmVaCreateSimplePulldownMenu (ui->menu,"prprofile",7,NULL,
		XmVaPUSHBUTTON, CS("Down"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Across"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Deep"), NULL, NULL, NULL,
		NULL);
	LIST (menu1);
	PBCALLBACK (0,UISaveDown);
	PBCALLBACK (1,UISaveAcross);
	PBCALLBACK (2,UISaveDeep);
	ui->menu = XmVaCreateSimplePulldownMenu (ui->menuBar,"status",ibar++,(XtCallbackProc)UIStatusChoice,
		XmVaPUSHBUTTON, CS("Dataset"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Data Values"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Data Axis0"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Data Axis1"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Data Axis2"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Data Axis3"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Data Axis4"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Data Axis5"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("View"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Down Axis"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Across Axis"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Deep Axis"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("4D Axis"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("5D Axis"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Color Axis"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Color"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Render"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Graphics"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Mouse Buttons"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Movie"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Picks"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Pick List"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Piks"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Edit"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Frame List"), NULL, NULL, NULL,
		NULL);
	ui->menu = XmVaCreateSimplePulldownMenu (ui->menuBar,"help",ibar++,(XtCallbackProc)UIHelpChoice,
		XmVaTITLE, CS("MORE'D TO TERMINAL WINDOW"),
		XmVaPUSHBUTTON, CS("Command Line Args"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("File Formats"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Display Windows"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Mouse Usage"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Main Functions"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("View Functions"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Orient Functions"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Size Functions"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Movie Functions"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Color Functions"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Pick Functions"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Edit Functions"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Section Functions"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Status Functions"), NULL, NULL, NULL,
		NULL);
	XtManageChild (ui->menuBar);
	}

/* view choice callback */
void UIStyleChoice (Widget widget,int item)
	{
	extern View view;

	/* set array parameters according to view */
	if (!ui) return;
	if (ui->shell) {
		XtPopdown (ui->shell);
		ui->shell = 0;
		}
	switch (item) {
	case VIEW_ARRAY:
		MovieOff();
		UIArrayRaise ();
		ViewArray (ui->across,ui->down,ui->start,ui->delta);
		break;
	case VIEW_FRONT:
		ViewSetMovie (MOVIE_FRONT);
		break;
	case VIEW_SIDE:
		ViewSetMovie (MOVIE_SIDE);
		break;
	case VIEW_TOP:
		ViewSetMovie (MOVIE_TOP);
		break;
	case VIEW_FENCE:
		MovieOff ();
		UIFenceRaise ();
		break;
	case VIEW_TRANSP:
		MovieOff();
		UITranspRaise ();
		UIToggleSet (ui->shape_stretch,0);
		UIToggleSet (ui->shape_true,0);
		UIToggleSet (ui->shape_pixel,1);
		break;
		}
	ViewSetStyle (item);
	ui->style = item;
	UISyzeReset ();
	}


/* color choice callback */
void UIColorChoice (Widget widget,int item)
	{
	if (item > 8) return;
	ColorSetChoice (item);
	ColorSwitch ();
	}

/* overlay choice callback */
void UIOverlayChoice ( Widget widget, int item)
	{
	ColorSetOverlay (item);
	ColorSwitch ();
	}

/* backround choice callback */
void UIMarkChoice ( Widget widget, int item)
	{
	if (item > 4) return;
	ColorSetMark (item);
	}

/* background choice callback */
void UIBackgroundChoice ( Widget widget, int item)
	{
	if (item > 4) return;
	ColorSetBackground (item);
	}

/* region neighborhood callback */
void UINeighborhoodChoice (Widget widget,int item)
	{
	switch (item) {
	case 0: RegionSetNeighborhood (MARK_FACE); break;
	case 1: RegionSetNeighborhood (MARK_FACE|MARK_EDGE); break;
	case 2: RegionSetNeighborhood (MARK_FACE|MARK_EDGE|MARK_CORNER); break;
		}
	}

/* status choice callback */
void UIStatusChoice (Widget widget,int item)
	{
	extern Data data;
	extern View view;

	/* switch on status menu entry */
	if (!view || !data) return;
	switch (item) {
	case 0: DataInfo(data); break;
	case 1: DataValueInfo(data); break;
	case 2: AxisInfo(DataAxis(data,DATA_VALUE)); break;
	case 3: AxisInfo(DataAxis(data,DATA_AXIS1)); break;
	case 4: AxisInfo(DataAxis(data,DATA_AXIS2)); break;
	case 5: AxisInfo(DataAxis(data,DATA_AXIS3)); break;
	case 6: AxisInfo(DataAxis(data,DATA_AXIS4)); break;
	case 7: AxisInfo(DataAxis(data,DATA_AXIS5)); break;
	case 8: ViewInfo(view); break;
	case 9: MapInfo(ViewMap(view,AXIS_DOWN)); break;
	case 10: MapInfo(ViewMap(view,AXIS_ACROSS)); break;
	case 11: MapInfo(ViewMap(view,AXIS_DEEP)); break;
	case 12: MapInfo(ViewMap(view,AXIS_4D)); break;
	case 13: MapInfo(ViewMap(view,AXIS_5D)); break;
	case 14: MapInfo(ViewMap(view,AXIS_COLOR)); break;
	case 15: ColorInfo(); break;
	case 16: /*RenderInfo(view);*/ break;
	case 17: DrawInfo(); break;
	case 18: UIMouseInfo(); break;
	case 19: MovieInfo(); break;
	case 20: PickInfo(); break;
	case 21: PickListInfo(); break;
	case 22: PikInfo(); break;
	case 23: RegionInfo(); break;
	case 24: PlaneListInfo(); break;
		}
	}

void UIHelpChoice (Widget widget,int item)
	{
	WidgetList list;

	switch (item) {
	case 0: UIHelpPrint ("USAGE","FILE FORMATS"); break;
	case 1: UIHelpPrint ("FILE FORMATS","WINDOWS"); break;
	case 2: UIHelpPrint ("WINDOWS","MOUSE USAGE"); break;
	case 3: UIHelpPrint ("MOUSE USAGE","INTERACTIVE"); break;
	case 4: UIHelpPrint ("MAIN","STYLE"); break;
	case 5: UIHelpPrint ("STYLE","ORIENT"); break;
	case 6: UIHelpPrint ("ORIENT","SIZE"); break;
	case 7: UIHelpPrint ("SIZE","MOVIE"); break;
	case 8: UIHelpPrint ("MOVIE","COLOR"); break;
	case 9: UIHelpPrint ("COLOR","PICK"); break;
	case 10: UIHelpPrint ("PICK","EDIT"); break;
	case 11: UIHelpPrint ("EDIT","SECTION"); break;
	case 12: UIHelpPrint ("SECTION","STATUS"); break;
	case 13: UIHelpPrint ("STATUS","HELP"); break;
	case 14: UIHelpPrint ("HELP","END"); break;
		}
	}

/* print text on screen */
void UIHelpPrint (char *start,char *finish)
	{
	char *startp=0, *finishp=0, save;
	extern char *help;

	startp = strstr (help,start);
	finishp = strstr (help,finish);
	if (startp == 0 || finishp == 0) return;
	UIInfo (startp);
	}

/* mouse info */
void UIMouseInfo (void)
	{
	UIMessage ("MOUSE LEFT: zoom; MIDDLE: navigate; RIGHT: pick");
	}

/* dump vgrid floats callback */
void UIDumpFloats (void)
	{
	Widget widget;
	extern Data data;
	string filename;

	if (!data) return;
	widget = XmCreatePromptDialog (ui->application, "files", NULL, 0);
	sprintf (filename, "%s_dump_vgrid", DataShortName(data));
	XtVaSetValues (widget, XmNselectionLabelString, CS("Type dump-vgrid-float file name:"),
			XmNtextString, CS (filename), NULL);
	XtAddCallback (widget, XmNokCallback, (XtCallbackProc)UIDumpFloats2, NULL);
	XtAddCallback (widget, XmNcancelCallback, (XtCallbackProc)XtDestroyWidget, NULL);
	XtManageChild (widget);
	}

void UIDumpFloats2 ( Widget widget, XtPointer stuff, XmFileSelectionBoxCallbackStruct *cbs)
	{
	extern Data data;
	char *filename;
	int fd;

	XmStringGetLtoR (cbs->value, XmSTRING_DEFAULT_CHARSET, &filename);
	if ((fd = creat (filename,0444)) < 0) {
		UIMessage ("cant create dump file");
		}
	else	{
		DataDumpFloats (data,filename,fd);
		XtDestroyWidget (widget);
		}
	}

/* dump vgrid bytess callback */
void UIDumpBytes (void)
	{
	Widget widget;
	extern Data data;
	string filename;

	if (!data) return;
	widget = XmCreatePromptDialog (ui->application, "files", NULL, 0);
	sprintf (filename, "%s_dump_vgrid", DataShortName(data));
	XtVaSetValues (widget, XmNselectionLabelString, CS("Type dump-vgrid-byte file name:"),
			XmNtextString, CS (filename), NULL);
	XtAddCallback (widget, XmNokCallback, (XtCallbackProc)UIDumpBytes2, NULL);
	XtAddCallback (widget, XmNcancelCallback, (XtCallbackProc)XtDestroyWidget, NULL);
	XtManageChild (widget);
	}

void UIDumpBytes2 ( Widget widget, XtPointer stuff, XmFileSelectionBoxCallbackStruct *cbs)
	{
	extern Data data;
	char *filename;
	int fd;

	XmStringGetLtoR (cbs->value, XmSTRING_DEFAULT_CHARSET, &filename);
	if ((fd = creat (filename,0444)) < 0) {
		UIMessage ("cant create dump file");
		}
	else	{
		DataDumpBytes (data,filename,fd);
		XtDestroyWidget (widget);
		}
	}

/* save parameters callback */
FILE *savefd = 0;
void UISavePar (void)
	{
	Widget widget;
	extern Data data;
	string filename;

	if (!data) return;
	widget = XmCreatePromptDialog (ui->application, "files", NULL, 0);
	sprintf (filename, "%s_save_H", DataShortName(data));
	XtVaSetValues (widget, XmNselectionLabelString, CS("Type save-pars file name:"),
			XmNtextString, CS (filename), NULL);
	XtAddCallback (widget, XmNokCallback, (XtCallbackProc)UISavePar2, NULL);
	XtAddCallback (widget, XmNcancelCallback, (XtCallbackProc)XtDestroyWidget, NULL);
	XtManageChild (widget);
	}

void UISavePar2 ( Widget widget, XtPointer stuff, XmFileSelectionBoxCallbackStruct *cbs)
	{
	extern Data data;
	extern View view;
	string filename;
	Message message;

	if (!data || !view) return;
	XmStringGetLtoR (cbs->value, XmSTRING_DEFAULT_CHARSET, (char**)&filename);
	if ((savefd = fopen (filename,"w+")) == NULL) {
		UIMessage ("cant create restart-par file");
		}
	else	{
		DataSavePar (data);
		ViewSavePar (view);
		ColorSavePar ();
		RenderSavePar ();
		MovieSavePar ();
		PickSavePar ();
		RegionSavePar ();
		fclose (savefd);
		XtDestroyWidget (widget);
		sprintf (message, "Restart parameters saved in file %s",filename);
		UIMessage (message);
		}
	}

/* write save message */
void UISaveMessage (Message message)
	{
	extern FILE *savefd;

	fprintf (savefd,"%s\n",message);
	}

/* exit program callback */
void UIQuit (void)
	{
	extern PikList pik;

	PikWrite (pik->file,fopen(pik->file,"w"));
	DrawFreePixmaps ();
	XCloseDisplay(UIDisplay());
	exit (0);
	}



/* default oreintation callback */
void UIOrient0 (void)
	{
	ViewOrient0 ();
	}

/* dump axis callbacks */
void UIDownDump (void)
	{
	extern View view;

	MapDump (ViewMap(view,AXIS_DOWN));
	}

void UIAcrossDump (void)
	{
	extern View view;

	MapDump (ViewMap(view,AXIS_ACROSS));
	}

void UIDeepDump (void)
	{
	extern View view;

	MapDump (ViewMap(view,AXIS_DEEP));
	}

/* swap axis callbacks */

void UISwapFrontSide (void)
	{
	ViewSwapAxis (AXIS_ACROSS,AXIS_DEEP);
	UISyzeReset ();
	UIArrayReset (0);
	if (ui->style == VIEW_ARRAY) UIArrayDraw();
	}

void UISwapSideTop (void)
	{
	ViewSwapAxis (AXIS_DOWN,AXIS_ACROSS);
	UISyzeReset ();
	if (ui->style == VIEW_ARRAY) UIArrayDraw();
	}

void UISwapTopFront (void)
	{
	ViewSwapAxis (AXIS_DOWN,AXIS_DEEP);
	UISyzeReset ();
	UIArrayReset (0);
	if (ui->style == VIEW_ARRAY) UIArrayDraw();
	}

void UISwapFrontExtra (void)
	{
	ViewSwapAxis (AXIS_DEEP,AXIS_4D);
	UISyzeReset ();
	UIArrayReset (0);
	if (ui->style == VIEW_ARRAY) UIArrayDraw();
	}

void UISwapSideExtra (void)
	{
	ViewSwapAxis (AXIS_ACROSS,AXIS_4D);
	UISyzeReset ();
	UIArrayReset (0);
	if (ui->style == VIEW_ARRAY) UIArrayDraw();
	}

void UISwapTopExtra (void)
	{
	ViewSwapAxis (AXIS_DOWN,AXIS_4D);
	UISyzeReset ();
	UIArrayReset (0);
	if (ui->style == VIEW_ARRAY) UIArrayDraw();
	}

/* axis flip callbacks */
void UIFlipDown (void)
	{
	ViewFlipAxis (AXIS_DOWN);
	UISyzeReset ();
	}

void UIFlipAcross (void)
	{
	ViewFlipAxis (AXIS_ACROSS);
	UISyzeReset ();
	}

void UIFlipDeep (void)
	{
	ViewFlipAxis (AXIS_DEEP);
	UISyzeReset ();
	}

/* set shape callback */
void UISizeChoice ( Widget widget, int item)
	{
	WidgetList list;

	if (item > 2) return;
	LIST (menu);
	XtVaSetValues (list[(item+1)%3],XmNset,False,NULL);
	XtVaSetValues (list[(item+2)%3],XmNset,False,NULL);
	ViewSetShape (item);
	}

/* interpolation choice callback */
void UIInterpolateToggle (Widget widget)
	{
	RenderToggleInterp ();
	ViewDrawAll ();
	}

/* default zoom callback */
void UISize0 (void)
	{
	RenderSetInterp (0);
	ViewWindow0 ();
	UISyzeReset ();
	}

/* default screen size callback (doesn't work) */
void UIScreen0 (void)
	{
	if (!ui) return;
	XtVaSetValues (ui->canvas,XmNwidth,ui->wide,XmNheight,ui->hite,NULL);
	}

/* region smooth callback */
void UISubvolumeSmooth (void)
	{
	extern Data data;

	DrawWatch (1);
	RegionSetValue (RegionBound(1),RegionBound(1));
	DataComputeHistogram (data);
	ViewDrawAll ();
	}

/* region smooth undo */
void UISmoothUndo (void)
	{
	extern Data data;
	
	DrawWatch (1);
	RegionRestoreValue ();
	DataComputeHistogram (data);
	ViewDrawAll ();
	}

/* clear picks callback */
void UIPickClear ( Widget widget, XButtonEvent *event)
	{
	PickClear0 ();
	RegionClear ();
	}

void UIEditGrade (void)
	{
	extern Data data;
	extern View view;

	DrawWatch (1);
	EditGrade (DataBuffer(data),ViewMap(view,AXIS_DOWN),ViewMap(view,AXIS_ACROSS),ViewMap(view,AXIS_DEEP));
	DataComputeHistogram (data);
	ViewDrawAll ();
	}

void UIGradeUndo (void)
	{
	extern Data data;

	DrawWatch (1);
	EditUndo (DataBuffer(data));
	DataComputeHistogram (data);
	ViewDrawAll ();
	}

void UIStatistics (void)
	{
	extern View view;
	extern Data data;
	int n, size, i, low, median, high;
	float dist[256];
	Message message;

	for (size=1, i=1; i<4; i++) size *= MapWindow (ViewMap(view,i));
	n = EditBox (DataBuffer(data),ViewMap(view,AXIS_DOWN),ViewMap(view,AXIS_ACROSS),ViewMap(view,AXIS_DEEP),dist);
	EditStats (n,dist,&low,&median,&high);
	printf ("box: n=%d low=%d median=%d high=%d\n", n,low,median,high);
	n = EditCube (DataBuffer(data),ViewMap(view,AXIS_DOWN),ViewMap(view,AXIS_ACROSS),ViewMap(view,AXIS_DEEP),dist);
	EditStats (n,dist,&low,&median,&high);
	printf ("cube: n=%d low=%d median=%d high=%d\n", n,low,median,high);
	UIMessage (message);
	}

void UIWakeup (void)
	{
	XmProcessTraversal(ui->canvas, XmTRAVERSE_CURRENT);
	}
/* pik write callback */
void UIPikWrite (void)
	{
	Widget widget;
	extern Data data;
	extern PikList pik;

	if (!data) return;
	widget = XmCreatePromptDialog (ui->application, "files", NULL, 0);
	XtVaSetValues (widget, XmNselectionLabelString, CS("Type write picks file name:"),
			XmNtextString, CS (pik->file), NULL);
	XtAddCallback (widget, XmNokCallback, (XtCallbackProc)UIPikWrite2, NULL);
	XtAddCallback (widget, XmNcancelCallback, (XtCallbackProc)XtDestroyWidget, NULL);
	XtManageChild (widget);
	}

void UIPikWrite2 ( Widget widget, XtPointer stuff, XmFileSelectionBoxCallbackStruct *cbs)
	{
	extern Data data;
	char *filename;
	FILE *fd;

	XmStringGetLtoR (cbs->value, XmSTRING_DEFAULT_CHARSET, &filename);
	if ((fd = fopen(filename,"w")) == NULL) {
		UIMessage ("cant create dump file");
		}
	else	{
		PikWrite (filename,fd);
		XtDestroyWidget (widget);
		}
	}

/* pik read callback */
void UIPikRead (void)
	{
	Widget widget;
	extern Data data;
	extern PikList pik;

	if (!data) return;
	widget = XmCreatePromptDialog (ui->application, "files", NULL, 0);
	XtVaSetValues (widget, XmNselectionLabelString, CS("Type write picks file name:"),
			XmNtextString, CS (pik->file), NULL);
	XtAddCallback (widget, XmNokCallback, (XtCallbackProc)UIPikRead2, NULL);
	XtAddCallback (widget, XmNcancelCallback, (XtCallbackProc)XtDestroyWidget, NULL);
	XtManageChild (widget);
	}

void UIPikRead2 ( Widget widget, XtPointer stuff, XmFileSelectionBoxCallbackStruct *cbs)
	{
	extern Data data;
	char *filename;
	extern PikList pik;
	FILE *fd;

	XmStringGetLtoR (cbs->value, XmSTRING_DEFAULT_CHARSET, &filename);
	if ((fd = fopen(filename,"r")) == NULL) {
		UIMessage ("cant create dump file");
		}
	else	{
		strcpy (pik->file,filename);
		fclose (fd);
		PikRead ();
		XtDestroyWidget (widget);
		}
	}

/* save front section callback */
void UISaveFront (void)
	{
	Widget widget;
	string filename;
	extern Data data;
	extern View view;

	if (!data) return;
	sprintf (filename,"%s_%s_%dx%d",
		DataShortName(data),
		AxisScript(MapAxis(ViewMap(view,AXIS_DEEP)),MapFrame(ViewMap(view,AXIS_DEEP))),
		MapWindow(ViewMap(view,AXIS_DOWN)),
		MapWindow(ViewMap(view,AXIS_ACROSS)) );
	widget = XmCreatePromptDialog (ui->application, "files", NULL, 0);
	XtVaSetValues (widget, XmNselectionLabelString, CS("Type write picks file name:"),
			XmNtextString, CS (filename), NULL);
	XtAddCallback (widget, XmNokCallback, (XtCallbackProc)UISaveFront2, NULL);
	XtAddCallback (widget, XmNcancelCallback, (XtCallbackProc)XtDestroyWidget, NULL);
	XtManageChild (widget);
	}

void UISaveFront2 ( Widget widget, XtPointer stuff, XmFileSelectionBoxCallbackStruct *cbs)
	{
	extern Data data;
	char *filename;
	FILE *fd;

	XmStringGetLtoR (cbs->value, XmSTRING_DEFAULT_CHARSET, &filename);
	if ((fd = fopen(filename,"w")) == NULL) {
		UIMessage ("cant create save file");
		}
	else	{
		fclose (fd);
		SaveFront (filename);
		XtDestroyWidget (widget);
		}
	}

/* save side section callback */
void UISaveSide (void)
	{
	Widget widget;
	string filename;
	extern Data data;
	extern View view;

	if (!data) return;
	sprintf (filename,"%s_%s_%dx%d",
		DataShortName(data),
		AxisScript(MapAxis(ViewMap(view,AXIS_ACROSS)),MapFrame(ViewMap(view,AXIS_ACROSS))),
		MapWindow(ViewMap(view,AXIS_DOWN)),
		MapWindow(ViewMap(view,AXIS_DEEP)) );
	widget = XmCreatePromptDialog (ui->application, "files", NULL, 0);
	XtVaSetValues (widget, XmNselectionLabelString, CS("Type write picks file name:"),
			XmNtextString, CS (filename), NULL);
	XtAddCallback (widget, XmNokCallback, (XtCallbackProc)UISaveSide2, NULL);
	XtAddCallback (widget, XmNcancelCallback, (XtCallbackProc)XtDestroyWidget, NULL);
	XtManageChild (widget);
	}

void UISaveSide2 ( Widget widget, XtPointer stuff, XmFileSelectionBoxCallbackStruct *cbs)
	{
	extern Data data;
	char *filename;
	FILE *fd;

	XmStringGetLtoR (cbs->value, XmSTRING_DEFAULT_CHARSET, &filename);
	if ((fd = fopen(filename,"w")) == NULL) {
		UIMessage ("cant create save file");
		}
	else	{
		fclose (fd);
		SaveSide (filename);
		XtDestroyWidget (widget);
		}
	}

/* save top section callback */
void UISaveTop (void)
	{
	Widget widget;
	string filename;
	extern Data data;
	extern View view;

	if (!data) return;
	sprintf (filename,"%s_%s_%dx%d",
		DataShortName(data),
		AxisScript(MapAxis(ViewMap(view,AXIS_DOWN)),MapFrame(ViewMap(view,AXIS_DOWN))),
		MapWindow(ViewMap(view,AXIS_DEEP)),
		MapWindow(ViewMap(view,AXIS_ACROSS)) );
	widget = XmCreatePromptDialog (ui->application, "files", NULL, 0);
	XtVaSetValues (widget, XmNselectionLabelString, CS("Type write picks file name:"),
			XmNtextString, CS (filename), NULL);
	XtAddCallback (widget, XmNokCallback, (XtCallbackProc)UISaveTop2, NULL);
	XtAddCallback (widget, XmNcancelCallback, (XtCallbackProc)XtDestroyWidget, NULL);
	XtManageChild (widget);
	}

void UISaveTop2 ( Widget widget, XtPointer stuff, XmFileSelectionBoxCallbackStruct *cbs)
	{
	extern Data data;
	char *filename;
	FILE *fd;

	XmStringGetLtoR (cbs->value, XmSTRING_DEFAULT_CHARSET, &filename);
	if ((fd = fopen(filename,"w")) == NULL) {
		UIMessage ("cant create save file");
		}
	else	{
		fclose (fd);
		SaveTop (filename);
		XtDestroyWidget (widget);
		}
	}

/* save down profile callback */
void UISaveDown (void)
	{
	Widget widget;
	string filename;
	extern Data data;
	extern View view;

	if (!data) return;
	sprintf (filename,"%s_%s_%s_%d",
		DataShortName(data),
		AxisScript(MapAxis(ViewMap(view,AXIS_ACROSS)),MapFrame(ViewMap(view,AXIS_ACROSS))),
		AxisScript(MapAxis(ViewMap(view,AXIS_DEEP)),MapFrame(ViewMap(view,AXIS_DEEP))),
		MapWindow(ViewMap(view,AXIS_DOWN)) );
	widget = XmCreatePromptDialog (ui->application, "files", NULL, 0);
	XtVaSetValues (widget, XmNselectionLabelString, CS("Type write picks file name:"),
			XmNtextString, CS (filename), NULL);
	XtAddCallback (widget, XmNokCallback, (XtCallbackProc)UISaveDown2, NULL);
	XtAddCallback (widget, XmNcancelCallback, (XtCallbackProc)XtDestroyWidget, NULL);
	XtManageChild (widget);
	}

void UISaveDown2 ( Widget widget, XtPointer stuff, XmFileSelectionBoxCallbackStruct *cbs)
	{
	extern Data data;
	char *filename;
	FILE *fd;

	XmStringGetLtoR (cbs->value, XmSTRING_DEFAULT_CHARSET, &filename);
	if ((fd = fopen(filename,"w")) == NULL) {
		UIMessage ("cant create save file");
		}
	else	{
		fclose (fd);
		SaveDown (filename);
		XtDestroyWidget (widget);
		}
	}

/* save across profile callback */
void UISaveAcross (void)
	{
	Widget widget;
	string filename;
	extern Data data;
	extern View view;

	if (!data) return;
	sprintf (filename,"%s_%s_%s_%d",
		DataShortName(data),
		AxisScript(MapAxis(ViewMap(view,AXIS_DEEP)),MapFrame(ViewMap(view,AXIS_DEEP))),
		AxisScript(MapAxis(ViewMap(view,AXIS_DOWN)),MapFrame(ViewMap(view,AXIS_DOWN))),
		MapWindow(ViewMap(view,AXIS_ACROSS)) );
	widget = XmCreatePromptDialog (ui->application, "files", NULL, 0);
	XtVaSetValues (widget, XmNselectionLabelString, CS("Type write picks file name:"),
			XmNtextString, CS (filename), NULL);
	XtAddCallback (widget, XmNokCallback, (XtCallbackProc)UISaveAcross2, NULL);
	XtAddCallback (widget, XmNcancelCallback, (XtCallbackProc)XtDestroyWidget, NULL);
	XtManageChild (widget);
	}

void UISaveAcross2 ( Widget widget, XtPointer stuff, XmFileSelectionBoxCallbackStruct *cbs)
	{
	extern Data data;
	char *filename;
	FILE *fd;

	XmStringGetLtoR (cbs->value, XmSTRING_DEFAULT_CHARSET, &filename);
	if ((fd = fopen(filename,"w")) == NULL) {
		UIMessage ("cant create save file");
		}
	else	{
		fclose (fd);
		SaveAcross (filename);
		XtDestroyWidget (widget);
		}
	}

/* save deep profile callback */
void UISaveDeep (void)
	{
	Widget widget;
	string filename;
	extern Data data;
	extern View view;

	if (!data) return;
	sprintf (filename,"%s_%s_%s_%d",
		DataShortName(data),
		AxisScript(MapAxis(ViewMap(view,AXIS_DOWN)),MapFrame(ViewMap(view,AXIS_DEEP))),
		AxisScript(MapAxis(ViewMap(view,AXIS_ACROSS)),MapFrame(ViewMap(view,AXIS_ACROSS))),
		MapWindow(ViewMap(view,AXIS_DEEP)) );
	widget = XmCreatePromptDialog (ui->application, "files", NULL, 0);
	XtVaSetValues (widget, XmNselectionLabelString, CS("Type write picks file name:"),
			XmNtextString, CS (filename), NULL);
	XtAddCallback (widget, XmNokCallback, (XtCallbackProc)UISaveDeep2, NULL);
	XtAddCallback (widget, XmNcancelCallback, (XtCallbackProc)XtDestroyWidget, NULL);
	XtManageChild (widget);
	}

void UISaveDeep2 (Widget widget,XtPointer stuff, XmFileSelectionBoxCallbackStruct *cbs)
	{
	extern Data data;
	char *filename;
	FILE *fd;

	XmStringGetLtoR (cbs->value, XmSTRING_DEFAULT_CHARSET, &filename);
	if ((fd = fopen(filename,"w")) == NULL) {
		UIMessage ("cant create save file");
		}
	else	{
		fclose (fd);
		SaveDeep (filename);
		XtDestroyWidget (widget);
		}
	}

#endif

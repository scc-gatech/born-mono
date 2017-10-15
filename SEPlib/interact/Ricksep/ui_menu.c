#include <sepConfig.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#if defined (MOTIF_FOUND) || defined(HAVE_ATHENA)
/*
user interface: menu setup and callbacks
*/
#include <stdio.h>
#include "rick.h"

extern UI ui;
extern Message message;
char buf_me[60000];
string filename;
string fileMessage;

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

/* main menu initialization */
int UIMenuInit (Widget parent)
	{
	int i, j, ibar=0;
	WidgetList list;
	int *cb[3];

	/* menubar */
	if (!ui) return(0);

	ui->menuBar = XmVaCreateSimpleMenuBar (parent, "menubar",
		XmVaCASCADEBUTTON, CS("Main"), NULL,
		XmVaCASCADEBUTTON, CS("View"), NULL,
		XmVaCASCADEBUTTON, CS("Orient"), NULL,
		XmVaCASCADEBUTTON, CS("Size"), NULL,
		XmVaCASCADEBUTTON, CS("Navigate"), NULL,
		XmVaCASCADEBUTTON, CS("Color"), NULL,
		XmVaCASCADEBUTTON, CS("Path"), NULL,
		XmVaCASCADEBUTTON, CS("Pick"), NULL,
		XmVaCASCADEBUTTON, CS("Status"), NULL,
		XmVaCASCADEBUTTON, CS("VelView"), NULL,
		XmVaCASCADEBUTTON, CS("Help"), NULL,
		NULL);

	/* main drop down menu */
	ui->menu = XmVaCreateSimplePulldownMenu (ui->menuBar,"main",ibar++,NULL,
		XmVaPUSHBUTTON, CS("Redraw"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Fix Picking"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Write Vgrid File (Floats) ..."), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Write Vgrid File (Bytes) ..."), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Write Parameter Restart File ..."), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Write History File ..."), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Load History File ..."), NULL, NULL, NULL,
                XmVaPUSHBUTTON, CS("Output display commands ..."), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Quit"), NULL, NULL, NULL,
		NULL);
	LIST (menu);
	PBCALLBACK (0,ViewDrawAll);
	PBCALLBACK (1,UIWakeup);
	PBCALLBACK (2,UIDumpFloats);
	PBCALLBACK (3,UIDumpBytes);
	PBCALLBACK (4,UISavePar);
	PBCALLBACK (5,UIHistorySave);
	PBCALLBACK (6,UIHistoryLoad);
        PBCALLBACK (7,UIViewCommands);
	PBCALLBACK (8,UIQuit);

	ui->menu = XmVaCreateSimplePulldownMenu (ui->menuBar,"view",ibar++,(XtCallbackProc)UIStyleChoice,
		XmVaRADIOBUTTON, CS("Front Face"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Side Face"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Top Face"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Three Faces"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Cube"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Array ..."), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Fence ..."), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Transparent Cube ..."), NULL, NULL, NULL,
		XmNradioAlwaysOne, True,
		XmNradioBehavior, True,
		XmVaSEPARATOR,
	        XmVaPUSHBUTTON, CS("Round Robin On/Off ('Shift'+'r')"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Next View ('>')"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Previous View ('<')"), NULL, NULL, NULL,
		NULL);
	LIST (menu);
	XtVaSetValues (list[VIEW_THREE],XmNset,True,NULL);

	ui->menu = XmVaCreateSimplePulldownMenu (ui->menuBar,"orient",ibar++,NULL,
		XmVaPUSHBUTTON, CS("Swap Side & Top Faces(12)"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Swap Front & Side Faces(23)"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Swap Top & Front Faces(13)"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Swap Deep & 4th Axes(14)"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Swap Across & 4th Axes(24)"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Swap Down & 4th Axes(34)"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Swap Deep & 5th Axes(15)"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Swap Across & 5th Axes(25)"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Swap Down & 5th Axes(35)"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Orientation Menu ..."), NULL, NULL, NULL,
                XmVaPUSHBUTTON, CS("Flip Down Direction"), NULL, NULL, NULL,
                XmVaPUSHBUTTON, CS("Flip Across Direction"), NULL, NULL, NULL,
                XmVaPUSHBUTTON, CS("Flip Deep Direction"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Move Frames to Middle"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Move Frames to Corner"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Initial Orientation"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Adjust Labels ..."), NULL, NULL, NULL,
		XmVaTITLE, CS("4th AXIS IS n4= AND HIDDEN"),
		NULL);
	LIST (menu);
	PBCALLBACK (0,UISwapSideTop);
	PBCALLBACK (1,UISwapFrontSide);
	PBCALLBACK (2,UISwapTopFront);
	PBCALLBACK (5,UISwapFrontExtra);
	PBCALLBACK (4,UISwapSideExtra);
	PBCALLBACK (3,UISwapTopExtra);
	PBCALLBACK (8,UISwapFrontExtra2);
	PBCALLBACK (7,UISwapSideExtra2);
	PBCALLBACK (6,UISwapTopExtra2);
	PBCALLBACK (9,UIOrientRaise);
	PBCALLBACK (10,UIFlipDown);
	PBCALLBACK (11,UIFlipAcross);
	PBCALLBACK (12,UIFlipDeep);
	PBCALLBACK (13,ViewFramesMiddle);
	PBCALLBACK (14,ViewFramesOrigin);
	PBCALLBACK (15,UIOrient0);
	PBCALLBACK (16,UILabelRaise);

	ui->menu = XmVaCreateSimplePulldownMenu (ui->menuBar,"size",ibar++,(XtCallbackProc)UISizeChoice,
		XmVaRADIOBUTTON, CS("Even Cube"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("True Proportions"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Sample Per Pixel"), NULL, NULL, NULL,
		XmVaTOGGLEBUTTON, CS("Interpolate"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Fine Control ..."), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Initial Size"), NULL, NULL, NULL,
		XmVaTITLE, CS("LEFT MOUSE BOX ZOOMS"),
		XmVaTITLE, CS("+ 'h' ONLY HORZ"),
		XmVaTITLE, CS("+ 'v' ONLY VERT"),
		XmNradioAlwaysOne, True,
		XmNradioBehavior, True,
		NULL);
	LIST (menu);
	ui->shape_stretch = list[0];
	ui->shape_true = list[1];
	ui->shape_pixel = list[2];
	XtVaSetValues (list[0],XmNset,True,NULL);
	XtVaSetValues (list[4],XmNset,False,NULL);
	TGCALLBACK (3,UIInterpolateToggle);
	PBCALLBACK (4,UISyzeRaise);
	PBCALLBACK (5,UISize0);

	ui->menu = XmVaCreateSimplePulldownMenu (ui->menuBar,"movie",ibar++,NULL,
		XmVaPUSHBUTTON, CS("Movie to Boundaries"), NULL, NULL, NULL,
		XmVaTOGGLEBUTTON, CS("High Speed"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Undo Move ('Ctrl'+'u')"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Redo Move ('Ctrl'+'r')"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Undo to Start"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Redo to End"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Animate Undo to Start"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Animate Redo to End"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Pause Animation"), NULL, NULL, NULL,
		XmVaTITLE, CS ("MIDDLE MOUSE CLICK XFRAMES"),
		XmVaTITLE, CS ("MIDDLE MOUSE DRAG MOVIE BOUNDS"),
		XmNradioAlwaysOne, True,
		NULL);
	LIST (menu);
	PBCALLBACK (0,ViewMovieFullBounds);
	TGCALLBACK (1,MovieToggleCache);
	PBCALLBACK (2,UIHistoryUndo);
	PBCALLBACK (3,UIHistoryRedo);
	PBCALLBACK (4,UIHistoryStart);
	PBCALLBACK (5,UIHistoryEnd);
	PBCALLBACK (6,UIHistoryAnimateUndo);
	PBCALLBACK (7,UIHistoryAnimateRedo);
	PBCALLBACK (8,UIHistoryAnimateOff);
	XtVaSetValues (list[1],XmNset,False,NULL);

	ui->menu = XmVaCreateSimplePulldownMenu (ui->menuBar,"color",ibar++,(XtCallbackProc)UIColorChoice,
		XmVaRADIOBUTTON, CS("Gray"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Straw"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Flag"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Tiger"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Blue"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Rainbow"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("AVO"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Veldata"), NULL, NULL, NULL,
		XmVaCASCADEBUTTON, CS("Overlay"), NULL,
		XmVaCASCADEBUTTON, CS("Mark"), NULL,
		XmVaCASCADEBUTTON, CS("Background"), NULL,
		XmVaPUSHBUTTON, CS("Flip Polarity"), NULL, NULL, NULL,
		XmNradioBehavior, True,
		XmNradioAlwaysOne, True,
		NULL);
	LIST (menu);
	PBCALLBACK (11,RenderTogglePolarity);
	XtVaSetValues (list[0],XmNset,True,NULL);

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

	ui->menu = XmVaCreateSimplePulldownMenu (ui->menuBar,"picks",ibar++,NULL,
		XmVaPUSHBUTTON, CS("Show/Hide Picks"), NULL, NULL, NULL,
	        XmVaPUSHBUTTON, CS("Change Pick Properties"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Fix Picking"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Write to File"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Read from File"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Increase Range"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Decrease Range"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Increase Draw Size"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Decrease Draw Size"), NULL, NULL, NULL,
		XmVaTITLE, CS("RIGHT MOUSE MAKES PICK"),
		XmVaTITLE, CS("+ 'a' KEY ADDS POINT"),
		XmVaTITLE, CS("+ 'm' KEY MOVES NEAREST POINT"),
		XmVaTITLE, CS("+ 'd' KEY DELETES NEAREST POINT"),
		XmVaTITLE, CS("+ 'q' or '?' KEY INQUIRES NEAREST POINT"),
		XmVaTITLE, CS("+ 's' KEY SELECTS SMOOTH REGION"),
		NULL);
	LIST (menu);
	PBCALLBACK (0,ViewTogglePick);
	PBCALLBACK (1,UIPikTextRaise);
	PBCALLBACK (2,UIWakeup);
	PBCALLBACK (3,UIPikWrite);
	PBCALLBACK (4,UIPikRead);
	PBCALLBACK (5,PikIncreaseRange);
	PBCALLBACK (6,PikDecreaseRange);
	PBCALLBACK (7,PikIncreaseSize);
	PBCALLBACK (8,PikDecreaseSize);

/*
	ui->menu = XmVaCreateSimplePulldownMenu (ui->menuBar,"edit",ibar++,NULL,
		XmVaPUSHBUTTON, CS("Clear Blob Pick"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Fill Blob With Edge Value"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Undo Blob Fill"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Interpolate Current Cube Edge Values Across Cube"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Undo Cube Edge Interpolation"), NULL, NULL, NULL,
		XmVaTITLE, CS("'s' KEY + RIGHT MOUSE DRAG PICKS SUBVOLUME"),
		XmVaTITLE, CS("SET SMOOTH VALUE FROM COLORBAR"),
		NULL);
	LIST (menu);
	PBCALLBACK (0,UIPickClear);
	PBCALLBACK (1,UISubvolumeSmooth);
	PBCALLBACK (2,UISmoothUndo);
	PBCALLBACK (3,UIEditGrade);
	PBCALLBACK (4,UIGradeUndo);
*/

	ui->menu = XmVaCreateSimplePulldownMenu (ui->menuBar,"pathview",ibar++,NULL,
		XmVaPUSHBUTTON, CS("Path View On/Off ('Shift'+'p')"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Path Start"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Path Middle"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Path End"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Animate Path Forward"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Animate Path Backward"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Pause Animation"), NULL, NULL, NULL,
		XmVaTITLE, CS("'+' MOVES FORWARD ON PATH"),
		XmVaTITLE, CS("'-' MOVES BACKWARD ON PATH"), 
		NULL);
        LIST (menu);
	PBCALLBACK (0,UIPathViewToggleOnOff);
	PBCALLBACK (1,UIPathViewStart);
	PBCALLBACK (2,UIPathViewMiddle);
	PBCALLBACK (3,UIPathViewEnd);
	PBCALLBACK (4,UIPathViewAnimateForward);
	PBCALLBACK (5,UIPathViewAnimateBackward);
	PBCALLBACK (6,UIPathViewAnimateOff);


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
        ui->menu = XmVaCreateSimplePulldownMenu (ui->menuBar,"velocity", ibar++, (XtCallbackProc)UIVelViewChoice,
                XmVaRADIOBUTTON, CS("Separate Cubes"), NULL, NULL, NULL,
                XmVaRADIOBUTTON, CS("Merge by Adjusting Intensities"), NULL, NULL, NULL,
                XmVaRADIOBUTTON, CS("Merge by Copying Differences"), NULL, NULL, NULL,
                XmNradioAlwaysOne, True,
                XmNradioBehavior, True,
                NULL);
        LIST (menu);
        XtVaSetValues (list[0],XmNset,True,NULL);
	ui->menu = XmVaCreateSimplePulldownMenu (ui->menuBar,"help",ibar++,(XtCallbackProc)UIHelpChoice,
		XmVaTITLE, CS("MORE'D TO TERMINAL WINDOW"),
		XmVaPUSHBUTTON, CS("Data Parameters"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Pick Parameters"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Display Parameters"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Multiple View Parameters"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Velocity Analysis Parameters"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("File Formats"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Windows"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Navigation Mouse Usage"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Picking Mouse Usage"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Annotate Mouse Usage"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Other Mouse Usage"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Main Functions"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("View Functions"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Orient Functions"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Size Functions"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Navigate Functions"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Color Functions"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Pick Functions"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Edit Functions"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("PathView Functions"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("VelView Functions"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Status Functions"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Help Functions"), NULL, NULL, NULL,
		XmVaPUSHBUTTON, CS("Examples"), NULL, NULL, NULL,
		NULL);
	XtManageChild (ui->menuBar);
	return(0);
	}

/* view choice callback */
int UIStyleChoice (Widget widget,int item_in) {
  View myv;
  
  /* set array parameters according to view */
  if (!ui) return(0);
  if (ui->shell) {
    XtPopdown (ui->shell);
    ui->shell = 0;
  }
  myv = current_view();
  ViewStyleChoice(myv, item_in);
}

/* animate undo sequence */
int UIHistoryAnimateUndo(Widget widget) {
  XtPointer dummy;
  MovieOff();
  UIPathViewAnimateOff(widget);
  ui->redoAnimate = 0;
  if (ui->undoAnimate == 1)
    return(0);
  ui->undoAnimate = 1;
  XtAppAddTimeOut(ui->context, (unsigned long) 2*CANVAS_REFRESH_WAIT, UIHistoryAnimateUndoUpdate, dummy);
  return(0);
}

/* animate redo sequence */
int UIHistoryAnimateRedo(Widget widget) {
  XtPointer dummy;
  MovieOff();
  UIPathViewAnimateOff(widget);
  ui->undoAnimate = 0;
  if (ui->redoAnimate == 1)
    return(0);
  ui->redoAnimate = 1;
  XtAppAddTimeOut(ui->context, (unsigned long) 2*CANVAS_REFRESH_WAIT, UIHistoryAnimateRedoUpdate, dummy);
  return(0);
}

/* animate path forward sequence */
int UIPathViewAnimateForward(Widget widget) {
  if (PathViewOn() == 0) return(0);

  MovieOff();
  UIHistoryAnimateOff(widget);
  ui->pathBackwardAnimate = 0;
  if (ui->pathForwardAnimate == 1)
    return(0);
  if (PathViewLength() <= 1) {
    PathViewMoveStart();
    ViewPathViewMove();
    return(0);
  }
  ui->pathForwardAnimate = 1;
  XtAppAddTimeOut(ui->context, (unsigned long) 2*CANVAS_REFRESH_WAIT, UIPathViewAnimateForwardUpdate, NULL);
  return(0);
}

/* animate path backward sequence */
int UIPathViewAnimateBackward(Widget widget) {
  if (PathViewOn() == 0) return(0);

  MovieOff();
  UIHistoryAnimateOff(widget);
  ui->pathForwardAnimate = 0;
  if (ui->pathBackwardAnimate == 1)
    return(0);
  if (PathViewLength() <= 1) {
    PathViewMoveStart();
    ViewPathViewMove();
    return(0);
  }
  ui->pathBackwardAnimate = 1;
  XtAppAddTimeOut(ui->context, (unsigned long) 2*CANVAS_REFRESH_WAIT, UIPathViewAnimateBackwardUpdate, NULL);
  return(0);
}

/* color choice callback */
int UIColorChoice (Widget widget,int item)
	{
	if (item > 8) return(0);
	ColorSetChoice (item);
	ColorSwitch ();
  return(0);
	}

/* overlay choice callback */
int UIOverlayChoice (Widget widget,int item)
	{
	ColorSetOverlay (item);
	ColorSwitch ();
  return(0);
	}

/* backround choice callback */
int UIMarkChoice (Widget widget,int item)
	{
	if (item > 4) return(0);
	ColorSetMark (item);
  return(0);
	}

/* background choice callback */
int UIBackgroundChoice (Widget widget,int item)
	{
	if (item > 4) return(0);
	ColorSetBackground (item);
  return(0);
	}

/* region neighborhood callback */
int UINeighborhoodChoice (Widget widget,int item)
	{
	switch (item) {
	case 0: RegionSetNeighborhood (MARK_FACE); break;
	case 1: RegionSetNeighborhood (MARK_FACE|MARK_EDGE); break;
	case 2: RegionSetNeighborhood (MARK_FACE|MARK_EDGE|MARK_CORNER); break;
		}
  return(0);
	}

/* status choice callback */
int UIStatusChoice (Widget widget,int item)
	{
	  View myv;
	  extern Render render;
	  myv = current_view();

	    switch (item) {
	    case 0: DataInfo(myv->data); break;
	    case 1: DataValueInfo(myv->data); break;
	    case 2: AxisInfo(DataAxis(myv->data,DATA_VALUE)); break;
	    case 3: AxisInfo(DataAxis(myv->data,DATA_AXIS1)); break;
	    case 4: AxisInfo(DataAxis(myv->data,DATA_AXIS2)); break;
	    case 5: AxisInfo(DataAxis(myv->data,DATA_AXIS3)); break;
	    case 6: AxisInfo(DataAxis(myv->data,DATA_AXIS4)); break;
	    case 7: AxisInfo(DataAxis(myv->data,DATA_AXIS5)); break;
	    case 8: ViewInfo(myv); break;
	    case 9: MapInfo(ViewMap(myv,AXIS_DOWN)); break;
	    case 10: MapInfo(ViewMap(myv,AXIS_ACROSS)); break;
	    case 11: MapInfo(ViewMap(myv,AXIS_DEEP)); break;
	    case 12: MapInfo(ViewMap(myv,AXIS_4D)); break;
	    case 13: MapInfo(ViewMap(myv,AXIS_5D)); break;
	    case 14: MapInfo(ViewMap(myv,AXIS_COLOR)); break;
	    case 15: ColorInfo(); break;
	    case 16: RenderInfo(render); break;
	    case 17: DrawInfo(); break;
	    case 18: UIMouseInfo(); break;
	    case 19: MovieInfo(); break;
	    case 20: PickInfo(); break;
	    case 21: PickListInfo(); break;
	    case 22: PikInfo(); break;
	    case 23: RegionInfo(); break;
	    case 24: PlaneListInfo(); break;
	    }
	    return(0);
	}

/** delay until after canvas settles down **/
int UIDelayMessage(Message message) {
  if (!ui) return(0);
  ui->messageOn = 1;
  strcpy(ui->statusMessage, message);
  return(0);
}

int UIDisplayDelayedMessage() {
  if (!ui) return(0);
  UIMessage(ui->statusMessage);
  return(0);
}

int UIHelpChoice (Widget widget,int item)
	{
	WidgetList list;

	switch (item) {
	case 0: UIHelpPrint ("DATA PARAMETERS","PICK PARAMETERS"); break;
	case 1: UIHelpPrint ("PICK PARAMETERS","DISPLAY PARAMETERS"); break;
	case 2: UIHelpPrint ("DISPLAY PARAMETERS","MULTIPLE VIEW PARAMETERS"); break;
	case 3: UIHelpPrint ("MULTIPLE VIEW PARAMETERS","VELOCITY ANALYSIS PARAMETERS"); break;
	case 4: UIHelpPrint ("VELOCITY ANALYSIS PARAMETERS","FILE FORMATS"); break;
	case 5: UIHelpPrint ("FILE FORMATS","WINDOWS"); break;
	case 6: UIHelpPrint ("WINDOWS","NAVIGATION MOUSE USAGE"); break;
	case 7: UIHelpPrint ("NAVIGATION MOUSE USAGE","PICKING MOUSE USAGE"); break;
	case 8: UIHelpPrint ("PICKING MOUSE USAGE","ANNOTATE MOUSE USAGE"); break;
	case 9: UIHelpPrint ("ANNOTATE MOUSE USAGE","OTHER MOUSE USAGE"); break;
	case 10: UIHelpPrint ("OTHER MOUSE USAGE","MAIN FUNCTIONS"); break;
	case 11: UIHelpPrint ("MAIN FUNCTIONS","VIEW FUNCTIONS"); break;
	case 12: UIHelpPrint ("VIEW FUNCTIONS","ORIENT FUNCTIONS"); break;
	case 13: UIHelpPrint ("ORIENT FUNCTIONS","SIZE FUNCTIONS"); break;
	case 14: UIHelpPrint ("SIZE FUNCTIONS","NAVIGATE FUNCTIONS"); break;
	case 15: UIHelpPrint ("NAVIGATE FUNCTIONS","COLOR FUNCTIONS"); break;
	case 16: UIHelpPrint ("COLOR FUNCTIONS","PICK FUNCTIONS"); break;
	case 17: UIHelpPrint ("PICK FUNCTIONS","EDIT FUNCTIONS"); break;
	case 18: UIHelpPrint ("EDIT FUNCTIONS","PATH VIEW FUNCTIONS"); break;
        case 19: UIHelpPrint ("PATH VIEW FUNCTIONS","SECTION FUNCTIONS"); break;
	case 20: UIHelpPrint ("VELOCITY VIEW FUNCTIONS","STATUS FUNCTIONS"); break;
        case 21: UIHelpPrint ("STATUS FUNCTIONS","HELP FUNCTIONS"); break;
	case 22: UIHelpPrint ("HELP FUNCTIONS","EXAMPLES"); break;
	case 23: UIHelpPrint ("EXAMPLES","END"); break;
		}
  return(0);
	}

/* print text on screen */
int UIHelpPrint (char *start,char *finish)
	{
	char *startp=NULL, *finishp=NULL, save;
	extern char *help;
	int nlen,i;

	
	startp = strstr (help,start);
	finishp = strstr (help,finish);
	if (startp == NULL || finishp == NULL){
		 return(0);
	}
 for (i=0;startp<finishp;i++, startp++){
		buf_me[i]=*startp;
	}
	buf_me[i]='\0';
	UIInfo (buf_me);
  return(0);
	}


int UIVelViewChoice (Widget widget,int item) {
  VelViewSetBuffer(item);
  UIDrawCanvas(widget);
}

/* mouse info */
int UIMouseInfo ()
	{
	UIDelayMessage ("MOUSE LEFT: zoom; MIDDLE: navigate; RIGHT: pick");
  return(0);
	}

/* dump vgrid floats callback */
int UIDumpFloats ()
	{
	Widget widget;
	View myv;

	myv=current_view();

	if (!myv->data) return(0);
	widget = XmCreatePromptDialog (ui->application, "files", NULL, 0);
	sprintf (filename, "%s_dump_vgrid", DataShortName(myv->data));
	XtVaSetValues (widget, XmNselectionLabelString, CS("Type dump-vgrid-float file name:"),
			XmNtextString, CS (filename), NULL);
	XtAddCallback (widget, XmNokCallback, (XtCallbackProc)UIDumpFloats2, NULL);
	XtAddCallback (widget, XmNcancelCallback, (XtCallbackProc)XtDestroyWidget, NULL);
	XtManageChild (widget);
  return(0);
	}

int UIDumpFloats2 (Widget widget,XtPointer stuff,XmFileSelectionBoxCallbackStruct *cbs)
	{
	char *filename;
	int fd;
	View myv;

	myv=current_view();

	XmStringGetLtoR (cbs->value, XmSTRING_DEFAULT_CHARSET, &filename);
	if ((fd = creat (filename,0444)) < 0) {
		UIMessage ("cant create dump file");
		}
	else	{
		DataDumpFloats (myv->data,filename,fd);
		XtDestroyWidget (widget);
		}
  return(0);
	}

/* dump vgrid bytess callback */
int UIDumpBytes ()
	{
	Widget widget;
	View myv;

	myv=current_view();
	if (!myv->data) return(0);
	widget = XmCreatePromptDialog (ui->application, "files", NULL, 0);
	sprintf (filename, "%s_dump_vgrid", DataShortName(myv->data));
	XtVaSetValues (widget, XmNselectionLabelString, CS("Type dump-vgrid-byte file name:"),
			XmNtextString, CS (filename), NULL);
	XtAddCallback (widget, XmNokCallback, (XtCallbackProc)UIDumpBytes2, NULL);
	XtAddCallback (widget, XmNcancelCallback, (XtCallbackProc)XtDestroyWidget, NULL);
	XtManageChild (widget);
  return(0);
	}

int UIDumpBytes2 (Widget widget,XtPointer stuff,XmFileSelectionBoxCallbackStruct *cbs)
	{
	char *filename;
	int fd;
	View myv;

	myv=current_view();

	XmStringGetLtoR (cbs->value, XmSTRING_DEFAULT_CHARSET, &filename);
	if ((fd = creat (filename,0444)) < 0) {
		UIMessage ("can't create dump file");
		}
	else	{
		DataDumpBytes (myv->data,filename,fd);
		XtDestroyWidget (widget);
		}
	return(0);
	}

/* save parameters callback */
FILE *savefd = 0;
int UISavePar ()
	{
	Widget widget;
	View myv;

	myv=current_view();

	if (!myv->data) return(0);
	widget = XmCreatePromptDialog (ui->application, "files", NULL, 0);
	sprintf (filename, "%s_save_H", DataShortName(myv->data));
	XtVaSetValues (widget, XmNselectionLabelString, CS("Type save-pars file name:"),
			XmNtextString, CS (filename), NULL);
	XtAddCallback (widget, XmNokCallback, (XtCallbackProc)UISavePar2, NULL);
	XtAddCallback (widget, XmNcancelCallback, (XtCallbackProc)XtDestroyWidget, NULL);
	XtManageChild (widget);
	return(0);
	}

int UISavePar2 (Widget widget,XtPointer stuff,XmFileSelectionBoxCallbackStruct *cbs)
	{
	Message message;
  View myv;
	char *ff;

  myv=current_view();


	ff=filename;

	if (!myv->data || !myv) return(0);
	XmStringGetLtoR (cbs->value, XmSTRING_DEFAULT_CHARSET, &ff);
	if ((savefd = fopen (filename,"w+")) == NULL) {
		UIMessage ("can't create restart-par file");
		}
	else	{
		DataSavePar (myv->data);
		ViewSavePar (myv);
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
   return(0);
	}

/* save history list to file */
int UIHistorySave() {
  Widget widget;
  static int i = 1;
  
  widget = XmCreatePromptDialog (ui->application, "files", NULL, 0);
  sprintf (filename, "%d.rick_hist", i);
  XtVaSetValues (widget, XmNselectionLabelString, CS("Type save-history file name:"),
		 XmNtextString, CS (filename), NULL);
  XtAddCallback (widget, XmNokCallback, (XtCallbackProc)UIHistorySave2, NULL);
  XtAddCallback (widget, XmNcancelCallback, (XtCallbackProc)XtDestroyWidget, NULL);
  XtManageChild (widget);
  i++;
  return(0);
}

int UIHistorySave2 (Widget widget,XtPointer stuff,
		    XmFileSelectionBoxCallbackStruct *cbs) {
  Message message;
  char *ff;
  View myv;

  myv = current_view();
  ff=filename;
  if (!myv->data || !myv) return(0);
  XmStringGetLtoR (cbs->value, XmSTRING_DEFAULT_CHARSET, &ff);
  if ((savefd = fopen (ff,"w+")) == NULL) {
    sprintf(fileMessage, "History List: cannot create file %s.", ff);
    UIDelayMessage(fileMessage);
  }
  else	{
    ViewHistorySaveFile();
    fclose (savefd);
    XtDestroyWidget (widget);
    sprintf(fileMessage, "History List: information saved in file %s.",ff);
    UIDelayMessage(fileMessage);
  }
  return(0);
}

/* load history list from file */
int UIHistoryLoad() {
  Widget widget;

  widget = XmCreatePromptDialog (ui->application, "files", NULL, 0);
  sprintf (filename, "_.rick_hist");
  XtVaSetValues (widget, XmNselectionLabelString, CS("Type save-history file name:"),
		 XmNtextString, CS (filename), NULL);
  XtAddCallback (widget, XmNokCallback, (XtCallbackProc)UIHistoryLoad2, NULL);
  XtAddCallback (widget, XmNcancelCallback, (XtCallbackProc)XtDestroyWidget, NULL);
  XtManageChild (widget);
  return(0);
}

int UIHistoryLoad2 (Widget widget,XtPointer stuff,
			 XmFileSelectionBoxCallbackStruct *cbs) {
  Message message;
  char *ff;
  View myv;
  int succeed;

  myv = current_view();
  ff=filename;
  if (!myv->data || !myv) return(0);
  XmStringGetLtoR (cbs->value, XmSTRING_DEFAULT_CHARSET, &ff);
  if ((savefd = fopen (ff,"r")) == NULL) {
    sprintf(fileMessage, "History List: cannot read file %s.", ff);
    UIDelayMessage(fileMessage);
  }
  else	{
    succeed = ViewHistoryLoadFile();
    fclose (savefd);
    XtDestroyWidget (widget);
    if (1 == succeed) {
      sprintf (fileMessage, "History List: information read from file %s.",ff);
      UIDelayMessage(fileMessage);
    }
  }
  return(0);
}

/* write save message */
int UISaveMessage (Message message)
	{
	extern FILE *savefd;

	fprintf (savefd,"%s\n",message);
 return(0);
	}

/* exit program callback */
int UIQuit ()
	{
	int i;
	View myv;
	myv=current_view();

	for(i=0; i < num_views(); i++){
		SetActiveView(i);
		myv=current_view();
		myv->close_view_func();
	  PikWrite (myv->data->pik->file,fopen(myv->data->pik->file,"w"));
	}
	DrawFreePixmaps ();
	XCloseDisplay(UIDisplay());
	exit (0);
 return(0);
	}



/* default oreintation callback */
int UIOrient0 ()
	{
	ViewsOrient0 ();
  return(0);
	}

/* dump axis callbacks */
int UIDownDump ()
	{
  View myv;

  myv=current_view();



	MapDump (ViewMap(myv,AXIS_DOWN));
  return(0);
	}

int UIAcrossDump ()
	{
  View myv;

  myv=current_view();

	MapDump (ViewMap(myv,AXIS_ACROSS));
  return(0);
	}

int UIDeepDump ()
	{
  View myv;

  myv=current_view();

	MapDump (ViewMap(myv,AXIS_DEEP));
  return(0);
	}

/* swap axis callbacks */

int UISwapFrontSide ()
	{
	ViewSwapAxis (AXIS_ACROSS,AXIS_DEEP);
	UISyzeReset ();
	UIArrayReset (0);
	if (ui->style == VIEW_ARRAY) UIArrayDraw();
  return(0);
	}

int UISwapSideTop ()
	{
	ViewSwapAxis (AXIS_DOWN,AXIS_ACROSS);
	UISyzeReset ();
	if (ui->style == VIEW_ARRAY) UIArrayDraw();
  return(0);
	}

int UISwapTopFront ()
	{
	ViewSwapAxis (AXIS_DOWN,AXIS_DEEP);
	UISyzeReset ();
	UIArrayReset (0);
	if (ui->style == VIEW_ARRAY) UIArrayDraw();
  return(0);
	}

int UISwapFrontExtra2 ()
	{
	ViewSwapAxis (AXIS_DEEP,AXIS_5D);
	UISyzeReset ();
	UIArrayReset (0);
	if (ui->style == VIEW_ARRAY) UIArrayDraw();
  return(0);
	}

int UISwapSideExtra2 ()
	{
	ViewSwapAxis (AXIS_ACROSS,AXIS_5D);
	UISyzeReset ();
	UIArrayReset (0);
	if (ui->style == VIEW_ARRAY) UIArrayDraw();
  return(0);
	}

int UISwapTopExtra2 ()
	{
	ViewSwapAxis (AXIS_DOWN,AXIS_5D);
	UISyzeReset ();
	UIArrayReset (0);
	if (ui->style == VIEW_ARRAY) UIArrayDraw();
  return(0);
	}

int UISwapFrontExtra ()
	{
	ViewSwapAxis (AXIS_DEEP,AXIS_4D);
	UISyzeReset ();
	UIArrayReset (0);
	if (ui->style == VIEW_ARRAY) UIArrayDraw();
  return(0);
	}

int UISwapSideExtra ()
	{
	ViewSwapAxis (AXIS_ACROSS,AXIS_4D);
	UISyzeReset ();
	UIArrayReset (0);
	if (ui->style == VIEW_ARRAY) UIArrayDraw();
  return(0);
	}

int UISwapTopExtra ()
	{
	ViewSwapAxis (AXIS_DOWN,AXIS_4D);
	UISyzeReset ();
	UIArrayReset (0);
	if (ui->style == VIEW_ARRAY) UIArrayDraw();
  return(0);
	}

/* axis flip callbacks */
int UIFlipDown ()
	{
	ViewFlipAxis (AXIS_DOWN);
	UISyzeReset ();
  return(0);
	}

int UIFlipAcross ()
	{
	ViewFlipAxis (AXIS_ACROSS);
	UISyzeReset ();
  return(0);
	}

int UIFlipDeep ()
	{
	ViewFlipAxis (AXIS_DEEP);
	UISyzeReset ();
  return(0);
	}

/* set shape callback */
int UISizeChoice (Widget widget,int item)
	{
	if (item > SHAPE_PIXEL || item < SHAPE_FIT) return(0);
	ViewSetShape (item);
	return(0);
	}

/* interpolation choice callback */
int UIInterpolateToggle (Widget widget)
	{
	RenderToggleInterp ();
	ViewDrawAll ();
  return(0);
	}

/* default zoom callback */
int UISize0 ()
	{
	RenderSetInterp (0);
	ViewWindow0 ();
	UISyzeReset ();
  return(0);
	}

/* default screen size callback (doesn't work) */
int UIScreen0 ()
	{
	if (!ui) return(0);
	XtVaSetValues (ui->canvas,XmNwidth,ui->wide,XmNheight,ui->hite,NULL);
  return(0);
	}

/* region smooth callback */
int UISubvolumeSmooth ()
	{
	View myv;
	myv=current_view();
	
	DrawWatch (1);
	RegionSetValue (RegionBound(1),RegionBound(1));
	DataComputeHistogram (myv->data);
	ViewDrawAll ();
  return(0);
	}

/* region smooth undo */
int UISmoothUndo ()
	{
	View myv;
	
	myv=current_view();
	
	DrawWatch (1);
	RegionRestoreValue ();
	DataComputeHistogram (myv->data);
	ViewDrawAll ();
  return(0);
	}

/* clear picks callback */
int UIPickClear (Widget widget,XButtonEvent event)
	{
	PickClear0 ();
	RegionClear ();
  return(0);
	}

int UIEditGrade ()
	{
  View myv;

  myv=current_view();

	DrawWatch (1);
	EditGrade (DataBuffer(myv->data),ViewMap(myv,AXIS_DOWN),ViewMap(myv,AXIS_ACROSS),ViewMap(myv,AXIS_DEEP));
	DataComputeHistogram (myv->data);
	ViewDrawAll ();
  return(0);
	}

int UIGradeUndo ()
	{
	View myv;

	myv=current_view();

	DrawWatch (1);
	EditUndo (DataBuffer(myv->data));
	DataComputeHistogram (myv->data);
	ViewDrawAll ();
  return(0);
	}

int UIStatistics ()
	{
  View myv;
	int n, size, i, low, median, high;
	float dist[256];
	Message message;

  myv=current_view();

	for (size=1, i=1; i<4; i++) size *= MapWindow (ViewMap(myv,i));
	n = EditBox (DataBuffer(myv->data),ViewMap(myv,AXIS_DOWN),ViewMap(myv,AXIS_ACROSS),ViewMap(myv,AXIS_DEEP),dist);
	EditStats (n,dist,&low,&median,&high);
	printf ("box: n=%d low=%d median=%d high=%d\n", n,low,median,high);
	n = EditCube (DataBuffer(myv->data),ViewMap(myv,AXIS_DOWN),ViewMap(myv,AXIS_ACROSS),ViewMap(myv,AXIS_DEEP),dist);
	EditStats (n,dist,&low,&median,&high);
	printf ("cube: n=%d low=%d median=%d high=%d\n", n,low,median,high);
	UIMessage (message);
  return(0);
	}

int UIWakeup ()
	{
	XmProcessTraversal(ui->canvas, XmTRAVERSE_CURRENT);
  return(0);
	}
/* pik write callback */
int UIPikWrite ()
	{
	Widget widget;
	View myv;
	myv=current_view();

	if (!myv->data) return(0);
	widget = XmCreatePromptDialog (ui->application, "files", NULL, 0);
	XtVaSetValues (widget, XmNselectionLabelString, CS("Type write picks file name:"),
			XmNtextString, CS (myv->data->pik->file), NULL);
	XtAddCallback (widget, XmNokCallback, (XtCallbackProc)UIPikWrite2, NULL);
	XtAddCallback (widget, XmNcancelCallback, (XtCallbackProc)XtDestroyWidget, NULL);
	XtManageChild (widget);
  return(0);
	}

int UIPikWrite2 (Widget widget,XtPointer stuff,XmFileSelectionBoxCallbackStruct *cbs)
	{
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
   return(0);
	}

/* pik read callback */
int UIPikRead ()
	{
	Widget widget;
	View myv;

	myv=current_view();
	

	if (!myv->data) return(0);
	widget = XmCreatePromptDialog (ui->application, "files", NULL, 0);
	XtVaSetValues (widget, XmNselectionLabelString, CS("Type read picks file name:"),
			XmNtextString, CS (myv->data->pik->file), NULL);
	XtAddCallback (widget, XmNokCallback, (XtCallbackProc)UIPikRead2, NULL);
	XtAddCallback (widget, XmNcancelCallback, (XtCallbackProc)XtDestroyWidget, NULL);
	XtManageChild (widget);
  return(0);
	}

int UIPikRead2 (Widget widget,XtPointer stuff,XmFileSelectionBoxCallbackStruct *cbs)
	{
	char *filename;
	FILE *fd;
	View myv;

	myv=current_view();

	XmStringGetLtoR (cbs->value, XmSTRING_DEFAULT_CHARSET, &filename);
	if ((fd = fopen(filename,"r")) == NULL) {
		UIMessage ("cant create dump file");
		}
	else	{
		strcpy (myv->data->pik->file,filename);
		fclose (fd);
		PikRead (myv->data);
		XtDestroyWidget (widget);
		}
   return(0);
	}

int UIPathViewToggleOnOff (Widget widget) {
  int state;
  state = PathViewOn();
  PathViewToggle();
  if (state != PathViewOn()) {
    UIMotionOff(widget);
    UIDrawCanvas(widget);
  }
  return(0);
}

int UIPathViewStart(Widget widget) {
  if (PathViewOn() == 0) return(0);

  UIMotionOff(widget);
  PathViewMoveStart();
  ViewPathViewMove();
  return(0);
}

int UIPathViewMiddle(Widget widget) {
  if (PathViewOn() == 0) return(0);

  UIMotionOff(widget);
  PathViewMoveMiddle();
  ViewPathViewMove();
  return(0);
}

int UIPathViewEnd(Widget widget) {
  if (PathViewOn() == 0) return(0);
  
  UIMotionOff(widget);
  PathViewMoveEnd();
  ViewPathViewMove();
  return(0);
}

/* save front section callback */
int UISaveFront ()
	{
	Widget widget;
  View myv;

  myv=current_view();

	if (!myv->data) return(0);
	sprintf (filename,"%s_%s_%dx%d",
		DataShortName(myv->data),
		AxisScript(MapAxis(ViewMap(myv,AXIS_DEEP)),MapFrame(ViewMap(myv,AXIS_DEEP))),
		MapWindow(ViewMap(myv,AXIS_DOWN)),
		MapWindow(ViewMap(myv,AXIS_ACROSS)) );
	widget = XmCreatePromptDialog (ui->application, "files", NULL, 0);
	XtVaSetValues (widget, XmNselectionLabelString, CS("Type write picks file name:"),
			XmNtextString, CS (filename), NULL);
	XtAddCallback (widget, XmNokCallback, (XtCallbackProc)UISaveFront2, NULL);
	XtAddCallback (widget, XmNcancelCallback, (XtCallbackProc)XtDestroyWidget, NULL);
	XtManageChild (widget);
   return(0);
	}

int UISaveFront2 (Widget widget,XtPointer stuff,XmFileSelectionBoxCallbackStruct *cbs)
	{
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
  return(0);
	}

/* save side section callback */
int UISaveSide ()
	{
	Widget widget;
  View myv;

  myv=current_view();



	if (!myv->data) return(0);
	sprintf (filename,"%s_%s_%dx%d",
		DataShortName(myv->data),
		AxisScript(MapAxis(ViewMap(myv,AXIS_ACROSS)),MapFrame(ViewMap(myv,AXIS_ACROSS))),
		MapWindow(ViewMap(myv,AXIS_DOWN)),
		MapWindow(ViewMap(myv,AXIS_DEEP)) );
	widget = XmCreatePromptDialog (ui->application, "files", NULL, 0);
	XtVaSetValues (widget, XmNselectionLabelString, CS("Type write picks file name:"),
			XmNtextString, CS (filename), NULL);
	XtAddCallback (widget, XmNokCallback, (XtCallbackProc)UISaveSide2, NULL);
	XtAddCallback (widget, XmNcancelCallback, (XtCallbackProc)XtDestroyWidget, NULL);
	XtManageChild (widget);
  return(0);
	}

int UISaveSide2 (Widget widget,XtPointer stuff,XmFileSelectionBoxCallbackStruct *cbs)
	{
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
	 return(0);
	}

/* save top section callback */
int UISaveTop ()
	{
	Widget widget;
  View myv;

  myv=current_view();



	if (!myv->data) return(0);
	sprintf (filename,"%s_%s_%dx%d",
		DataShortName(myv->data),
		AxisScript(MapAxis(ViewMap(myv,AXIS_DOWN)),MapFrame(ViewMap(myv,AXIS_DOWN))),
		MapWindow(ViewMap(myv,AXIS_DEEP)),
		MapWindow(ViewMap(myv,AXIS_ACROSS)) );
	widget = XmCreatePromptDialog (ui->application, "files", NULL, 0);
	XtVaSetValues (widget, XmNselectionLabelString, CS("Type write picks file name:"),
			XmNtextString, CS (filename), NULL);
	XtAddCallback (widget, XmNokCallback, (XtCallbackProc)UISaveTop2, NULL);
	XtAddCallback (widget, XmNcancelCallback, (XtCallbackProc)XtDestroyWidget, NULL);
	XtManageChild (widget);
  return(0);
	}

 int UISaveTop2 (Widget widget,XtPointer stuff,XmFileSelectionBoxCallbackStruct *cbs)
	{
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
  return(0);
	}

/* save down profile callback */
int UISaveDown ()
	{
	Widget widget;
  View myv;

  myv=current_view();



	if (!myv->data) return(0);
	sprintf (filename,"%s_%s_%s_%d",
		DataShortName(myv->data),
		AxisScript(MapAxis(ViewMap(myv,AXIS_ACROSS)),MapFrame(ViewMap(myv,AXIS_ACROSS))),
		AxisScript(MapAxis(ViewMap(myv,AXIS_DEEP)),MapFrame(ViewMap(myv,AXIS_DEEP))),
		MapWindow(ViewMap(myv,AXIS_DOWN)) );
	widget = XmCreatePromptDialog (ui->application, "files", NULL, 0);
	XtVaSetValues (widget, XmNselectionLabelString, CS("Type write picks file name:"),
			XmNtextString, CS (filename), NULL);
	XtAddCallback (widget, XmNokCallback, (XtCallbackProc)UISaveDown2, NULL);
	XtAddCallback (widget, XmNcancelCallback, (XtCallbackProc)XtDestroyWidget, NULL);
	XtManageChild (widget);
 return(0);
	}

int  UISaveDown2 (Widget widget,XtPointer stuff,XmFileSelectionBoxCallbackStruct *cbs)
	{
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
  return(0);
	}

/* save across profile callback */
int UISaveAcross ()
	{
	Widget widget;
  View myv;

  myv=current_view();



	if (!myv->data) return(0);
	sprintf (filename,"%s_%s_%s_%d",
		DataShortName(myv->data),
		AxisScript(MapAxis(ViewMap(myv,AXIS_DEEP)),MapFrame(ViewMap(myv,AXIS_DEEP))),
		AxisScript(MapAxis(ViewMap(myv,AXIS_DOWN)),MapFrame(ViewMap(myv,AXIS_DOWN))),
		MapWindow(ViewMap(myv,AXIS_ACROSS)) );
	widget = XmCreatePromptDialog (ui->application, "files", NULL, 0);
	XtVaSetValues (widget, XmNselectionLabelString, CS("Type write picks file name:"),
			XmNtextString, CS (filename), NULL);
	XtAddCallback (widget, XmNokCallback, (XtCallbackProc)UISaveAcross2, NULL);
	XtAddCallback (widget, XmNcancelCallback, (XtCallbackProc)XtDestroyWidget, NULL);
	XtManageChild (widget);
 return(0);
	}

int UISaveAcross2 (Widget widget,XtPointer stuff,XmFileSelectionBoxCallbackStruct *cbs)
	{
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
  return(0);
	}

/* save deep profile callback */
int UISaveDeep ()
	{
	Widget widget;
  View myv;

  myv=current_view();



	if (!myv->data) return(0);
	sprintf (filename,"%s_%s_%s_%d",
		DataShortName(myv->data),
		AxisScript(MapAxis(ViewMap(myv,AXIS_DOWN)),MapFrame(ViewMap(myv,AXIS_DEEP))),
		AxisScript(MapAxis(ViewMap(myv,AXIS_ACROSS)),MapFrame(ViewMap(myv,AXIS_ACROSS))),
		MapWindow(ViewMap(myv,AXIS_DEEP)) );
	widget = XmCreatePromptDialog (ui->application, "files", NULL, 0);
	XtVaSetValues (widget, XmNselectionLabelString, CS("Type write picks file name:"),
			XmNtextString, CS (filename), NULL);
	XtAddCallback (widget, XmNokCallback, (XtCallbackProc)UISaveDeep2, NULL);
	XtAddCallback (widget, XmNcancelCallback, (XtCallbackProc)XtDestroyWidget, NULL);
	XtManageChild (widget);
  return(0);
	}

int UISaveDeep2 (Widget widget,XtPointer stuff,XmFileSelectionBoxCallbackStruct *cbs)
	{
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
  return(0);
	}
int UIViewCommands ()
        {
         ViewCommands();
        }
#endif

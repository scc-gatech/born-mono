#include <sepConfig.h>
#if defined (MOTIF_FOUND) || defined(HAVE_ATHENA)
/*
User_interface: canvas controls
*/
#include <stdio.h>
#include "rick.h"

extern UI ui;
extern Message message;

/* duplicate entries for mouse+key are needed for different window managers */
/* canvas mouse callbacks */
char canvas_trans[] =
	"#replace\n\
        Shift<Key>m:            UIMenuToggleOnOff()\n\
        Shift<Key>p:            UIPathViewToggleOnOff()\n\
        Shift<Key>r:            UIRoundRobinToggle()\n\
        Ctrl<Key>p:             UIMotionOff()\n\
        Ctrl<Key>u:             UIHistoryUndo()\n\
        Ctrl<Key>r:             UIHistoryRedo()\n\
        Shift<Key>x:            UIAcrossLargeX()\n\
        Shift<Key>y:            UIDownLargeY()\n\
        Shift<Key>z:            UIDeepLargeZ()\n\
        <Key>h,<Btn1Down>:      UIHZoomStart()\n\
        <Key>v,<Btn1Down>:      UIVZoomStart()\n\
	<Btn1Down>:	        UIZoomStart()\n\
	<Btn1Motion>:	        UIZoomDrag()\n\
	<Btn1Up>:	        UIZoomEnd()\n\
	<Btn2Down>:	        UIFrameStart()\n\
	<Btn2Motion>:	        UIFrameDrag()\n\
	<Btn2Up>:	        UIFrameEnd()\n\
        <Key>a,<Btn3Down>:      UIPikAdd()\n\
	<Btn3Down>,<Key>a:	UIPikAdd()\n\
        <Key>b,<Btn3Down>:      UIPikBest()\n\
	<Btn3Down>,<Key>b:	UIPikBest()\n\
        <Key>m,<Btn3Down>:      UIPikMove()\n\
	<Btn3Down>,<Key>m:	UIPikMove()\n\
        <Key>f,<Btn3Down>:      UIFinish()\n\
	<Btn3Down>,<Key>f:	UIFinish()\n\
        <Key>p,<Btn3Motion>:    UIPikDrag()\n\
	<Btn3Motion>,<Key>p:	UIPikDrag()\n\
        <Key>p,<Btn3Down>:      UIPikStart()\n\
	<Btn3Down>,<Key>p:	UIPikStart()\n\
        <Key>p,<Btn3Up>:        UIPikEnd()\n\
	<Btn3Up>,<Key>p:	UIPikEnd()\n\
        <Key>c,<Btn3Down>:      UIPikSymbol()\n\
	<Btn3Down>,<Key>c:	UIPikSymbol()\n\
        <Key>d,<Btn3Down>:      UIPikDelete()\n\
	<Btn3Down>,<Key>d:	UIPikDelete()\n\
        <Key>q,<Btn3Down>:      UIPikQuery()\n\
	<Btn3Down>,<Key>q:	UIPikQuery()\n\
        <Key>?,<Btn3Down>:      UIPikQuery()\n\
	<Btn3Down>,<Key>?:	UIPikQuery()\n\
        <Key>l,<Btn3Up>:        UIDrawIndLine()\n\
	<Btn3Down>,<Key>l:	UIDrawIndLine()\n\
        <Key>l,<Btn3Down>:      UIEraseIndLine()\n\
	<Btn3Up>,<Key>l:	UIEraseIndLine()\n\
        <Key>s,<Btn3Motion>:    UISubvolumeDrag()\n\
	<Btn3Motion>,<Key>s:	UISubvolumeDrag()\n\
        <Key>s,<Btn3Down>:      UISubvolumeStart()\n\
	<Btn3Down>,<Key>s:	UISubvolumeStart()\n\
        <Key>s,<Btn3Up>:        UISubvolumeEnd()\n\
	<Btn3Up>,<Key>s:	UISubvolumeEnd()\n\
        <Key>g,<Btn3Motion>:    UIGrowDrag()\n\
	<Btn3Motion>,<Key>g:	UIGrowDrag()\n\
        <Key>g,<Btn3Down>:      UIGrowStart()\n\
	<Btn3Down>,<Key>g:	UIGrowStart()\n\
        <Key>g,<Btn3Up>:        UIGrowEnd()\n\
	<Btn3Up>,<Key>g:	UIGrowEnd()\n\
        <Key>e,<Btn3Motion>:    UIEraseDrag()\n\
	<Btn3Motion>,<Key>e:	UIEraseDrag()\n\
        <Key>e,<Btn3Down>:      UIEraseStart()\n\
	<Btn3Down>,<Key>e:	UIEraseStart()\n\
        <Key>e,<Btn3Down>:      UIEraseEnd()\n\
        <Btn3Up>,<Key>e:        UIEraseEnd()\n\
        <Key>-:                 UIPathViewBackward()\n\
        <Key>=:                 UIPathViewForward()\n\
	<Key>r:	                UIRun()\n\
	<Key>f:	                UIFast()\n\
	<Key>s:	                UISlow()\n\
	<Key>o:	                UIOverlay()\n\
	<Key>v:	                UIViewChangef()\n\
	<Key>V:	                UIViewChangeb()\n\
        <Key>x:                 UIAcrossSmallX()\n\
        <Key>y:                 UIDownSmallY()\n\
        <Key>z:                 UIDeepSmallZ()\n\
        <Key><:                 UIRoundRobinBackward()\n\
        <Key>>:                 UIRoundRobinForward()\n\
	<Btn3Down>:	        UIPick()";

char colorbar_trans[] = 
	"#replace\n\
	<BtnDown>:	        UIColorbarStart()\n\
	<BtnMotion>:	        UIColorbarDrag()\n\
	<BtnUp>:	        UIColorbarEnd()";

XtActionsRec ui_actions[] = {
	{"UIZoomStart", (XtActionProc)UIZoomStart},
	{"UIHZoomStart", (XtActionProc)UIHZoomStart},
	{"UIVZoomStart", (XtActionProc)UIVZoomStart},
	{"UIZoomDrag", (XtActionProc)UIZoomDrag},
	{"UIZoomEnd", (XtActionProc)UIZoomEnd},
	{"UIFrameStart", (XtActionProc)UIFrameStart},
	{"UIFrameDrag", (XtActionProc)UIFrameDrag},
	{"UIFrameEnd", (XtActionProc)UIFrameEnd},
	{"UISubvolumeStart", (XtActionProc)UISubvolumeStart},
	{"UISubvolumeDrag", (XtActionProc)UISubvolumeDrag},
	{"UISubvolumeEnd", (XtActionProc)UISubvolumeEnd},
	{"UIGrowStart", (XtActionProc)UIGrowStart},
	{"UIGrowDrag", (XtActionProc)UIGrowDrag},
	{"UIGrowEnd", (XtActionProc)UIGrowEnd},
	{"UIPikStart", (XtActionProc)UIPikStart},
	{"UIPikDrag", (XtActionProc)UIPikDrag},
	{"UIPikEnd", (XtActionProc)UIPikEnd},
	{"UIEraseStart", (XtActionProc)UIEraseStart},
	{"UIEraseDrag", (XtActionProc)UIEraseDrag},
	{"UIEraseEnd", (XtActionProc)UIEraseEnd},
	{"UIPick", (XtActionProc)UIPick},
	{"UIPikAdd", (XtActionProc)UIPikAdd},
	{"UIPikBest", (XtActionProc)UIPikBest},
	{"UIFinish", (XtActionProc)UIFinish},
	{"UIPikMove", (XtActionProc)UIPikMove},
	{"UIRun", (XtActionProc)UIRun},
	{"UIViewChangef", (XtActionProc)UIViewChangef},
	{"UIViewChangeb", (XtActionProc)UIViewChangeb},
	{"UIFast", (XtActionProc)UIFast},
	{"UISlow", (XtActionProc)UISlow},
	{"UIOverlay", (XtActionProc)UIOverlay},
	{"UIPikSymbol", (XtActionProc)UIPikSymbol},
	{"UIPikDelete", (XtActionProc)UIPikDelete},
	{"UIPikQuery", (XtActionProc)UIPikQuery},
	{"UIDrawIndLine", (XtActionProc)UIDrawIndLine},
	{"UIEraseIndLine", (XtActionProc)UIEraseIndLine},
	{"PikUndo", (XtActionProc)PikUndo},
	{"UIColorbarStart", (XtActionProc)UIColorbarStart},
	{"UIColorbarDrag", (XtActionProc)UIColorbarDrag},
	{"UIColorbarEnd", (XtActionProc)UIColorbarEnd},
        {"UIAcrossLargeX", (XtActionProc)UIAcrossLargeX},
	{"UIDownLargeY", (XtActionProc)UIDownLargeY},
	{"UIDeepLargeZ", (XtActionProc)UIDeepLargeZ},
	{"UIAcrossSmallX", (XtActionProc)UIAcrossSmallX},
	{"UIDownSmallY", (XtActionProc)UIDownSmallY},
	{"UIDeepSmallZ", (XtActionProc)UIDeepSmallZ},
	{"UIHistoryUndo", (XtActionProc)UIHistoryUndo},
	{"UIHistoryRedo", (XtActionProc)UIHistoryRedo},
	{"UIMotionOff", (XtActionProc)UIMotionOff},
	{"UIRoundRobinBackward", (XtActionProc)UIRoundRobinBackward},
	{"UIRoundRobinForward", (XtActionProc)UIRoundRobinForward},
	{"UIRoundRobinToggle", (XtActionProc)UIRoundRobinToggle},
	{"UIPathViewForward", (XtActionProc)UIPathViewForward},
	{"UIPathViewBackward", (XtActionProc)UIPathViewBackward},
	{"UIPathViewToggleOnOff", (XtActionProc)UIPathViewToggleOnOff},
	{"UIMenuToggleOnOff", (XtActionProc)UIMenuToggleOnOff},
};
int ui_nactions = sizeof (ui_actions) / sizeof (ui_actions[0]);

/* return canvas size */
int UICanvasSize (int *wide,int *hite)
	{
	if (!ui) return(0);
	*wide = 0;
	*hite = 0;
	XtVaGetValues (ui->canvas,XmNwidth,wide,XmNheight,hite,NULL);
#ifndef LINUX
	*wide /= 65536;
	*hite /= 65536;
#endif
  return(0);
	}

/* return color bar size */
int UIColorbarSize (int *wide,int *hite)
	{
	if (!ui) return(0);
	*wide = 0;
	*hite = 0;
	XtVaGetValues (ui->colorbar,XmNwidth,wide,XmNheight,hite,NULL);
#ifndef LINUX
	*wide /= 65536;
	*hite /= 65536;
#endif
	return(0);
	}

/* callbacks:
	call one or routines of other objects with arguments
	fetch state from widget
	set state of user interface to be conistent
if a callback does not fetch or set state and is only one routine,
	the it is called directly from other objects
*/
/* draw color bar after exposure or resize callback */
int UIDrawColorbar (Widget widget)
{
  int wide, hite;

  if (!ui) return(0);
  if (ui->first) {
    XtAppAddTimeOut(ui->context, (unsigned long) 1000, UIColorbarCheckUpdate, NULL);
    if (ui->first == 1) {
      MainFirst();
      ui->first = 2;
    }
    else if (ui->first == 2) {
      ui->first = 0;
    }
  }
  ui->colorbarActive++;
  ui->colorbarActive = ui->colorbarActive % 10000;
  UIColorbarResizing(1);
  UIColorbarRelocating(1);
  return(0);
}

/* return canvas location (upper left) */
int UICanvasLocation (int *x,int *y) {
  if (!ui) return(0);
  *x = 0;
  *y = 0;
  XtVaGetValues (ui->main, XmNx, x, XmNy, y, NULL);
  return(0);
}

/* return colorbar location (upper left) */
int UIColorbarLocation (int *x, int *y) {
  if (!ui) return(0);
  *x = 0;
  *y = 0;
  XtVaGetValues (ui->colorbar, XmNx, x, XmNy, y, NULL);
  return(0);
}

/* draw canvas quickly but safely */
int UIDrawCanvasMovie (Widget widget)
{
  if (UICanvasResizing(0))
    UIDrawCanvas(widget);
  else {
    ViewSize0();
    ViewDrawAll();
  }
  return(0);
}

/* draw canvas after exposure or resize callback */
int UIDrawCanvas (Widget widget)
{
  if (!ui) return(0);
  // first time handling
  if (ui->first) {
    XtAppAddTimeOut(ui->context, (unsigned long) 2*CANVAS_REFRESH_WAIT, UICanvasCheckUpdate, NULL);
    if (ui->first == 1) {
      MainFirst();
      ui->first = 2;
    }
    else if (ui->first == 2) 
      ui->first = 0;
  }

  // pause movies
  if ((ui->movieOn = MovieRun()) != 0) {
    MovieOff();
  }
  else if ((ui->historyOn = UIHistoryAnimateRun()) != 0) {
    UIHistoryAnimateOff(NULL);
  }
  else if ((ui->pathOn = UIPathViewAnimateRun()) != 0) {
    UIPathViewAnimateOff(NULL);
  }

  // update states
  ui->canvasActive++;
  ui->canvasActive = ui->canvasActive % 10000;
  UICanvasResizing(1);
  UICanvasRelocating(1);
  return(0);
}

/* start zoom window callback */
int UIEraseStart (Widget widget,XButtonEvent *event)
	{
	if (!ui || !ui->canvasStable) {
	  ui->eraseDragValid = 0;
	  return(0);
	}
	ui->eraseDragValid = 1;
	ui->x1 = event->x;
	ui->y1 = event->y;
  return(0);
	}

/* start zoom window callback */
int UIGrowStart (Widget widget,XButtonEvent *event)
	{
	if (!ui || !ui->canvasStable) {
	  ui->growDragValid = 0;
	  return(0);
	}
	ui->growDragValid = 1;
	ui->x1 = event->x;
	ui->y1 = event->y;
  return(0);
	}
/* start zoom window callback */
int UIPikStart (Widget widget,XButtonEvent *event)
	{
	if (!ui || !ui->canvasStable) {
	  ui->pikDragValid = 0;
	  return(0);
	}
	ui->pikDragValid = 1;
	ui->x1 = event->x;
	ui->y1 = event->y;
  return(0);
	}

/* start zoom window callback */
int UIZoomStart (Widget widget,XButtonEvent *event)
	{
	  if (!ui || !ui->canvasStable) {
	    ui->zoomDragValid = 0;
	    return(0);
	  }
	  ui->zoomDragValid = 1;
	  ui->hzoom = 1;
	  ui->vzoom = 1;
	  ui->x1 = event->x;
	  ui->y1 = event->y;
  return(0);
	}

int UIHZoomStart (Widget widget,XButtonEvent *event)
	{
	if (!ui || !ui->canvasStable) {
	  ui->zoomDragValid = 0;
	  return(0);
	}
	ui->zoomDragValid = 1;
	ui->hzoom = 1;
	ui->x1 = event->x;
	ui->y1 = event->y;
  return(0);
	}

int UIVZoomStart (Widget widget,XButtonEvent *event)
	{
	if (!ui || !ui->canvasStable) {
	  ui->zoomDragValid = 0;
	  return(0);
	}
	ui->zoomDragValid = 1;
	ui->vzoom = 1;
	ui->x1 = event->x;
	ui->y1 = event->y;
  return(0);
	}

/* colorbar pick start callback */
int UIColorbarStart (Widget widget,XButtonEvent *event)
	{
	if (!ui || !ui->colorbarStable) {
	  ui->colorbarDragValid = 0;
	  return(0);
	}
	ui->colorbarDragValid = 1;
	DrawWindow (UIColorbarWindow());
	ui->x1 = event->x;
	ui->y1 = event->y;
	UIColorbarDrag (widget,event);
	return(0);
	}

/* zoom window drag callback */
int UIZoomDrag (Widget widget,XButtonEvent *event)
	{
	int width,height,xc,yc;
	if (!ui || !ui->canvasStable || !ui->zoomDragValid) return(0);
	if(grab_do_ano()==1){
		AnoOval (ui->x1,ui->y1,ui->x2,ui->y2,ERASE);
		AnoOval (ui->x1,ui->y1,ui->x2=event->x,ui->y2=event->y,DRAW);
	}
	else{
		DrawBox (ui->x1,ui->y1,ui->x2,ui->y2,ERASE_ALL);
		DrawBox (ui->x1,ui->y1,ui->x2=event->x,ui->y2=event->y,DRAW);
	}
  return(0);
	}

/* colorbar pick drag callback */
int UIColorbarDrag (Widget widget,XButtonEvent *event)
	{
	View myv;
  
  myv=current_view();

	if (!ui || !ui->colorbarStable || !ui->colorbarDragValid) return(0);
	ColorbarSetMark (MapMap(ViewMap(myv,AXIS_COLOR),event->x),
		MapMap(ViewMap(myv,AXIS_COLOR),event->x),TOP_MARK);
  return(0);
	}

/* smooth drag callback */
int UISubvolumeDrag (Widget widget,XButtonEvent *event)
	{
	PickPoint_ pick;

	if (!ui || !ui->canvasStable || !ui->subvolumeDragValid) return(0);
	if ((ui->x1-event->x)*(ui->x1-event->x)+(ui->y1-event->y)*(ui->y1-event->y)<25) return(0);
	DrawArrow (ui->x1,ui->y1,ui->x2,ui->y2,10,ERASE);
	DrawArrow (ui->x1,ui->y1,ui->x2=event->x,ui->y2=event->y,10,DRAW);
	PickDecode (event->x,event->y,&pick,1);
	ui->v2 = pick.index[DATA_VALUE];
	ColorbarSetMark (ui->v1,ui->v2,BOTTOM_MARK);
  return(0);
	}

/* frame drag callback */
int UIGrowDrag (Widget widget,XButtonEvent *event)
	{
	  if (!ui || !ui->canvasStable || !ui->growDragValid) return(0);
	DrawBox (ui->x1,ui->y1,ui->x2,ui->y2,ERASE_ALL);
	DrawBox (ui->x1,ui->y1,ui->x2=event->x,ui->y2=event->y,DRAW);
  return(0);
}
/* frame drag callback */
int UIPikDrag (Widget widget,XButtonEvent *event)
	{
	  if (!ui || !ui->canvasStable || !ui->pikDragValid) return(0);

	PickPoint_ pick;
	DrawLine (ui->x1,ui->y1,ui->x2,ui->y2,ERASE_ALL);
	DrawLine (ui->x1,ui->y1,ui->x2=event->x,ui->y2=event->y,DRAW);
  return(0);
}
int UIEraseDrag (Widget widget,XButtonEvent *event)
	{
	  if (!ui || !ui->canvasStable || !ui->eraseDragValid) return(0);
	  DrawBox (ui->x1,ui->y1,ui->x2,ui->y2,ERASE_ALL);
	DrawBox (ui->x1,ui->y1,ui->x2=event->x,ui->y2=event->y,DRAW);
  return(0);
}

/* frame drag callback */
int UIFrameDrag (Widget widget,XButtonEvent *event)
	{
	  if (!ui || !ui->canvasStable || !ui->frameDragValid) return(0);
	PickPoint_ pick;
	if(grab_do_ano()!=1){ 
	DrawLine (ui->x1,ui->y1,ui->x2,ui->y2,ERASE_ALL);
	DrawLine (ui->x1,ui->y1,ui->x2=event->x,ui->y2=event->y,DRAW);
	PickDecode (event->x,event->y,&pick,1);
	}
	else{
	DrawBox (ui->x1,ui->y1,ui->x2,ui->y2,ERASE_ALL);
	DrawBox (ui->x1,ui->y1,ui->x2=event->x,ui->y2=event->y,DRAW);
	PickDecode (event->x,event->y,&pick,1);
	}
  return(0);
	}

int UIGrowEnd (Widget widget,XButtonEvent *event)
	{
	  if (!ui || !ui->canvasStable || !ui->growDragValid) return(0);
	DrawBox (ui->x1,ui->y1,ui->x2,ui->y2,ERASE);
	RunGrowPik(ui->x1,ui->y1,ui->x2,ui->y2);
	PathViewUpdatePicks();
	UIDrawCanvas(NULL);
	return(0);
}

int UIPikEnd (Widget widget,XButtonEvent *event)
	{
	  if (!ui || !ui->canvasStable || !ui->pikDragValid) return(0);
	DrawLine (ui->x1,ui->y1,ui->x2,ui->y2,ERASE);
	RunAutoPik(ui->x1,ui->y1,ui->x2,ui->y2);
	PathViewUpdatePicks();
	UIDrawCanvas(NULL);
	return(0);
}

int UIEraseEnd (Widget widget,XButtonEvent *event)
	{
	  if (!ui || !ui->canvasStable || !ui->eraseDragValid) return(0);
	DrawBox (ui->x1,ui->y1,ui->x2,ui->y2,ERASE);
	ErasePikWindow(ui->x1,ui->y1,ui->x2,ui->y2);
	PathViewUpdatePicks();
	UIDrawCanvas(NULL);
	return(0);
}


/* zoom window end callback */
int UIZoomEnd (Widget widget,XButtonEvent *event)
	{
	  int iview;
	  if (!ui || !ui->canvasStable || !ui->zoomDragValid) return(0);
	  iview = FindView(event->x,event->y);
	  if (iview != NO_INDEX) {
	    SetActiveView(iview);
	    UIDrawColorbar(NULL);
	  }

	  if(grab_do_ano()==1){
	    AnoOval (ui->x1,ui->y1,ui->x2,ui->y2,ERASE);
	    AddAnoOval(ui->x1,ui->y1,ui->x2,ui->y2);
	    UIFinishPanelRaise();
	  }
	  else{
	    DrawBox (ui->x1,ui->y1,ui->x2,ui->y2,ERASE);
	    if (ui->x1 == event->x && ui->y1 == event->y) {
	      UISize0 ();
	    }
	    else	{
	      ViewWindow (ui->x1,ui->y1,ui->x2=event->x,ui->y2=event->y,ui->hzoom,ui->vzoom);
	      ui->hzoom = 0;
	      ui->vzoom = 0;
	      ui->x1 = NO_INDEX;
	      ui->y1 = NO_INDEX;
	      ui->x2 = NO_INDEX;
	      ui->y2 = NO_INDEX;
	      UISizeReset ();
	    }
	  }
	  return(0);
	}

/* colorbar pick end callback */
int UIColorbarEnd (Widget widget,XButtonEvent *event)
	{
	  if (!ui || !ui->colorbarStable || !ui->colorbarDragValid) return(0);
	  View myv;
	  
	  myv=current_view();
	ColorbarSetMark (MapMap(ViewMap(myv,AXIS_COLOR),event->x),
		MapMap(ViewMap(myv,AXIS_COLOR),event->x),TOP_MARK);
	DrawWatch (1);
	RegionSetValue (MapMap(ViewMap(myv,AXIS_COLOR),event->x),RegionBound(1));
	DrawWatch (0);
	ViewDrawAll ();
	ColorbarSetMark (NO_INDEX,NO_INDEX,TOP_MARK);
	ColorbarSetMark (NO_INDEX,NO_INDEX,BOTTOM_MARK);
	ui->x1 = NO_INDEX;
	ui->y1 = NO_INDEX;
	ui->x2 = NO_INDEX;
	ui->y2 = NO_INDEX;
	DrawWindow (UICanvasWindow());
  return(0);
	}

/* smooth end callback */
int UISubvolumeEnd (Widget widget,XButtonEvent *event)
	{
	  if (!ui || !ui->canvasStable || !ui->subvolumeDragValid) return(0);
	PickPoint_ pick1, pick2;

	if (!ui) return(0);
	DrawArrow (ui->x1,ui->y1,ui->x2,ui->y2,10,ERASE);
	PickDecode (ui->x1,ui->y1,&pick1,1);
	PickDecode (ui->x2,ui->y2,&pick2,1);
	if (!PickSameFrame (&pick1,&pick2)) return(0);
	RegionSetBound (ui->v1,0);
	RegionSetBound (ui->v2,1);
	DrawWatch (1);
	RegionMark0 ();
	DrawWatch (0);
	ui->x1 = NO_INDEX;
	ui->y1 = NO_INDEX;
	ui->x2 = NO_INDEX;
	ui->y2 = NO_INDEX;
  return(0);
	}

/* report pick point values */
int UIPick (Widget widget,XButtonEvent *event)
	{
	PickPoint_ pick;

	PickDecode (event->x,event->y,&pick,1);
  return(0);
	}

/* movie callback */
int UIFrameStart (Widget widget,XButtonEvent *event)
	{
	if (!ui || !ui->canvasStable) {
	  ui->frameDragValid = 0;
	  return(0);
	}
	ui->frameDragValid = 1;
	ui->x1 = event->x;
	ui->y1 = event->y;
  return(0);
	}

int UIFrameEnd (Widget widget,XButtonEvent *event)
	{
	  if (!ui || !ui->canvasStable || !ui->frameDragValid) return(0);
	PickPoint_ pick1, pick2;
	int iaxis, iaxis1, sep[VIEW_NAXIS], iview;
	View myv;
  
        iview = FindView(event->x,event->y);
	if (iview != NO_INDEX) {
	  SetActiveView(iview);
	  UIDrawColorbar(NULL);
	}
	myv=current_view();

	if (!ui) return(0);
	PickDecode (ui->x1,ui->y1,&pick1,1);
	DrawLine (ui->x1,ui->y1,ui->x2,ui->y2,ERASE);
	ui->x2 = event->x;
	ui->y2 = event->y;
	PickDecode (ui->x2,ui->y2,&pick2,1);
	for (iaxis=AXIS_DOWN; iaxis<=AXIS_DEEP; iaxis++) {
		iaxis1 = AxisDir(MapAxis(ViewMap(myv,iaxis)));
		sep[iaxis1] = pick1.index[iaxis1] - MapFrame(ViewMap(myv,iaxis));
		sep[iaxis1] *= sep[iaxis1];
		}
	if ((ui->x1-ui->x2)*(ui->x1-ui->x2)+(ui->y1-ui->y2)*(ui->y1-ui->y2) < 30) {
		if(grab_do_ano()!=1) ViewSetFrames (event->x,event->y);
			else{
				AddAnoTextPt(ui->x2,ui->y2);
	      UIAnoTextRaise();
			}
		}
	else{
		if(grab_do_ano()==1){ /*Draw a rectangle */
				AddAnoRect(ui->x1,ui->y1,ui->x2,ui->y2);
				UIFinishPanelRaise();
			} 
			else{
	
	switch (pick1.iaxis[AXIS_DEEP]) {
	case AXIS_DEEP:
		if ((ui->x1-ui->x2)*(ui->x1-ui->x2)>(ui->y1-ui->y2)*(ui->y1-ui->y2)) {
			MapSetFrameBounds (ViewMap(myv,AXIS_ACROSS),pick1.index[AXIS_ACROSS],pick2.index[AXIS_ACROSS]);
			ViewSetMovie (AXIS_ACROSS);
			MovieOn ();
			}
		else	{
			MapSetFrameBounds (ViewMap(myv,AXIS_DOWN),pick1.index[AXIS_DOWN],pick2.index[AXIS_DOWN]);
			ViewSetMovie (AXIS_DOWN);
			MovieOn ();
			}
		break;
	case AXIS_ACROSS:
		if ((ui->x1-ui->x2)*(ui->x1-ui->x2)>(ui->y1-ui->y2)*(ui->y1-ui->y2)) {
			MapSetFrameBounds (ViewMap(myv,AXIS_DEEP),pick1.index[AXIS_DEEP],pick2.index[AXIS_DEEP]);
			ViewSetMovie (AXIS_DEEP);
			MovieOn ();
			}
		else	{
			MapSetFrameBounds (ViewMap(myv,AXIS_DOWN),pick1.index[AXIS_DOWN],pick2.index[AXIS_DOWN]);
			ViewSetMovie (AXIS_DOWN);
			MovieOn ();
			}
		break;
	case AXIS_DOWN:
		if ((ui->x1-ui->x2)*(ui->x1-ui->x2)>(ui->y1-ui->y2)*(ui->y1-ui->y2)) {
			MapSetFrameBounds (ViewMap(myv,AXIS_ACROSS),pick1.index[AXIS_ACROSS],pick2.index[AXIS_ACROSS]);
			ViewSetMovie (AXIS_ACROSS);
			MovieOn ();
			}
		else	{
			MapSetFrameBounds (ViewMap(myv,AXIS_DEEP),pick1.index[AXIS_DEEP],pick2.index[AXIS_DEEP]);
			ViewSetMovie (AXIS_DEEP);
			MovieOn ();
			}
		break;
		}
	}

	ui->x1 = NO_INDEX;
	ui->y1 = NO_INDEX;
	ui->x2 = NO_INDEX;
	ui->y2 = NO_INDEX;
	}
	return(0);
	}

/* add point point callback */
int UIPikBest (Widget widget,XButtonEvent *event)
	{
	PikBest ();
	PathViewUpdatePicks();
	UIDrawCanvas(NULL);
	return(0);
	}

/* add point point callback */
int UIPikAdd (Widget widget,XButtonEvent *event)
	{
	if(grab_do_ano()!=1){ 
	  PikAdd (event->x,event->y);
	}
	else{
	  AnoAdd(event->x,event->y);
	}
	PathViewUpdatePicks();
	UIDrawCanvas(NULL);
  return(0);
	}

int UIFinish (Widget widget,XButtonEvent *event)
	{
	  if(grab_do_ano()==1 && grab_last_op()!=ANO_FINISHED)
	    UIFinishPanelRaise();
	  else PikGrow3D();
	  return(0);
	}

/* insert a pick into the pick line */
int UIPickInsert (Widget widget,XButtonEvent *event)
	{
	PickInsert (event->x,event->y);
	return(0);
	}

/* start region picking */
int UISubvolumeStart (Widget widget,XButtonEvent *event)
	{
	PickPoint_ pick;

	if (!ui || !ui->canvasStable) {
	  ui->subvolumeDragValid = 0;
	  return(0);
	}
	ui->subvolumeDragValid = 1;
	ui->x1 = event->x;
	ui->y1 = event->y;
	PickDecode (event->x,event->y,&pick,1);
	RegionSetSeed (pick.index);
	ui->v1 = pick.index[DATA_VALUE];
  return(0);
	}

int UIEraseIndLine(Widget widget,XButtonEvent *event)
{
int iold,i;
View myv;
Plane plane;
extern Plane planelist;
PickPoint_ pick;
Pik pik1,pik2;
int x1,x2,y1,y2;

	myv=current_view();
	PickDecode (ui->x1,ui->y1,&pick,1);
	 for(i=1;i<DATA_NAXIS; i++) pik2.loc[i]=pik1.loc[i]=pick.index[i];
  pik1.loc[0]=pik2.loc[0]=3;
  pik1.loc[myv->data->pik->depend]=0;

   for (plane=planelist; plane < planelist+NPLANE &&
    plane->attr->orient != NO_INDEX; plane++) {
    if(AxisDir(MapAxis(PlaneHmap(plane))) == myv->data->pik->depend ||
      AxisDir(MapAxis(PlaneVmap(plane))) == myv->data->pik->depend ){
      if( AxisDir(MapAxis(PlaneVmap(plane)))==myv->data->pik->depend)
           pik2.loc[myv->data->pik->depend]=AxisSize(MapAxis(plane->vmap))-1;
      if( AxisDir(MapAxis(PlaneHmap(plane)))==myv->data->pik->depend)
           pik2.loc[myv->data->pik->depend]=AxisSize(MapAxis(plane->hmap))-1;
        if(1==x_y_from_pik_plane(pik1,plane,&x1,&y1) && 
        x_y_from_pik_plane(pik2,plane,&x2,&y2))
					DrawLine(x1,y1,x2,y2,ERASE);

    }
  }
return(0);
}

int UIDrawIndLine(Widget widget,XButtonEvent *event)
{
int iold,i;
View myv;
Plane plane;
extern Plane planelist;
PickPoint_ pick;
Pik pik1,pik2;
int x1,x2,y1,y2,i1,i2;

	myv=current_view();
	ui->x1 = event->x;
	ui->y1 = event->y;
	PickDecode (event->x,event->y,&pick,1);
	for(i=1;i<DATA_NAXIS; i++) pik2.loc[i]=pik1.loc[i]=pick.index[i];
	pik1.loc[0]=pik2.loc[0]=3;
	pik1.loc[myv->data->pik->depend]=0;

	 for (plane=planelist; plane < planelist+NPLANE &&
    plane->attr->orient != NO_INDEX; plane++) {
		if(AxisDir(MapAxis(PlaneHmap(plane))) == myv->data->pik->depend ||
      AxisDir(MapAxis(PlaneVmap(plane))) == myv->data->pik->depend ){
			if( AxisDir(MapAxis(PlaneVmap(plane)))==myv->data->pik->depend)
           pik2.loc[myv->data->pik->depend]=AxisSize(MapAxis(plane->vmap))-1;
			if( AxisDir(MapAxis(PlaneHmap(plane)))==myv->data->pik->depend)
           pik2.loc[myv->data->pik->depend]=AxisSize(MapAxis(plane->hmap))-1;
				if(1==x_y_from_pik_plane(pik1,plane,&x1,&y1) &&
				x_y_from_pik_plane(pik2,plane,&x2,&y2))
					DrawLine(x1,y1,x2,y2,DRAW);

		}
	}
return(0);
}



int UIPikMove (Widget widget,XButtonEvent *event)
  {
  PikMove (event->x,event->y);
  PathViewUpdatePicks();
  UIDrawCanvas(NULL);
  return(0);
  }

int UIFast (Widget widget,XButtonEvent *event)
  {
    if (!ui->canvasStable) return(0);

    int speed;
    MovieDeltaSpeed(5);
    speed = MovieGetSpeed();
    speed = MAX(speed,0);
    speed = MIN(speed,100);
    XtVaSetValues(ui->movieSpeed, XmNvalue, speed, NULL);
    return(0);
  }
int UIViewChangeb (Widget widget,XButtonEvent *event)
  {
    if (!ui->canvasStable) return(0);

  ViewChange(-1);
  return(0);
  }
int UIViewChangef (Widget widget,XButtonEvent *event)
  {
    if (!ui->canvasStable) return(0);

  ViewChange(1);
  return(0);
  }
int UISlow (Widget widget,XButtonEvent *event)
  {
    if (!ui->canvasStable) return(0);

    int speed;
    MovieDeltaSpeed(-5);
    speed = MovieGetSpeed();
    speed = MAX(speed,0);
    speed = MIN(speed,100);
    XtVaSetValues(ui->movieSpeed, XmNvalue, speed, NULL);
    return(0);
  }
int UIOverlay (Widget widget,XButtonEvent *event)
  {
    if (!ui->canvasStable) return(0);

  DrawOverlayToggle();
  return(0);
  }
int UIRun (Widget widget,XButtonEvent *event)
  {
    if (!ui->canvasStable) return(0);
    Widget child = 0;
    char buffer[32];
    
    UIHistoryAnimateOff(widget);
    UIPathViewAnimateOff(widget);
    MovieOn();

    sprintf(buffer, "button_0");
    child = XtNameToWidget(ui->movieOnOff,buffer);
    if (child != (Widget)0)
      XmToggleButtonSetState(child, XmSET, True);
    return(0);
  }

/* delete pick point callback */
int UIPikDelete (Widget widget,XButtonEvent *event)
        {
	PikDelete (event->x,event->y);
	PathViewUpdatePicks();
	UIDrawCanvas(NULL);
  return(0);
	}

/* delete pick point callback */
int UIPikSymbol (Widget widget,XButtonEvent *event)
	{
	PikSymbol();
  return(0);
	}

/* query pik point callback */
int UIPikQuery (Widget widget,XButtonEvent *event)
	{
	int i;
	if(grab_do_ano()==1){
		AnoModify(event->x,event->y);
	}
	else{
		PikQuery (event->x,event->y);
	}
  return(0);
}

int UIMotionOff(Widget widget) {
  Widget child = 0;
  char buffer[32];
  int play;

  MovieOff();
  UIHistoryAnimateOff(widget);
  UIPathViewAnimateOff(widget);

  sprintf(buffer, "button_1");
  child = XtNameToWidget(ui->movieOnOff,buffer);
  if (child != (Widget)0)
    XmToggleButtonSetState(child, XmSET, True);

  return(0);
}

int UIMotionRun() {
  int result;
  result = MovieRun();
  result = result || UIPathViewAnimateRun();
  result = result || UIHistoryAnimateRun();
  return (result);
}

/**********************************************************
 * Menu toggling
 **********************************************************/

int UIMenuToggleOnOff(Widget widget) {
  int w, h;

  if (!ui) return(0);
  UIMotionOff(widget);

  // add/remove components
  if (ui->menuOn) {
    XtUnmanageChild(ui->menuBar);
    XtUnmanageChild(ui->control1);
    XtUnmanageChild(ui->control2);
  }
  else {
    XtManageChild(ui->menuBar);
    XtManageChild(ui->control1);
    XtManageChild(ui->control2);
  }
  ui->menuOn = 1 - ui->menuOn;
  ui->menuWait = 1;

  // force resize to update component sizes
  XtVaGetValues(ui->base, XmNheight, &h, XmNwidth, &w, NULL);
  XtVaSetValues(ui->base, XmNheight, h+1, XmNwidth, w+1, NULL);
  XmUpdateDisplay(ui->base);
  XtVaSetValues(ui->base, XmNheight, h-1, XmNwidth, w-1, NULL);
  XmUpdateDisplay(ui->base);
  XtAppAddTimeOut(ui->context, (unsigned long) 3*CANVAS_REFRESH_WAIT, UIMenuToggleUpdate, NULL);
  return(0);
}

void UIMenuToggleUpdate(XtPointer ptr, XtIntervalId *id) {
  ui->menuWait = 0;
}

/**********************************************************
 * Directional movement by keys
 **********************************************************/

int UIAcrossSmallX (Widget widget, XButtonEvent *event) {
  if (!ui->canvasStable) return(0);
  Widget child = 0;
  char buffer[32];

  UIMotionOff(widget);
  if (UILocalDirection() == SMALL_X_DIR) {
    moveit();
  }
  else {
    sprintf(buffer, "button_%d", SMALL_X_DIR);
    child = XtNameToWidget(ui->movieDir,buffer);
    if (child != 0)
      XmToggleButtonSetState(child, XmSET, True);
  }
  return(0);
}

int UIAcrossLargeX (Widget widget, XButtonEvent *event) {
  if (!ui->canvasStable) return(0);
  Widget child = 0;
  char buffer[32];

  UIMotionOff(widget);
  if (UILocalDirection() == LARGE_X_DIR) {
    moveit();
  }
  else {
    sprintf(buffer, "button_%d", LARGE_X_DIR);
    child = XtNameToWidget(ui->movieDir,buffer);
    if (child != 0)
      XmToggleButtonSetState(child, XmSET, True);
  }
  return(0);
}

int UIDownSmallY (Widget widget, XButtonEvent *event) {
  if (!ui->canvasStable) return(0);
  Widget child = 0;
  char buffer[32];

  UIMotionOff(widget);
  if (UILocalDirection() == SMALL_Y_DIR) {
    moveit();
  }
  else {
    sprintf(buffer, "button_%d", SMALL_Y_DIR);
    child = XtNameToWidget(ui->movieDir,buffer);
    if (child != 0)
      XmToggleButtonSetState(child, XmSET, True);
  }
  return(0);
}

int UIDownLargeY (Widget widget, XButtonEvent *event) {
  if (!ui->canvasStable) return(0);
  Widget child = 0;
  char buffer[32];

  UIMotionOff(widget);
  if (UILocalDirection() == LARGE_Y_DIR) {
    moveit();
  }
  else {
    sprintf(buffer, "button_%d", LARGE_Y_DIR);
    child = XtNameToWidget(ui->movieDir,buffer);
    if (child != 0)
      XmToggleButtonSetState(child, XmSET, True);
  }
  return(0);
}

int UIDeepSmallZ (Widget widget, XButtonEvent *event) {
  if (!ui->canvasStable) return(0);
  Widget child = 0;
  char buffer[32];

  UIMotionOff(widget);
  if (UILocalDirection() == SMALL_Z_DIR) {
    moveit();
  }
  else {
    sprintf(buffer, "button_%d", SMALL_Z_DIR);
    child = XtNameToWidget(ui->movieDir,buffer);
    if (child != 0)
      XmToggleButtonSetState(child, XmSET, True);
  }
  return(0);
}

int UIDeepLargeZ (Widget widget, XButtonEvent *event) {
  if (!ui->canvasStable) return(0);
  Widget child = 0;
  char buffer[32];

  UIMotionOff(widget);
  if (UILocalDirection() == LARGE_Z_DIR) {
    moveit();
  }
  else {
    sprintf(buffer, "button_%d", LARGE_Z_DIR);
    child = XtNameToWidget(ui->movieDir,buffer);
    if (child != 0)
      XmToggleButtonSetState(child, XmSET, True);
  }
  return(0);
}

/**********************************************************
 * Velocity view
 **********************************************************/

int UIVelViewShiftRight(Widget widget) {
  int param;

  XtVaGetValues(ui->velocityParam, XmNvalue, &param, NULL);
  param += BUFFER_PARAM_RESOLUTION;
  if (param > BUFFER_PARAM_HIGH)
    param = BUFFER_PARAM_HIGH;
  XtVaSetValues(ui->velocityParam, XmNvalue, param, NULL);
  VelViewSetParam(param);
  if (VelViewOn())
    UIDrawCanvas(widget);
  return(0);
}

int UIVelViewShiftLeft(Widget widget) {
  int param;

  XtVaGetValues(ui->velocityParam, XmNvalue, &param, NULL);
  param -= BUFFER_PARAM_RESOLUTION;
    if (param < BUFFER_PARAM_LOW)
    param = BUFFER_PARAM_LOW;
  XtVaSetValues(ui->velocityParam, XmNvalue, param, NULL);
  VelViewSetParam(param);
  if (VelViewOn())
    UIDrawCanvas(widget);
  return(0);
}

/**********************************************************
 * History list
 **********************************************************/

int UIHistoryUndo(Widget widget, XButtonEvent *event) {
  UIMotionOff(widget);
  ViewHistoryUndo(DRAW);
  return(0);
}

int UIHistoryRedo(Widget widget, XButtonEvent *event) {
  UIMotionOff(widget);
  ViewHistoryRedo(DRAW);
  return(0);
}

int UIHistoryStart(Widget widget, XButtonEvent *event) {
  UIHistoryAnimateOff(widget);
  ViewHistoryStart();
  return(0);
}

int UIHistoryEnd(Widget widget, XButtonEvent *event) {
  UIHistoryAnimateOff(widget);
  ViewHistoryEnd();
  return(0);
}

void UIHistoryAnimateUndoUpdate(XtPointer ptr, XtIntervalId *id) {
  if (ui->undoAnimate == 1 && !ViewHistoryAtStart()) {
    MovieOff();
    UIPathViewAnimateOff(NULL);
    ViewHistoryUndo(DRAW);

#if defined(XAW)
    XtAppAddTimeOut(ui->context, (unsigned long) 3*MovieDelay()+1, UIHistoryAnimateUndoUpdate, ptr);
#endif
#if defined(MOTIF_FOUND)
    XtAppAddTimeOut(ui->context, (unsigned long) 30*MovieDelay()+1, UIHistoryAnimateUndoUpdate, ptr);
#endif

  }
  else {
    UIDrawCanvas(NULL);
    ui->undoAnimate = 0;
  }
}

void UIHistoryAnimateRedoUpdate(XtPointer ptr, XtIntervalId *id) {
  if (ui->redoAnimate == 1 && !ViewHistoryAtEnd()) {
    MovieOff();
    UIPathViewAnimateOff(NULL);
    ViewHistoryRedo(DRAW);

#if defined(XAW)
    XtAppAddTimeOut(ui->context, (unsigned long) 3*MovieDelay()+1, UIHistoryAnimateRedoUpdate, ptr);
#endif
#if defined(MOTIF_FOUND)
    XtAppAddTimeOut(ui->context, (unsigned long) 30*MovieDelay()+1, UIHistoryAnimateRedoUpdate, ptr);
#endif

  }
  else {
    UIDrawCanvas(NULL);
    ui->redoAnimate = 0;
  }
}

int UIHistoryAnimateOff(Widget widget) {
  ui->undoAnimate = 0;
  ui->redoAnimate = 0;
  return(0);
}

int UIHistoryAnimateRun() {
  if (ui->undoAnimate)
    return(-1);
  else if (ui->redoAnimate)
    return(1);
  else
    return(0);
}

/**********************************************************
 * Screen update
 **********************************************************/

int UICanvasResizing (int update) {
  if (!ui) return(0);
  int wide, hite, resizing;
  UICanvasSize(&wide, &hite);
  resizing = (wide != ui->canvasWide || hite != ui->canvasHite);
  if (resizing && update) {
    ui->canvasWide = wide;
    ui->canvasHite = hite;
  }
  return (resizing);
}

int UICanvasRelocating(int update) {
  if (!ui) return(0);
  int x, y, relocating;
  UICanvasLocation(&x, &y);
  relocating = (x != ui->canvasX || y != ui->canvasY);
  if (relocating && update) {
    ui->canvasX = x;
    ui->canvasY = y;
  }
  return (relocating);
}

int UIColorbarResizing(int update) { 
  if (!ui) return(0);
  int wide, hite, resizing;
  UIColorbarSize(&wide, &hite);
  resizing = (wide != ui->colorbarWide || hite != ui->colorbarHite);
  if (resizing && update) {
    ui->colorbarWide = wide;
    ui->colorbarHite = hite;
  }
  return (resizing);
}

int UIColorbarRelocating(int update) {
  if (!ui) return(0);
  int x, y, relocating;
  UIColorbarLocation(&x, &y);
  relocating = (x != ui->colorbarX || y != ui->colorbarY);
  if (relocating && update) {
    ui->colorbarX = x;
    ui->colorbarY = y;
  }
  return (relocating);
}

void UIColorbarCheckUpdate(XtPointer ptr, XtIntervalId *id) {
  int wide, hite;
  UIColorbarSize(&wide, &hite);

  if (ui->colorbarActive != ui->colorbarActivePrev || ui->menuWait) {
    ui->colorbarActivePrev = ui->colorbarActive;
    ui->colorbarStable = 0;
  }
  else if (!ui->colorbarStable || UIColorbarResizing(1)) {
    ui->colorbarStable = 1;
    ColorbarDraw();
  }
  XtAppAddTimeOut(ui->context, (unsigned long) COLORBAR_REFRESH_WAIT, UIColorbarCheckUpdate, ptr);
}

void UICanvasCheckUpdate(XtPointer ptr, XtIntervalId *id) {
  if (ui->canvasActive != ui->canvasActivePrev || ui->menuWait) {
    ui->canvasActivePrev = ui->canvasActive;
    ui->canvasStable = 0;
  }

  // second condition needed to update window minimization, which apparently
  // does not trigger a callback to UIDrawCanvas
  else if (!ui->canvasStable || UICanvasResizing(1)) {
    ui->canvasStable = 1;
    ViewSize0();
    ViewDrawAll();
      
    // restart paused movie
    if (ui->movieOn != 0) {
      MovieOn();
      ui->movieOn = 0;
    }
    else if (ui->historyOn != 0) {
      if (ui->historyOn == 1) UIHistoryAnimateRedo(NULL);
      else UIHistoryAnimateUndo(NULL);
      ui->historyOn = 0;
    }
    else if (ui->pathOn != 0) {
      if (ui->pathOn == 1) UIPathViewAnimateForward(NULL);
      else UIPathViewAnimateBackward(NULL);
      ui->pathOn = 0;
    }
    else
      UIMotionOff(NULL);

    // make sure delayed message is displayed
    if (ui->messageOn != 0) {
      UIDisplayDelayedMessage(NULL);
      ui->messageOn = 0;
    }
  }
  XtAppAddTimeOut(ui->context, (unsigned long) CANVAS_REFRESH_WAIT, UICanvasCheckUpdate, ptr);
}

/**********************************************************
 * Round robin
 **********************************************************/

int UIRoundRobinToggle(Widget widget) {
  ViewRoundRobinToggle(DRAW);
  return(0);
}

int UIRoundRobinForward(Widget widget) {
  ViewRoundRobinForward(DRAW);
  return(0);
}

int UIRoundRobinBackward(Widget widget) {
  ViewRoundRobinBackward(DRAW);
  return(0);
}

/**********************************************************
 * Path view
 **********************************************************/

int UIPathViewForward(Widget widget) {
  if (PathViewOn() == 0) return(0);

  UIMotionOff(widget);
  PathViewMoveIncrement();
  ViewPathViewMove();
  return(0);
}

int UIPathViewBackward(Widget widget) {
  if (PathViewOn() == 0) return(0);

  UIMotionOff(widget);
  PathViewMoveDecrement();
  ViewPathViewMove();
  return(0);
}

void UIPathViewAnimateForwardUpdate(XtPointer ptr, XtIntervalId *id) {
  if (ui->pathForwardAnimate == 1) {
    MovieOff();
    UIHistoryAnimateOff(NULL);
    PathViewMoveIncrement();
    ViewPathViewMove();

#if defined(XAW)
    XtAppAddTimeOut(ui->context, (unsigned long) MovieDelay()+1, UIPathViewAnimateForwardUpdate, ptr);
#endif
#if defined(MOTIF_FOUND)
    XtAppAddTimeOut(ui->context, (unsigned long) 10*MovieDelay()+1, UIPathViewAnimateForwardUpdate, ptr);
#endif

  }
  else {
    UIDrawCanvas(NULL);
    ui->pathForwardAnimate = 0;
  }
}

void UIPathViewAnimateBackwardUpdate(XtPointer ptr, XtIntervalId *id) {
  if (ui->pathBackwardAnimate == 1) {
    MovieOff();
    UIHistoryAnimateOff(NULL);
    PathViewMoveDecrement();
    ViewPathViewMove();

#if defined(XAW)
    XtAppAddTimeOut(ui->context, (unsigned long) MovieDelay()+1, UIPathViewAnimateBackwardUpdate, ptr);
#endif
#if defined(MOTIF_FOUND)
    XtAppAddTimeOut(ui->context, (unsigned long) 10*MovieDelay()+1, UIPathViewAnimateBackwardUpdate, ptr);
#endif

  }
  else {
    UIDrawCanvas(NULL);
    ui->pathBackwardAnimate = 0;
  }
}

int UIPathViewAnimateOff(Widget widget) {
  ui->pathForwardAnimate = 0;
  ui->pathBackwardAnimate = 0;
  return(0);
}

int UIPathViewAnimateRun() {
  if (ui->pathForwardAnimate)
    return(1);
  else if (ui->pathBackwardAnimate)
    return(-1);
  else
    return(0);
}

#endif

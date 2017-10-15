#include <sepConfig.h>
#if defined (MOTIF_FOUND) || defined(HAVE_ATHENA)
/*
user interface: permanent control panel plus various popup panels
*/
#include <stdio.h>
#include "rick.h"
#include <stdlib.h>
#if defined (HAVE_STDLIB_H)
#include<stdlib.h>
#else
#endif /* HAVE_STDLIB  */

extern UI ui;
extern Message message;
#ifndef MIN
#define MIN(a,b) ( ((a)<(b)) ? (a):(b) )
#endif
#ifndef MAX
#define MAX(a,b) ( ((a)>(b)) ? (a):(b) )
#endif


static int local_dir=0;

/********************************* CONTROL PANEL ******************************/
#define	CS	XmStringCreateSimple
/* label generator */
#undef LABEL
#define LABEL(name) \
	 XtVaCreateManagedWidget (name,xmLabelWidgetClass,parent,NULL);
/* button generator */
#undef BUTTON
#define	BUTTON(name,callback) {\
	Widget widget;\
	widget = XtVaCreateManagedWidget (name,xmPushButtonWidgetClass,parent,NULL);\
	XtAddCallback (widget,XmNactivateCallback,(XtCallbackProc)callback,NULL);\
	}
/* slider generator */
#undef SLIDER
#define	SLIDER(name,var,value,callback) {\
	if (name[0] != '\0') {\
	XtVaCreateManagedWidget (name,xmLabelWidgetClass,parent,NULL);}\
	ui->var = XtVaCreateManagedWidget (name,xmScaleWidgetClass,parent,\
		XmNorientation, XmHORIZONTAL,\
		XmNvalue, (int)(100*value),\
		NULL);\
	XtAddCallback (ui->var,XmNvalueChangedCallback,(XtCallbackProc)callback,NULL);\
	}

/* initialize controls */
int UIControlInit1 (Widget parent)
	{
	Widget widget;
	WidgetList list;

	if (!ui) return(0);
	/* control panel */
	ui->control1 = XtVaCreateManagedWidget ("control1",xmRowColumnWidgetClass,parent,
		XmNwidth, UI_WIDE,
		/* controls laid out left to right */
		XmNorientation, XmHORIZONTAL,
		XmNpacking, XmPACK_TIGHT,
		NULL);

       parent = XtVaCreateManagedWidget ("frame",xmFrameWidgetClass,ui->control1,NULL);
       ui->movieOnOff = XmVaCreateSimpleRadioBox (parent, "movie", 1,
                (XtCallbackProc)UIMovie,
		XmVaRADIOBUTTON, CS("GO"),NULL,NULL,NULL,
		XmVaRADIOBUTTON, CS("NO"),NULL,NULL,NULL,
		XmNorientation, XmHORIZONTAL,
                NULL);
	XtManageChild (ui->movieOnOff);
	XtManageChild (parent);

       parent = XtVaCreateManagedWidget ("frame",xmFrameWidgetClass,ui->control1,NULL);
       widget = XmVaCreateSimpleRadioBox (parent, "annotate", 1,
                (XtCallbackProc)UIAno,
		XmVaRADIOBUTTON, CS("Annotate"),NULL,NULL,NULL,
		XmVaRADIOBUTTON, CS("Normal"),NULL,NULL,NULL,
		XmNorientation, XmHORIZONTAL,
                NULL);
	XtManageChild (widget);
	XtManageChild (parent);

	parent = XtVaCreateManagedWidget ("frame",xmFrameWidgetClass,ui->control1,NULL);
        ui->movieDir = XmVaCreateSimpleRadioBox (parent, "dir", 0,
                (XtCallbackProc)UIDirection,
		XmVaRADIOBUTTON, CS("z"),NULL,NULL,NULL,
		XmVaRADIOBUTTON, CS("Z"),NULL,NULL,NULL,
		XmVaRADIOBUTTON, CS("x"),NULL,NULL,NULL,
		XmVaRADIOBUTTON, CS("X"),NULL,NULL,NULL,
		XmVaRADIOBUTTON, CS("y"),NULL,NULL,NULL,
		XmVaRADIOBUTTON, CS("Y"),NULL,NULL,NULL,
		XmNorientation, XmHORIZONTAL,
                NULL);
	XtManageChild (ui->movieDir);
	XtManageChild (parent);

	parent = ui->control1;
	SLIDER ("SPEED",movieSpeed,1.0,UISpeed);
	SLIDER ("VEL VIEW",velocityParam,0.5,UIVelViewScale);
	return(0);
	}

int UIControlInit2 (Widget parent)
	{
	if (!ui) return(0);
	/* control panel */
	ui->control2 = XtVaCreateManagedWidget ("control2",xmRowColumnWidgetClass,parent,
		XmNwidth, UI_WIDE,
		/* controls laid out left to right */
		XmNorientation, XmHORIZONTAL,
		XmNpacking, XmPACK_TIGHT,
		NULL);

	parent = ui->control2;
	SLIDER ("CONTRAST",contrast,0.5,UIContrast);
	SLIDER ("CENTER",contrast0,0.5,UIContrast0);
	BUTTON ("RESET",UIResetContrast);
	BUTTON ("STEP",UIStep);
   return(0);
	}


int UIVelViewScale (Widget widget, XtPointer client, XmScaleCallbackStruct *data) {
  VelViewSetParam(data->value);
  if (VelViewOn())
    UIDrawCanvas(widget);
  UIWakeup();
  return(0);
}

/* toggle movie */
int UIAno (Widget widget,int item)
	{
	static int toggle2 = 0;
	int sd;

	toggle2 = 1 - toggle2;
	if (toggle2) return(0);
	if(item==1) sd=0; else sd=1;
	set_do_ano(sd);
	UIWakeup ();
	return(0);
	}
int UIDisplayMethod (Widget widget,int item)
	{
	static int toggle3 = 0;

	toggle3 = 1 - toggle3;
	if (toggle3) return(0);
	set_display(item);
 	return(0);
	}
int UIDoSearchMethod (Widget widget,int item)
	{
	static int toggle3 = 0;

	toggle3 = 1 - toggle3;
	if (toggle3) return(0);
	set_search(item);
 	return(0);
	}
int UIDoDepend (Widget widget,int item)
	{
	static int toggle3 = 0;

	toggle3 = 1 - toggle3;
	if (toggle3) return(0);
	set_depend(item);
 	return(0);
	}
int UIDoCor (Widget widget,int item)
	{
	static int toggle3 = 0;

	toggle3 = 1 - toggle3;
	if (toggle3) return(0);
	set_run_cor(toggle3);
 	return(0);
	}
/* toggle movie */
int UIClosePoly (Widget widget,int item)
	{
	static int toggle3 = 0;

	toggle3 = 1 - toggle3;
	if (toggle3) return(0);
	switch (item){
	case 0: 
		set_close_status(0,0);
		break;
	case 1:
		set_close_status(1,0);
		break;
	case 2: 
		set_close_status(0,1);
		break;
	default:break;
		}
/*	UIWakeup ();*/
 	return(0);
	}

/* toggle movie */
int UIMovie (Widget widget,int item)
	{
	static int toggle = 0;

	toggle = 1 - toggle;
	if (toggle) return(0);
	switch (item) {
	case 0: 
	  MovieOn(); 
	  break;
	case 1: 
	  UIMotionOff(widget); 
	  break;
		}
	UIWakeup();
	return(0);
	}


/**
local_dir
0 deep back
1 deep forward
2 across left
3 across right
4 down up
5 down down

**/

/* toggle direction */
int UIDirection (Widget widget,int item)
	{
	static int toggle = 0;
	toggle = 1 - toggle;
	if (toggle) return(0);

   local_dir=item;
   UIWakeup();
  return(moveit());
}

/* change direction without moving */
int UIDirectionNoMove (Widget widget, int item) {
  local_dir = item;
  return(0);
}

int UILocalDirection() {
  return local_dir;
}

int moveit(){
  if (!View3D()) return(0);
  switch (local_dir) {
  case LARGE_Y_DIR:
    ViewSetMovie (MOVIE_TOP);
    MovieSetDir (MOVIE_FORWARD);
    if (!MovieRun()) ViewDrawMovie ();
    break;
  case SMALL_Y_DIR:
    ViewSetMovie (MOVIE_TOP);
    MovieSetDir (MOVIE_REVERSE);
    if (!MovieRun()) ViewDrawMovie ();
    break;
  case LARGE_X_DIR:
    ViewSetMovie (MOVIE_SIDE);
    MovieSetDir (MOVIE_FORWARD);
    if (!MovieRun()) ViewDrawMovie ();
    break;
  case SMALL_X_DIR:
    ViewSetMovie (MOVIE_SIDE);
    MovieSetDir (MOVIE_REVERSE);
    if (!MovieRun()) ViewDrawMovie ();
    break;
  case LARGE_Z_DIR:
    ViewSetMovie (MOVIE_FRONT);
    MovieSetDir (MOVIE_REVERSE);
    if (!MovieRun()) ViewDrawMovie ();
    break;
  case SMALL_Z_DIR:
    ViewSetMovie (MOVIE_FRONT);
    MovieSetDir (MOVIE_FORWARD);
    if (!MovieRun()) ViewDrawMovie ();
    break;
  }
  return(0);
}

/* get toggle state */
int UIGetToggle (Widget widget)
	{
	int state = 0;

	XtVaGetValues (widget,XmNset,&state,NULL);
	return (state);
	}

/* set toggle widget state */
int UIToggleSet (Widget widget,int state)
	{
	XtVaSetValues (widget,XmNset,state,NULL);
	return(0);
	}

/* set slider widget value between 0 and 1 */
int UISetSlider (Widget widget,float value)
	{
	XtVaSetValues (widget,XmNvalue,(int)(100*value),NULL);
  return(0);
	}

/* set slider widget value between 0 and 1 */
int UIGetSlider (Widget widget)
	{
	int value = 0;

	XtVaGetValues (widget,XmNvalue,&value,NULL);
	return (value);
	}



int UIwind (Widget widget,XtPointer client,XmScaleCallbackStruct *data)
	{
	set_wind(data->value);
	return(0);
}
int UIradius (Widget widget,XtPointer client,XmScaleCallbackStruct *data)
	{
	set_radius(data->value);
	return(0);
}

int UIsradius (Widget widget,XtPointer client,XmScaleCallbackStruct *data)
	{
	return(grab_pik_radius(data->value));
}

int UIpikj (Widget widget,XtPointer client,XmScaleCallbackStruct *data)
	{
	set_pikj(data->value);
	return(0);
}
int UInpath (Widget widget,XtPointer client,XmScaleCallbackStruct *data)
	{
	set_path(data->value);
	return(0);
}
int UIFat (Widget widget,XtPointer client,XmScaleCallbackStruct *data)
	{
	set_ano_fat(data->value);
	return(0);
}

int UIScale (Widget widget,XtPointer client,XmScaleCallbackStruct *data)
	{
	set_ano_size(data->value);
	return(0);
}
int UIRotate (Widget widget,XtPointer client,XmScaleCallbackStruct *data)
	{
	set_ano_rotate(data->value);
	return(0);
}



/* contrast slider callback */

/* speed slider callback */
int UISpeed (Widget widget,XtPointer client,XmScaleCallbackStruct *data)
	{
	MovieSetSpeed (data->value);
	UIWakeup();
	return(0);
	}

/* contrast slider callback */
int UIContrast (Widget widget,XtPointer client,XmScaleCallbackStruct *data)
	{
	ColorSetContrast (data->value);
	ColorSwitch ();
	UIDrawCanvas(NULL);
	UIWakeup();
  return(0);
	}

int UIContrast0 (Widget widget,XtPointer client,XmScaleCallbackStruct *data)
	{
	ColorSetContrast0 (data->value);
	ColorSwitch ();
	UIDrawCanvas(NULL);
	UIWakeup();
  return(0);
	}

/* reset contrast */
int UIStep ()
	{
	if (!ui) return(0);

  moveit(0);
}
int UIResetContrast ()
	{
	if (!ui) return(0);
	ColorSetContrast (CONTRAST);
	ColorSetContrast0 (CONTRAST0);
	UISetSlider (ui->contrast,0.5);
	UISetSlider (ui->contrast0,0.5);
	ColorSwitch ();
	UIDrawCanvas(NULL);
	UIWakeup();
  return(0);
	}

/********************************* SIZE PANEL ******************************/
/* bounds callback for array settings and sizing */
int UISizeRaise ()
	{
	UISizeInit ();
	UISizeReset ();
	XtPopup (ui->s_shell,XtGrabNone);
  return(0);
	}

/* initialize size control panel */
int UISizeInit ()
	{
	string svalue;
	Widget widget, parent;

	if (ui->s_shell) return(0);
	ui->s_shell = XtVaCreatePopupShell ("size",xmDialogShellWidgetClass,ui->main,
		XmNtitle, CS("SIZE PARAMETERS"),
		NULL);
	ui->s_base = XtVaCreateWidget ("s_base",xmRowColumnWidgetClass,ui->s_shell,
		XmNnumColumns, 8,
		XmNpacking, XmPACK_COLUMN,
		XmNorientation, XmHORIZONTAL,
		NULL);
#undef LABEL
#define	LABEL(name) XtVaCreateManagedWidget (name, xmLabelWidgetClass, parent,\
		NULL);
#undef LABEL1
#define	LABEL1(name,var) ui->var = XtVaCreateManagedWidget (name, xmLabelWidgetClass, parent,\
		NULL);
#undef BUTTON
#define BUTTON(name,callback) {\
	Widget widget;\
	widget = XtVaCreateManagedWidget (name, xmPushButtonWidgetClass, parent, NULL);\
	XtAddCallback (widget,XmNactivateCallback,(XtCallbackProc)callback,NULL);\
	}
#undef SLIDER
#define	SLIDER(axis,var,var1) {\
	Widget widget;\
	widget = XtVaCreateWidget ("slider", xmRowColumnWidgetClass, parent,\
		XmNorientation, XmHORIZONTAL,\
		NULL);\
	ui->var = XtVaCreateManagedWidget ("var", xmScaleWidgetClass, widget,\
		XmNshowValue, True,\
		XmNorientation, XmHORIZONTAL,\
		NULL);\
	XtAddCallback (ui->var,XmNdragCallback,(XtCallbackProc)UISizeSlider,NULL);\
	ui->var1 = XtVaCreateManagedWidget ("var1", xmTextFieldWidgetClass, widget,\
		XmNcolumns, 6,\
		NULL);\
	XtAddCallback (ui->var1,XmNactivateCallback,(XtCallbackProc)UISizeText,NULL);\
	XtManageChild (widget);\
	ui->s_slist[ui->s_nslider] = ui->var;\
	ui->s_vlist[ui->s_nslider] = ui->var1;\
	ui->s_alist[ui->s_nslider] = axis;\
	ui->s_nslider++;\
		}
	ui->s_nslider = 0;
	parent = ui->s_base;
	LABEL("SET SIZE:");
	LABEL1("DOWN:n1",s_label[AXIS_DOWN]);
	LABEL1("ACROSS:n2",s_label[AXIS_ACROSS]);
	LABEL1("DEEP:n3",s_label[AXIS_DEEP]);
	LABEL("MINIMUM");
	SLIDER(AXIS_DOWN,s_mins[AXIS_DOWN],s_minv[AXIS_DOWN]);
	SLIDER(AXIS_ACROSS,s_mins[AXIS_ACROSS],s_minv[AXIS_ACROSS]);
	SLIDER(AXIS_DEEP,s_mins[AXIS_DEEP],s_minv[AXIS_DEEP]);
	LABEL("MAXIMUM");
	SLIDER(AXIS_DOWN,s_maxs[AXIS_DOWN],s_maxv[AXIS_DOWN]);
	SLIDER(AXIS_ACROSS,s_maxs[AXIS_ACROSS],s_maxv[AXIS_ACROSS]);
	SLIDER(AXIS_DEEP,s_maxs[AXIS_DEEP],s_maxv[AXIS_DEEP]);
	LABEL("FRAME");
	SLIDER(AXIS_DOWN,s_frames[AXIS_DOWN],s_framev[AXIS_DOWN]);
	SLIDER(AXIS_ACROSS,s_frames[AXIS_ACROSS],s_framev[AXIS_ACROSS]);
	SLIDER(AXIS_DEEP,s_frames[AXIS_DEEP],s_framev[AXIS_DEEP]);
	LABEL("FRAME-INC");
	SLIDER(AXIS_DOWN,s_dframes[AXIS_DOWN],s_dframev[AXIS_DOWN]);
	SLIDER(AXIS_ACROSS,s_dframes[AXIS_ACROSS],s_dframev[AXIS_ACROSS]);
	SLIDER(AXIS_DEEP,s_dframes[AXIS_DEEP],s_dframev[AXIS_DEEP]);
	LABEL("PIXELS");
	SLIDER(NO_INDEX,s_sizes[AXIS_DOWN],s_sizev[AXIS_DOWN]);
	SLIDER(NO_INDEX,s_sizes[AXIS_ACROSS],s_sizev[AXIS_ACROSS]);
	SLIDER(NO_INDEX,s_sizes[AXIS_DEEP],s_sizev[AXIS_DEEP]);
	BUTTON("EXECUTE SETTINGS",UISizeDraw);
	BUTTON("CURRENT SETTINGS",UISizeReset);
	BUTTON("INITIAL SETTINGS",UISizeInitial);
	BUTTON("CLOSE",UISizeClose);
	XtManageChild (ui->s_base);
  return(0);
	}

/* size control panel draw callback */
int UISizeDraw ()
	{
	int imap, first, last, frame, dframe, size;
	View myv;

	myv=current_view();

	for (imap=1; imap<4; imap++) {
		XtVaGetValues (ui->s_mins[imap],XmNvalue,&first,NULL);
		XtVaGetValues (ui->s_maxs[imap],XmNvalue,&last,NULL);
		XtVaGetValues (ui->s_frames[imap],XmNvalue,&frame,NULL);
		XtVaGetValues (ui->s_dframes[imap],XmNvalue,&dframe,NULL);
		XtVaGetValues (ui->s_sizes[imap],XmNvalue,&size,NULL);
		MapSet (ViewMap(myv,imap),MapAxis(ViewMap(myv,imap)),size,first,last,first,last,dframe);
		MapSetFrame (ViewMap(myv,imap),frame);
		}
	UIDrawCanvas(NULL);
  return(0);
	}

/* set size control panel to current image values */
int UISizeReset ()
	{
	int imap, islider;
	string svalue;
	View myv;

	myv=current_view();

	if (!ui->s_shell) return(0);
	for (imap=1; imap<4; imap++) {
		sprintf (svalue,"%s:%s",MapName(ViewMap(myv,imap)),AxisLabel(MapAxis(ViewMap(myv,imap))));
		XtVaSetValues (ui->s_label[imap],XmNlabelString,CS(svalue),
			NULL);
		XtVaSetValues (ui->s_mins[imap],XmNmaximum,AxisSize(MapAxis(ViewMap(myv,imap)))-1,
			XmNvalue, MapFirst(ViewMap(myv,imap)),
			NULL);
		XtVaSetValues (ui->s_maxs[imap],XmNmaximum,AxisSize(MapAxis(ViewMap(myv,imap)))-1,
			XmNvalue, MapLast(ViewMap(myv,imap)),
			NULL);
		XtVaSetValues (ui->s_frames[imap],XmNmaximum,AxisSize(MapAxis(ViewMap(myv,imap)))-1,
			XmNvalue, MapFrame(ViewMap(myv,imap)),
			NULL);
		XtVaSetValues (ui->s_dframes[imap],XmNminimum,1,
			XmNmaximum,100,
			XmNvalue, MapDmovie(ViewMap(myv,imap)),
			NULL);
		XtVaSetValues (ui->s_sizes[imap],XmNminimum,16,
			XmNmaximum,1000,
			XmNvalue,MapSize(ViewMap(myv,imap)),
			NULL);
		}
	for (islider=0; islider<ui->s_nslider; islider++) {
		UISizeSlider (ui->s_slist[islider]);
		}
	return(0);
	}

/* set size control panel to initial size values */
int UISizeInitial ()
	{
	int imap, islider;
	View myv;

	myv=current_view();

	if (!ui->s_shell) return(0);
	for (imap=1; imap<4; imap++) {
		XtVaSetValues (ui->s_mins[imap],XmNvalue,0,NULL);
		XtVaSetValues (ui->s_maxs[imap],XmNvalue,AxisSize(MapAxis(ViewMap(myv,imap)))-1,NULL);
		XtVaSetValues (ui->s_frames[imap],XmNvalue,0,NULL);
		}
	for (islider=0; islider<ui->s_nslider; islider++) {
		UISizeSlider (ui->s_slist[islider]);
		}
	return(0);
	}

/* close size control panel */
int UISizeClose ()
	{
	XtPopdown (ui->s_shell);
  return(0);
	}

/* synchronize size slider with slider text */
int UISizeSlider (Widget widget)
	{
	int value;
	string svalue;
	int islider;
	View myv;

	myv=current_view();

	for (islider=0; islider<ui->s_nslider; islider++) {
		if (ui->s_slist[islider] == widget) break;
		}
	if (islider == ui->s_nslider) return(0);
	XtVaGetValues (widget,XmNvalue,&value,NULL);
	if (ui->s_alist[islider] == NO_INDEX) {
		sprintf (svalue, "%d", value);
		}
	else	{
		sprintf (svalue, "%g", AxisValue (MapAxis(ViewMap(myv,ui->s_alist[islider])),value));
		}
	XtVaSetValues (ui->s_vlist[islider],XmNvalue,svalue,NULL);
  return(0);
	}

/* synchronize slider text with size slider */
int UISizeText (Widget widget)
	{
	char *svalue;
	int islider, value;
	double atof();
	View myv;

	myv=current_view();

	for (islider=0; islider<ui->s_nslider; islider++) {
		if (ui->s_vlist[islider] == widget) break;
		}
	XtVaGetValues (widget,XmNvalue,&svalue,NULL);
	if (ui->s_alist[islider] == NO_INDEX) {
		value = atoi (svalue);
		}
	else	{
		value = AxisIndex(MapAxis(ViewMap(myv,ui->s_alist[islider])),atof(svalue));
		}
	XtVaSetValues (ui->s_slist[islider],XmNvalue,value,NULL);
	return(0);
	}

/********************************* ARRAY PANEL ******************************/
/* initialize array control panel */
int UIArrayInit ()
	{
	Widget widget, parent;
	View myv;

	myv=current_view();

	ui->across = 0;
	if (ui->a_shell) return(0);
	ui->a_shell = XtVaCreatePopupShell ("array", xmDialogShellWidgetClass,ui->main,
		XmNtitle, CS("ARRAY PARAMETERS"),
		NULL);
	ui->a_base = XtVaCreateWidget ("base", xmRowColumnWidgetClass,ui->a_shell,
		XmNorientation, XmVERTICAL,
		NULL);
	XtVaCreateManagedWidget ("ARRAY PARAMETERS:",xmLabelWidgetClass,ui->a_base,NULL);
	ui->a_dir = XmVaCreateSimpleRadioBox (ui->a_base, "dir", 
		AxisDir (MapAxis (ViewMap(myv,AXIS_DEEP))) - 1,
		(XtCallbackProc)UIArrayDir,
		XmVaRADIOBUTTON, CS(AxisLabel(DataAxis(myv->data,DATA_AXIS1))),NULL,NULL,NULL,
		XmVaRADIOBUTTON, CS(AxisLabel(DataAxis(myv->data,DATA_AXIS2))),NULL,NULL,NULL,
		XmVaRADIOBUTTON, CS(AxisLabel(DataAxis(myv->data,DATA_AXIS3))),NULL,NULL,NULL,
		XmVaRADIOBUTTON, CS(AxisLabel(DataAxis(myv->data,DATA_AXIS4))),NULL,NULL,NULL,
		NULL);
	XtManageChild (ui->a_dir);
	parent = ui->a_dir;
	ui->a_across = XtVaCreateManagedWidget ("across", xmScaleWidgetClass, ui->a_base,
		XmNminimum, 1,
		XmNmaximum, 50,
		XmNwidth, 250,
		XmNshowValue, True,
		XmNtitleString, CS("ACROSS"),
		XmNorientation, XmHORIZONTAL,
		NULL);
	XtAddCallback (ui->a_across,XmNvalueChangedCallback,(XtCallbackProc)UIArrayEndAdjust,NULL);
	ui->a_down = XtVaCreateManagedWidget ("down", xmScaleWidgetClass, ui->a_base,
		XmNminimum, 1,
		XmNmaximum, 50,
		XmNwidth, 250,
		XmNshowValue, True,
		XmNtitleString, CS("DOWN"),
		XmNorientation, XmHORIZONTAL,
		NULL);
	XtAddCallback (ui->a_down,XmNvalueChangedCallback,(XtCallbackProc)UIArrayEndAdjust,NULL);
	ui->a_start = XtVaCreateManagedWidget ("start", xmScaleWidgetClass, ui->a_base,
		XmNwidth, 250,
		XmNshowValue, True,
		XmNtitleString, CS("START"),
		XmNorientation, XmHORIZONTAL,
		NULL);
	XtAddCallback (ui->a_start,XmNvalueChangedCallback,(XtCallbackProc)UIArrayEndAdjust,NULL);
	ui->a_delta = XtVaCreateManagedWidget ("delta", xmScaleWidgetClass, ui->a_base,
		XmNminimum, 1,
		XmNmaximum, 50,
		XmNwidth, 250,
		XmNvalue, ui->delta,
		XmNshowValue, True,
		XmNtitleString, CS("DELTA"),
		XmNorientation, XmHORIZONTAL,
		NULL);
	XtAddCallback (ui->a_delta,XmNvalueChangedCallback,(XtCallbackProc)UIArrayEndAdjust,NULL);
	ui->a_end = XtVaCreateManagedWidget ("end", xmScaleWidgetClass, ui->a_base,
		XmNminimum, 0,
		XmNwidth, 250,
		XmNshowValue, True,
		XmNtitleString, CS("END"),
		XmNorientation, XmHORIZONTAL,
		NULL);
	XtAddCallback (ui->a_end,XmNvalueChangedCallback,(XtCallbackProc)UIArrayDeltaAdjust,NULL);
	XtVaCreateManagedWidget ("line", xmSeparatorWidgetClass, ui->a_base,NULL);
	widget = XtVaCreateManagedWidget ("DRAW", xmPushButtonWidgetClass, ui->a_base, NULL);
	XtAddCallback (widget,XmNactivateCallback,(XtCallbackProc)UIArrayDraw,NULL);
	widget = XtVaCreateManagedWidget ("CLOSE", xmPushButtonWidgetClass, ui->a_base, NULL);
	XtAddCallback (widget,XmNactivateCallback,(XtCallbackProc)UIArrayClose,NULL);
	XtManageChild (ui->a_base);
	XtManageChild (ui->a_shell);
	return(0);
	}

/* set array control panel direction when orientation changes */
int UIArrayDir (Widget widget,int item)
	{
	if (XmToggleButtonGetState(widget) == True) {
		UIArrayReset (item+1);
		}
	return(0);
	}

/* fetch current array control panel settings */
int UIArrayRaise ()
	{
	UIArrayInit ();
	UIArrayReset (0);
	ui->shell = ui->a_shell;
	XtPopup (ui->a_shell,XtGrabNone);
	return(0);
	}

int UIArrayReset (int dir)
	{
	Axis axis;
	Map map;
	int imap, delta;
	WidgetList list;
	View myv;

	myv=current_view();

	if (ui->a_shell == 0) return(0);
	if (!dir) {
		map = ViewMap(myv,AXIS_DEEP);
		ui->dir = AxisDir(MapAxis(map));
		XtVaGetValues (ui->a_dir,XmNchildren,&list,NULL);
		dir = ui->dir - 1;
		XtVaSetValues (list[dir],XmNset,True,NULL);
		XtVaSetValues (list[(dir+1)%3],XmNset,False,NULL);
		XtVaSetValues (list[(dir+2)%3],XmNset,False,NULL);
		}
	else	{
		ui->dir = dir;
		for (imap=1; imap<=3; imap++) {
			if (ui->dir == AxisDir(MapAxis(map=myv->map[imap]))) break;
			}
		}
	axis = DataAxis(myv->data,ui->dir);
	UIArrayShape (MapNFrame(map),&ui->across,&ui->down,&ui->delta);
	XtVaSetValues (ui->a_across,XmNvalue,ui->across,NULL);
	XtVaSetValues (ui->a_down,XmNvalue,ui->down,NULL);
	ui->start = MapLow(map);
	delta = rint2(AxisDelta(axis) * AxisScale(axis));
	XtVaSetValues (ui->a_delta,
		XmNdecimalPoints, AxisPrec(axis),
		XmNminimum, delta,
		XmNmaximum, (AxisSize(axis)/3+1)*delta,
		XmNvalue, ui->delta*delta,
		NULL);
	XtVaSetValues (ui->a_start,
		XmNdecimalPoints, AxisPrec(axis),
		XmNminimum, AxisScaledValue(axis,0),
		XmNmaximum, AxisScaledValue(axis,AxisSize(axis)-1),
		XmNvalue, AxisScaledValue(axis,ui->start),
		NULL);
	XtVaSetValues (ui->a_end,
		XmNdecimalPoints, AxisPrec(axis),
		XmNminimum, AxisScaledValue(axis,0),
		XmNmaximum, AxisScaledValue(axis,AxisSize(axis)-1),
		XmNvalue, AxisScaledValue(axis,
			(ui->across*ui->down*ui->delta+ui->start)<
			(AxisSize(axis)-1)?
			AxisScaledValue(axis,ui->across*ui->down*ui->delta+ui->start):
			AxisScaledValue(axis,AxisSize(axis)-1) ),
		NULL);
	return(0);
	}

/* execute array control panel settings */
int UIArrayDraw ()
	{
	Axis axis;
	int iaxis, value;
	View myv;

	myv=current_view();

	if (ui->dir != AxisDir(axis=MapAxis(ViewMap(myv,AXIS_DEEP)))) {
		if (ui->dir == AxisDir(axis=MapAxis(ViewMap(myv,AXIS_ACROSS)))) {
			MapSwap (ViewMap(myv,AXIS_DEEP),ViewMap(myv,AXIS_ACROSS));
			}
		else	{
			axis = MapAxis(ViewMap(myv,AXIS_DOWN));
			MapSwap (ViewMap(myv,AXIS_DEEP),ViewMap(myv,AXIS_DOWN));
			}
		}
	XtVaGetValues (ui->a_across,XmNvalue,&ui->across,NULL);
	XtVaGetValues (ui->a_down,XmNvalue,&ui->down,NULL);
	XtVaGetValues (ui->a_delta,XmNvalue,&value,NULL);
	ui->delta = AxisScaledIndex(axis,value+AxisScaledValue(axis,0));
	XtVaGetValues (ui->a_start,XmNvalue,&value,NULL);
	ui->start = AxisScaledIndex(axis,value);
	ViewArray (myv,ui->across,ui->down,ui->start,ui->delta);
	UISizeReset ();
	return(0);
	}

/* close array control panel */
int UIArrayClose ()
	{
	XtPopdown (ui->a_shell);
	ui->shell = 0;
	return(0);
	}

/* synchronize array end slider with other array slider adjustments */
int UIArrayEndAdjust ()
	{
	Axis axis;
	int max, value;
	View myv;

	myv=current_view();

	axis = DataAxis(myv->data,ui->dir);
	XtVaGetValues (ui->a_across,XmNvalue,&ui->across,NULL);
	XtVaGetValues (ui->a_down,XmNvalue,&ui->down,NULL);
	XtVaGetValues (ui->a_delta,XmNvalue,&value,NULL);
	ui->delta = AxisScaledIndex(axis,value+AxisScaledValue(axis,0));
	XtVaGetValues (ui->a_start,XmNvalue,&value,NULL);
	ui->start = AxisScaledIndex(axis,value);
	XtVaGetValues (ui->a_end,XmNmaximum,&value,NULL);
	max = AxisScaledIndex(axis,value);
	XtVaSetValues (ui->a_end,
		XmNvalue,max<
		(ui->start+ui->down*ui->across*ui->delta) ?
		AxisScaledValue(axis,max) :
		AxisScaledValue(axis,ui->start+ui->down*ui->across*ui->delta),
		NULL);
	return(0);
	}

/* synchronize array delta slider with array end slider changes */
int UIArrayDeltaAdjust ()
	{
	int last, max, value;
	Axis axis;
	View myv;

	myv=current_view();
	


	axis = DataAxis(myv->data,ui->dir);
	XtVaGetValues (ui->a_across,XmNvalue,&ui->across,NULL);
	XtVaGetValues (ui->a_down,XmNvalue,&ui->down,NULL);
	XtVaGetValues (ui->a_start,XmNvalue,&value,NULL);
	ui->start = AxisScaledIndex(axis,value);
	XtVaGetValues (ui->a_end,XmNvalue,&value,NULL);
	last = AxisScaledIndex(axis,value);
	ui->delta = (last - ui->start - 1) / (ui->across * ui->down);
	ui->delta = ui->delta > 1 ? ui->delta : 1;
/*	ui->delta = ui->delta < 50 ? ui->delta : 50;*/
	XtVaSetValues (ui->a_delta,
		XmNvalue,(int)rint2(AxisScale(axis)*AxisDelta(axis)*ui->delta),
		NULL);
	return(0);
	}

int acrosS[] = {1,1,2,3,2,3,3,3,3,3};
int dowN[] = {1,1,1,1,2,2,2,3,3,3};
int UIArrayShape (int n,int *across,int *down,int *delta)
	{
	double sqrt();
	if (n < 10) {
		*across = acrosS[n];
		*down = dowN[n];
		*delta = 1;
		}
	else	{
		*across = 3;
		*down = 3;
		*delta = n / 9;
		}
/*
	int across_, down_;

	if (n < 4) {
		across_ = n;
		down_ = 1;
		}
	else	{
		across_ = (int)sqrt((double)n);
		down_ = n / across_;
		if (across_ * down_ < n) across_++;
		}
	across_ = across_ > 1 ? across_ : 1;
	down_ = down_ > 1 ? down_ : 1;
	across_ = across_ < 50 ? across_ : 50;
	down_ = down_ < 50 ? down_ : 50;
*/
	return(0);
	}

/********************************* ORIENT PANEL ******************************/
int UIOrientInit ()
	{
	string svalue;
	Widget parent;

	if (ui->o_shell) return(0);
	ui->o_shell = XtVaCreatePopupShell ("Orient",xmDialogShellWidgetClass,ui->main,
		XmNtitle, CS("LABEL PARAMETERS"),
		NULL);
	ui->o_base = XtVaCreateWidget ("o_base",xmRowColumnWidgetClass,ui->o_shell,
		XmNnumColumns, 6,
		XmNpacking, XmPACK_COLUMN,
		XmNorientation, XmHORIZONTAL,
		NULL);
#undef LABEL
#define	LABEL(name) XtVaCreateManagedWidget (name, xmLabelWidgetClass, parent,\
		NULL);
#undef FIELD
#define FIELD(var) \
	ui->var = XtVaCreateManagedWidget ("var", xmTextFieldWidgetClass, parent,\
		XmNcolumns, 6,\
		NULL);
#undef BUTTON
#define BUTTON(name,callback) {\
	Widget widget;\
	widget = XtVaCreateManagedWidget (name, xmPushButtonWidgetClass, parent, NULL);\
	XtAddCallback (widget,XmNactivateCallback,(XtCallbackProc)callback,NULL);\
	}
	parent = ui->o_base;
	LABEL("AXIS1");LABEL("VIEW") FIELD(o_axis[0]); LABEL("POSITION"); FIELD(o_pos[0]);
	LABEL("AXIS2");LABEL("VIEW") FIELD(o_axis[1]); LABEL("POSITION"); FIELD(o_pos[1]);
	LABEL("AXIS3");LABEL("VIEW") FIELD(o_axis[2]); LABEL("POSITION"); FIELD(o_pos[2]);
	LABEL("AXIS4");LABEL("VIEW") FIELD(o_axis[3]); LABEL("POSITION"); FIELD(o_pos[3]);
	LABEL("AXIS5");LABEL("VIEW") FIELD(o_axis[4]); LABEL("POSITION"); FIELD(o_pos[4]);

	BUTTON("EXECUTE SETTINGS",UIOrientDraw);
	BUTTON("CLOSE",UIOrientClose);
	XtManageChild (ui->o_base);
	return(0);
	}

/********************************* LABEL PANEL ******************************/
int UILabelInit ()
	{
	string svalue;
	Widget parent;

	if (ui->l_shell) return(0);
	ui->l_shell = XtVaCreatePopupShell ("label",xmDialogShellWidgetClass,ui->main,
		XmNtitle, CS("LABEL PARAMETERS"),
		NULL);
	ui->l_base = XtVaCreateWidget ("l_base",xmRowColumnWidgetClass,ui->l_shell,
		XmNnumColumns, 9,
		XmNpacking, XmPACK_COLUMN,
		XmNorientation, XmHORIZONTAL,
		NULL);
#undef LABEL
#define	LABEL(name) XtVaCreateManagedWidget (name, xmLabelWidgetClass, parent,\
		NULL);
#undef FIELD
#define FIELD(var) \
	ui->var = XtVaCreateManagedWidget ("var", xmTextFieldWidgetClass, parent,\
		XmNcolumns, 6,\
		NULL);
#undef BUTTON
#define BUTTON(name,callback) {\
	Widget widget;\
	widget = XtVaCreateManagedWidget (name, xmPushButtonWidgetClass, parent, NULL);\
	XtAddCallback (widget,XmNactivateCallback,(XtCallbackProc)callback,NULL);\
	}
	parent = ui->l_base;
	LABEL("TITLE");
	FIELD(l_title);
	LABEL("");
	LABEL("");
	LABEL("");
	LABEL("");
	LABEL("COLOR");
	LABEL("DOWN");
	LABEL("ACROSS");
	LABEL("DEEP");
	LABEL("LABEL");
	FIELD(l_label[AXIS_COLOR]);
	FIELD(l_label[AXIS_DOWN]);
	FIELD(l_label[AXIS_ACROSS]);
	FIELD(l_label[AXIS_DEEP]);
	LABEL("ORIGIN");
	FIELD(l_first[AXIS_COLOR]);
	FIELD(l_first[AXIS_DOWN]);
	FIELD(l_first[AXIS_ACROSS]);
	FIELD(l_first[AXIS_DEEP]);
	LABEL("DELTA");
	FIELD(l_delta[AXIS_COLOR]);
	FIELD(l_delta[AXIS_DOWN]);
	FIELD(l_delta[AXIS_ACROSS]);
	FIELD(l_delta[AXIS_DEEP]);
	LABEL("MIN TIC");
	FIELD(l_tic0[AXIS_COLOR]);
	FIELD(l_tic0[AXIS_DOWN]);
	FIELD(l_tic0[AXIS_ACROSS]);
	FIELD(l_tic0[AXIS_DEEP]);
	LABEL("TIC INC");
	FIELD(l_dtic[AXIS_COLOR]);
	FIELD(l_dtic[AXIS_DOWN]);
	FIELD(l_dtic[AXIS_ACROSS]);
	FIELD(l_dtic[AXIS_DEEP]);
	LABEL("MAX TIC");
	FIELD(l_tic2[AXIS_COLOR]);
	FIELD(l_tic2[AXIS_DOWN]);
	FIELD(l_tic2[AXIS_ACROSS]);
	FIELD(l_tic2[AXIS_DEEP]);
	BUTTON("EXECUTE SETTINGS",UILabelDraw);
	BUTTON("CURRENT SETTINGS",UILabelReset);
	BUTTON("CLOSE",UILabelClose);
	XtManageChild (ui->l_base);
	return(0);
	}

int UIVplotPanelRaise(){
	/* Set the correct fat value */
	UIVplotInit();
	XtPopup(ui->vplot_shell,XtGrabNone);
	return(0);
}


int UIFinishPanelRaise(){
	/* Set the correct fat value */
	string svalue;
	UIFinishInit();
	XtVaSetValues (ui->d_fat,XmNvalue,grab_ano_fat(),NULL);
	XtPopup(ui->d_shell,XtGrabNone);
	return(0);
}
int UIVplotInit ()
	{
	string svalue;
	Widget parent,widget;

	if (ui->vplot_shell) return(0);
	ui->vplot_shell = XtVaCreatePopupShell ("vplot",xmDialogShellWidgetClass,ui->main,
		XmNtitle, CS("VPLOT PARAMETERS"),
		NULL);

	ui->vplot_base = XtVaCreateWidget ("vplot_base",xmRowColumnWidgetClass,ui->vplot_shell,
		XmNnumColumns, 8,
		XmNpacking, XmPACK_COLUMN,
		XmNorientation, XmHORIZONTAL,
		NULL);

parent = ui->vplot_base;
#undef LABEL
#define	LABEL(name) XtVaCreateManagedWidget (name, xmLabelWidgetClass, parent,\
		NULL);
#undef FIELD
#define FIELD(var) \
	ui->var = XtVaCreateManagedWidget ("var", xmTextFieldWidgetClass, parent,\
		XmNcolumns, 6,\
		NULL);
#undef BUTTON
#define BUTTON(name,callback) {\
	Widget widget;\
	widget = XtVaCreateManagedWidget (name, xmPushButtonWidgetClass, parent, NULL);\
	XtAddCallback (widget,XmNactivateCallback,(XtCallbackProc)callback,NULL);\
	}

LABEL("Grey: No scalebar xll=2.29  yll=1.28  xur=12.53 yur=8.94 ");
LABEL("Grey: Vertical scalebar xll=2.29  yll=1.28  xur=11.20 yur=8.94 ");

LABEL("Grey: Horizontal scalebar xll=2.29  yll=2.04  xur=11.2 yur=8.94 ");
LABEL("Cubeplot: No scalebar xll=2.29  yll=1.26  xur=12.53 yur=9.11 ");

LABEL("Cubeplot:scalebar xll=2.29  yll=2.86  xur=12.53 yur=9.11");
LABEL(" ");

LABEL("xll");
FIELD(vplot_xll);


LABEL("yll");
FIELD(vplot_yll);

LABEL("xur");
FIELD(vplot_xur);

LABEL("yur");
FIELD(vplot_yur);
parent = ui->vplot_base;
BUTTON("FINISHED",UICloseVplot);
XtManageChild (ui->vplot_base);
return(0);
}


int UIFinishInit ()
	{
	string svalue;
	Widget parent,widget;

	if (ui->d_shell) return(0);
	ui->d_shell = XtVaCreatePopupShell ("finish",xmDialogShellWidgetClass,ui->main,
		XmNtitle, CS("FINISH PARAMETERS"),
		NULL);

	ui->d_base = XtVaCreateWidget ("d_base",xmRowColumnWidgetClass,ui->d_shell,
		XmNnumColumns, 4,
		XmNpacking, XmPACK_COLUMN,
		XmNorientation, XmHORIZONTAL,
		NULL);

#undef BUTTON
#define BUTTON(name,callback) {\
	Widget widget;\
	widget = XtVaCreateManagedWidget (name, xmPushButtonWidgetClass, parent, NULL);\
	XtAddCallback (widget,XmNactivateCallback,(XtCallbackProc)callback,NULL);\
	}


	parent = ui->d_base;
	BUTTON("Delete",UIDeletePolyAno);
	BUTTON("Finished",UIFinishPolyAno);

#undef SLIDER
#define	SLIDER(name,var,min,value,max,callback) {\
	if (name[0] != '\0') {\
	XtVaCreateManagedWidget (name,xmLabelWidgetClass,parent,NULL);}\
	ui->var = XtVaCreateManagedWidget (name,xmScaleWidgetClass,parent,\
   XmNtitleString, CS(name),\
	  XmNminimum, min,\
    XmNmaximum, max,\
    XmNwidth, 100,\
    XmNshowValue, True,\
    XmNorientation, XmHORIZONTAL,\
    NULL);\
	XtAddCallback (ui->var,XmNvalueChangedCallback,(XtCallbackProc)callback,NULL);\
	}

	SLIDER("FAT",d_fat,0,grab_ano_fat(),30,UIFat);
   
#undef LABEL
#define	LABEL(name) XtVaCreateManagedWidget (name, xmLabelWidgetClass, parent,\
		NULL);
	LABEL("Polyline Objects");

	parent = ui->d_base;
       widget = XmVaCreateSimpleRadioBox (parent, "Close Poly", grab_do_close(),
                (XtCallbackProc)UIClosePoly,
		XmVaRADIOBUTTON, CS("Open Polyline"),NULL,NULL,NULL,
		XmVaRADIOBUTTON, CS("Close Polygon"),NULL,NULL,NULL,
		XmVaRADIOBUTTON, CS("Arrow"),NULL,NULL,NULL,
		XmNorientation, XmHORIZONTAL,
                NULL);
	XtManageChild (widget);
	XtManageChild (ui->d_base);
 return(0);
}


int UIPikTextInit ()
	{
	string svalue;
	Widget parent,widget;

	if (ui->pik_shell) return(0);
	ui->pik_shell = XtVaCreatePopupShell ("finish",xmDialogShellWidgetClass,ui->main,
		XmNtitle, CS("FINISH PARAMETERS"),
		NULL);

	ui->pik_base = XtVaCreateWidget ("pik_base",xmRowColumnWidgetClass,ui->pik_shell,
		XmNnumColumns, 18,
		XmNpacking, XmPACK_COLUMN,
		XmNorientation, XmHORIZONTAL,
		NULL);
	parent = ui->pik_base;

#undef BUTTON
#define BUTTON(name,callback) {\
	Widget widget;\
	widget = XtVaCreateManagedWidget (name, xmPushButtonWidgetClass, parent, NULL);\
	XtAddCallback (widget,XmNactivateCallback,(XtCallbackProc)callback,NULL);\
	}


	parent = ui->pik_base;
	BUTTON("Change",UIFinishSymbol);
	BUTTON("Finished",UIFinishSymbolDown);

/*NOW ENTER THE TEXT */
	parent = ui->pik_base;
#undef LABEL
#define	LABEL(name) XtVaCreateManagedWidget (name, xmLabelWidgetClass, parent,\
		NULL);
#undef FIELD
#define FIELD(var,callback) \
	ui->var = XtVaCreateManagedWidget ("var", xmTextFieldWidgetClass, parent,\
		XmNcolumns, 6,\
		NULL);

	LABEL("TEXT");
	FIELD(pik_text,UIText);


#undef SLIDER
#define	SLIDER(name,var,min,value,max,callback) {\
	if (name[0] != '\0') {\
	XtVaCreateManagedWidget (name,xmLabelWidgetClass,parent,NULL);}\
	ui->var = XtVaCreateManagedWidget (name,xmScaleWidgetClass,parent,\
	  XmNminimum, min,\
    XmNmaximum, max,\
    XmNwidth, 50,\
		XmNvalue, value,\
    XmNshowValue, True,\
    XmNorientation, XmHORIZONTAL,\
    NULL);\
	XtAddCallback (ui->var,XmNvalueChangedCallback,(XtCallbackProc)callback,NULL);\
	}

	
	SLIDER("Search radius",pik_radius,0,grab_pik_radius(),30,UIradius);
	SLIDER("Pik j display",pik_j,1,grab_pik_j(),20,UIpikj);
	SLIDER("Pathes",pik_npaths,1,grab_pik_npath(),30,UInpath);
	SLIDER("Correlation hwidth",pik_nwind,0,grab_pik_nwind(),360,UIwind);
#undef FIELD2
#define FIELD2(var) \
	ui->var = XtVaCreateManagedWidget ("var", xmTextFieldWidgetClass, parent,\
		XmNcolumns, 6,\
		NULL);
	LABEL("MIN TOLERANCE");FIELD2(pik_min_tol);
	LABEL("MAX_TOLERANCE");FIELD2(pik_max_tol);
	LABEL("DELTA TOLERANCE");FIELD2(pik_dtol);

#undef LABEL
#define	LABEL(name) XtVaCreateManagedWidget (name, xmLabelWidgetClass, parent,\
		NULL);
	LABEL("Run Correlation");

	parent = ui->pik_base;
       widget = XmVaCreateSimpleRadioBox (parent, "RUN COR", 1,
                (XtCallbackProc)UIDoCor,
		XmVaRADIOBUTTON, CS("No"),NULL,NULL,NULL,
		XmVaRADIOBUTTON, CS("Yes"),NULL,NULL,NULL,
		XmNorientation, XmHORIZONTAL,
                NULL);
	XtManageChild (widget);
	LABEL("Dependent axis");




       widget = XmVaCreateSimpleRadioBox (ui->pik_base, "INDEPENDANT AXIS", 0,
                (XtCallbackProc)UIDoDepend,
		XmVaRADIOBUTTON, CS("1"),NULL,NULL,NULL,
		XmVaRADIOBUTTON, CS("2"),NULL,NULL,NULL,
		XmVaRADIOBUTTON, CS("3"),NULL,NULL,NULL,
		XmVaRADIOBUTTON, CS("4"),NULL,NULL,NULL,
		XmVaRADIOBUTTON, CS("5"),NULL,NULL,NULL,
		XmNorientation, XmHORIZONTAL,
                NULL);

	XtManageChild (widget);

/*
	LABEL("Search Method");

       widget = XmVaCreateSimpleRadioBox (ui->pik_base, "SEARCH", 0,
                (XtCallbackProc)UIDoSearchMethod,
		XmVaRADIOBUTTON, CS("VIT"),NULL,NULL,NULL,
		XmVaRADIOBUTTON, CS("HARLAN"),NULL,NULL,NULL,
		XmNorientation, XmHORIZONTAL,
                NULL);
	XtManageChild (widget);
*/
	LABEL("Display Method");

       widget = XmVaCreateSimpleRadioBox (ui->pik_base, "Display", 0,
                (XtCallbackProc)UIDisplayMethod,
		XmVaRADIOBUTTON, CS("X"),NULL,NULL,NULL,
		XmVaRADIOBUTTON, CS("Lines"),NULL,NULL,NULL,
		XmNorientation, XmHORIZONTAL,
                NULL);

	XtManageChild (widget);

	XtManageChild (ui->pik_base);
 return(0);
}






int UIAnoTextInit ()
	{
	string svalue;
	Widget parent,widget;

	if (ui->text_shell) return(0);
	ui->text_shell = XtVaCreatePopupShell ("finish",xmDialogShellWidgetClass,ui->main,
		XmNtitle, CS("FINISH PARAMETERS"),
		NULL);

	ui->text_base = XtVaCreateWidget ("text_base",xmRowColumnWidgetClass,ui->text_shell,
		XmNnumColumns, 5,
		XmNpacking, XmPACK_COLUMN,
		XmNorientation, XmHORIZONTAL,
		NULL);
	parent = ui->text_base;

#undef BUTTON
#define BUTTON(name,callback) {\
	Widget widget;\
	widget = XtVaCreateManagedWidget (name, xmPushButtonWidgetClass, parent, NULL);\
	XtAddCallback (widget,XmNactivateCallback,(XtCallbackProc)callback,NULL);\
	}


	parent = ui->text_base;
	BUTTON("Delete",UIDeleteTextAno);
	BUTTON("Finished",UIFinishTextAno);

#undef SLIDER
#define	SLIDER(name,var,min,value,max,callback) {\
	if (name[0] != '\0') {\
	XtVaCreateManagedWidget (name,xmLabelWidgetClass,parent,NULL);}\
	ui->var = XtVaCreateManagedWidget (name,xmScaleWidgetClass,parent,\
   XmNtitleString, CS(name),\
	  XmNminimum, min,\
    XmNmaximum, max,\
    XmNwidth, 100,\
    XmNshowValue, True,\
		XmNvalue, value,\
    XmNorientation, XmHORIZONTAL,\
    NULL);\
	XtAddCallback (ui->var,XmNvalueChangedCallback,(XtCallbackProc)callback,NULL);\
	}

	
	SLIDER("FAT",text_fat,0,grab_ano_fat(),30,UIFat);
	SLIDER("SIZE",text_scale,1,grab_ano_size(),30,UIScale);
	SLIDER("ROTATE",text_rotate,0,grab_ano_rotate(),360,UIRotate);



/*NOW ENTER THE TEXT */
	parent = ui->text_base;
#undef LABEL
#define	LABEL(name) XtVaCreateManagedWidget (name, xmLabelWidgetClass, parent,\
		NULL);
#undef FIELD
#define FIELD(var,callback) \
	ui->var = XtVaCreateManagedWidget ("var", xmTextFieldWidgetClass, parent,\
		XmNcolumns, 6,\
		NULL);

	LABEL("TEXT");
	FIELD(text_label,UIText);


	XtManageChild (ui->text_base);
 return(0);
}

int UIAnoTextRaise ()
	{
	string svalue;
	UIAnoTextInit ();
 	sprintf(svalue,"%s",grab_ano_text());
 	XtVaSetValues (ui->text_label,XmNlabelString,&svalue,NULL);
	XtVaSetValues (ui->text_rotate,XmNvalue,grab_ano_rotate(),NULL);
	XtVaSetValues (ui->text_scale,XmNvalue,grab_ano_size(),NULL);
	XtPopup (ui->text_shell,XtGrabNone);
	return(0);
}

int UIPikTextRaise (char symbol)
	{
	string svalue;
	UIMotionOff(NULL);
	UIPikTextInit();
 	sprintf(svalue,"%c",symbol);
 	XtVaSetValues (ui->pik_text,XmNvalue,svalue,NULL);
	sprintf (svalue,"%f",grab_dtol());
	XtVaSetValues (ui->pik_dtol,XmNvalue,svalue,NULL);
	sprintf (svalue,"%f",grab_min_tol());
	XtVaSetValues (ui->pik_min_tol,XmNvalue,svalue,NULL);
	sprintf (svalue,"%f",grab_max_tol());
	XtVaSetValues (ui->pik_max_tol,XmNvalue,svalue,NULL);
	XtPopup (ui->pik_shell,XtGrabNone);
	return(0);
}



int UIDeleteTextAno(){
	DeletePolyAno();
	XtPopdown (ui->text_shell);
	return(0);
}
int UIFinishSymbolDown(){
	UIFinishSymbol();

	XtPopdown (ui->pik_shell);
	return(0);
}
int UIFinishSymbol(){
  int rad,path,wind,runcor;
  char *svalue;
  XtVaGetValues (ui->pik_text,XmNvalue,&svalue,NULL);
  set_pik_info(svalue);
  XtVaGetValues (ui->pik_dtol,XmNvalue,&svalue,NULL);
	set_dtol(atof(svalue));
  XtVaGetValues (ui->pik_min_tol,XmNvalue,&svalue,NULL);
	set_min_tol(atof(svalue));
  XtVaGetValues (ui->pik_max_tol,XmNvalue,&svalue,NULL);
	set_max_tol(atof(svalue));
	UIDrawCanvas(NULL);
	return(0);
}
int UIFinishTextAno(){
	char *svalue;
 	XtVaGetValues (ui->text_label,XmNvalue,&svalue,NULL);
	set_ano_text(svalue);
	XtPopdown (ui->text_shell);
	AnoDraw(0,DRAW);
	return(0);
}
int UIDeletePolyAno(){
	DeletePolyAno();
	XtPopdown (ui->d_shell);
  return(0);
}
int UIFinishPolyAno(){;
	FinishPolyAno();
	XtPopdown (ui->d_shell);
  return(0);
}


int UILabelRaise ()
	{
	UILabelInit ();
	UILabelReset ();
	XtPopup (ui->l_shell,XtGrabNone);
  return(0);
	}


int UIOrientRaise ()
	{
	UIOrientInit ();
	UIOrientReset ();
	XtPopup (ui->o_shell,XtGrabNone);
  return(0);
	}

int UIOrientReset(){
	string svalue;
	View myv;
	int i,iax;

	myv=current_view();

		for(i=0; i < 5; i++){
			iax=AxisDir(MapAxis(myv->map[i+1])); /*data of given view axis*/
			sprintf (svalue,"%d",i+1);
			XtVaSetValues (ui->o_axis[iax-1],XmNvalue,svalue,NULL);
			sprintf (svalue,"%d",myv->map[i+1]->frame);
			XtVaSetValues (ui->o_pos[iax-1] ,XmNvalue,svalue,NULL);
		}

return(0);
}


int UILabelReset ()
	{
	int i;
	string svalue;
	View myv;

	myv=current_view();

	XtVaSetValues (ui->l_title,XmNvalue,DataTitle(myv->data),NULL);
	for (i=0; i<4; i++) {
		XtVaSetValues (ui->l_label[i],XmNvalue,AxisLabel(MapAxis(myv->map[i])),NULL);
		sprintf (svalue,"%g",AxisFirst(MapAxis(myv->map[i])));
		XtVaSetValues (ui->l_first[i],XmNvalue,svalue,NULL);
		sprintf (svalue,"%g",AxisDelta(MapAxis(myv->map[i])));
		XtVaSetValues (ui->l_delta[i],XmNvalue,svalue,NULL);
		sprintf (svalue,"%g",MapTic0(myv->map[i]));
		XtVaSetValues (ui->l_tic0[i],XmNvalue,svalue,NULL);
		sprintf (svalue,"%g",MapDtic(myv->map[i]));
		XtVaSetValues (ui->l_dtic[i],XmNvalue,svalue,NULL);
		sprintf (svalue,"%g",MapTic2(myv->map[i]));
		XtVaSetValues (ui->l_tic2[i],XmNvalue,svalue,NULL);
		}
	  return(0);
	}


int UIOrientDraw ()
	{
	char *svalue;
	View myv;
	int norder[5];
	char temp_ch[1024];
	int j,i;

	myv=current_view();
	for(i=0; i < 5 ; i++) norder[i]=-1;
	for(i=0; i < 5 ; i++){
		XtVaGetValues (ui->o_axis[i],XmNvalue,&svalue,NULL);
		j=atoi(svalue);
		if(j>0 && j < 6)  norder[j-1]=i;
	}
	j=0;
	for(i=0; i < 5 ; i++){ if(norder[i]!=-1) j++;}
	if(j==5){
		reorient(myv,norder);	
		for(i=0; i < 5 ; i++){
			XtVaGetValues (ui->o_pos[norder[i]-1],XmNvalue,&svalue,NULL);
			j=atoi(svalue);
			if(myv->map[i]->last > myv->map[i]->first)
				j=MIN(MAX(myv->map[i]->first,j),myv->map[i]->last);
			else
				j=MAX(MIN(myv->map[i]->first,j),myv->map[i]->last);
			MapSetFrame(myv->map[i],j);
		}
		UIDrawCanvas(NULL);
		UIDrawColorbar(NULL);
	}
	UIOrientReset ();
	return(0);
	}

int UILabelDraw ()
	{
	char *svalue;
	int i, daxis, dtics;
	View myv;

	myv=current_view();

	XtVaGetValues (ui->l_title,XmNvalue,&svalue,NULL);
	strcpy (myv->data->title,svalue);
	for (i=0; i<4; i++) {
		daxis = 0;
		dtics = 0;
		XtVaGetValues (ui->l_label[i],XmNvalue,&svalue,NULL);
		SetAxisLabel(myv->map[i]->axis, svalue);
		XtVaGetValues (ui->l_first[i],XmNvalue,&svalue,NULL);
		if (myv->map[i]->axis->first != atof(svalue)) {
			myv->map[i]->axis->first = atof(svalue);
			daxis = 1;
			}
		XtVaGetValues (ui->l_delta[i],XmNvalue,&svalue,NULL);
		if (myv->map[i]->axis->delta != atof(svalue)) {
			myv->map[i]->axis->delta = atof(svalue);
			daxis = 1;
			}
		XtVaGetValues (ui->l_tic0[i],XmNvalue,&svalue,NULL);
		if (myv->map[i]->tic0 != atof(svalue)) {
			myv->map[i]->tic0 = atof(svalue);
			dtics = 1;
			}
		XtVaGetValues (ui->l_dtic[i],XmNvalue,&svalue,NULL);
		if (myv->map[i]->dtic != atof(svalue)) {
			myv->map[i]->dtic = atof(svalue);
			dtics = 1;
			}
		XtVaGetValues (ui->l_tic2[i],XmNvalue,&svalue,NULL);
		if (myv->map[i]->tic2 != atof(svalue)) {
			myv->map[i]->tic2 = atof(svalue);
			dtics = 1;
			}
		if (daxis) {
			AxisSetValues (myv->map[i]->axis);
			MapSetTics (myv->map[i]);
			}
		}
	UIDrawCanvas(NULL);
	UIDrawColorbar(NULL);
	UILabelReset ();
	return(0);
	}

int UIOrientClose ()
	{
	UIOrientDraw();
	XtPopdown (ui->o_shell);
  return(0);
	}

int UILabelClose ()
	{
	XtPopdown (ui->l_shell);
  return(0);
	}

int UICloseVplot ()
	{
  char *svalue;
  double atof();
	float xll,yll,xur,yur;

  /* copy in the values */
	 XtVaGetValues (ui->vplot_xll,XmNvalue,&svalue,NULL);
	 xll=atof(svalue);
	 XtVaGetValues (ui->vplot_yll,XmNvalue,&svalue,NULL);
	 yll=atof(svalue);
	 XtVaGetValues (ui->vplot_xur,XmNvalue,&svalue,NULL);
	 xur=atof(svalue);
	 XtVaGetValues (ui->vplot_yur,XmNvalue,&svalue,NULL);
	 yur=atof(svalue);
	set_vplot_size(xll,xur,yll,yur);
	 XtPopdown (ui->vplot_shell);
/*	 OverlayFinish();*/
   return(0);
	}

/********************************* TRANSPARENT PANEL ******************************/
/* initialize transparency control panel */
int UITranspInit ()
	{
	Widget widget, parent;

	if (ui->t_shell) return(0);
	ui->t_shell = XtVaCreatePopupShell ("transparent",xmDialogShellWidgetClass,ui->main,
		XmNtitle, CS("TRANSPARANCY PARAMETERS"),
		NULL);
	ui->t_base = XtVaCreateWidget ("base",xmRowColumnWidgetClass,ui->t_shell,
		XmNorientation, XmVERTICAL,
		NULL);

	 XtVaCreateManagedWidget ("TRANSP PARAMETERS:",xmLabelWidgetClass,ui->t_base,NULL);
	widget = XtVaCreateManagedWidget ("low_transp",xmScaleWidgetClass,ui->t_base,
		XmNvalue, RENDER_LOW_TRANSP,
		XmNshowValue, True,
		XmNtitleString, CS("MIN TRANSPARENCY"),
		XmNorientation, XmHORIZONTAL,
		NULL);
	XtAddCallback (widget,XmNvalueChangedCallback,(XtCallbackProc)UITranspLow,NULL);
	widget = XtVaCreateManagedWidget ("high_transp",xmScaleWidgetClass,ui->t_base,
		XmNvalue, RENDER_HIGH_TRANSP,
		XmNshowValue, True,
		XmNtitleString, CS("MAX TRANSPARENCY"),
		XmNorientation, XmHORIZONTAL,
		NULL);
	XtAddCallback (widget,XmNvalueChangedCallback,(XtCallbackProc)UITranspHigh,NULL);
	widget = XtVaCreateManagedWidget ("vol_transp",xmScaleWidgetClass,ui->t_base,
		XmNvalue, RENDER_VOL_TRANSP,
		XmNshowValue, True,
		XmNtitleString, CS("TRANSPARENCY"),
		XmNorientation, XmHORIZONTAL,
		NULL);
	XtAddCallback (widget,XmNvalueChangedCallback,(XtCallbackProc)UITranspGradient,NULL);
	widget = XmVaCreateSimpleRadioBox (ui->t_base, "rate",1,(XtCallbackProc)UITranspRate,
		XmVaRADIOBUTTON, CS("Draw Altogether"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Draw Tenth Chunks"), NULL, NULL, NULL,
		XmVaRADIOBUTTON, CS("Draw Each Plane"), NULL, NULL, NULL,
		NULL);
	XtManageChild (widget);
	XtVaCreateManagedWidget ("line",xmSeparatorWidgetClass, ui->t_base, NULL);
	widget = XtVaCreateManagedWidget ("DRAW", xmPushButtonWidgetClass, ui->t_base, NULL);
	XtAddCallback (widget,XmNactivateCallback,(XtCallbackProc)ViewDrawAll,NULL);
	widget = XtVaCreateManagedWidget ("CLOSE", xmPushButtonWidgetClass, ui->t_base, NULL);
	XtAddCallback (widget,XmNactivateCallback,(XtCallbackProc)UITranspClose,NULL);

	XtManageChild (ui->t_base);
	XtManageChild (ui->t_shell);
  return(0);
	}

int UITranspRaise ()
	{
	UITranspInit ();
	XtPopup (ui->t_shell,XtGrabNone);
  return(0);
	}

int UITranspClose ()
	{
	XtPopdown (ui->t_shell);
	ui->shell = 0;
  return(0);
	}

int UITranspLow (Widget widget,XtPointer client,XmScaleCallbackStruct *data)
	{
	RenderSetLow (data->value);
  return(0);
	}

int UITranspHigh (Widget widget,XtPointer client,XmScaleCallbackStruct *data)
	{
	RenderSetHigh (data->value);
  return(0);
	}

int UITranspGradient (Widget widget,XtPointer client,XmScaleCallbackStruct *data)
	{
	RenderSetGradient (data->value);
  return(0);
	}

int UITranspRate (Widget widget, int item)
	{
	ViewSetTranspRate (item);
   return(0);
	}

int UIFenceInit ()
	{
	Widget widget;

	if (ui->f_shell) return(0);
	ui->f_shell = XtVaCreatePopupShell ("fence",xmDialogShellWidgetClass,ui->main,
		XmNtitle, CS("FENCE PARAMETERS"),
		NULL);
	ui->f_base = XtVaCreateWidget ("base",xmRowColumnWidgetClass,ui->f_shell,
		XmNorientation, XmVERTICAL,
		NULL);
	XtVaCreateManagedWidget ("FENCE PARAMETERS:",xmLabelWidgetClass,ui->f_base,NULL);
	widget = XtVaCreateManagedWidget ("TOGGLE FRONT", xmPushButtonWidgetClass, ui->f_base, NULL);
	XtAddCallback (widget,XmNactivateCallback,(XtCallbackProc)UIFenceFront,NULL);
	widget = XtVaCreateManagedWidget ("TOGGLE SIDE", xmPushButtonWidgetClass, ui->f_base, NULL);
	XtAddCallback (widget,XmNactivateCallback,(XtCallbackProc)UIFenceSide,NULL);
	widget = XtVaCreateManagedWidget ("TOGGLE TOP", xmPushButtonWidgetClass, ui->f_base, NULL);
	XtAddCallback (widget,XmNactivateCallback,(XtCallbackProc)UIFenceTop,NULL);
	widget = XtVaCreateManagedWidget ("transp",xmScaleWidgetClass,ui->f_base,
		XmNorientation, XmHORIZONTAL,
		XmNvalue, 0,
		XmNtitleString, CS("TRANSPARENCY"),
		XmNshowValue, True,
		NULL);
	XtAddCallback (widget,XmNvalueChangedCallback,(XtCallbackProc)UIFenceOpacity,NULL);
	XtVaCreateManagedWidget ("line",xmSeparatorWidgetClass, ui->f_base, NULL);
	widget = XtVaCreateManagedWidget ("DRAW", xmPushButtonWidgetClass, ui->f_base, NULL);
	XtAddCallback (widget,XmNactivateCallback,(XtCallbackProc)ViewDrawAll,NULL);
	widget = XtVaCreateManagedWidget ("CLOSE", xmPushButtonWidgetClass, ui->f_base, NULL);
	XtAddCallback (widget,XmNactivateCallback,(XtCallbackProc)UIFenceClose,NULL);
	XtManageChild (ui->f_base);
	XtManageChild (ui->f_shell);
	return(0);
	}

int UIFenceRaise ()
	{
	UIFenceInit ();
	ui->shell = ui->f_shell;
	XtPopup (ui->f_shell,XtGrabNone);
  return(0);
	}

int UIFenceClose ()
	{
	XtPopdown (ui->f_shell);
	ui->shell = 0;
  return(0);
	}

int UIFenceFront ()
	{
	ViewToggleFence(DRAW_FRONT);
  return(0);
	}

int UIFenceSide ()
	{
	ViewToggleFence(DRAW_SIDE);
  return(0);
	}

int UIFenceTop ()
	{
	ViewToggleFence(DRAW_TOP);
  return(0);
	}

int UIFenceOpacity (Widget widget,XtPointer client,XmScaleCallbackStruct *data)
	{
	RenderSetFenceTransp (data->value);
	return(0);
	}

int UIInfoInit ()
	{
	Widget widget;
  char *help2="NONE";

	ui->i_shell = XtVaCreatePopupShell ("text",xmDialogShellWidgetClass,ui->main,
		XmNtitle, CS("HELP INFORMATION"),
		NULL);
	ui->i_base = XtVaCreateWidget ("base", xmRowColumnWidgetClass,ui->i_shell,
		XmNorientation, XmVERTICAL,
		NULL);
	XtVaCreateManagedWidget ("HELP INFORMATION:",xmLabelWidgetClass,ui->i_base,NULL);
	ui->i_text = XmCreateScrolledText (ui->i_base, "textw", NULL,(Cardinal) 0);
	XtVaSetValues (ui->i_text,
		XmNvalue, help2,
		XmNcolumns, 100,
		XmNrows, 30,
		XmNeditMode, XmMULTI_LINE_EDIT,
		XmNeditable, False,
		NULL);
	XtManageChild (ui->i_text);
	XtVaCreateManagedWidget ("line", xmSeparatorWidgetClass, ui->i_base,NULL);
	widget = XtVaCreateManagedWidget ("CLOSE", xmPushButtonWidgetClass, ui->i_base,
		NULL);
	XtAddCallback (widget,XmNactivateCallback,(XtCallbackProc)UIInfoClose,NULL);
	XtManageChild (ui->i_base);
	XtManageChild (ui->i_shell);
  return(0);
	}

int UIInfo (char *text)
	{
	if (ui->i_shell == 0) UIInfoInit ();
	DrawWatch (1);
/*	XtVaSetValues (ui->i_text,XmNtopCharacter,(int)(text-help),NULL);*/
	XtVaSetValues (ui->i_text,XmNvalue,text,NULL);
	XtPopup (ui->i_shell,XtGrabNone);
	DrawWatch (0);
  return(0);
	}

int UIInfoClose ()
	{
	XtPopdown (ui->i_shell);
  return(0);
	}

int UISyzeRaise ()
	{
	UISyzeInit ();
	UISyzeReset ();
	XtPopup (ui->z_shell,XtGrabNone);
  return(0);
	}

int UISyzeInit ()
	{
	Widget parent;

	if (ui->z_shell) return(0);
	ui->z_shell = XtVaCreatePopupShell ("syze",xmDialogShellWidgetClass,ui->main,
		XmNtitle, CS("SIZE PARAMETERS"),
		NULL);
	ui->z_base = XtVaCreateWidget ("z_base",xmRowColumnWidgetClass,ui->z_shell,
		XmNorientation, XmVERTICAL,
		NULL);
#undef LABEL
#define	LABEL(name,var) ui->var = XtVaCreateManagedWidget (name,xmLabelWidgetClass,parent,NULL);
#undef BUTTON
#define BUTTON(name,callback) {\
	Widget widget;\
	widget = XtVaCreateManagedWidget (name,xmPushButtonWidgetClass, parent, NULL);\
	XtAddCallback (widget,XmNactivateCallback,(XtCallbackProc)callback,NULL);\
	}
#undef SLIDER
#define SLIDER(name,var) \
	ui->var = XtVaCreateManagedWidget (name, xmScaleWidgetClass, parent,\
		XmNtitleString, CS(name),\
		XmNwidth, 250,\
		XmNshowValue, True,\
		XmNorientation, XmHORIZONTAL,\
		NULL);
	parent = ui->z_base;
	SLIDER("DOWN: min",z_min[AXIS_DOWN]);
	SLIDER("DOWN: max",z_max[AXIS_DOWN]);
	SLIDER("DOWN: frame",z_frame[AXIS_DOWN]);
	SLIDER("DOWN: pixels",z_pixels[AXIS_DOWN]);
	XtVaCreateManagedWidget ("line", xmSeparatorWidgetClass, ui->z_base,NULL);
	SLIDER("ACROSS: min",z_min[AXIS_ACROSS]);
	SLIDER("ACROSS: max",z_max[AXIS_ACROSS]);
	SLIDER("ACROSS: frame",z_frame[AXIS_ACROSS]);
	SLIDER("ACROSS: pixels",z_pixels[AXIS_ACROSS]);
	XtVaCreateManagedWidget ("line", xmSeparatorWidgetClass, ui->z_base,NULL);
	SLIDER("DEEP: min",z_min[AXIS_DEEP]);
	SLIDER("DEEP: max",z_max[AXIS_DEEP]);
	SLIDER("DEEP: frame",z_frame[AXIS_DEEP]);
	SLIDER("DEEP: pixels",z_pixels[AXIS_DEEP]);
	XtVaCreateManagedWidget ("line", xmSeparatorWidgetClass, ui->z_base,NULL);
	BUTTON("DRAW",UISyzeDraw);
	BUTTON("RESTORE",UISyzeReset);
	BUTTON("INITIAL",UISyzeInitial);
	BUTTON("CLOSE",UISyzeClose);
	XtManageChild (ui->z_base);
  return(0);
	}

int UISyzeReset ()
	{
	int imap;
	Map map;
	Axis axis;
	string svalue;
	View myv;

	myv=current_view();

	if (!ui->z_shell) return(0);
	for (imap=1; imap<4; imap++) {
		map = ViewMap(myv,imap);
		axis = MapAxis(map);
		sprintf (svalue,"%s:%s:MINIMUM",MapName(map),AxisLabel(axis));
		XtVaSetValues (ui->z_min[imap],
/*			XmNtitleString, CS(svalue),*/
			XmNdecimalPoints, AxisPrec(axis),
			XmNminimum, AxisScaledValue(axis,0),
			XmNmaximum, AxisScaledValue(axis,AxisSize(axis)-1),
			XmNvalue, AxisScaledValue(axis,map->first),
			NULL);
		sprintf (svalue,"%s:%s:MAXIMUM",MapName(map),AxisLabel(axis));
		XtVaSetValues (ui->z_max[imap],
/*			XmNtitleString, CS(svalue),*/
			XmNdecimalPoints, AxisPrec(axis),
			XmNminimum, AxisScaledValue(axis,0),
			XmNmaximum, AxisScaledValue(axis,AxisSize(axis)-1),
			XmNvalue, AxisScaledValue(axis,map->last),
			NULL);
		sprintf (svalue,"%s:%s:FRAME",MapName(map),AxisLabel(axis));
		XtVaSetValues (ui->z_frame[imap],
/*			XmNtitleString, CS(svalue),*/
			XmNdecimalPoints, AxisPrec(axis),
			XmNminimum, AxisScaledValue(axis,0),
			XmNmaximum, AxisScaledValue(axis,AxisSize(axis)-1),
			XmNvalue, AxisScaledValue(axis,map->frame),
			NULL);
		sprintf (svalue,"%s:%s:PIXELS",MapName(map),AxisLabel(axis));
		XtVaSetValues (ui->z_pixels[imap],
/*			XmNtitleString, CS(svalue),*/
			XmNminimum, 1,
			XmNmaximum, 1000,
			XmNvalue, MapSize(map),
			NULL);
		}
  return(0);
	}

int UISyzeClose ()
	{
	XtPopdown (ui->z_shell);
  return(0);
	}

int UISyzeInitial ()
	{
	int imap;
	Axis axis;
	View myv;

	myv=current_view();

	if (!ui->z_shell) return(0);
	for (imap=1; imap<4; imap++) {
		axis = MapAxis(ViewMap(myv,imap));
		XtVaSetValues (ui->z_min[imap],XmNvalue,AxisScaledValue(axis,0),NULL);
		XtVaSetValues (ui->z_max[imap],XmNvalue,AxisScaledValue(axis,AxisSize(axis)-1),NULL);
		XtVaSetValues (ui->z_frame[imap],XmNvalue,AxisScaledValue(axis,0),NULL);
		}
  return(0);
	}
		
int UISyzeDraw ()
	{
	int imap, min, max, frame, pixels;
	Map map;
	Axis axis;
	extern int cur_view;
	View myv;

	myv=current_view();

	for (imap=1; imap<4; imap++) {
		XtVaGetValues (ui->z_min[imap],XmNvalue,&min,NULL);
		XtVaGetValues (ui->z_max[imap],XmNvalue,&max,NULL);
		XtVaGetValues (ui->z_frame[imap],XmNvalue,&frame,NULL);
		XtVaGetValues (ui->z_pixels[imap],XmNvalue,&pixels,NULL);
		map = ViewMap (myv,imap);
		axis = MapAxis (map);
		MapSet (map,
			axis,
			pixels,
			AxisScaledIndex (axis,min),
			AxisScaledIndex (axis,max),
			AxisScaledIndex (axis,min),
			AxisScaledIndex (axis,max),
			map->dframe);
		MapSetFrame (map,AxisScaledIndex (axis,frame));
		}
	UIDrawCanvas(NULL);
  return(0);
	}
#endif

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
 *  source file:   ./filters/xtlib/popup.c
 *
 * Dave Nichols (SEP), July 18 1992
 *      Inserted this sample edit history entry.
 *
 * Stewart A. Levin (Mobil), Feb 19, 1993
 *      Removed erroneous ; after actionPointPopupConfirm function body.
 *
 * Stewart A. Levin (SEP), May 22, 1994
 *      Add explicit XtNinput specification to allow use with OpenWindows
 */


#include <string.h>
#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <X11/IntrinsicP.h>
#include <X11/Xatom.h>
#include <X11/Object.h>
#include <X11/ObjectP.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include <X11/Xaw/Box.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Command.h>
#include <X11/Shell.h>
#include <X11/Xaw/Dialog.h>
#include <X11/Xaw/Cardinals.h>

#include "xtpen.h"

typedef union
{
  int i;
  float f;
} IntFloatUnion;


#include <assert.h>

static Widget PopUpWidget;
static Widget PointLabelWidget;
static Widget xOffset,yOffset;
static Widget xLabel,yLabel;
static Widget Color,Fat,Boxit,Size,XOVAL,YOVAL,POINTER;

static FILE* outfp;
static float vpx, vpy;


/*ARGSUSED*/
void PopupConfirm(Widget w, XtPointer client_data, XtPointer call_data)
{
  float xoff,yoff;
	int   col,fat,box,point;
	float size;
  float x_oval,y_oval;
	char boxit[128],arrow[129];
	

  sscanf(XawDialogGetValueString(xOffset),"%f",&xoff );
  sscanf(XawDialogGetValueString(yOffset),"%f",&yoff );
  sscanf(XawDialogGetValueString(Color),"%d",&col );
  sscanf(XawDialogGetValueString(Fat),"%d",&fat );
  sscanf(XawDialogGetValueString(Size),"%f",&size );
  sscanf(XawDialogGetValueString(Boxit),"%s",boxit );
  sscanf(XawDialogGetValueString(YOVAL),"%f",&y_oval );
  sscanf(XawDialogGetValueString(XOVAL),"%f",&x_oval );
  sscanf(XawDialogGetValueString(POINTER),"%s",arrow );
	if(boxit[0]=='n') box=0;
	else box=1;
	if(arrow[0]=='n') point=0;
	else point=1;

  fprintf( outfp,"x0=%f y0=%f label=\"%s\" xt=%f yt=%f lab_fat=%d lab_color=%d boxit=%d size=%f pointer=%d x_oval=%f y_oval=%f \n",vpx,vpy,
   	   XawDialogGetValueString(PointLabelWidget), xoff, yoff,fat,col,box,size,point,x_oval,y_oval);
  fflush( outfp );
  XtPopdown( PopUpWidget );
}

/*ARGSUSED1*/
void actionPointPopupConfirm(
Widget w,
XEvent *event,
String *params,
Cardinal *num_params)
{ PopupConfirm(w,NULL,NULL); }

/* ARGSUSED */
void PopupCancel(Widget w, XtPointer client_data, XtPointer call_data)
{
  XtPopdown( PopUpWidget );
}

/*ARGSUSED1*/
void actionPointPopupCancel(
Widget w,
XEvent *event,
String *params,
Cardinal *num_params)
{ PopupCancel(w,NULL,NULL); }

static void set_labels(float x,float y)
{
   Arg arg[1];
   char text[32];

   sprintf( text, "%6.2f ", x );
   XtSetArg( arg[0], XtNlabel, text );
   XtSetValues( xLabel , arg, ONE );

   sprintf( text, "%6.2f ", y );
   XtSetArg( arg[0], XtNlabel, text );
   XtSetValues( yLabel , arg, ONE );
}


void doPointPopup(
FILE* fp,
float x,
float y
                 )
{
  Arg argList[20];
  int args;
  
  Window root, child;
  static Widget cancelWidget, confirmWidget;
  static Widget vpaneWidget, labelBox, offsetBox,otherBox,otherBox2;

  char xoff_string[32];
  char yoff_string[32];
  char text_string[32];
	char box_string[32];
	char size_string[32];
	char color_string[32];
	char fat_string[32];
	char pointer_string[32];
	char xov_string[32];
	char yov_string[32];
  
  static Boolean inited = False;
  int root_x, root_y, child_x, child_y;
  unsigned int buttons;
  
  Dimension width;
  Dimension height;
  Dimension px,py,pw,ph;

  vpx=x; vpy=y; outfp= fp;

  /*
   * Find out where the mouse is, so we can put the confirmation
   * box right there.
   */
  XQueryPointer( pen_display,pen_window,
		&root, &child,
		&root_x, &root_y, &child_x, &child_y, &buttons);
  /*
   * If we need to construct the label box do that,
   * otherwise just reset the position and callbacks and
   * put it up again.
   */
  if (!inited) {
    /*
     * The confirmation box will be a pop-up widget.
     */
    args = 0;
    XtSetArg(argList[args],XtNinput, True); args++;
    PopUpWidget =
      XtCreatePopupShell("Point", transientShellWidgetClass,
			 xtpen, argList, args);
    
      args = 0;
      vpaneWidget = XtCreateWidget("vpane",panedWidgetClass,
                PopUpWidget,
                NULL, 0);

    /*
     * Make a box to put the labels in.
     */

    labelBox = XtCreateManagedWidget("labelBox", boxWidgetClass,
				vpaneWidget, NULL, 0);

    xLabel = XtCreateManagedWidget("000.00",labelWidgetClass,labelBox,NULL,0);
    yLabel = XtCreateManagedWidget("000.00",labelWidgetClass,labelBox,NULL,0);

    offsetBox = XtCreateManagedWidget("offsetBox", boxWidgetClass,
				vpaneWidget, NULL, 0);
    
    /* dialogs for the offsets */
    args = 0;
    strcpy(pointer_string, "yes" );
    XtSetArg( argList[0], XtNvalue, (XtArgVal)pointer_string ); args++;
    XtSetArg( argList[1], XtNlabel, (XtArgVal)"Do arrow" ); args++;
    XtSetArg( argList[2], XtNleft, XtChainLeft ); args++;
    POINTER = XtCreateManagedWidget("dialog1", dialogWidgetClass,
					       offsetBox, argList, args);
    args = 0;
    sprintf( xoff_string, "%4.2f", 1.0 );
    XtSetArg( argList[0], XtNvalue, (XtArgVal)xoff_string ); args++;
    XtSetArg( argList[1], XtNlabel, (XtArgVal)"x-off" ); args++;
    XtSetArg( argList[2], XtNfromHoriz, POINTER); args++;
    xOffset = XtCreateManagedWidget("dialog1", dialogWidgetClass,
					       offsetBox, argList, args);

    args = 0;
    sprintf( yoff_string, "%4.2f", 1.0 );
    XtSetArg( argList[0], XtNvalue, (XtArgVal)yoff_string ); args++;
    XtSetArg( argList[1], XtNlabel, (XtArgVal)"y-off" ); args++;
    XtSetArg( argList[2], XtNfromHoriz, xOffset); args++;
    yOffset = XtCreateManagedWidget("dialog2", dialogWidgetClass,
					      offsetBox, argList, args);
    args = 0;
    sprintf( xov_string, "%4.2f", 0.0 );
    XtSetArg( argList[0], XtNvalue, (XtArgVal)xov_string ); args++;
    XtSetArg( argList[1], XtNlabel, (XtArgVal)"Oval-x" ); args++;
    XtSetArg( argList[2], XtNfromHoriz, yOffset); args++;
    XOVAL = XtCreateManagedWidget("dialog2", dialogWidgetClass,
					      offsetBox, argList, args);
    args = 0;
    sprintf( yov_string, "%4.2f", 0.0 );
    XtSetArg( argList[0], XtNvalue, (XtArgVal)yov_string ); args++;
    XtSetArg( argList[1], XtNlabel, (XtArgVal)"Oval-y" ); args++;
    XtSetArg( argList[2], XtNfromHoriz, XOVAL); args++;
    YOVAL = XtCreateManagedWidget("dialog2", dialogWidgetClass,
					      offsetBox, argList, args);

		/*addition features to set */
    otherBox = XtCreateManagedWidget("otherBox", boxWidgetClass,
				vpaneWidget, NULL, 0);

    args = 0;
    sprintf( fat_string, "%3d", 0);
    XtSetArg( argList[0], XtNvalue, (XtArgVal)fat_string ); args++;
    XtSetArg( argList[1], XtNlabel, (XtArgVal)"fat" ); args++;
    XtSetArg( argList[2], XtNleft, XtChainLeft ); args++;
    Fat = XtCreateManagedWidget("dialog3", dialogWidgetClass,
					      otherBox, argList, args);

    args = 0;
    sprintf( color_string, "%2d", 7);
    XtSetArg( argList[0], XtNvalue, (XtArgVal)color_string ); args++;
    XtSetArg( argList[1], XtNlabel, (XtArgVal)"color" ); args++;
    XtSetArg( argList[2], XtNfromHoriz, Fat); args++;
    Color = XtCreateManagedWidget("dialog4", dialogWidgetClass,
					      otherBox, argList, args);

    otherBox2 = XtCreateManagedWidget("otherBox", boxWidgetClass,
				vpaneWidget, NULL, 0);

    args = 0;
    sprintf( size_string, "%6.3f", .25);
    XtSetArg( argList[0], XtNvalue, (XtArgVal)size_string ); args++;
    XtSetArg( argList[1], XtNlabel, (XtArgVal)"Size" ); args++;
    XtSetArg( argList[2], XtNfromHoriz, Color); args++;
    Size = XtCreateManagedWidget("dialog5", dialogWidgetClass,
					      otherBox2, argList, args);

    args = 0;
    sprintf( box_string, "%s","yes");
    XtSetArg( argList[0], XtNvalue, (XtArgVal)box_string ); args++;
    XtSetArg( argList[1], XtNlabel, (XtArgVal)"Box" ); args++;
    XtSetArg( argList[2], XtNfromHoriz, Size); args++;
    Boxit = XtCreateManagedWidget("dialog6", dialogWidgetClass,
					      otherBox2, argList, args);


    
    /*
     * Dialog for the label.
     */
    args = 0;
    strcpy(text_string,"");
    XtSetArg( argList[0], XtNvalue, (XtArgVal)text_string); args++;
    XtSetArg( argList[1], XtNlabel, (XtArgVal)"Label for this point" ); args++;
    PointLabelWidget = XtCreateManagedWidget("dialog", dialogWidgetClass,
					       vpaneWidget, argList, args);
    
    /*
     * Confirmation button.
     */
    confirmWidget = XtCreateManagedWidget("confirm", commandWidgetClass,
					  PointLabelWidget , NULL, 0);
    XtAddCallback( confirmWidget ,  XtNcallback, PopupConfirm, NULL );
    
    /*
     * Cancellation button.
     */
    cancelWidget = XtCreateManagedWidget("cancel", commandWidgetClass,
					 PointLabelWidget , NULL, 0);
    XtAddCallback( cancelWidget ,  XtNcallback, PopupCancel, NULL );

    /*
     * Let the shell widget know we're here.
     */
    XtManageChild(vpaneWidget);
    XtRealizeWidget(PopUpWidget);
    inited = True;
  }

  set_labels( vpx,vpy);
  
  /*
   * Take some pains to center the popup on the pointer, but be certain
   * the thing is visible, else they can never exit
   */
  args = 0;
  XtSetArg(argList[args], XtNx, &px); args++;
  XtSetArg(argList[args], XtNy, &py); args++;
  XtSetArg(argList[args], XtNwidth, &pw); args++;
  XtSetArg(argList[args], XtNheight, &ph); args++;
  XtGetValues(xtpen, argList,args);
  
  args = 0;
  XtSetArg(argList[args], XtNwidth, &width); args++;
  XtSetArg(argList[args], XtNheight, &height); args++;
  XtGetValues(PopUpWidget,argList,args);
  
  root_x -= (width/2);
  root_y -= (3*height/4);
  
  if ( root_x < 0 ) root_x = 0;
  if ( root_y < 0 ) root_y = 0;
  
  args = 0;
  XtSetArg(argList[args], XtNx, root_x); args++;
  XtSetArg(argList[args], XtNy, root_y); args++;
  XtSetValues(PopUpWidget,argList,args);
  
  /* reset the string */
  args = 0;
  strcpy(text_string,"");
  XtSetArg( argList[0], XtNvalue, (XtArgVal)text_string); args++;
  XtSetValues(PointLabelWidget, argList, args);
  
  /* pop it up */
  XtPopup(PopUpWidget, XtGrabExclusive);
}

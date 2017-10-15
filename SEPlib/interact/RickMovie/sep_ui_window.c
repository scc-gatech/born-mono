#include <sepConfig.h>
#if defined (HAVE_MOTIF) || defined(HAVE_ATHENA)
/*
user interface: setup windows
*/
#include <Xm/DrawingA.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/MainW.h>
#include <stdio.h>
#include "main.h"
#include "ui.h"
#include "axis.h"
#include "data.h"
#include "map.h"
#include "render.h"
#include "view.h"
#include "region.h"
#include "pick.h"
#include "movie.h"
#include "color.h"
#include "colorbar.h"

UI ui;
Message message;
#define	CS	XmStringCreateSimple

/* initialize user interface */
void UIInit (int argc, char **argv)
	{

	NEW (UI,ui,1);
	/* set state variables */
	ui->wide = UI_HITE;
	ui->hite = UI_HITE;
	ui->first = 1;
	ui->hzoom = 0;
	ui->vzoom = 0;
	ui->x1 = NO_INDEX;
	ui->y1 = NO_INDEX;
	ui->x2 = NO_INDEX;
	ui->y2 = NO_INDEX;
	ui->v1 = NO_INDEX;
	ui->v2 = NO_INDEX;
	ui->across = 0;
	ui->down = 0;
	ui->delta = 1;
	ui->pickmode = PICK_VALUE;
	ui->a_shell = 0;
	ui->s_shell = 0;
	ui->z_shell = 0;
	ui->l_shell = 0;
	ui->t_shell = 0;
	ui->f_shell = 0;
	ui->shell = 0;
	ui->style = VIEW_CUBE;
	/* start up motif application */
	ui->application = XtVaAppInitialize (&ui->context, "test", NULL, 0, &argc, argv, NULL,
		NULL);
	/* motif main window */
	ui->main = XtVaCreateManagedWidget ("main",xmMainWindowWidgetClass,ui->application,NULL);
	/* make the menubar */
	UIMenuInit (ui->main);
	/* create window array */
	UIWindowInit (ui->main);
	XtRealizeWidget (ui->application);
	XmProcessTraversal(ui->canvas, XmTRAVERSE_CURRENT);
	}

/* main loop */
void UIMain (void)
	{
	if (!ui) return;
	XtAppMainLoop (ui->context);
	}

/* animation timer */
void UITimer ( int delay, XtTimerCallbackProc action)
	{
	if (!ui) return;
	if (delay <= 0 && ui->timer) {
		XtRemoveTimeOut (ui->timer);
		}
	else	{
		ui->timer = XtAppAddTimeOut (ui->context,delay,action,NULL);
		}
	}

/* create array of windows */
void UIWindowInit (Widget parent)
	{
	Widget frame;
	extern View view;
	extern char canvas_trans[], colorbar_trans[];
	extern XtActionsRec ui_actions[];
	extern int ui_nactions;

	if (!ui) return;
	ui->base = XtVaCreateManagedWidget ("base",xmFormWidgetClass,ui->main,NULL);
	if (GETPARFLOAT ("width","f",&ui->width)) {
		if (ui->width < 10.) {
			ui->wide = ui->width * DisplayWidth (UIDisplay(),UIScreen());
			}
		else if (ui->width > 63) {
			ui->wide = ui->width;
			}
		}
	if (GETPARFLOAT ("height","f",&ui->height)) {
		if (ui->height < 10.) {
			ui->hite = ui->height * DisplayHeight (UIDisplay(),UIScreen());
			}
		else if (ui->height > 63) {
			ui->hite = ui->height;
			}
		}
	/* framed message area */
	frame = XtVaCreateManagedWidget ("frame",xmFrameWidgetClass,ui->base,
		XmNtopAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		NULL);
	ui->message = XtVaCreateManagedWidget ("message", xmLabelWidgetClass, frame,
		XmNalignment, XmALIGNMENT_BEGINNING,
		NULL);
	/* controls below the message */
	frame = XtVaCreateManagedWidget ("frame",xmFrameWidgetClass,ui->base,
		XmNtopAttachment, XmATTACH_WIDGET,
		XmNtopWidget, frame,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		NULL);
	UIControlInit1 (frame);
	frame = XtVaCreateManagedWidget ("frame",xmFrameWidgetClass,ui->base,
		XmNtopAttachment, XmATTACH_WIDGET,
		XmNtopWidget, frame,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		NULL);
	UIControlInit2 (frame);
	/* colorbar below the controls */
	frame = XtVaCreateManagedWidget ("frame",xmFrameWidgetClass,ui->base,
		XmNtopAttachment, XmATTACH_WIDGET,
		XmNtopWidget, frame,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		NULL);
	ui->colorbar = XtVaCreateManagedWidget ("colorbar",xmDrawingAreaWidgetClass,frame,
		XmNheight, COLORBAR_THICK,
		XmNwidth, ui->wide,
		XmNbackground, 0,
		XmNtranslations, XtParseTranslationTable(colorbar_trans),
		NULL);
	XtAddCallback (ui->colorbar,XmNexposeCallback,(XtCallbackProc)UIDrawColorbar,NULL);
	/* drawing area below color bar */
	frame = XtVaCreateManagedWidget ("frame",xmFrameWidgetClass,ui->base,
		XmNtopAttachment, XmATTACH_WIDGET,
		XmNtopWidget, frame,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		NULL);
	ui->canvas = XtVaCreateManagedWidget ("canvas",xmDrawingAreaWidgetClass,frame,
		XmNheight, ui->hite,
		XmNwidth, ui->wide,
		XmNbackground, 0,
		XmNtranslations, XtParseTranslationTable(canvas_trans),
	NULL);
	XtAddCallback (ui->canvas,XmNexposeCallback,(XtCallbackProc)UIDrawCanvas,NULL);
	XtAppAddActions(ui->context,ui_actions,ui_nactions);
	strcpy (ui->font,UI_FONT);
	GETPARSTRING ("font","s",ui->font);
	}


/* return display */
Display* UIDisplay (void)
	{
	if (!ui) return ((Display *) NULL);
	return ((Display*)XtDisplay(ui->base));
	}

/* return screen */
int UIScreen (void)
	{
	return (0);
	}

/* return canvas */
XID
UICanvasWindow (void)
	{
	if (!ui) return ((XID) 0);
	return ((XID)XtWindow(ui->canvas));
	}

/* return color bar */
XID
UIColorbarWindow (void)
	{
	if (!ui) return ((XID) 0);
	return ((XID)XtWindow(ui->colorbar));
	}

/* return main window */
XID
UIMainWindow (void)
	{
	if (!ui) return ((XID) 0);
	return ((XID)XtWindow(ui->application));
	}

/* print message in UI window */
void UIMessage (Message message)
	{
	if (!ui) return;
	XtVaSetValues (ui->message,XmNlabelString,CS(message),NULL);
	}

/* return font */
XFontStruct*
UIFont (int size)
	{
	return ((XFontStruct*)XLoadQueryFont(UIDisplay(),ui->font));
	}

/* return first state */
int UIFirst (void)
	{
	if (!ui) return(0);
	return (ui->first);
	}

#endif

#include <sepConfig.h>
#if defined (HAVE_MOTIF) || defined(HAVE_ATHENA)
/*
Xwindows drawing primitives
*/
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#if defined(XAW)
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#endif
#ifdef XVIEW
#include <xview/xview.h>
#include <xview/canvas.h>
#include <xview/panel.h>
#endif
#include <stdio.h>
#include "main.h"
#include "axis.h"
#include "data.h"
#include "map.h"
#include "render.h"
#include "view.h"
#if defined(XAW)
#include "xaw.h"
#endif
#ifdef XVIEW
#include "xv.h"
#endif
#include "draw.h"
#include "color.h"
#include "ui.h"

Draw draw = 0;

/* initialize draw object */
void DrawInit (void)
	{
	int i,j;
	Colormap dd;
	XColor ss;
	

	extern Data data;

	NEW (Draw,draw,1);
	/* windows */
	draw->display = (Display*)UIDisplay();
	draw->screen = UIScreen();
	draw->window = UICanvasWindow();
	/* colors */
/*
	if (XMatchVisualInfo (draw->display,draw->screen,8,PseudoColor,&draw->vinfo) == 0) {
		err ("Visual mismatch of XWindows display");
		}
*/





	draw->visual = DefaultVisual (draw->display,draw->screen);

	draw->cmap = XCreateColormap (draw->display,draw->window,draw->visual,AllocAll);
	XInstallColormap (draw->display,draw->cmap);

	for (i=0; i<255; i++) {
		
		draw->color[i].pixel = i;
		draw->color[i].flags = DoRed | DoGreen | DoBlue;
		}




	XQueryColors (draw->display,DefaultColormap(draw->display,draw->screen),draw->color,255);
	XStoreColors (draw->display,draw->cmap,draw->color,255);
	XSetWindowColormap (draw->display,UIMainWindow(),draw->cmap);
	XSetWindowColormap (draw->display,UICanvasWindow(),draw->cmap);
	XSetWindowColormap (draw->display,UIColorbarWindow(),draw->cmap);
	draw->ncolor = NCOLOR;

/*

	draw->visual = DefaultVisual (draw->display,draw->screen);

	

	system("sleep 1");
	draw->cmap = XCreateColormap (draw->display,draw->window,draw->visual,AllocAll);
	XInstallColormap (draw->display,draw->cmap);

	fprintf(stderr,"before default \n");
	dd=DefaultColormap(draw->display,draw->screen);
	fprintf(stderr,"after default \n");

	for (i=0; i<255; i++) {
		draw->color[i].red = i;
		draw->color[i].green = i;
		draw->color[i].blue = i;
		draw->color[i].pixel = i;
		draw->color[i].flags = DoRed | DoGreen | DoBlue;
		ss.pixel = i;
		ss.flags = DoRed | DoGreen | DoBlue;
		fprintf(stderr,"here i go %d \n",i);
		XQueryColor (draw->display,dd,&ss);
		fprintf(stderr,"here i go %d \n",i);
	XStoreColor (draw->display,draw->cmap,&(draw->color[i]));
	}



	fprintf(stderr,"here i am one \n");

	fprintf(stderr,"here i am one \n");
	XStoreColors (draw->display,draw->cmap,draw->color,255);
	fprintf(stderr,"here i am one \n");
	XSetWindowColormap (draw->display,UIMainWindow(),draw->cmap);
	XSetWindowColormap (draw->display,UICanvasWindow(),draw->cmap);
	fprintf(stderr,"here i am one \n");
	XSetWindowColormap (draw->display,UIColorbarWindow(),draw->cmap);
	draw->ncolor = NCOLOR;
	fprintf(stderr,"here i am one \n");



*/


	GETPARINT ("ncolor","d",&draw->ncolor);
	draw->base = NXCOLOR - draw->ncolor;
	draw->base -= draw->base % 2;
	/* graphics */
	draw->gc = XCreateGC (draw->display,draw->window,0,0);
	XSetBackground (draw->display,draw->gc,draw->base);
	XSetForeground (draw->display,draw->gc,draw->base+1);
	draw->font = (XFontStruct*)UIFont (18);
	XSetFont (draw->display,draw->gc,draw->font->fid);
	draw->image = 0;
	draw->mask = OVERLAY_MASK;
	/* stored pixmaps */
	draw->npmax = DataMaxDim (data);
	NEW (Pixmap*,draw->pms,draw->npmax);
	for (i=0; i < draw->npmax; i++) draw->pms[i] = 0;
	draw->npm = 0;
	draw->h0 = 0;
	draw->v0 = 0;
	draw->nh = 0;
	draw->nv = 0;
	draw->watch = XCreateFontCursor(draw->display,XC_watch);
	DrawWatch (1);
	}

/* draw image data at specified rectangle */
void DrawImage (Render render,int h0,int v0,int nh,int nv)
                      /* transfer this window of the image */
	{
	int x, y, wide, hite;
	static int count=0;
	if (!draw || !render) return;
	UICanvasSize (&wide,&hite);
	/* create XWindows image of correct size */
	if (!draw->image || wide != draw->wide || hite != draw->hite) {
		draw->image = (XImage*)XCreateImage (draw->display,
			draw->visual,8,ZPixmap,0,(char*)RenderBuffer(render),
			wide,hite,16,wide);
		draw->wide = wide;
		draw->hite = hite;
		}
	draw->image->data = (char*)RenderBuffer(render);
	DrawMask (IMAGE_MASK);
	XPutImage (draw->display,draw->window,draw->gc,draw->image,
		h0,v0,h0,v0,nh,nv);
	XFlush (draw->display);
	}

/* draw a line */
void DrawLine (int x1,int y1,int x2,int y2,int mode)
	{
	if (!draw) return;
	switch (mode) {
	case ERASE:
		DrawMask (OVERLAY_MASK);
		DrawColor (ERASE_COLOR);
		XDrawLine (draw->display,draw->window,draw->gc,
			x1,y1,x2,y2);
		break;
	case DRAW:
		DrawMask (OVERLAY_MASK);
		DrawColor (OVERLAY_COLOR);
		XDrawLine (draw->display,draw->window,draw->gc,
			x1,y1,x2,y2);
		break;
		}
	}

/* draw an arrow */
void DrawArrow (int x1,int y1,int x2,int y2,int wide,int mode)
	{
	extern double atan2(), sin(), cos();
	float theta, pi4= .7853982;
	int x, y;

	if (!draw) return;
	switch (mode) {
	case ERASE:
		DrawMask (OVERLAY_MASK);
		DrawColor (ERASE_COLOR);
		break;
	case DRAW:
		DrawMask (OVERLAY_MASK);
		DrawColor (OVERLAY_COLOR);
		break;
		}
	if (y1 == y2 && x1 == x2) return;
	theta = atan2 ((double)(y2-y1),(double)(x2-x1));
	XDrawLine (draw->display,draw->window,draw->gc,
		x1,y1,x2,y2);
	x = x1 + wide * cos (theta + pi4);
	y = y1 + wide * sin (theta + pi4);
	XDrawLine (draw->display,draw->window,draw->gc,
		x,y,x1,y1);
	x = x1 + wide * cos (theta - pi4);
	y = y1 + wide * sin (theta - pi4);
	XDrawLine (draw->display,draw->window,draw->gc,
		x,y,x1,y1);
	}


/* draw a box */
void DrawBox (int x1,int y1,int x2,int y2,int mode)
	{
	if (!draw) return;
	switch (mode) {
	case ERASE:
		DrawMask (OVERLAY_MASK);
		DrawColor (ERASE_COLOR);
		break;
	case DRAW:
		DrawMask (OVERLAY_MASK);
		DrawColor (OVERLAY_COLOR);
		break;
		}
	XDrawRectangle (draw->display,draw->window,draw->gc,
		x1<x2?x1:x2,y1<y2?y1:y2,x1<x2?x2-x1:x1-x2,y1<y2?y2-y1:y1-y2);
	}

/* draw a filled rectangle */
void DrawSheet (int x1,int y1,int x2,int y2,int mode)
	{
	if (!draw) return;
	switch (mode) {
	case ERASE:
		DrawMask (OVERLAY_MASK);
		DrawColor (ERASE_COLOR);
		break;
	case DRAW:
		DrawMask (OVERLAY_MASK);
		DrawColor (OVERLAY_COLOR);
		break;
		}
	XFillRectangle (draw->display,draw->window,draw->gc,
		x1<x2?x1:x2,y1<y2?y1:y2,x1<x2?x2-x1:x1-x2,y1<y2?y2-y1:y1-y2);
	}

/* set the drawing color */
void DrawColor (int color)
	{
	if (!draw) return;
	if (color >= 0) {
		XSetForeground (draw->display,draw->gc,color);
		}
	else	{
		XSetForeground (draw->display,draw->gc,
			BlackPixel(draw->display,draw->screen));
		}
	}

/* set the drawing mask */
void DrawMask (int mask)
	{
	if (!draw) return;
	XSetPlaneMask (draw->display,draw->gc,mask);
	}

/* SetDrawingWindow */
void DrawWindow (int window)
	{
	int wide, hite;

	if (!draw) return;
	draw->window = window;
	}

/* clear the drawing screen */
void DrawClear (void)
	{
	int wide, hite;

	if (!draw) return;
	DrawMask (CLEAR_MASK);
	XSetForeground (draw->display,draw->gc,draw->base);
	UICanvasSize (&wide,&hite);
	XFillRectangle (draw->display,draw->window,draw->gc,0,0,wide,hite);
	XSetForeground (draw->display,draw->gc,1);
	XFlush (draw->display);
	}

/* draw text */
void DrawText (int x,int y,int align,char *text)
	{
	int d1, d2, d3;
	XCharStruct cstruct;

	if (!draw) return;
	DrawColor (OVERLAY_COLOR);
	DrawMask (OVERLAY_MASK);
	XTextExtents (draw->font,text,strlen(text),&d1,&d2,&d3,&cstruct);
	if (align&TEXT_H50) x -= cstruct.width / 2;
	if (align&TEXT_H100) x -= cstruct.width;
	if (align&TEXT_V50) y += cstruct.ascent / 2;
	if (align&TEXT_V0) y += cstruct.ascent;
	if (align&TEXT_V100) y -= 5;
	XDrawImageString (draw->display,draw->window,draw->gc,x,y,text,
		strlen(text));
	XFlush (draw->display);
	}

/* set portion of color table */
void DrawColors ( unsigned short red[], unsigned short green[], unsigned short blue[], int ncolor)
	{
	int i;

	if (!draw) return;
	for (i=1; i<ncolor; i++) {
		draw->color[i].pixel = i + draw->base;
		draw->color[i].red = red[i];
		draw->color[i].green = green[i];
		draw->color[i].blue = blue[i];
		draw->color[i].flags = DoRed | DoGreen | DoBlue;
		}
	XStoreColors (draw->display,draw->cmap,draw->color+1,ncolor-2);
	XFlush (draw->display);
	}

/* return color table base */
int DrawColorBase(void)
	{
	if (!draw) return (NO_INDEX);
	return (draw->base);
	}

/* return number of colors */
int DrawColorSize (void)
	{
	if (!draw) return (NO_INDEX);
	return (draw->ncolor);
	}

/* change single color */
void DrawColor1 (int index,float red,float green,float blue)
	{
	if (!draw) return;
	if (index < 0 || index > 255) return;
	draw->color[0].pixel = index;
	draw->color[0].red = red * COLOR_SCALE;
	draw->color[0].green = green * COLOR_SCALE;
	draw->color[0].blue = blue * COLOR_SCALE;
	draw->color[0].flags = DoRed | DoGreen | DoBlue;
	XStoreColor (draw->display,draw->cmap,draw->color);
	}

/* draw an existing pixmap */
int DrawPixmap (int i)
	{
	extern Draw draw;

	if (!draw || i > draw->npmax) return(0);
	if (draw->pms[i]) {
		XSetPlaneMask (draw->display,draw->gc,255);
		XCopyArea (draw->display,draw->pms[i],UICanvasWindow(),draw->gc,
			0,0,draw->nh,draw->nv,draw->h0,draw->v0);
		return (1);
		}
	else return (0);
	}

/* save an image region in a pixmap */
int DrawSavePixmap (int i)
	{
	extern Draw draw;
	extern int _core;

	if (!draw || i > draw->npmax) return(0);
	RenderRect (&draw->h0,&draw->v0,&draw->nh,&draw->nv);
	if (i > draw->npmax || draw->npm * draw->nh * draw->nv > _core/2) return(0);
	if (!draw->pms[i]) {
		draw->pms[i] = XCreatePixmap (draw->display,draw->window,draw->nh,draw->nv,8);
		draw->npm++;
		}
	XSetPlaneMask (draw->display,draw->gc,255);
	XCopyArea (draw->display,UICanvasWindow(),draw->pms[i],draw->gc,draw->h0,draw->v0,draw->nh,draw->nv,0,0);
	return (1);
	}

/* free all pixmap storage */
void DrawFreePixmaps (void)
	{
	int i;

	if (!draw) return;
	for (i=0; i<draw->npmax; i++) {
		if (draw->pms[i]) {
			XFreePixmap (draw->display,draw->pms[i]);
			draw->pms[i] = 0;
			}
		}
	draw->npm = 0;
	draw->h0 = 0;
	draw->v0 = 0;
	draw->nh = 0;
	draw->nv = 0;
	}

/* print draw info */
void DrawInfo (void)
	{
	extern Draw draw;
	Message message;

	if (!draw) return;
	sprintf (message, "Draw: wide=%d hite=%d npmax=%d npm=%d h0=%d v0=%d nh=%d nv=%d\n",
			draw->wide,
			draw->hite,
			draw->npmax,
			draw->npm,
			draw->h0,
			draw->v0,
			draw->nh,
			draw->nv);
	UIMessage (message);
	}

void DrawWatch (int mode)
	{
	if (mode) {
		XDefineCursor (draw->display,UICanvasWindow(),draw->watch);
		XDefineCursor (draw->display,UIColorbarWindow(),draw->watch);
		}
	else	{
		XUndefineCursor (draw->display,UICanvasWindow());
		XUndefineCursor (draw->display,UIColorbarWindow());
		}
	XFlush (draw->display);
	}
#endif

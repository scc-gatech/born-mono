#include <sepConfig.h>
#if defined (MOTIF_FOUND) || defined(HAVE_ATHENA)
/*
colorbar object
*/
#include <X11/Xlib.h>
#include <X11/Xutil.h>
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
#include "rick.h"
#if defined(XAW)
#include "xaw.h"
#endif
#ifdef XVIEW
#include "xv.h"
#endif

Colorbar colorbar;

/* initialize colorbar */
void ColorbarInit ()
	{
	NEW (Colorbar,colorbar,1);
	colorbar->low[BOTTOM_MARK] = NO_INDEX;
	colorbar->high[BOTTOM_MARK] = NO_INDEX;
	colorbar->low[TOP_MARK] = NO_INDEX;
	colorbar->high[TOP_MARK] = NO_INDEX;
	colorbar->buffer = 0;
	colorbar->image = 0;
	}

/* draw horizontal colorbar */
void ColorbarDraw ()
	{
	int i, wide, hite, color,factor,j;
	int x1, y1, x2, y2;
	register byte bp, be;
	extern Draw draw;
	View myv;
  Buffer temp_buf;

	myv=current_view();
	if (!colorbar) return;
#ifdef XVIEW
	if (UIFirst()) return;
#endif

	UIColorbarSize (&wide,&hite);

  if(draw->depth==8) factor=1;
  else{
  	if(draw->depth==16) factor=2;
  	if(draw->depth==24 || draw->depth==32) factor=4;
    NEW(Buffer,temp_buf,wide*hite);
     }
	MapSetSize (myv->map[AXIS_COLOR],wide);
	FREE (colorbar->buffer);
	NEW (Buffer,colorbar->buffer,wide*hite*factor);

  if(draw->depth==8) bp=colorbar->buffer;
  else bp=temp_buf;

	for (i=0; i<wide; i++) {
		*bp++ = ((((ColorSize()) * i) / (wide - 1)) / 2) *2 + ColorBase();
		}

  if(draw->depth==8) be=colorbar->buffer+wide*hite;
  else be=temp_buf+wide*hite;

	for (; bp<be; bp++) {
		*bp = *(bp-wide);
		}
	DrawWindow (UIColorbarWindow());
	DrawClear ();




  if(draw->depth==8){
	colorbar->image = (XImage*)XCreateImage (draw->display,draw->visual,
   draw->depth, ZPixmap,0,(char*)colorbar->buffer,wide,hite,8,0);
  }
  else{
    if(draw->depth==16){
	   colorbar->image = (XImage*)XCreateImage (draw->display,draw->visual,
      draw->depth, ZPixmap,0,(char*)colorbar->buffer,wide,hite,16,0);
    }
    else if(draw->depth==24 || draw->depth==32){
	   colorbar->image = (XImage*)XCreateImage (draw->display,draw->visual,
     draw->depth, ZPixmap,0,(char*)colorbar->buffer,wide,hite,32,0);
    }
    create_pad_image(wide,hite,temp_buf,colorbar->buffer,colorbar->image);
    colorbar->image->data=colorbar->buffer;
	}

  if(draw->depth!=8) free(temp_buf);

	XPutImage (draw->display,UIColorbarWindow(),draw->gc,colorbar->image,
		0,0,0,0,wide,hite);

	ColorbarOverlay ();
	}

/* draw colorbar overlays: labels, hitogram and pick bars */
void ColorbarOverlay ()
	{
	int wide, hite, i, x1, y1, x2, y2;
	float tic;
	string label;
	View myv;

	myv=current_view();

	if (!colorbar) return;
	/* draw into colorbar */
	DrawWindow (UIColorbarWindow());
	/* clear */
	UIColorbarSize (&wide,&hite);
/*	DrawSheet (0,0,wide,hite,ERASE);*/
	/* tics */
	for (tic=MapTic0(myv->map[AXIS_COLOR]);
		tic<MapTic2(myv->map[AXIS_COLOR])+.5*MapDtic(myv->map[AXIS_COLOR]); 
		tic+=MapDtic(myv->map[AXIS_COLOR])) {
		x1 = MapIndex (myv->map[AXIS_COLOR],tic);
		y1 = 0;
		sprintf (label,MapFormat(myv->map[AXIS_COLOR]),tic);
		if (x1 < 25) {
/*			DrawText (x1,y1+TIC_LENGTH,TEXT_V0|TEXT_H0,label);*/
			DrawString (x1,y1+hite/2,0,label,7,DRAW);
			}
		else if (x1 > wide-25) {
/*			DrawText (x1,y1+TIC_LENGTH,TEXT_V0|TEXT_H100,label);*/
			DrawString (x1,y1+hite/2,0,label,7,DRAW);
			}
		else	{
/*			DrawText (x1,y1+TIC_LENGTH,TEXT_V0|TEXT_H50,label);*/
			DrawString (x1,y1+hite/2,0,label,7,DRAW);
			}
		DrawLine (x1,y1,x1,y1+TIC_LENGTH,DRAW);
		}
	/* histogram */
	x1 = 0;
	y1 = hite - (hite * DataHistogram(ColorBase())) / 100;
	for (i=0; i<ColorSize(); i++) {
		x2 = (i * (wide-1)) / ColorSize();
		y2 = (i * DataValueSize (myv->data)) / ColorSize() + DataValueBase(myv->data);
		y2 = hite - (hite * DataHistogram(y2)) / 100 - 1;
		DrawLine (x1,y1,x2,y2,DRAW);
		x1 = x2;
		y1 = y2;
		}
	/* top pick bar */
	if (colorbar->low[TOP_MARK] > NO_INDEX) {
		DrawSheet ((colorbar->low[TOP_MARK]*wide)/DataValueSize (myv->data),0,
			(colorbar->high[BOTTOM_MARK]*wide)/DataValueSize (myv->data),hite/3,DRAW);
		}
	/* bottom pick bar */
	if (colorbar->low[BOTTOM_MARK] > NO_INDEX) {
		x1 = (colorbar->low[BOTTOM_MARK] * wide) / DataValueSize (myv->data);
		x2 = wide / (2 * DataValueSize (myv->data));
		DrawSheet (x1-x2,hite/2,x1+x2,hite,DRAW);
		if (colorbar->low[BOTTOM_MARK] != colorbar->high[BOTTOM_MARK]) {
			x2 = (colorbar->high[BOTTOM_MARK] * wide) / DataValueSize (myv->data);
			DrawSheet (x1,(2*hite)/3,x2,hite,DRAW);
			}
		}
	DrawWindow (UICanvasWindow());
	/* reset draw widnow to canvas */
	}

/* set colorbar mark */
void ColorbarSetMark (int low,int high,int index)
	{
	if (!colorbar) return;
	colorbar->low[index] = low;
	colorbar->high[index] = high;
	ColorbarOverlay();
	}

/* return colorbar marks */
void ColorbarMark (int *low,int *high,int index)
	{
	*low = NO_INDEX;
	*high = NO_INDEX;
	if (!colorbar || index < 0 || index > 2) return;
	*low = colorbar->low[index];
	*high = colorbar->high[index];
	}
#endif

#include <sepConfig.h>
#include <math.h>
#if defined (MOTIF_FOUND) || defined(HAVE_ATHENA)
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
#include "rotated.h"
#include "rick.h"
#if defined(XAW)
#include "xaw.h"
#endif
#ifdef XVIEW
#include "xv.h"
#endif


Draw draw = 0;
int iold_color=1;
int overlay_toggle=1;
	void find_visual();

/* initialize draw object */
void DrawInit ()
	{
	int i,j;
	Colormap dd;
	XColor ss;
	View myv;
        char **font_list;
        int count;
	

	myv=current_view();

	NEW (Draw,draw,1);
	/* windows */
	draw->display = (Display*)UIDisplay();
	draw->screen = UIScreen();
	draw->window = UICanvasWindow();
	/* colors */
	find_visual();


   if(draw->truecol==0){
     draw->cmap = XCreateColormap (draw->display,draw->window,draw->visual,AllocAll);
  XInstallColormap (draw->display,draw->cmap);
     for(i=0;i<DrawMaxColor(); i++) set_color_table ( i,(unsigned long) i,(unsigned long) i,(unsigned long) i,0);
 XQueryColors (draw->display,DefaultColormap(draw->display,draw->screen),draw->color,DrawMaxColor()-1);
  XStoreColors (draw->display,draw->cmap,draw->color,DrawMaxColor()-1);

		}

	XSetWindowColormap (draw->display,UIMainWindow(),draw->cmap);
	XSetWindowColormap (draw->display,UICanvasWindow(),draw->cmap);
	XSetWindowColormap (draw->display,UIColorbarWindow(),draw->cmap);
	GETPARINT("ncolor","d",&draw->ncolor);
	draw->base = DrawMaxColor() - draw->ncolor;
	draw->base -= draw->base % 2;
	/* graphics */
	draw->gc = XCreateGC (draw->display,draw->window,0,0);
	XSetBackground (draw->display,draw->gc,draw->base);
	XSetForeground (draw->display,draw->gc,draw->base+1);
	draw->font = (XFontStruct*)UIFont (18);
        if(!draw->font){
          font_list = XListFonts (draw->display, "*courier*180*", 1, &count);
          if(count==0)
          font_list = XListFonts (draw->display, "*courier*", 1, &count);
          if (count > 0)
          {     /* found the font we want, use the first match */
             draw->font =(XFontStruct*) XLoadQueryFont (draw->display, font_list[0]);
                XFreeFontNames (font_list);
          }
          else  err("Font loading problem \n");


       }
	XSetFont (draw->display,draw->gc,draw->font->fid);
	draw->image = 0;
	/* stored pixmaps */
	draw->npmax = DataMaxDim (myv->data);
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
	{
	int x, y, wide, hite,igetch;
	static int count=0,index;
  static unsigned char *raster2,*raster3;
  int   format,xoffset,bitmap_pad,bytes_per_line,do32,i,j,widthpad;
  unsigned int  depth,xwidth,xheight;
  unsigned long   xcol;
unsigned char  *rp, *re, *rp3;
unsigned char  *rp2, mask,*tip;


	if (!draw || !render) return;
	UICanvasSize (&wide,&hite);
	/* create XWindows image of correct size */
  if(draw->depth==8){
	  if (!draw->image || wide != draw->wide || hite != draw->hite) {
	  	draw->image = (XImage*)XCreateImage (draw->display,
	  		draw->visual,draw->depth,ZPixmap,0,(char*)RenderBuffer(render),
	  		wide,hite,8,wide);
		}
	draw->image->data = (char*)RenderBuffer(render);
  }
  else if(draw->depth==16){
      draw->image = (XImage*)XCreateImage(draw->display,
     draw->visual,draw->depth,ZPixmap,0,
    (char*)RenderBuffer2(render),wide,hite,16,0);
   create_pad_image(wide, hite, RenderBuffer(render), 
      RenderBuffer2(render),draw->image);
	}
  else if(draw->depth==24 || draw->depth==32){
      draw->image = (XImage*)XCreateImage(draw->display,
     draw->visual,draw->depth,ZPixmap,0,
    (char*)RenderBuffer2(render),wide,hite,32,0);
   create_pad_image(wide, hite, RenderBuffer(render), 
      RenderBuffer2(render),draw->image);
	}

	  	draw->wide = wide;
	  	draw->hite = hite;
	DrawMask (IMAGE_MASK);
	XPutImage (draw->display,draw->window,draw->gc,draw->image,
		h0,v0,h0,v0,nh,nv);
	}


  void quick_redo(void){
  View myv;
  int igetch;
  int i;
  myv=current_view();
  if(DrawOverlay()==1) ViewDrawPart(DRAW_FRONT|DRAW_SIDE|DRAW_TOP|DRAW_AXES);
	

}


  void create_pad_image(int wide, int hite, Buffer input, Buffer output, XImage *image){
	int x, y;
	static int count=0,index;
  static unsigned char *raster2,*raster3;
  int   format,xoffset,bitmap_pad,bytes_per_line,do32,i,j,widthpad;
  unsigned int  depth,xwidth,xheight;
  unsigned long   xcol;
unsigned char  *rp, *re, *rp3;
unsigned char  *rp2, mask,*tip;

widthpad=wide;
 if( draw->depth == 16){
  /* convert raster3 to a 12/15/16 bit image in raster2 */
   raster2 = (unsigned char *) output;
   depth = draw->depth; format = ZPixmap;
   xoffset = 0; xwidth = wide; xheight = hite;
   bitmap_pad = 16; bytes_per_line = 0;


      raster3=(unsigned char*)input;
   rp3 = raster3;

   if (image->byte_order == MSBFirst) {

           for (i=0, rp2=raster2; i<hite; i++) {
       rp3 = raster3 + i*widthpad;
       for (j=0, tip=rp2; j<wide; j++, rp3++) {
         xcol = draw->map[*rp3];
         *tip++ = xcol >>8 & 0xff;
         *tip++ = xcol & 0xff;
       }
       rp2 += image->bytes_per_line;
		}
   } else {  /* LSBFirst */

     for (i=0, rp2=raster2; i<hite; i++) {
       rp3 = raster3 + i*widthpad;
       for (j=0, tip=rp2; j<wide; j++, rp3++) {
         xcol = draw->map[*rp3];
         *tip++ = xcol & 0xff;
         *tip++ = xcol >>8 & 0xff;
       }
       rp2 += image->bytes_per_line;
     }
   }
	    image->data = (char*)raster2;
	}
 else if(draw->depth==24 || draw->depth==32){
     raster2 = (unsigned char *)output;
      depth = draw->depth; format = ZPixmap;
    xoffset = 0; xwidth = wide; xheight = hite;
      bitmap_pad = 32; bytes_per_line = 0;

     do32 = (image->bits_per_pixel == 32);
    widthpad = ((wide + 1) / 2) * 2;
    widthpad =wide;
      
      raster3=(unsigned char*)input;
     if (image->byte_order == MSBFirst) {
           for (i=0, rp2=raster2; i<hite; i++) {
        rp3 = raster3 + i*widthpad;
              for (j=0, tip=rp2; j<wide; j++, rp3++) {
                 xcol = draw->map[*rp3];
                 if (do32) *tip++ = 0;
                 *tip++ = (xcol>>16) & 0xff;
                 *tip++ = (xcol>>8) & 0xff;
                 *tip++ =  xcol & 0xff;
               }
               rp2 += image->bytes_per_line;
            }

        } else {  /* LSBFirst */
            for (i=0, rp2=raster2; i<hite; i++) {
        rp3 = raster3 + i*widthpad;
              for (j=0, tip=rp2; j<wide; j++, rp3++) {
                xcol = draw->map[*rp3];
                *tip++ =  xcol & 0xff;
                *tip++ = (xcol>>8) & 0xff;
                *tip++ = (xcol>>16) & 0xff;
                if (do32) *tip++ = 0;
              }
              rp2 += image->bytes_per_line;
            }
        }
	    image->data = (char*)raster2;

}


}

/* draw a line */



void DrawLine (int x1,int y1,int x2,int y2,int mode)
	{
	if (!draw) return;
/*   XSetFunction(draw->display,draw->gc,GXxor);*/

  if(draw->depth!=8 && DrawOverlay()!=1 &&  MovieRun()!=0) {
     return;
}

  
	switch (mode) {
	case ERASE:
  case ERASE_ALL:
     if(draw->depth==8){
				DrawMask (OVERLAY_MASK);
				DrawColor (ERASE_COLOR);
				XDrawLine (draw->display,draw->window,draw->gc,
			    x1,y1,x2,y2);
     } 
     else if(using_overlays() && mode==ERASE_ALL==0){
			 quick_redo();
			}
     break;
	case DRAW:
    if(draw->depth==8) DrawMask(OVERLAY_MASK);
    DrawColor(OVERLAY_COLOR);
		XDrawLine (draw->display,draw->window,draw->gc,
			x1,y1,x2,y2);
		break;
		}
	}

void SetFat(int fat)
{
int width;

width=fat;

XSetLineAttributes(draw->display, draw->gc, width, LineSolid, CapNotLast, JoinMiter);

}

void DrawOval(int x,int y,int width,int height,int mode)
{
	if (!draw) return;
	switch (mode) {
	case ERASE:
  case ERASE_ALL:
   if(draw->depth==8){
		DrawMask (OVERLAY_MASK);
		DrawColor (ERASE_COLOR);
		XDrawArc (draw->display,draw->window,draw->gc,
			x,y,width,height,0,360*64);
		}
    else if(mode==ERASE_ALL && using_overlays()==0){
         quick_redo();
		}
		break;
	case DRAW:
		if(draw->depth==8) DrawMask (OVERLAY_MASK);
		DrawColor (OVERLAY_COLOR);
		XDrawArc (draw->display,draw->window,draw->gc,
			x,y,width,height,0,360*64);
		break;
		}
}






/* draw an arrow */
void DrawArrow (int x1,int y1,int x2,int y2,int wide,int mode)
	{
	float theta, pi4= .7853982;
	int x, y;

	if (!draw) return;
	if (y1 == y2 && x1 == x2) return;
	theta = atan2 ((double)(y2-y1),(double)(x2-x1));
	switch (mode) {
	case ERASE:
  case ERASE_ALL:
    if(draw->depth==8){
		DrawMask (OVERLAY_MASK);
		DrawColor (ERASE_COLOR);
	XDrawLine (draw->display,draw->window,draw->gc,
		x1,y1,x2,y2);
	x = x1 + wide * cos ((double)(theta + pi4));
	y = y1 + wide * sin ((double)(theta + pi4));
	XDrawLine (draw->display,draw->window,draw->gc,
		x,y,x1,y1);
	x = x1 + wide * cos ((double)(theta - pi4));
	y = y1 + wide * sin ((double)(theta - pi4));
	XDrawLine (draw->display,draw->window,draw->gc,
		x,y,x1,y1);
		}
    else if(mode==ERASE_ALL && using_overlays()==0){
      quick_redo();
		}
		break;
	case DRAW:
    if(draw->depth==8) DrawMask (OVERLAY_MASK);
		DrawColor (OVERLAY_COLOR);
	XDrawLine (draw->display,draw->window,draw->gc,
		x1,y1,x2,y2);
	x = x1 + wide * cos ((double)(theta + pi4));
	y = y1 + wide * sin ((double)(theta + pi4));
	XDrawLine (draw->display,draw->window,draw->gc,
		x,y,x1,y1);
	x = x1 + wide * cos ((double)(theta - pi4));
	y = y1 + wide * sin ((double)(theta - pi4));
	XDrawLine (draw->display,draw->window,draw->gc,
		x,y,x1,y1);
		break;
		}
	}


/* draw a box */
void DrawString (int x1,int y1,int angle,char *mys,int size,int mode)
	{
	float mangle;
	float scale;

	if (!draw) return;
	 mangle=angle;
	switch (mode) {
	case ERASE:
  case ERASE_ALL:
   if(draw->depth==8){
 		DrawMask (OVERLAY_MASK);
		DrawColor (ERASE_COLOR);
		scale=(float)(size)/8.;
		XRotSetMagnification(scale);
		XRotDrawAlignedString(draw->display, draw->font, mangle,draw->window,
 	  draw->gc,x1,y1,mys,MCENTRE);
   }
    else if(mode==ERASE_ALL && using_overlays()==0){
       quick_redo();
		}
		break;
	case DRAW:
    if(draw->depth==8){
			DrawMask (OVERLAY_MASK);
			DrawColor (OVERLAY_COLOR);
		}
		scale=(float)(size)/8.;
		XRotSetMagnification(scale);
		XRotDrawAlignedString(draw->display, draw->font, mangle,draw->window,
 	  draw->gc,x1,y1,mys,MCENTRE);
		break;
		}
	mangle=angle;


	}



void DrawBox (int x1,int y1,int x2,int y2,int mode)
	{
  View myv;
	if (!draw) return;
  /*set the mask */


	switch (mode) {

	case ERASE:
   case ERASE_ALL:
   if(draw->depth==8) {
     DrawMask(OVERLAY_MASK);
     DrawColor(ERASE_COLOR);
	  XDrawRectangle (draw->display,draw->window,draw->gc,
		 x1<x2?x1:x2,y1<y2?y1:y2,x1<x2?x2-x1:x1-x2,y1<y2?y2-y1:y1-y2);
   }
    else if(mode==ERASE_ALL && using_overlays()==0){
       quick_redo();
		}
		break;
	case DRAW:
    if(draw->depth==8) DrawMask(OVERLAY_MASK);
		DrawColor (OVERLAY_COLOR);
	XDrawRectangle (draw->display,draw->window,draw->gc,
		x1<x2?x1:x2,y1<y2?y1:y2,x1<x2?x2-x1:x1-x2,y1<y2?y2-y1:y1-y2);
		break;
		}




	}







/* draw a filled rectangle */
void DrawSheet (int x1,int y1,int x2,int y2,int mode)
	{
	if (!draw) return;
	switch (mode) {
	case ERASE:
  case ERASE_ALL:
		if(draw->depth==8){
		DrawMask (OVERLAY_MASK);
		DrawColor (ERASE_COLOR);
	XFillRectangle (draw->display,draw->window,draw->gc,
		x1<x2?x1:x2,y1<y2?y1:y2,x1<x2?x2-x1:x1-x2,y1<y2?y2-y1:y1-y2);
		}
    else if(mode==ERASE_ALL && using_overlays()==0){
			 quick_redo();
		}
		break;
	case DRAW:
		if(draw->depth==8) DrawMask (OVERLAY_MASK);
		DrawColor (OVERLAY_COLOR);
	XFillRectangle (draw->display,draw->window,draw->gc,
		x1<x2?x1:x2,y1<y2?y1:y2,x1<x2?x2-x1:x1-x2,y1<y2?y2-y1:y1-y2);
		break;
		}
	}

/* set the drawing color */
void DrawColor (int color)
	{
	if (!draw) return;
	if (color >= 0) {
/*MYCOLOR*/
		if(draw->depth>8){
			 XSetForeground (draw->display,draw->gc,draw->map[color+DrawColorBase()]);
		}
    else XSetForeground (draw->display,draw->gc,color);
		}
	else	{
		XSetForeground (draw->display,draw->gc,
			BlackPixel(draw->display,draw->screen));
		}
	}

/* set the drawing mask */
void DrawMask (long mask)
/*MASK*/
	{
  long outmask;
	if (!draw) return;
  switch(mask){
		case OVERLAY_MASK: outmask=draw->overlay_mask; break;
		case IMAGE_MASK: outmask=draw->image_mask; break;
		case CLEAR_MASK: outmask=draw->clear_mask; break;
    default: outmask=mask; break;
	}
	XSetPlaneMask (draw->display,draw->gc,outmask);
	}

/* SetDrawingWindow */
void DrawWindow (int window)
	{
	int wide, hite;

	if (!draw) return;
	draw->window = window;
	}

/* clear the drawing screen */
void DrawClear ()
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
  if(draw->depth==8){ 
		DrawMask (OVERLAY_MASK);
	}
	else{
		 DrawMask(AllPlanes);
	}
	
	XTextExtents (draw->font,text,strlen(text),&d1,&d2,&d3,&cstruct);
	if (align&TEXT_H50) x -= cstruct.width / 2;
	if (align&TEXT_H100) x -= cstruct.width;
	if (align&TEXT_V50) y += cstruct.ascent / 2;
	if (align&TEXT_V0) y += cstruct.ascent;
	if (align&TEXT_V100) y -= 5;
   DrawString(x,y,0,text,7,DRAW);
	}





/* set portion of color table */
void DrawColors (unsigned short *red,unsigned short *green,unsigned short *blue,int ncolor)
	{
	int i;

	if (!draw) return;
	for (i=1; i<ncolor; i++) {
    set_color_table(i,red[i],green[i],blue[i],draw->base);
		}
	XFlush (draw->display);
	}

/* return color table base */
int DrawColorBase()
	{
	if (!draw) return (NO_INDEX);
	return (draw->base);
	}

/* return number of colors */
int DrawColorSize ()
	{
	if (!draw) return (NO_INDEX);
	return (draw->ncolor);
	}

/* change single color */
void DrawColor1 (int index,float red,float green,float blue)
	{
	if (!draw) return;
	if (index < 0 || index > DrawMaxColor()-1) return;
   set_color_table(0,(unsigned long)(red*COLOR_SCALE),
      (unsigned long)(green*COLOR_SCALE),(unsigned long)(blue*COLOR_SCALE),draw->base);
	}

/* draw an existing pixmap */
int  DrawPixmap (int i)
	{

	if (!draw || i > draw->npmax) return(0);
	if (draw->pms[i]) {
		XSetPlaneMask (draw->display,draw->gc,DrawMaxColor()-1);
		XCopyArea (draw->display,draw->pms[i],UICanvasWindow(),draw->gc,
			0,0,draw->nh,draw->nv,draw->h0,draw->v0);
		return (1);
		}
	else return (0);
	}

/* save an image region in a pixmap */
int DrawSavePixmap (int i)
	{
	extern int _core;

	if (!draw || i > draw->npmax) return(0);
	RenderRect (&draw->h0,&draw->v0,&draw->nh,&draw->nv);
	if (i > draw->npmax || draw->npm * draw->nh * draw->nv > _core/2) return(0);
	if (!draw->pms[i]) {
		draw->pms[i] = XCreatePixmap (draw->display,draw->window,draw->nh,draw->nv,8);
		draw->npm++;
		}
	XSetPlaneMask (draw->display,draw->gc,DrawMaxColor()-1);
	XCopyArea (draw->display,UICanvasWindow(),draw->pms[i],draw->gc,draw->h0,draw->v0,draw->nh,draw->nv,0,0);
	return (1);
	}

/* free all pixmap storage */
void DrawFreePixmaps ()
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
int DrawInfo ()
	{
	Message message;

	if (!draw) return(0);
	sprintf (message, "Draw: wide=%d hite=%d npmax=%d npm=%d h0=%d v0=%d nh=%d nv=%d\n",
			draw->wide,
			draw->hite,
			draw->npmax,
			draw->npm,
			draw->h0,
			draw->v0,
			draw->nh,
			draw->nv);
	UIDelayMessage (message);
	return(0);
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

/*from xtcolor in SEPlib*/

int alloc_colormap(  int ncolor)
{
    Colormap colormap;
    unsigned int ncol ;
    int sucess;
    unsigned long planeMask;
    int mincol,maxcol;


    mincol=maxcol=ncolor;
   mincol=16;

    /* try to allocate these cells in the current colormap */
    ncol = maxcol;
    colormap = draw->cmap;


     sucess=0;
    if( ! sucess ){
    colormap = XCopyColormapAndFree( draw->display,draw->cmap);
         ncol = maxcol;
         do{
           sucess = XAllocColorCells(draw->display,colormap,
      0,&planeMask, 0,(unsigned long*)draw->map,ncol ) ;
           if( !sucess ) ncol = ncol-1;

         } while( !sucess && ncol >=mincol );
    }
    draw->cmap = colormap;
    return ncol;
}

void set_color_table( int col, unsigned long ired, unsigned long igreen,unsigned long  iblue,int base )
{
XColor           pen_color;


    draw->red[col+base] = ired;
    draw->green[col+base] = igreen;
    draw->blue[col+base] = iblue;

    /* put the color in the colormap, we specify the pixel value from the
       values in the table "map".
     */
    draw->color[col].red = draw->red[col+base] ;
    draw->color[col].green = draw->green[col+base];
    draw->color[col].blue = draw->blue[col+base];
    draw->color[col].flags = DoRed | DoGreen | DoBlue;


    if( draw->truecol==1 ) {
   XAllocColor(draw->display,draw->cmap,&draw->color[col]);

   draw->map[col+base] = draw->color[col].pixel;

    }else{
  /* Pesudo-Color we suppy the pixel */
      draw->color[col].pixel=col+base;
      XStoreColor(draw->display,draw->cmap,&draw->color[col]);
    }

}


int DrawDepth(){

return(draw->depth);

}
int using_overlays(){


if(draw->depth==8 || DrawOverlay()!=1) return(1);
else return(0);

}
int DrawMaxColor()
{
return(draw->nxcolor);
}

int DrawNcolor(){
int ncolor;
if(0==draw) GETPARINT("ncolor","d",&ncolor);
else ncolor=draw->ncolor;
return(ncolor);
}

void find_visual(){
	Arg   args[20];
  Visual    *default_visual;
int found=0;

  default_visual = DefaultVisual( draw->display, draw->screen);
	if(XMatchVisualInfo (draw->display,draw->screen,8,PseudoColor,&draw->vinfo)){
    if(draw->vinfo.visual->visualid  == default_visual->visualid){
       found=1;
   draw->visual=draw->vinfo.visual;
   		 draw->depth=8; draw->truecol=0;
   		 draw->image_mask=IMAGE_MASK; draw->clear_mask=CLEAR_MASK;
   		 draw->overlay_mask=OVERLAY_MASK;
   		 draw->nxcolor=256;
   		 draw->ncolor=132;
		}
  }
	if(XMatchVisualInfo (draw->display,draw->screen,32,TrueColor,&draw->vinfo)){
    if(draw->vinfo.visual->visualid  == default_visual->visualid){
       found=1;
   draw->visual=draw->vinfo.visual;
    draw->depth=32; draw->truecol=1;
    draw->image_mask=draw->clear_mask=draw->overlay_mask=AllPlanes;
    draw->nxcolor=256;
    draw->ncolor=200;
		}
  }
	if(XMatchVisualInfo (draw->display,draw->screen,24,TrueColor,&draw->vinfo)){
    if(draw->vinfo.visual->visualid  == default_visual->visualid){
       found=1;
   draw->visual=draw->vinfo.visual;
    draw->depth=24; draw->truecol=1;
    draw->image_mask=draw->clear_mask=draw->overlay_mask=AllPlanes;
    draw->nxcolor=256;
    draw->ncolor=200;
		}
  }
	if(XMatchVisualInfo (draw->display,draw->screen,16,TrueColor,&draw->vinfo)){
    if(draw->vinfo.visual->visualid  == default_visual->visualid){
       found=1;
   draw->visual=draw->vinfo.visual;
    draw->depth=16; draw->truecol=1;
    draw->image_mask=draw->clear_mask=draw->overlay_mask=AllPlanes;
    draw->nxcolor=256;
    draw->ncolor=200;
		}
  }

	if(found==0) err("no acceptable visual found \n");


   draw->iamdefault=1;
	 draw->cmap=DefaultColormap(draw->display, draw->screen);

}
int DrawOverlay(){
 return(overlay_toggle);
}


void DrawOverlayToggle(){

overlay_toggle=overlay_toggle*-1;

}

#endif

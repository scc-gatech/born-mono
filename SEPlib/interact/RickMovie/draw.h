#ifndef RICKMOVIE_DRAW_H
#define RICKMOVIE_DRAW_H
/*
draw object definition

Graphics system dependent information
XView/Windows here
*/
#include <X11/Xutil.h>
#include <X11/Xlib.h>
/* constants */
#define	OVERLAY_COLOR	1
#define	ERASE_COLOR	0
#define	IMAGE_MASK	254
#define	OVERLAY_MASK	1
#define	CLEAR_MASK	255
#define	NXCOLOR		256

/* Draw object */
typedef struct {
/* mostly XWindows variables */
	Display	*display;	/* XWindows */
	int	screen;	/* XWindows */
	Visual	*visual;
	Colormap	cmap;	/* color map */
	GC	gc;	/* XWindows */
	XImage	*image;	/* XWindows */
	XID	window;	/* XWindows */
	XFontStruct	*font;	/* XWindows */
	Cursor	watch;	/* watch cursor */
	Pixmap	*pms;	/* list of pixmaps */
	int	h0, v0, nh, nv; /* pixmap coordinates */
	int	npmax;	/* number of pixmaps */
	int	npm;	/* number of pixmaps allocated */
	int	wide, hite;	/* previouse window size */
	int	ncolor;	/* number of continguous colors allocated */
	int	base; /* color table bias */
	XColor	color[NXCOLOR];	/* color table */
	 XVisualInfo vinfo;
	unsigned long	mask;	/* plane mask */
	} *Draw;

/* API */
extern void DrawInit (void);
extern void DrawImage (Render render,int h0,int v0,int nh,int nv);
extern void DrawLine (int x1,int y1,int x2,int y2,int mode);
extern void DrawArrow (int x1,int y1,int x2,int y2,int wide,int mode);
extern void DrawBox (int x1,int y1,int x2,int y2,int mode);
extern void DrawSheet (int x1,int y1,int x2,int y2,int mode);
extern void DrawColor (int color);
extern void DrawMask (int mask);
extern void DrawWindow (int window);
extern void DrawClear (void);
extern void DrawText (int x,int y,int align,char *text);
extern void DrawColors ( unsigned short red[], unsigned short green[], unsigned short blue[], int ncolor);
extern int DrawColorBase(void);
extern int DrawColorSize (void);
extern void DrawColor1 (int index,float red,float green,float blue);
extern int DrawPixmap (int i);
extern int DrawSavePixmap (int i);
extern void DrawFreePixmaps (void);
extern void DrawInfo (void);
extern void DrawWatch (int mode);

#endif

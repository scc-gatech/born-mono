#ifndef DRAW_RICK
#define DRAW_RICK 2


/*
draw object definition

Graphics system dependent information
XView/Windows here
*/

/* constants */
#define	OVERLAY_COLOR	1
#define	ERASE_COLOR	0
#define	IMAGE_MASK  254
#define	OVERLAY_MASK	1
#define	CLEAR_MASK 255

/* Draw object */
typedef struct {
/* mostly XWindows variables */
	Display	*display;	/* XWindows */
	int	screen;	/* XWindows */
  int depth;
  int truecol;
	Visual	*visual;
	Colormap	cmap;	/* color map */
	GC	gc;	/* XWindows */
	GC	overlay;	/* XWindows */
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
	int	nxcolor;	/* number of continguous colors allocated */
  int iamdefault; /*whether or not we have the default visual*/
	int	base; /* color table bias */
  unsigned long map[NXCOLOR]; /*map for color manipulaiton */
  unsigned long red[NXCOLOR];
  unsigned long green[NXCOLOR];
  unsigned long blue[NXCOLOR];
	XColor	color[NXCOLOR];	/* color table */
	 XVisualInfo vinfo;
	unsigned long	overlay_mask;	/* plane mask */
	unsigned long	image_mask;	/* plane mask */
	unsigned long	clear_mask;	/* plane mask */
	} *Draw;

/* API */
void DrawSheet (int x1,int y1,int x2,int y2,int mode);
void quick_redo(void);

#endif

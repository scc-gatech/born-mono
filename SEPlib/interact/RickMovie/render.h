#ifndef RICKMOVIE_RENDER_H
#define RICKMOVIE_RENDER_H
/*
render object definition

The render has several zoom axes, drawing buffer, zbuffer and shadow buffer
Independent of graphics calls
Uses axis, data, and zoom definitions
*/

/* contants */
#define	RENDER_INTERP	16777216
#define	NALPHA		256
#define	RENDER_POLARITY		1
#define	RENDER_FENCE_TRANSP	1
#define RENDER_LOW_TRANSP	0
#define RENDER_HIGH_TRANSP	100
#define	RENDER_VOL_TRANSP	100

/* a shadow contains index information about an render: drawing side + data index */
typedef unsigned int* Shadow;
typedef unsigned int Shadow_;
/* Zbuffer records depth of object in depth axis samples */
typedef unsigned int* Zbuffer;
typedef unsigned int Zbuffer_;
/* Render object */
typedef struct {
	int	wide;	/* drawing surface dimensions */
	int	hite;
	int	h0, v0, nh, nv;	/* window last drawn */
	int	interp;		/* interpolation flag */
	int	polarity;	/* color mapping polarity */
	int	fence_transp;	/* fence transparency (0-100) */
	int	low_transp;	/* low opacity value */
	int	high_transp;	/* low opacity value */
	int	vol_transp;	/* opacity gradient */
	float	alpha[NALPHA];	/* alpha channel for transparency */
	unsigned char	cmap[256];	/* data -> color map */
	unsigned char	tmap[65536];	/* transparency map */
	Buffer	image;	/* render pixel buffer */
	Shadow	shadow;	/* render shadow buffer */
	Zbuffer	zbuffer;	/* zbuffer to record render depth */
	} *Render;

/* render attribute list */
#define	RENDER_HORZ	1
#define	RENDER_VERT	2
#define	RENDER_DEEP	3
typedef struct {
	int	orient;	/* RENDER_HORZ, RENDER_VERT, RENDER_DEEP */
	int	image;	/* draw image */
	int	shadow;	/* draw shadow */
	int	zbuffer;	/* draw zbuffer */
	int	box;	/* draw bounding box */
	int	axes;	/* draw axes */
	int	lines;	/* draw cross lines */
	int	pick;	/* draw picks */
	int	skew;	/* draw skewed */
	int	transp;	/* draw transparent */
	int	mark;	/* record the frame in plane list */
	} *RenderAttr;

/* typed returns */
extern Render RenderInit (void);
extern void RenderSize (int wide,int hite);
extern void RenderHorz (Data data,Map hmap,Map vmap,Map zmap,Map map4,Map map5,int h0,int v0,Render render,int margins[],RenderAttr attr);
extern void RenderVert (Data data,Map hmap,Map vmap,Map zmap,Map map4,Map map5,int h0,int v0,Render render,int margins[],RenderAttr attr);
extern void RenderDeep (Data data,Map hmap,Map vmap,Map zmap,Map map4,Map map5,int h0,int v0,Render render,int margins[],RenderAttr attr);
extern int RenderBufferValue ( Render render, int x, int y);
extern int RenderShadowValue ( Render render, int x, int y);
extern Buffer RenderBuffer (Render render);
extern void RenderDraw (void);
extern void RenderInfo (Render render);
void RenderSavePar (void);
extern void RenderLine (Render render,int x0,int y0,int x1,int y1,int color);
extern void RenderSetInterp (int mode);
extern void RenderTogglePolarity (void);
extern void RenderToggleInterp (void);
extern void RenderSetFenceTransp (int transparency);
extern void RenderSetLow (int low);
extern void RenderSetHigh (int high);
extern void RenderSetGradient (int vol_transp);
extern void RenderClear (void);
extern void RenderMap (Render render);
extern void RenderMapDump (void);
extern void RenderImageDump (void);
extern void RenderRect (int *h0,int *v0,int *nh,int *nv);

#endif

#ifndef RICKMOVIE_COLORBAR_H
#define RICKMOVIE_COLORBAR_H
/*
Colorbar object
*/

#define BOTTOM_MARK	0
#define	TOP_MARK	1

typedef struct {
	XImage	*image;	/* color bar image */
	Buffer	buffer;		/* color bar data */
	int	low[2];	/* low mark */
	int	high[2];	/* high mark */
	} *Colorbar;

/* API */
extern void ColorbarInit (void);
extern void ColorbarDraw (void);
extern void ColorbarOverlay (void);
extern void ColorbarSetMark (int low,int high,int index);
extern void ColorbarMark (int *low,int *high,int index);

#endif

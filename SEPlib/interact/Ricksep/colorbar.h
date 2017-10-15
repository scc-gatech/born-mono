#ifndef RICK_COLORB
#define RICK_COLORB 1
/*
Colorbar object
*/

#define BOTTOM_MARK	0
#define	TOP_MARK	1

typedef struct {
	XImage	*image;	        /* color bar image */
	Buffer	buffer;		/* color bar data */
	int	low[2];	        /* low mark */
	int	high[2];	/* high mark */
	} *Colorbar;


#endif

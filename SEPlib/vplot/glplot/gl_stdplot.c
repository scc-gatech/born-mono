#include <glplot.h>
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_stdplot ## SUF (struct datainfo ## SUF *data, \
            struct coordinfo ## SUF *coordinate, \
	    struct axisinfo ## SUF * axis1, struct axisinfo ## SUF *axis2,  \
	    struct gridinfo ## SUF *grid, struct titleinfo ## SUF *title, \
 	    int counter, int fastplt, int wantframe, int wantframenum)
#else
#define A1(FTYP,SUF) \
int gl_stdplot ## SUF (data, coordinate, axis1, axis2, grid, title, counter, fastplt, wantframe, wantframenum) \
    struct datainfo ## SUF *data; \
    struct coordinfo ## SUF *coordinate; \
    struct axisinfo ## SUF *axis1; \
    struct axisinfo ## SUF *axis2; \
    struct gridinfo ## SUF *grid; \
    struct titleinfo ## SUF *title; \
    int             counter, fastplt; \
    int             wantframe, wantframenum;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
 \
    /* \
     * declaration of internal variables for Graph  \
     */ \
 \
    if (fastplt < 20) \
    { \
 \
	/* initialization for plotting axes */ \
 \
	gl_clip ## SUF (-VP_MAX, -VP_MAX, VP_MAX, VP_MAX); \
	gl_color ## SUF (axis1->col[counter]); \
	gl_fat ## SUF (axis1->fat[counter]); \
 \
	/* If a frame is wanted draw it */ \
	if (wantframe) \
	    if (fastplt < 16) \
	    { \
		gl_plotframe ## SUF (coordinate, axis1->col[counter]); \
	    } \
	if (fastplt < 14) \
	{ \
 \
	    /* If  y axis is wanted draw it */ \
 \
	    if (((wantframe == 0) || (axis1->axisor != coordinate->min1)) && (axis1->wantaxis)) \
		gl_plotaxis ## SUF (axis1, coordinate, counter); \
	    if ((axis2->wantaxis) && fastplt < 12 ) \
	    { \
             if (axis2->dnum != 0.) \
		{ \
                    if (axis2->ntic != 0) \
		         gl_plottic ## SUF (coordinate, axis2, counter); \
		    gl_labeltic ## SUF (coordinate, axis2); \
                 } \
		/* Label y axis */ \
		gl_labelaxis ## SUF (coordinate, axis2); \
	    } \
	    /* If only x axis is wanted draw it */ \
	    if (((wantframe == 0) || (axis2->axisor != coordinate->min2)) && (axis2->wantaxis)) \
		gl_plotaxis ## SUF (axis2, coordinate, counter); \
	    if ((axis1->wantaxis) && (fastplt < 12)) \
	    { \
                if ((axis1->dnum != 0.)) \
                { \
		if (axis1->ntic != 0) \
		    gl_plottic ## SUF (coordinate, axis1, counter); \
		gl_labeltic ## SUF (coordinate, axis1); \
		} \
                /* Label x axis */ \
		gl_labelaxis ## SUF (coordinate, axis1 ); \
 \
	    } \
	    if (fastplt < 8) \
	    { \
		if (grid->grid1) \
		{ \
		    gl_plotgrid ## SUF (coordinate, axis1, grid, counter); \
		} \
		if (grid->grid2) \
		{ \
		    gl_plotgrid ## SUF (coordinate, axis2, grid, counter); \
		} \
	    } \
 \
	}			/* end of fastplot 14 */ \
	/* label title */ \
	    if ( title->wanttitle  && (fastplt < 3)) \
        { \
	if (*title->wheretitle == 't' || *title->wheretitle == 'b') \
	    gl_plottitle ## SUF (coordinate, title, axis1, counter); \
	if (*title->wheretitle == 'l' || *title->wheretitle == 'r') \
	    gl_plottitle ## SUF (coordinate, title, axis2, counter); \
        } \
	/* label movie frame if needed */ \
	if (wantframenum > 0 && data->n3 > 1 && fastplt < 13) \
	{ \
	    gl_framenum ## SUF (counter, data->d3, data->o3, coordinate->min1, coordinate->min2, axis1->labelsz); \
 \
	} \
    }				/* end of fastplot 20 */ \
	return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

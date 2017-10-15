/*
*
*
*This routine will plot a label in what position it is told to
*
*
*/
#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_labelaxis ## SUF (struct coordinfo ## SUF  *coord, struct axisinfo ## SUF  *axis)
#else
#define A1(FTYP,SUF) \
int gl_labelaxis ## SUF (coord, axis) \
    struct coordinfo ## SUF  *coord; \
    struct axisinfo ## SUF  *axis;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
FTYP            vs, ch, xc, yc, position1, position2; \
FTYP            x, y, xup, yup, xpath, ypath; \
 \
    ch = axis->labelsz / 33.; \
    vs = 1.5 * ch * 5. / 10.; \
	switch (*(axis->wherelabel)) \
	{	 \
	case 't': \
	    position1 = (coord->max1 + coord->min1) / 2; \
	    position2 = coord->max2; \
	    gl_umove ## SUF (position1, position2); \
	    gl_where ## SUF (&xc, &yc); \
	    y = yc + (ch) + (2 *  vs); \
	    x = xc; \
	    xpath = ch ; \
	    ypath = 0.; \
	    yup = ch; \
	    xup = 0.; \
	    break; \
	case 'b': \
	    position1 = (coord->max1 + coord->min1) / 2; \
	    position2 = coord->min2; \
	    gl_umove ## SUF (position1, position2); \
	    gl_where ## SUF (&xc, &yc); \
	    y = yc - ch - (2 * vs);   \
	    x = xc; \
	    xpath = ch ; \
	    yup = ch; \
	    ypath = 0.; \
	    xup = 0.; \
	    break; \
	case 'l': \
	    position1 = coord->min1; \
	    position2 = (coord->min2 + coord->max2) / 2.; \
	    gl_umove ## SUF (position1, position2); \
	    gl_where ## SUF (&xc, &yc); \
	    x = xc - ch - (2 *  vs); \
	    y = yc; \
            if (coord->labelrot == -1) \
             { \
               x = x -(ch  + vs) ;  \
             } \
	    xpath = 0.; \
	    ypath = coord->labelrot * ch; \
	    xup = coord->labelrot * -ch; \
	    yup = 0.; \
	    break; \
	case 'r': \
	    position1 = coord->max1; \
	    position2 = (coord->min2 + coord->max2) / 2.; \
	    gl_umove ## SUF (position1, position2); \
	    gl_where ## SUF (&xc, &yc); \
	    x = xc + ((1 * ch) + (2 * vs)); \
	    y = yc; \
            if (coord->labelrot == -1) \
             { \
               x = x + (ch  + vs) ;  \
             } \
	    xpath = 0.; \
	    ypath = coord->labelrot * ch ; \
	    xup = coord->labelrot * -ch; \
	    yup = 0.; \
	    break; \
	} \
        vp_fat ## SUF  (axis->labelfat); \
        gl_gtext ## SUF (x,y,xpath,ypath,xup,yup,axis->label,axis->wherelabel ); \
	return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

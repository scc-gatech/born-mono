/*
*
*
*    This routine will plot the axes
*
*/
#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_plotaxis ## SUF (struct axisinfo ## SUF  *axis, struct coordinfo ## SUF  *coord, int counter )
#else
#define A1(FTYP,SUF) \
int  gl_plotaxis ## SUF (axis, coord, counter ) \
    struct axisinfo ## SUF  *axis; \
    struct coordinfo ## SUF  *coord;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
        gl_fat ## SUF (axis->fat[counter]); \
    	if (*axis->wherelabel == 't' || *axis->wherelabel == 'b') \
    	{ \
		gl_umove ## SUF (coord->min1, axis->axisor); \
		gl_udraw ## SUF (coord->max1, axis->axisor); \
    	} \
 \
    	if (*axis->wherelabel == 'r' || *axis->wherelabel == 'l') \
    	{ \
		gl_umove ## SUF (axis->axisor, coord->min2); \
		gl_udraw ## SUF (axis->axisor, coord->max2); \
	} \
	return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

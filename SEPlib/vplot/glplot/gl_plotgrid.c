/*
*
*
*   This routine will plot a grid for one axis depending on which one is 
*	specified
*
*
* 	Biondo: 12/1996: use fat
*
*/
#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_plotgrid ## SUF (struct coordinfo ## SUF *coordinate, \
struct axisinfo ## SUF *axis, struct gridinfo ## SUF *grid, int counter)
#else
#define A1(FTYP,SUF) \
int gl_plotgrid ## SUF (coordinate, axis, grid, counter) \
struct coordinfo ## SUF  *coordinate; \
struct axisinfo ## SUF  *axis; \
struct gridinfo ## SUF  *grid; \
int counter;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
FTYP           pos1, pos2, pos3, pos4, num, xmax;  \
FTYP           gnum, min1, min2, max1, max2, num0; \
       min1 = coordinate->min1; \
       max1 = coordinate->max1; \
       min2 = coordinate->min2; \
       max2 = coordinate->max2;  \
       num0 = axis->num0; \
    if (*axis->wherelabel == 't' || *axis->wherelabel == 'b') \
    {  \
       min1 = coordinate->min1; \
       max1 = coordinate->max1; \
       min2 = coordinate->min2; \
       max2 = coordinate->max2;  \
       num0 = axis->num0; \
       gnum = grid->g1num; \
			 if(coordinate->transp==0) gnum = grid->g1num; \
			 else gnum=grid->g2num; \
    } \
    if (*axis->wherelabel == 'l' || *axis->wherelabel == 'r') \
    { \
       min1 = coordinate->min2; \
       max1 = coordinate->max2; \
       min2 = coordinate->min1; \
       max2 = coordinate->max1; \
       num0 = axis->num0;  \
			 if(coordinate->transp==0) gnum = grid->g2num; \
			 else gnum=grid->g1num; \
    } \
   gl_fat ## SUF (grid->fat); \
    if (( grid->col[counter] >= 0 ) && ( grid->col[counter] <= 7 )) \
    gl_color ## SUF (grid->col[counter]); \
    else  \
    gl_color ## SUF (axis->col[counter]); \
   if ( num0 >= max1 ) \
    num0 = min1 + (max1 - min1 ) / 2.; \
    for (num = num0; num <= max1; num += gnum) \
    { \
	if (fabs(num) < ((max1 - min1) / 10000)) \
	{ \
	    num = 0.0; \
	} \
	if (*axis->wherelabel == 't' || *axis->wherelabel == 'b') \
	{ \
	    pos1 = num; \
	    pos2 = min2; \
	    pos3 = num; \
	    pos4 = max2; \
      xmax = max1; \
			if(coordinate->xreverse) gl_rotate1 ## SUF (&pos1,min1,max1); \
	    pos3 = pos1; \
	} \
	if (*axis->wherelabel == 'l' || *axis->wherelabel == 'r') \
	{ \
	    pos1 = min2; \
	    pos2 = num; \
	    pos3 = max2; \
      xmax = max2; \
			/* time to rotate */ \
			if(coordinate->yreverse) gl_rotate1 ## SUF (&pos2,min1,max1); \
	    pos4 = pos2; \
	} \
	gl_umove ## SUF (pos1, pos2); \
	gl_udraw ## SUF (pos3, pos4); \
    } \
	return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

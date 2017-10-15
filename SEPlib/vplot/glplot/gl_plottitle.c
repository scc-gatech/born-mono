/*
*
*
*This routine will plot the plot's title
*
*
*/
#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_plottitle ## SUF (struct coordinfo ## SUF  *coord, \
                         struct titleinfo ## SUF  *title, \
                         struct axisinfo ## SUF  *axis, int counter)
#else
#define A1(FTYP,SUF) \
int gl_plottitle ## SUF (coord, title, axis, counter) \
struct coordinfo ## SUF *coord; \
struct axisinfo ## SUF *axis; \
struct titleinfo ## SUF *title; \
int  counter;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
int             temp; \
FTYP           labelvs, labelch, ch, vs, xc, yc, position1, position2; \
FTYP           x, y, xup, yup, xpath, ypath; \
char            titletemp[280]; \
    ch = title->titlesz / 33.; \
    vs = ch * 6. / 10.; \
    temp = title->titlefat; \
    gl_fat ## SUF (temp); \
    gl_color ## SUF (axis->col[counter]); \
    labelch = 0.; \
    labelvs = 0.;  \
     \
    if ((*title->wheretitle == 't')  && ( *axis->wherelabel == 't' )) \
     { \
       labelch = axis->labelsz / 33.; \
       labelvs = 1.5 * labelch * 5. / 10.;   \
     } \
    if ((*title->wheretitle == 'b')  && ( *axis->wherelabel == 'b' )) \
     { \
       labelch = axis->labelsz / 33.; \
       labelvs = 1.5 *labelch * 5. / 10.;   \
     } \
    if ((*title->wheretitle == 'l')  && ( *axis->wherelabel == 'l' )) \
     { \
       labelch = axis->labelsz / 33.; \
       labelvs = 1.5 *labelch * 5. / 10.;   \
     } \
    if ((*title->wheretitle == 'r')  && ( *axis->wherelabel == 'r' )) \
     { \
       labelch = axis->labelsz / 33.; \
       labelvs = 1.5 *labelch * 5. / 10.;   \
     } \
      \
	switch (*title->wheretitle) \
	{ \
	case 't': \
	    position1 = (coord->max1 + coord->min1) / 2; \
	    position2 = coord->max2; \
	    gl_umove ## SUF  (position1, position2); \
	    gl_where ## SUF  (&xc, &yc); \
	    x = xc; \
	    y = yc  + vs + (1 * labelch)  +  (3 * labelvs); \
	    ypath = 0.; \
	    xpath = title->titlesz / 33; \
	    yup = ch; \
	    xup = 0.; \
	    break; \
	case 'b': \
	    position1 = (coord->max1 + coord->min1) / 2; \
	    position2 = coord->min2; \
	    gl_umove ## SUF  (position1, position2); \
	    gl_where ## SUF  (&xc, &yc); \
            y = yc - vs - ((1 * labelch)  + (3 * labelvs)); \
	    x = xc; \
            ypath = 0.; \
	    xpath = ch; \
	    yup = ch; \
	    xup = 0.; \
	    break; \
	case 'l': \
	    position1 = coord->min1; \
	    position2 = (coord->min2 + coord->max2) / 2.; \
	    gl_umove ## SUF  (position1, position2); \
	    gl_where ## SUF  (&xc, &yc ); \
	    x = xc - vs - (1 * labelch) - (3 * labelvs); \
	    y = yc; \
            if (coord->labelrot == -1) \
            { \
              x = x - ch - vs - labelvs; \
            } \
              xpath = 0.; \
	    ypath = coord->labelrot * ch; \
	    xup = coord->labelrot * -ch; \
	    yup = 0.; \
	    break; \
	case 'r': \
	    position1 = coord->max1; \
	    position2 = (coord->min2 + coord->max2) / 2.; \
	    gl_umove ## SUF  (position1, position2); \
	    gl_where ## SUF  (&xc, &yc); \
	    x = xc + vs + ((1 * labelch) + ( 3 * labelvs)); \
	    y = yc; \
            if (coord->labelrot == -1) \
            { \
              x = x  + ch + vs + labelvs; \
            } \
            xpath = 0.; \
	    ypath = coord->labelrot * ch; \
	    xup = coord->labelrot * -ch; \
	    yup = 0.; \
	    break; \
	} \
    gl_gtext ## SUF (x, y, xpath, ypath, xup, yup ,title->title, title->wheretitle); \
	return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

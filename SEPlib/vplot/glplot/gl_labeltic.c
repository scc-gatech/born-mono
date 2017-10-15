/*
*	gl_labeltic(num0, ymin, ymax, xmin, xmax, dnum, wherenumber, ch, vs, 
*	            axisorig, labelsz, fastplt, whichaxis)
*
*	This routine will label the tics
*
*/
#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_labeltic ## SUF (struct coordinfo ## SUF *coord, struct axisinfo ## SUF *axis)
#else
#define A1(FTYP,SUF) \
int gl_labeltic ## SUF (coord, axis) \
    struct coordinfo ## SUF *coord; \
    struct axisinfo ## SUF *axis;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
char            string[10]; \
int             temp1;  \
FTYP           nummax, num, xc, yc, min1, min2, max1, max2, point1, point2, xmax; \
FTYP           x, y, xup, yup, xpath, ypath, temp, num1, ch, vs, num0, dnum; \
 \
    ch = axis->labelsz / 33.; \
    vs = 1.5 * (ch * 5. / 10.); \
    vp_fat ## SUF  (axis->labelfat); \
    num0 = axis->num0; \
    dnum = axis->dnum; \
    xmax = coord->max1; \
    if (*axis->wherelabel == 'l' || *axis->wherelabel == 'r') \
    { \
	min1 = coord->min2; \
	max1 = coord->max2; \
	min2 = coord->min1; \
	max2 = coord->max1; \
    } \
    if (*axis->wherelabel == 't' || *axis->wherelabel == 'b') \
    { \
	max1 = coord->max1; \
	min1 = coord->min1; \
	max2 = coord->max2; \
	min2 = coord->min2; \
    } \
    if (max1 < min1) \
    { \
	temp = min1; \
	min1 = max1; \
	max1 = temp; \
    } \
    if (num0 >= max1) \
	num0 = min1 + (max1 - min1) / 2.; \
 \
 \
    if ((coord->xreverse) && (*axis->wherelabel == 't' || *axis->wherelabel == 'b')) \
    { \
        nummax = num0; \
        num0 = min1 + max1 - num0; \
	for (num = num0; num >= min1; num -= dnum) \
         { \
	    gl_makelabel ## SUF (num, nummax, coord, axis);  \
            nummax = nummax +  dnum ;   \
         } \
 \
    } \
    else  \
    if ((coord->yreverse) && (*axis->wherelabel == 'l' || *axis->wherelabel == 'r')) \
    { \
        nummax = num0; \
        num0 = min1 + max1 - num0; \
        temp1 = 0;  \
	for (num = num0; num >= min1; num -= dnum) \
        { \
	    gl_makelabel ## SUF (num, nummax, coord, axis); \
            nummax = nummax +  dnum ;   \
    } \
         } \
    else \
    { \
	for (num = num0; num <= max1; num += dnum) \
         { \
	    gl_makelabel ## SUF (num,  num, coord, axis); \
          } \
    } \
	return 0; \
}

#ifdef USE_PROTO
#define A3(FTYP,SUF) \
int gl_makelabel ## SUF (FTYP num, FTYP num2, struct coordinfo ## SUF *coord, struct axisinfo ## SUF *axis)
#else
#define A3(FTYP,SUF) \
int gl_makelabel ## SUF ( num, num2, coord, axis) \
FTYP num, num2; \
struct coordinfo ## SUF *coord; \
struct axisinfo ## SUF *axis;
#endif
#define A4(FTYP,FMT,SUF) \
{ \
char            string[10]; \
FTYP            xc, yc, min1, min2, max1, max2, point1, point2, xmax; \
FTYP            x, y, xup, yup, xpath, ypath, temp, num1, ch, vs ; \
    ch = axis->labelsz / 33.; \
    vs = 1.5 * (ch * 5. / 10.); \
    if (*axis->wherelabel == 'l' || *axis->wherelabel == 'r') \
    { \
	min1 = coord->min2; \
	max1 = coord->max2; \
	min2 = coord->min1; \
	max2 = coord->max1; \
    } \
    if (*axis->wherelabel == 't' || *axis->wherelabel == 'b') \
    { \
	max1 = coord->max1; \
	min1 = coord->min1; \
	max2 = coord->max2; \
	min2 = coord->min2; \
    } \
 \
 \
    if (max1 < min1) \
    { \
	temp = min1; \
	min1 = max1; \
	max1 = temp; \
    } \
 \
 \
 \
    if ((coord->xreverse)  && (*axis->wherelabel == 't' || *axis->wherelabel == 'b')) \
    { \
	temp = min1; \
	min1 = max1; \
	max1 = temp; \
 \
    } \
    else  \
    if ((coord->yreverse) && (*axis->wherelabel == 'l' || *axis->wherelabel == 'r')) \
    { \
	temp = min1; \
	min1 = max1; \
	max1 = temp; \
    } \
 \
 \
 \
    if (fabs (num) < ((max1 - min1) / 10000)) \
    { \
	num = 0.0; \
    } \
    if (*axis->wheretics != 'a') \
    { \
	switch (*axis->wherelabel) \
	{ \
	case 't': \
	    point1 = num; \
	    point2 = max2; \
	    break; \
	case 'b': \
	    point1 = num; \
	    point2 = min2; \
	    break; \
	case 'l': \
	    point1 = min2; \
	    point2 = num; \
	    break; \
	case 'r': \
	    point1 = max2; \
	    point2 = num; \
	    break; \
	} \
	gl_umove ## SUF (point1, point2); \
	gl_where ## SUF (&xc, &yc); \
 \
    } \
    else \
    { \
	if (*axis->wherelabel == 't' || *axis->wherelabel == 'b') \
	{ \
	    point1 = num; \
	    point2 = axis->axisor; \
	} \
	if (*axis->wherelabel == 'l' || *axis->wherelabel == 'r') \
	{ \
	    point1 = axis->axisor; \
	    point2 = num; \
	} \
	gl_umove ## SUF (point1, point2); \
	gl_where ## SUF (&xc, &yc); \
    } \
	num1 = num2; \
    if (num1 < 0) \
    { \
	num1 = -1 * num1; \
	if (num1 < .0000001) \
	{ \
	    num1 = 0.0; \
	} \
	else \
	    num1 = num2; \
    } \
    else \
    { \
	if (num1 < .0000001) \
	{ \
	    num1 = 0.0; \
	} \
	else \
	    num1 = num2; \
    } \
    sprintf (string, "%1.5g", num1); \
    switch (*axis->wherelabel) \
    { \
    case 'l': \
	y = yc; \
	x = xc - vs; \
        if (coord->labelrot == -1) \
        { \
            x = x - ch - vs; \
        } \
	xpath = 0.; \
	ypath =  coord->labelrot * ch; \
	xup =  coord->labelrot * -ch; \
	yup = 0; \
	break; \
    case 'r': \
	y = yc; \
	x = xc + vs; \
        if (coord->labelrot == -1) \
        { \
            x = x + ch + vs; \
        } \
	xpath = 0.; \
	ypath = coord->labelrot * ch; \
	xup = coord->labelrot * -ch; \
	yup = 0; \
	break; \
    case 't': \
	y = yc + vs; \
	x = xc; \
	yup = ch; \
	xpath = ch; \
	ypath = 0.; \
	xup = 0.; \
	break; \
    case 'b': \
	y = yc - vs; \
	x = xc; \
	yup = ch; \
	xup = 0.; \
	xpath = ch; \
	ypath = 0.; \
	break; \
    } \
    gl_gtext ## SUF (x, y, xpath, ypath, xup, yup, string, axis->wherelabel); \
 \
	return 0; \
}

A1(float, )
A2(float,"f", )
A3(float, )
A4(float,"f", )

A1(double,_g)
A2(double,"g",_g)
A3(double,_g)
A4(double,"g",_g)

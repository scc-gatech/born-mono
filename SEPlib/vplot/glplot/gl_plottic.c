/*
*
*
*This routine will plot the tics for the axes.  
*
*
*/
#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
extern int gl_maketic ## SUF (FTYP, struct coordinfo ## SUF *, \
                   struct axisinfo ## SUF *); \
int gl_plottic ## SUF ( struct coordinfo ## SUF *coord,struct axisinfo ## SUF * axis, int counter)
#else
#define A1(FTYP,SUF) \
extern int gl_maketic ## SUF (); \
int gl_plottic ## SUF (coord, axis, counter) \
    struct coordinfo ## SUF *coord; \
    struct axisinfo ## SUF *axis; \
    int             counter;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
FTYP           xc, yc, position1, position2, position3, position4, xmax, min1, min2, max1, max2, num, tic, num0, dtic, axisor, dnum, vs; \
int             length, i, temp2; \
FTYP           temp1, ch; \
char            string[10]; \
 \
    ch = (axis->labelsz / 33.); \
    vs = ch * 5. / 10.; \
    num0 = axis->num0; \
    dnum = axis->dnum; \
    vp_fat ## SUF  (axis->fat[counter]); \
    if (*axis->wherelabel == 'l' || *axis->wherelabel == 'r') \
    { \
	min1 = coord->min2; \
	max1 = coord->max2; \
	min2 = coord->min1; \
	max2 = coord->max1; \
	xmax = coord->max1; \
    } \
    if (*axis->wherelabel == 't' || *axis->wherelabel == 'b') \
    { \
	min1 = coord->min1; \
	max1 = coord->max1; \
	min2 = coord->min2; \
	max2 = coord->max2; \
	xmax = coord->max1; \
    } \
 \
    if (max1 < min1) \
    { \
	temp1 = min1; \
	min1 = max1; \
	max1 = temp1; \
    }  \
    if (num0 >= max1) \
	num0 = min1 + (max1 - min1) / 2.;  \
 \
    if ((coord->xreverse) && (*axis->wherelabel == 't' || *axis->wherelabel == 'b')) \
    { \
	temp1 = min1; \
	min1 = max1; \
	max1 = temp1; \
	dnum = dnum; \
        num0 = min1 + max1 - num0; \
 \
    } \
    if ((coord->yreverse) && (*axis->wherelabel == 'l' || *axis->wherelabel == 'r')) \
    { \
        num0 = min1 + max1 - num0; \
	temp1 = min1; \
	min1 = max1; \
	max1 = temp1; \
	dnum = dnum; \
    } \
    if ((coord->yreverse) && (*axis->wherelabel == 'l' || *axis->wherelabel == 'r')) \
    { \
	for (num = num0; num >= max1; num -= dnum) \
	    gl_maketic ## SUF (num, coord, axis); \
    } \
    else \
    if ((coord->xreverse) && (*axis->wherelabel == 't' || *axis->wherelabel == 'b')) \
    { \
	for (num = num0; num >= max1; num -= dnum) \
	    gl_maketic ## SUF (num, coord, axis); \
    } \
    else \
    { \
	for (num = num0; num <= max1; num += dnum) \
	    gl_maketic ## SUF (num, coord, axis); \
    } \
	return 0; \
}


/**
*
*This routine will plot the tics for the axes.  
*
*
*/
#ifdef USE_PROTO
#define A3(FTYP,SUF) \
int gl_maketic ## SUF (FTYP num, struct coordinfo ## SUF *coord, \
                   struct axisinfo ## SUF *axis)
#else
#define A3(FTYP,SUF) \
int gl_maketic ## SUF (num, coord, axis) \
    FTYP           num; \
    struct axisinfo ## SUF *axis; \
    struct coordinfo ## SUF *coord;
#endif
#define A4(FTYP,FMT,SUF) \
{ \
FTYP           xc, yc, position1, position2, position3, position4, xmax, min1, max1, min2, max2, tic, num0, dtic, axisor, dnum, vs; \
int             length, i; \
FTYP           temp1, ch; \
char            string[10]; \
 \
    ch = (axis->labelsz / 33.); \
    vs = ch * 5. / 10.; \
    num0 = axis->num0; \
    dnum = axis->dnum; \
    if (*axis->wherelabel == 'l' || *axis->wherelabel == 'r') \
    { \
	min1 = coord->min2; \
	min2 = coord->min1; \
	max1 = coord->max2; \
	max2 = coord->max1; \
	xmax = coord->max1; \
    } \
    if (*axis->wherelabel == 't' || *axis->wherelabel == 'b') \
    { \
	min1 = coord->min1; \
	min2 = coord->min2; \
	max1 = coord->max1; \
	max2 = coord->max2; \
	xmax = coord->max1; \
    } \
    if (fabs (num) < ((max1 - min1) / 10000)) \
    { \
	num = 0.0; \
    } \
    if (*axis->wheretics != 'a') \
    { \
	switch (*axis->wherelabel) \
	{ \
	case 't': \
	    position1 = num; \
	    position2 = max2; \
	    break; \
	case 'b': \
	    position1 = num; \
	    position2 = min2; \
	    break; \
	case 'r': \
	    position1 = max2; \
	    position2 = num; \
	    break; \
	case 'l': \
	    position1 = min2; \
	    position2 = num; \
	    break; \
	} \
	gl_umove ## SUF (position1, position2); \
	gl_where ## SUF (&xc, &yc); \
	switch (*axis->wherelabel) \
	{ \
	case 'b': \
	    position3 = xc; \
	    position4 = yc - vs; \
	    break; \
	case 't': \
	    position3 = xc; \
	    position4 = yc + vs; \
	    break; \
	case 'l': \
	    position3 = xc - vs; \
	    position4 = yc; \
	    break; \
	case 'r': \
	    position3 = xc + vs; \
	    position4 = yc; \
	    break; \
	} \
	gl_draw ## SUF (position3, position4); \
    } \
    else \
    { \
	switch (*axis->wherelabel) \
	{ \
	case 'b': \
	    position1 = num; \
	    position2 = axis->axisor; \
	    break; \
	case 't': \
	    position1 = num; \
	    position2 = axis->axisor; \
	    break; \
	case 'l': \
	    position1 = axis->axisor; \
	    position2 = num; \
	    break; \
	case 'r': \
	    position1 = axis->axisor; \
	    position2 = num; \
	    break; \
	} \
	gl_umove ## SUF (position1, position2); \
	gl_where ## SUF (&xc, &yc); \
	switch (*axis->wherelabel) \
	{ \
	case 'b': \
	    position3 = xc; \
	    position4 = yc - vs; \
	    break; \
	case 't': \
	    position3 = xc; \
	    position4 = yc + vs; \
	    break; \
	case 'l': \
	    position3 = xc - vs; \
	    position4 = yc; \
	    break; \
	case 'r': \
	    position3 = xc + vs; \
	    position4 = yc; \
	    break; \
	} \
	gl_draw ## SUF (position3, position4); \
    } \
    if (axis->dtic != dnum) \
    { \
	for (tic = axis->num0; tic <= max1; tic += axis->dtic) \
	{ \
	    switch (*axis->wherelabel) \
	    { \
	    case 'b': \
		position1 = tic; \
		position2 = min2; \
		break; \
	    case 't': \
		position1 = tic; \
		position2 = max2; \
		break; \
	    case 'l': \
		position1 = min2; \
		position2 = tic; \
		break; \
	    case 'r': \
		position1 = max2; \
		position2 = tic; \
		break; \
	    } \
	    gl_umove ## SUF (position1, position2); \
	    gl_where ## SUF (&xc, &yc); \
	    if (axis->axisor == min2) \
	    { \
		switch (*axis->wherelabel) \
		{ \
		case 'b': \
		    position3 = xc; \
		    position4 = yc - vs / 2.; \
		    break; \
		case 't': \
		    position3 = xc; \
		    position4 = yc + vs / 2.; \
		    break; \
		case 'l': \
		    position3 = xc - vs / 2.; \
		    position4 = yc; \
		    break; \
		case 'r': \
		    position3 = xc - vs / 2.; \
		    position4 = yc; \
		    break; \
		} \
	    } \
	    else \
	    { \
		switch (*axis->wherelabel) \
		{ \
		case 'b': \
		    position3 = xc; \
		    position4 = yc - vs / 4.; \
		    break; \
		case 't': \
		    position3 = xc; \
		    position4 = yc + vs / 4.; \
		    break; \
		case 'l': \
		    position3 = xc - vs / 4.; \
		    position4 = yc; \
		    break; \
		case 'r': \
		    position3 = xc - vs / 4.; \
		    position4 = yc; \
		    break; \
		} \
 \
	    } \
	    gl_draw ## SUF (position3, position4); \
	} \
    } \
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

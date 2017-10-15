/*
 * Modifications:
 * W. Bauske IBM 04-01-91
 *	RS/6000 uses macros for log10() and pow()
 */
#include <stdio.h>
#include<string.h>
#include <math.h>
static float    sqr[3] = {1.414214, 3.162278, 7.071068};
static float    vint[4] = {1., 2., 5., 10.};
#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_getscl ## SUF (struct coordinfo ## SUF  *coord, struct axisinfo ## SUF  *axis)
#else
#define A1(FTYP,SUF) \
int gl_getscl ## SUF (coord, axis) \
struct coordinfo ## SUF *coord; \
struct axisinfo ## SUF *axis;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
FTYP           temp, temp1, temp2, temp3, temp4, temp5, tempdnum, num; \
FTYP           inch, min, max, num0, dnum; \
double          div, div1, div3, a, b ; \
int             div2, i, length, counter; \
char            string[100]; \
    if (*axis->wherelabel == 't' || *axis->wherelabel == 'b' ) \
    { \
     min = coord->min1; \
     max = coord->max1; \
     inch = axis->inch; \
     num0 = axis->num0; \
      \
    } \
    if (*axis->wherelabel == 'l' || *axis->wherelabel == 'r' ) \
    { \
     min = coord->min2; \
     max = coord->max2; \
     inch = axis->inch; \
     num0 = axis->num0; \
    } \
    if (inch <= 0) \
    { \
	fprintf (stderr, "non positive line length in get_scl"); \
	exit (-1); \
    } \
    if (fabs(max - min) < .000001) \
    { \
	axis->dnum = .5; \
	return(0); \
    } \
    if (max > min) \
	temp = max - min; \
    else \
	temp = min - max; \
    if ((fabs(temp)/inch) < .0000001) \
    { \
    if (max > min) \
    { \
	temp = max - min; \
	axis->dnum = .5; \
    } \
    else \
    { \
	temp = min - max; \
	axis->dnum = -.5; \
    } \
	return(0); \
    } \
    div = temp / (inch); \
    div1 = log10 (div); \
    div2 = div1; \
    if (div < 1.) \
	div2--; \
    b = div / pow (10., (double) div2); \
    for (i = 0; i < 3 && b >= sqr[i]; i++); \
    tempdnum = vint[i] * pow (10., (double) div2); \
    temp4 = tempdnum; \
    length = 0; \
    counter = (int) (fabs((max - min)) / tempdnum); \
    for (i= 0; i < counter; i++) \
    { \
        num = num0 + (i * tempdnum); \
        temp5 = fabs(num); \
        if (temp5 < (fabs((max - min)) / 10000)) \
             num = 0.; \
	sprintf (string, "%1.5g", num); \
	if (strlen (string) > length) \
	    length = strlen (string); \
    } \
    temp1 = axis->labelsz / 33.; \
    temp2 = inch / temp1; \
    temp3 = (length + 1.5) * ((fabs((max - min)) / tempdnum)); \
    if (temp2 < temp3) \
	tempdnum = tempdnum * 2.; \
    if (tempdnum != temp4) \
    { \
	while (temp2 < temp3) \
	{ \
        length = 0; \
        counter = (int) (fabs((max - min)) / tempdnum); \
        for (i= 0; i < counter; i++) \
        { \
            num = num0 + (i * tempdnum); \
        temp5 = fabs(num); \
        if (temp5 < (fabs((max - min)) / 10000)) \
             num = 0.; \
	    sprintf (string, "%1.5g", num); \
	    if (strlen (string) > length) \
	       length = strlen (string); \
        } \
	    temp1 = axis->labelsz / 33.; \
	    temp2 = inch / temp1; \
	    temp3 = (length + 1) * ((fabs((max - min)) / tempdnum)); \
	    if (temp2 < temp3) \
		tempdnum = tempdnum * 2.; \
	} \
    } \
	axis->dnum = tempdnum; \
 \
	return(0); \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

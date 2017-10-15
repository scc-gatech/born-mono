/*
 * gl_opttic(min,max,inch,num0,dnum,dnumtic,labelsz)
 * determines the optimum tic mark interval
 *
 * inputs:
 * outputs:
 *
 * Author - S. Cole   11 Dec92
 *  Code taken from gl_getscl and made into standalone routine.
 */
#include <string.h>
#include <stdio.h>
#include <math.h>
static double    sqr[3] = {1.4142135623731, 3.16227766016838, 7.071067811865475244};
static double    vint[4] = {1., 2., 5., 10.};
#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_opttic ## SUF (FTYP min, FTYP max, FTYP inch, FTYP num0, FTYP *dnumtic, FTYP labelsz)
#else
#define A1(FTYP,SUF) \
int gl_opttic ## SUF (min, max, inch, num0, dnumtic, labelsz) \
FTYP min, max, inch, num0; \
FTYP *dnumtic; \
FTYP labelsz;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
FTYP           temp, temp1, temp2, temp3, temp4, temp5, tempdnum, num; \
double          div, div1, div3, a, b ; \
int             div2, i, length, counter; \
char            string[100]; \
 \
    if (fabs(max - min) < .000001) \
    { \
	*dnumtic = .5; \
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
	*dnumtic = .5; \
    } \
    else \
    { \
	temp = min - max; \
	*dnumtic = -.5; \
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
    temp1 = labelsz / 33.; \
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
	    temp1 = labelsz / 33.; \
	    temp2 = inch / temp1; \
	    temp3 = (length + 1) * ((fabs((max - min)) / tempdnum)); \
	    if (temp2 < temp3) \
		tempdnum = tempdnum * 2.; \
	} \
    } \
	*dnumtic = tempdnum; \
 \
	return(0); \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

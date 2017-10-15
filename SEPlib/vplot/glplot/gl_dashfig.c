/*
*  gl_dashfig(dash, n3_loop)
*
*  Input is dash->dashtype 0-9
*	0 continuous   DEFAULT
*	1 fine dash
*	2 fine dot
*	3 dash
*	4 large dash
*	5 dot dash
*	6 large dash small dash
*	7 double dot
*	8 double dash
*	9 loose dash  The part after the decimal point determines 
*                     the pattern repetition interval
*
*     	Output is dash[0], gap[0], dash[1], gap[1]	
*       determining the type and size of the dashing
*
*
*	END OF SELF DOC
*/
#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_dashfig ## SUF (struct dashinfo ## SUF *dash, int n3_loop)
#else
#define A1(FTYP,SUF) \
int gl_dashfig ## SUF (dash, n3_loop) \
    struct dashinfo ## SUF *dash; \
    int             n3_loop;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
  float   patsize; \
    switch ( (int) dash->dashtype[n3_loop])	/* determining type of dash */ \
    { \
    case 1: \
	patsize = .3; \
	dash->dash[0] = dash->dash[1] = 1.; \
	dash->gap[0] = dash->gap[1] = 2.; \
	break; \
    case 2: \
	patsize = .2; \
	dash->dash[0] = dash->dash[1] = 1.; \
	dash->gap[0] = dash->gap[1] = 6.; \
	break; \
    case 3: \
	patsize = .4; \
	dash->dash[0] = dash->dash[1] = 4.; \
	dash->gap[0] = dash->gap[1] = 2.; \
	break; \
    case 4: \
	patsize = .6; \
	dash->dash[0] = dash->dash[1] = 3.; \
	dash->gap[0] = dash->gap[1] = 2.; \
	break; \
    case 5: \
	patsize = .5; \
	dash->dash[0] = (.3); \
	dash->dash[1] = 3.; \
	dash->gap[0] = dash->gap[1] = 1.; \
	break; \
    case 6: \
	patsize = .6; \
	dash->dash[0] = 4.; \
	dash->dash[1] = 2.; \
	dash->gap[0] = dash->gap[1] = 1.; \
	break; \
    case 7: \
	patsize = .4; \
	dash->dash[0] = dash->dash[1] = 1.; \
	dash->gap[0] = 2.; \
	dash->gap[1] = 4.; \
	break; \
    case 8: \
	patsize = .8; \
	dash->dash[0] = dash->dash[1] = 5.; \
	dash->gap[0] = 2.; \
	dash->gap[1] = 4.; \
	break; \
    case 9: \
	patsize = .6; \
	dash->dash[0] = dash->dash[1] = 1.; \
	dash->gap[0] = dash->gap[1] = 1.; \
	break; \
    case 0: \
	dash->dash[0] = 0.; \
	dash->gap[0] = 0.; \
	dash->dash[1] = 0.; \
	dash->gap[1] = 0.; \
	break; \
    default: \
	dash->dash[0] = 0.; \
	dash->gap[0] = 0.; \
	dash->dash[1] = 0.; \
	dash->gap[1] = 0.; \
	break; \
    } \
    if (dash->dash[0] + dash->dash[1] + dash->gap[0] + dash->gap[1] != 0.) \
    { \
	/* \
	 * checking to see if default case  \
	 */ \
	/*If not default case then find the decimal part of dash->dashtype*/ \
 \
	dash->dashtype[n3_loop] -= (float) (int ) (dash->dashtype[n3_loop]); \
/* If dash->dashtypeis 0 then set dash->dashtype to .4 */ \
 \
	if (dash->dashtype[n3_loop] == 0.) \
	    dash->dashtype[n3_loop] = (.4); \
/* Computing the pattern size for scaling  */ \
	patsize *= dash->dashtype[n3_loop] / (.4 * (dash->dash[0] + dash->dash[1] + dash->gap[0] + dash->gap[1])); \
/* scaling by the pattern size */ \
	dash->dash[0] *= patsize; \
	dash->dash[1] *= patsize; \
	dash->gap[0] *= patsize; \
	dash->gap[1] *= patsize; \
    } \
	return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

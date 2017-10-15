/*
*	gl_framenumber(n3_loop, d3, 03, xmin,ymin, labelsz)
*
*       Plots the frame number when in the movie option 
*
* modified - S. Cole - 24Nov92
*   changed default format to 3 decimal places. also if the
*   value passed is an integer, this routine figures it out
*   and plots it as such.
*
*/
#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_framenum ## SUF (int n3_loop, FTYP d3, FTYP o3, FTYP xmin, FTYP ymin, \
	     FTYP labelsz )
#else
#define A1(FTYP,SUF) \
int gl_framenum ## SUF (n3_loop, d3, o3, xmin, ymin, labelsz) \
    int             n3_loop; \
    FTYP           d3, o3, xmin, ymin, labelsz;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
 \
FTYP           ch, vs, x, y, xpath, ypath, xup, yup, temp; \
FTYP          xc, yc; \
char            string[80]; \
int fat; \
 \
    fat=0; getch("framenumfat","d",&fat); \
    vp_fat ## SUF  (fat); \
    temp = n3_loop * d3 + o3; \
    sprintf (string, "%.3f", temp); \
    if (temp-((int) temp) == 0.) sprintf (string, "%.0f", temp); \
    vp_umove ## SUF  (xmin, ymin); \
    vp_where ## SUF  (&xc, &yc); \
    ch = labelsz / 33.; \
    vs = ch * 5. / 10.; \
    gl_tjust ("b"); \
    x = xc; \
    y = yc -  3 * ch - 2 * vs ; \
    ypath = 0.; \
    xpath = ch; \
    yup = ch; \
    xup = 0.; \
    vp_gtext ## SUF  (x, y, xpath, ypath, xup, yup, string); \
	return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

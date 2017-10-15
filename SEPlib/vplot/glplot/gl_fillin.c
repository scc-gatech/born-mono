/*
*
*
*This routine will fill in side the frame around the picture
*
*
*/
#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_fillin ##SUF (struct coordinfo ##SUF *coordinate, struct colorinfo ##SUF *color)
#else
#define A1(FTYP,SUF) \
int gl_fillin ## SUF (coordinate, color) \
    struct coordinfo ## SUF *coordinate; \
    struct colorinfo ## SUF *color;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
FTYP           xp[4], yp[4]; \
int             lp, fat, xmask, ymask; \
    vp_coltab ## SUF  (8, color->fillcol[0], color->fillcol[1], color->fillcol[2]); \
    vp_color ## SUF  (8); \
    xp[0] = coordinate->min1; \
    xp[1] = coordinate->max1; \
    xp[2] = coordinate->max1; \
    xp[3] = coordinate->min1; \
    yp[0] = coordinate->min2; \
    yp[1] = coordinate->min2; \
    yp[2] = coordinate->max2; \
    yp[3] = coordinate->max2; \
    lp = 4; \
    fat = 0; \
    ymask = 1; \
    xmask = 1; \
    vp_uarea ## SUF  (xp, yp, lp, fat, xmask, ymask); \
	return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

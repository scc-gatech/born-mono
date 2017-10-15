/*
* This routine draws a frame and an axis around a rectangular bar.
* Part of the code was taken from Steve's gl_simpleaxis.c
*
* Author - Hector Urdaneta (SEP)   Jan 3 95
*/
#include <glplot.h>
#define UNIT      1.000001
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_barplot ## SUF (struct plotposition ## SUF *posit, struct axisinfo ## SUF *axis,   \
  float *minval, float *maxval, char *type, int reverse, int counter)
#else
#define A1(FTYP,SUF) \
int gl_barplot ## SUF (posit, axis, minval, maxval, type, reverse, counter) \
struct plotposition ## SUF *posit; \
struct axisinfo ## SUF *axis; \
float *minval, *maxval; \
char *type; \
int reverse; \
int counter;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
    FTYP pad, ch; \
    FTYP x1, x2, y1, y2; \
    FTYP loc, num, ltic; \
    FTYP xpath, ypath, xup, yup; \
    char string[10]; \
    FTYP costh, sinth, xpos, ypos; \
    FTYP dxtic, dytic; \
    int logic; \
    FTYP num0,loc0; \
 \
    /* plot frame */     \
 \
		  vp_fat ## SUF (axis->labelfat); \
 \
    gl_color ## SUF (axis->col[counter]); \
    gl_move ## SUF (posit->xll, posit->yll); \
    gl_draw ## SUF (posit->xll, posit->yur); \
    gl_draw ## SUF (posit->xur, posit->yur); \
    gl_draw ## SUF (posit->xur, posit->yll); \
    gl_draw ## SUF (posit->xll, posit->yll); \
 \
    if(axis->wantaxis) { \
 \
	/* pad is the space skipped between tic and number, \
	   and number and label. ltic is the longitude of the tic */ \
	ltic = .1; \
	pad = 0.15; \
	ch = axis->labelsz / 33.; \
  \
	if(type[0] == 'h') { \
 \
	    x1 = posit->xll; \
	    y1 = posit->yll; \
	    x2 = posit->xur; \
	    y2 = posit->yll; \
	 \
	} else { /* type[0] == 'v' */ \
	    x2 = posit->xur; \
	    y2 = posit->yur; \
	    x1 = posit->xur; \
	    y1 = posit->yll; \
	} \
 \
	gl_move ## SUF (x1,y1); \
	gl_draw ## SUF (x2,y2); \
 \
	/* x and y sizes of tic marks */ \
	costh = (x2 - x1) / axis->inch; \
	sinth = (y2 - y1) / axis->inch; \
	dxtic = ltic * sinth; \
	dytic = -ltic * costh; \
 \
	/*  \
	 * Figure out which quadrant we are in. \
	 * This determines which side of the line the tic \
	 * marks will go on, and how the text will be aligned. \
	 * For each quadrant we compute the vplot text path \
	 * and up vectors. \
	 */ \
	if (x1 <= x2 && y1 <= y2) { \
	    vp_tjust ## SUF (TH_CENTER,TV_TOP); \
	    xpath = ch * costh; \
	    ypath = ch * sinth; \
	    xup = -ch * sinth; \
	    yup = ch * costh; \
	} else if (x1 > x2 && y1 <= y2) {	 \
	    vp_tjust ## SUF (TH_CENTER,TV_BOTTOM); \
	    xpath = -ch * costh; \
	    ypath = -ch * sinth; \
	    xup = ch * sinth; \
	    yup = -ch * costh; \
	} else if (x1 > x2 && y1 > y2) {	 \
	    vp_tjust ## SUF (TH_CENTER,TV_BOTTOM); \
	    xpath = -ch * costh; \
	    ypath = -ch * sinth; \
	    xup = ch * sinth; \
	    yup = -ch * costh; \
	} else if (x1 <= x2 && y1 > y2) {	 \
	    vp_tjust ## SUF (TH_CENTER,TV_TOP); \
	    xpath = ch * costh; \
	    ypath = ch * sinth; \
	    xup = -ch * sinth; \
	    yup = ch * costh; \
	} \
 \
	if(axis->dnum==0.) logic=0; \
	else logic=1.; \
	/* find the optimum tic mark interval */ \
	if(!reverse) { \
	    axis->num0 = minval[counter]; \
	    if(logic==0) { \
		gl_opttic  ## SUF (minval[counter], maxval[counter], axis->inch, axis->num0, &axis->dnum, axis->labelsz);	} \
	    if(maxval[counter] < minval[counter]) axis->dnum *= -1; \
	} else { \
	    axis->num0 = minval[counter]; \
	    if(logic==0) { \
		gl_opttic  ## SUF (maxval[counter], minval[counter], axis->inch, axis->num0, &axis->dnum, axis->labelsz);       } \
	    if(maxval[counter] > minval[counter]) axis->dnum *= -1; \
	    axis->num0 = maxval[counter]; \
	} \
 \
	/* compute tic mark spacing */ \
	axis->dtic = axis->dnum / (maxval[counter]-minval[counter]) * axis->inch; \
	if(axis->dtic < 0.) axis->dtic *= -1; \
 \
 \
	if(axis->dnum>0.){ \
		if(axis->num0 < 0.){ \
			num0=axis->dnum*((int)(axis->num0/axis->dnum)); \
		} \
		else { \
			num0=axis->dnum*((int)(axis->num0/axis->dnum+.999999)); \
		} \
			loc0=(num0-axis->num0)*axis->dtic/axis->dnum; \
	} \
	else{ \
		num0=axis->dnum*((int)(axis->num0/axis->dnum)); \
		loc0=(num0-axis->num0)*axis->dtic/axis->dnum; \
	} \
	 \
 \
 \
 \
	 \
	/* \
	 * move to each tic mark location, draw the tic and the number \
	 */ \
	for (loc=loc0, num=num0; loc <= axis->inch; loc+=axis->dtic, num+=axis->dnum) {  \
	    sprintf(string,"%1.3g",num); \
	    xpos = x1 + loc * costh; \
	    ypos = y1 + loc * sinth; \
	    vp_move ## SUF (xpos,ypos); \
	    vp_draw ## SUF (xpos+dxtic,ypos+dytic); \
	    vp_gtext ## SUF (xpos+dxtic+pad*sinth,ypos+dytic-pad*costh,xpath,ypath,xup,yup,string); \
	} \
	/* now the axis label */ \
	xpos = x1 + loc/2. * costh + dxtic + ch * sinth + 2. * pad * sinth; \
	ypos = y1 + loc/2. * sinth + dytic - ch * costh - 2. * pad * costh; \
  xpos=  x1 + axis->inch * costh * .5 + dxtic + ch * sinth + 2. * pad * sinth; \
  ypos=  y1 + axis->inch * sinth * .5 + dytic - ch * costh - 2. * pad * costh; \
 \
	vp_gtext ## SUF (xpos,ypos,xpath,ypath,xup*1.1,yup*1.1,axis->label); \
 \
		if(logic==0) axis->dnum=0.; \
    } \
	 \
	return 0; \
}
A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

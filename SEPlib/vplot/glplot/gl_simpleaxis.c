/*
* This routine draws and annotates an axis. Any orientation.
* Axis goes from (x1,y1) to (x2,y2). Tic marks are perpendicular
* to it, on the right side as you go from point 1 to point 2.
*
* Author - S. Cole (SEP)   11 Dec 92
*/
#include "glplot.h"

#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_simpleaxis ## SUF ( FTYP x1, FTYP y1, FTYP x2, FTYP y2, FTYP num1, FTYP num2, \
  FTYP dnum, FTYP dnumticin, FTYP ltic, char* label, int lsz,int lfat, int labelrot, \
int n_num,FTYP o_num, FTYP d_num )
#else
#define A1(FTYP,SUF) \
int gl_simpleaxis ## SUF ( x1, y1, x2, y2, num1, num2, \
  dnum, dnumticin, ltic, label, lsz, lfat, labelrot, \
  n_num, o_num, d_num ) \
FTYP x1, x2, y1, y2, num1, num2, dnum, dnumtic, ltic,o_num, d_num; \
int lsz, lfat, labelrot, n_num; \
char *label;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
FTYP		ch, xpath, ypath, xup, yup; \
char            string[10]; \
FTYP		dist, costh, sinth, dtic, xpos, ypos,xpost; \
FTYP		dxtic, dytic, loc, num; \
FTYP		pad; \
FTYP		dnumtic,axispos; \
FTYP          labelsz,xt,dt; \
int            inum; \
 \
     labelsz=lsz; \
     gl_fat ## SUF (lfat); \
 \
    /* pad is the space skipped between tic and number, and number and label */ \
    pad = 0.15; \
    ch = labelsz / 33.; \
 \
    gl_color ## SUF  (7); \
    gl_move ## SUF (x1,y1); \
    gl_draw ## SUF (x2,y2); \
 \
    /* compute sines and cosines of axis angle */ \
    dist = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)); \
    costh = (x2 - x1)/dist; \
    sinth = (y2 - y1)/dist; \
    /* x and y sizes of tic marks */ \
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
    /* call gl_opttic to find the optimum tic mark interval */ \
    dnumtic = dnumticin; \
    if (dnumtic == 0.) gl_opttic ## SUF (num1,num2,dist,num1,&dnumtic,labelsz); \
    if (num1 > num2) dnumtic *= -1.; \
 \
    /* figure out the tic mark spacing */ \
    dtic = dnumtic/(num2-num1) * dist;  \
    if (dtic < 0.) dtic *= -1.; \
 \
    /* \
     * move to each tic mark location, draw the tic and the number \
     */ \
    xt=x1; \
    dt=0; \
    if(labelrot==1){ \
      ypath=ypath*-1; \
      xup=xup*-1; \
      xt=x1+xup; \
      dt=xup; \
    } \
     \
   if(n_num!=-1) { \
     if(d_num==-1. || o_num==-1) seperr("must specify nnum, onum, and dnum"); \
      for(inum=0; inum < n_num; inum++){ \
         axispos=o_num+d_num*inum; \
         loc=(axispos-num1)/(num2-num1)*dist; \
         if(loc >0 && loc < dist){ \
           sprintf(string,"%1.5g",axispos); \
	  xpost = xt + loc * costh; \
    	xpos = x1 + loc * costh; \
	ypos = y1 + loc * sinth; \
        vp_move ## SUF (xpos,ypos); \
	vp_draw ## SUF (xpos+dxtic,ypos+dytic); \
	vp_gtext ## SUF (xpost+dxtic+pad*sinth,ypos+dytic-pad*costh,xpath,ypath,xup,yup,string); \
         } \
      } \
    for (loc=0., num=num1; loc < dist  ; loc+=dtic, num+=dnumtic) ypos=3; \
   } \
    else{ \
    for (loc=0., num=num1; loc < dist  ; loc+=dtic, num+=dnumtic) { \
        inum=inum+1; \
        sprintf(string,"%1.5g",num); \
	xpost = xt + loc * costh; \
	xpos = x1 + loc * costh; \
	ypos = y1 + loc * sinth; \
        vp_move ## SUF (xpos,ypos); \
	vp_draw ## SUF (xpos+dxtic,ypos+dytic); \
	vp_gtext ## SUF (xpost+dxtic+pad*sinth,ypos+dytic-pad*costh,xpath,ypath,xup,yup,string); \
    } \
   } \
    /* now the axis label */ \
    xpos = xt + dt+ loc/2. * costh + dxtic + ch * sinth + 2. * pad * sinth; \
    ypos = y1 + loc/2. * sinth + dytic - ch * costh - 2. * pad * costh; \
    vp_gtext ## SUF (xpos,ypos,xpath,ypath,xup*1.5,yup*1.5,label); \
 \
	return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

/*
*
*  This subroutine will getch in the variables needed to initialize
*  the plot window. This routine will fetch xll, yll, *  xur, yur.  
*  If the user does not set  any of these variables then
*  a default value will be used.  The structures and SCREENWD and
*  SCREENHT  are defined in glplot.h
*
*/
#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_coordint ## SUF ( struct plotposition ## SUF  *pos, struct coordinfo ## SUF  *coord, struct axisinfo ## SUF \
*axis1, struct axisinfo ## SUF  *axis2)
#else
#define A1(FTYP,SUF) \
int gl_coordint ## SUF (pos, coord, axis1, axis2) \
    struct plotposition ## SUF *pos; \
    struct coordinfo ## SUF *coord; \
    struct axisinfo ## SUF *axis1; \
    struct axisinfo ## SUF *axis2;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
int             temp1, temp2; \
FTYP		crowd, crowd1, crowd2; \
    /* \
     * initialization of lower left corner counter  \
     */ \
    temp1 = 0; \
    /* \
     * initialization of upper right corner counter  \
     */ \
    temp2 = 0; \
    /* \
     * if user does not specify a screenwd a default one defined in glplot.h \
     * will be used  \
     */ \
    if (getch ("screenratio", FMT , &pos->screenratio) == 0) \
	pos->screenratio = SCREEN_RATIO; \
    if (getch ("screenht", FMT , &pos->screenht) == 0) \
	pos->screenht =SCREENHT; \
    if (getch ("screenwd", FMT , &pos->screenwd) == 0) \
	pos->screenwd = pos->screenht / pos->screenratio; \
 \
/* want to crowd off the axes?   .75 < crowd  < 1.00 \
   want to crowd the  1-axis ?   .75 < crowd1 < 1.00 \
   want to crowd the  2-axis ?   .75 < crowd2 < 1.00 \
   */ \
     if (!getch ("crowd" , FMT , &crowd ))  crowd  = .75; \
     if (!getch ("crowd1", FMT , &crowd1))  crowd1 = crowd; \
     if (!getch ("crowd2", FMT , &crowd2))  crowd2 = crowd; \
 \
/* calculate inches */ \
     if (!getch ("xinch", FMT , &axis1->inch))  axis1->inch = pos->screenwd * crowd1; \
     if (!getch ("yinch", FMT , &axis2->inch))  axis2->inch = pos->screenht * crowd2; \
    /* \
 \
     * initialization of xll, yll, xur, yur to 0.  \
     */ \
 \
    pos->xll = 0; \
    pos->xur = pos->xll; \
    pos->yll = 0; \
    pos->yur = pos->yll; \
 \
    /* \
     * if xll is input the temp1 is 1  \
     */ \
    if (getch ("xll", FMT , &pos->xll)) \
	temp1 = 1; \
    /* \
     * if yll is input the temp1 is temp1 + 1  \
     */ \
    if (getch ("yll", FMT , &pos->yll)) \
	temp1 = temp1 + 2; \
    switch (temp1) \
    { \
	/* \
	 * if temp1 is 1 then yll was not set  \
	 */ \
    case 1: \
	seperr ("yll was not set\n"); \
	/* \
	 * reseting xll back to 0 will be using inch1 and inch2 to determine \
	 * plot position and size  \
	 */ \
	pos->xll = 0; \
	temp1 = 0; \
	break; \
	/* \
	 * if temp1 is 2 then xll was not set  \
	 */ \
    case 2: \
	seperr ("xll was not set \n"); \
	pos->yll = 0; \
	temp1 = 0; \
	/* \
	 * reseting yll back to 0 will be using inch1 and inch2 to determine \
	 * plot position and size  \
	 */ \
	break; \
    } \
    if (getch ("xur", FMT , &pos->xur)) \
	temp2 = 1; \
    if (getch ("yur", FMT , &pos->yur)) \
	temp2 = temp2 + 2; \
    switch (temp2) \
    { \
    case 1: \
	seperr ("yur was not set\n"); \
	pos->xur = pos->xll; \
	temp2 = 0; \
	/* \
	 * reseting xur back to 0 will be using inch1 and inch2 to determine \
	 * plot position and size  \
	 */ \
	break; \
    case 2: \
	seperr ("xur was not set\n "); \
	pos->yur = pos->yll; \
	temp2 = 0; \
	/* \
	 * reseting yur back to 0 will be using inch1 and inch2 to determine \
	 * plot position and size  \
	 */ \
	break; \
    } \
 \
    if (temp1 == 0 && temp2 != 0) \
    { \
	pos->xll = pos->xur - axis1->inch; \
	pos->yll = pos->yur - axis2->inch; \
    } \
    if (temp1 == 3 && temp2 == 0) \
    { \
	pos->xur = pos->xll + axis1->inch; \
	pos->yur = pos->yll + axis2->inch; \
    } \
    if (temp1 == 3 && temp2 == 3) \
    { \
       axis1->inch = pos->xur - pos->xll; \
       axis2->inch = pos->yur - pos->yll; \
    } \
    if (temp1 == 0 && temp2 == 0) \
    { \
     float marg1, marg2; \
     marg1 =  (pos->screenwd - axis1->inch); \
     marg2 =  (pos->screenht - axis2->inch); \
     pos->xll = marg1 * 2./3.; \
     pos->yll = marg2 * 1./2.; \
     pos->xur = pos->screenwd - marg1 * 1./3.; \
     pos->yur = pos->screenht - marg2 * 1./2.; \
    } \
    /* \
     * fetching transp,  transp default needs to be set in the main \
     * program \
     */ \
      getch ("transp","1", &coord->transp); \
      getch ("xreverse","1", &coord->xreverse); \
      getch ("yreverse","1", &coord->yreverse); \
      if (getch ("labelrot", "1", &coord->labelrot) == 0 ){ \
           coord->labelrot = -1 ; \
} \
      else{ \
         if(coord->labelrot == 1) \
            coord->labelrot = -1;   \
         if(coord->labelrot == 0) \
            coord->labelrot = 1;   \
      } \
	return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

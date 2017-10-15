/*
* This routine initializes scale bar device.
* 
* Author - Hector Urdaneta (SEP)   Jan 3 95
*
*Edit History
*	
*  Bob - bad hack to allow minval and maxval to be passed in rather
*         requiring from other source (getch, file)
*/
#include <glplot.h>
#include <string.h>

#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_barint ## SUF (struct plotposition ## SUF *position, struct axisinfo ## SUF *axis1,struct plotposition ## SUF *barposit, struct axisinfo ## SUF *baraxis,                       float *minval, float *maxval, char *bartype,int *barreverse,                   int nplots,int cubeplot)
#else
#define A1(FTYP,SUF) \
int gl_barint (position, axis1, barposit, baraxis, minval, maxval, bartype,                   barreverse, nplots,cubeplot) \
struct plotposition ## SUF *position, *barposit; \
struct axisinfo ## SUF *axis1, *baraxis; \
float *minval, *maxval; \
char *bartype; \
int *barreverse, nplots,cubeplot;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
 \
    int kk, ii; \
    char wherebarlabel[2], wherebartics[10], label3[280]; \
    int esize, numminval, nummaxval; \
    int wantbaraxis,nf,tempi; \
		int flat,ierr_bar; \
    FTYP dbarnum; \
		float mypct,tempf; \
 \
    /* A vertical scale bar is the default */ \
		if(cubeplot==1) bartype[0] = 'h'; \
		else bartype[0] = 'v'; \
    ierr_bar=getch("bartype","s",bartype); \
    if(bartype[0] != 'h' && bartype[0] != 'v') \
        seperr("bartype option not implemented\n"); \
 \
    /* scale bar width in inches */ \
    if(!getch("barwidth", FMT, &barposit->screenwd)) barposit->screenwd = .36; \
 \
    /* Read in maxvals and minvals. First asume that the user \
     * is passing the values in the command line as:  \
     * maxval=..,..,..,... \
     * minval=..,..,..,... \
     * If not look in the history file  */ \
    kk = 0; \
    if(kk = fetch("minval", "f", minval)) { \
	/* calculate # of minvals and exit if greater than NPMAX */    \
 \
	if (kk > NPMAX){ \
	    seperr ("Too many values for minval were entered, exceeding NPMAX\n"); \
	    exit(-1); \
	/* look to see if the # of minvals is less than the # of plots. \
	 * This could be the case in which the user may want all the \
	 * scale bars to have the same minvals */ \
	} else if (kk < nplots) { \
	    for (ii = kk; ii < nplots; ii++) { \
		minval[ii] = minval[kk-1]; \
	    } \
	} \
    /* Finally check if the minvals are specified in a file */ \
    } else if(auxin("mintmp")) { \
	auxpar("esize","d",&esize,"mintmp"); \
	auxpar("n1","d",&numminval,"mintmp"); \
	/* The # of minvals need to be equal to the # of plots */ \
	if(numminval > nplots) numminval = nplots; \
 \
	sreed("mintmp", minval, numminval*esize); \
 \
	if (numminval < nplots) { \
	    for (ii = numminval; ii < nplots; ii++) { minval[ii] = minval[kk-1]; } \
	} \
    } \
	/* this is a terrible hack, but without changing interface this seems \
    to be to only way to support minval and maxval not to be calculated \
    in the calling program */ \
	else if(minval[0] ==0 && maxval[0] ==0 && (minval[1] != 0 || maxval[1] !=0)){ \
			kk=1; minval[0]=minval[1]; maxval[0]=maxval[1]; \
	    for (ii = kk; ii < nplots; ii++) { minval[ii] = minval[kk-1];  maxval[ii]=maxval[0];} \
	} \
/*	else{*/ \
/*	   seperr ("In order to have a scale bar you must specify its minimum value");*/ \
/*	    exit(-1);*/ \
/*	}*/ \
 \
    /* Same for the maxvals */ \
    kk = 0; \
    if(kk = fetch("maxval","f",maxval)) { \
	/* calculate # of maxvals and exit if greater than NPMAX */    \
 \
	if (kk > NPMAX) { \
	    seperr ("Too many values for maxval were entered, exceeding NPMAX\n"); \
	    exit(-1); \
	} else if (kk < nplots) { \
	    for (ii = kk; ii < nplots; ii++) { \
			maxval[ii] = maxval[kk-1]; \
	    } \
	} \
    } else if(auxin("maxtmp")) { \
	auxpar("esize","d",&esize,"maxtmp"); \
	auxpar("n1","d",&nummaxval,"maxtmp"); \
	if(nummaxval > nplots) nummaxval = nplots; \
 \
	sreed("maxtmp", maxval, nummaxval*esize); \
 \
	if (nummaxval < nplots) { \
	    for (ii = nummaxval; ii < nplots; ii++) { \
		maxval[ii] = maxval[kk-1]; \
	    } \
	} \
    } \
 \
    /* Bar Axis info */ \
		if( 1==getch("xll","f",&tempf)|| 1==getch("xur","f",&tempf)|| \
      1==getch("yll","f",&tempf) || 1==getch("yur","f",&tempf)){ \
				nf=1; \
				 if(0==getch("bar.xll","f",&tempf))  \
						seperr("If specifyin xll, yll, xur, or yur must specify bar.xll\n"); \
				 else barposit->xll=tempf; \
				 if(0==getch("bar.yll","f",&tempf))  \
						seperr("If specifyin xll, yll, xur, or yur must specify bar.yll\n"); \
				 else barposit->yll=tempf; \
				 if(0==getch("bar.xur","f",&tempf))  \
						seperr("If specifyin xll, yll, xur, or yur must specify bar.xur\n"); \
				 else barposit->xur=tempf; \
				 if(0==getch("bar.yur","f",&tempf))  \
						seperr("If specifyin xll, yll, xur, or yur must specify bar.yur\n"); \
				 else barposit->yur=tempf; \
			} \
			else nf=0; \
 \
 \
    /* For a horiz. bar && barreverse == FALSE then bar goes \
       from min (left) to max (right) value, while for a vert.  \
       bar the max is located at the top and the min at the \
       bottom. */ \
    if(!getch("barreverse", "1", barreverse)) *barreverse = 0; \
 \
    if(!getch("barlabelsz", FMT, &baraxis->labelsz)) { \
	baraxis->labelsz = axis1->labelsz; } \
    if(!getch("barlabelfat", "d", &baraxis->labelfat)) { \
	baraxis->labelfat = axis1->labelfat; } \
 \
    /* Initialize barlabel to be label3. */ \
		strcpy(label3, " "); \
    fetch ("label3", "s", label3); \
    strcpy(baraxis->label, label3); \
    getch ("barlabel", "s", baraxis->label); \
 \
    for (kk = 0; kk < NPMAX; kk++) { \
	baraxis->fat[kk] = axis1->fat[kk]; \
	baraxis->col[kk] = axis1->col[kk]; \
    } \
 \
    /* check if user wants a scale bar axis */ \
    baraxis->wantaxis = 1; \
    if (getch ("wantbaraxis", "1", &wantbaraxis)) { \
	if(!wantbaraxis) baraxis->wantaxis=0; \
    } \
 \
    /* check to see where to put the tics for the scale bar axis */ \
    if (! getch ("wherebartics", "s", wherebartics)) strcpy(wherebartics," "); \
    strcpy(baraxis->wheretics, wherebartics); \
 \
    /* Spacing of tics for bar scale */ \
    baraxis->dnum = 0.; \
    if (getch ("dbarnum", FMT, &dbarnum) != 0) \
	baraxis->dnum = dbarnum; \
 \
       \
if(bartype[0] == 'h') { \
		if(nf==0){ \
	barposit->xll = position->xll; \
	barposit->xur = position->xur; \
	barposit->yll = .12 * position->screenht; \
	barposit->yur = barposit->yll + barposit->screenwd; \
	 \
 \
	position->yur += .4*barposit->screenwd; \
	if(cubeplot==1) \
	position->yll += .12*position->screenht + barposit->screenwd; \
	else \
	position->yll += .04*position->screenht + barposit->screenwd; \
	} \
 \
	baraxis->inch = barposit->xur - barposit->xll; \
 \
	if (getch ("wherebarlabel","s",wherebarlabel)) { \
	    switch (*wherebarlabel) { \
	        case 't': \
	        case 'b': \
		strcpy ( baraxis->wherelabel, wherebarlabel ); \
		break; \
	        default: \
		seperr("wherebarlabel should be either t or b for a horizontal scale bar\n"); \
	    } \
	} \
     \
    } else {     \
 \
	if(nf==0){ \
	barposit->xur = .88 * position->screenwd; \
	barposit->xll = barposit->xur - barposit->screenwd; \
	barposit->yll = position->yll; \
	barposit->yur = position->yur; \
	 \
 \
	position->xur -= (.07*position->screenwd + barposit->screenwd); \
	position->xll -= .5*barposit->screenwd; \
	} \
	baraxis->inch = barposit->yur - barposit->yll; \
 \
	if (getch ("wherebarlabel","s",wherebarlabel)) { \
	    switch (*wherebarlabel) { \
	        case 'r': \
	        case 'l': \
		strcpy ( baraxis->wherelabel, wherebarlabel ); \
		break; \
	        default: \
		seperr("wherebarlabel should be either r or l for a vertical scale bar\n"); \
	    } \
	} \
    } \
 \
 \
 \
 \
 \
 \
 \
 \
	return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

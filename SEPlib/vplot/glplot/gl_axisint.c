/*
*Reads input to initialize the length of axis and the number of tics between
*label tics
*
*end self doc
*/
#include <stdio.h>
#include <string.h>
#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_axisint ## SUF (struct axisinfo ## SUF *axis1, \
                       struct axisinfo ## SUF *axis2, \
                       struct coordinfo ## SUF *coord, \
                       struct plotposition ## SUF *position)
#else
#define A1(FTYP,SUF) \
int gl_axisint ## SUF (axis1, axis2, coord, position) \
    struct axisinfo ## SUF *axis1; \
    struct axisinfo ## SUF *axis2; \
    struct coordinfo ## SUF *coord; \
    struct plotposition ## SUF *position; /* isn't used ?? */
#endif
#define A2(FTYP,FMT,SUF) \
{\
int             wantaxis, i, kk, counter, n1tic, n2tic; \
FTYP            dnum, labelsz, tempmin; \
int             *axisfat/*[NPMAX + 1]*/, *axiscol/*[NPMAX + 1]*/, labelfat; \
char            wheretics[10], label[280], wherelabel1[1], wherelabel2[1]; \
int             numdxnum, numdynum; \
int             numo1, numo2, numaxisor1, numaxisor2; \
FTYP            tempnum; \
/* \
 * initializing axisfat and axiscol \
 */ \
\
    axisfat = (int *) calloc(2*(NPMAX+1),sizeof(int)); \
    axiscol = axisfat + NPMAX+1; \
    if(axisfat == ((int *) NULL)) { \
       seperr("gl_axisint memory allocation failure\n"); \
       exit(-1); \
    } \
    for (i = 0; i < NPMAX + 1; i++) \
    { \
	/*axisfat[i] = 0;*/ \
	axiscol[i] = 7; \
    } \
	labelsz = 8; \
	labelfat = 0; \
    /* \
     * fetching axisfat , if fetched then find # fetched  \
     */ \
	kk = 0; \
    if (getch ("axisfat", "d", axisfat)) \
    { \
	/* calculating # of axisfat fetched */ \
	while (axisfat[kk] != 0) \
	    kk++; \
	/* \
	 * checking to see if # of axisfats fetched exceed NPMAX if exceed \
	 * NPMAX will exit   \
	 */ \
	if (kk > NPMAX) \
	{ \
	    seperr ("Too many values for axisfat were entered, exceeding NPMAX\n"); \
	    exit (-1); \
	} \
    } \
    /* assigning axisfat to the structures */ \
    for (i = 0; i < kk; i++) \
    { \
	axis1->fat[i] = axisfat[i]; \
	axis2->fat[i] = axisfat[i]; \
    } \
    /* \
     * assigning axisfat to the structures after the initial count has been \
     * done.  Axisfat will rotate the fatness  \
     */ \
    for (i = kk; i < NPMAX + 1; i++) \
    { \
	if (kk == 0) \
	    kk++; \
	axis1->fat[i] = axisfat[i % kk]; \
	axis2->fat[i] = axisfat[i % kk]; \
    } \
 \
    /* \
     * fetching axiscol , if fetched then find # fetched  \
     */ \
    if (getch ("axiscol", "d", axiscol)) \
    { \
	kk = 0; \
	/* calculating # of axiscol fetched */ \
	while (axiscol[kk] != 7) \
	    kk++; \
	/* checking to see if # of axiscol fetched exceed NPMAX */ \
	/* if exceed NPMAX will exit  */ \
	if (kk > NPMAX) \
	{ \
	    seperr ("Too many values for axiscol were entered, exceeding NPMAX\n"); \
	    exit (-1); \
	} \
    } \
    /* assigning axiscol to the structures */ \
    for (i = 0; i < kk; i++) \
    { \
	axis1->col[i] = axiscol[i]; \
	axis2->col[i] = axiscol[i]; \
    } \
    /* \
     * assigning axiscol to the structures after the initial count has been \
     * done.  Axiscol will rotate the color of the axis  \
     */ \
    for (i = kk; i < NPMAX + 1; i++) \
    { \
	if (kk == 0) \
	    kk++; \
	axis1->col[i] = axiscol[i % kk]; \
	axis2->col[i] = axiscol[i % kk]; \
    } \
    free(axisfat); \
 \
    /* \
     * fetching labelfat , if fetched then find # fetched  \
     */ \
    getch ("labelfat", "d", &labelfat); \
    axis1->labelfat = labelfat;     \
    axis2->labelfat = labelfat;     \
    /* \
     * fetching labelsz , if fetched then find # fetched  \
     */ \
    getch ("labelsz", FMT , &labelsz); \
    axis1->labelsz = labelsz;     \
    axis2->labelsz = labelsz;     \
    /* initialize the axis1.label to be blank */ \
    strcpy(axis1->label, " "); \
    /* fetching the axis1.label */ \
    fetch ("label1", "s", axis1->label); \
    /* \
     * The defaults are not set in here for wherelabel a value needs to be \
     * for it  your main program should set the defaults, will fetch the \
     * override values  \
     */ \
    if (getch ("wherexlabel", "s", wherelabel1)) \
       switch (*wherelabel1) \
       { \
       case 't': \
       case 'b': \
       strcpy ( axis1->wherelabel, wherelabel1); \
       break; \
       default: \
           fprintf (stderr, "wherexlabel should be either t or b not %s \n", wherelabel1); \
       } \
    if (getch ("whereylabel", "s", wherelabel2)) \
       switch (*wherelabel2) \
       { \
       case 'r': \
       case 'l': \
       strcpy ( axis2->wherelabel, wherelabel2); \
       break; \
       default: \
           fprintf (stderr, "whereylabel should be either l or r not %s \n", wherelabel2); \
       } \
    /* checking to see if wantaxis is fetched */ \
    if (getch ("wantaxis", "1", &wantaxis) == 0)  \
    { \
	/* setting the default to put the axes on the plot */ \
        wantaxis = 1; \
	axis1->wantaxis = 1; \
	axis2->wantaxis = 1; \
    } \
    /* if no axis is wanted then set the structures */ \
    if (wantaxis == 0) \
    { \
	axis1->wantaxis=0; \
	axis2->wantaxis=0; \
    } \
    /* else check for each individual axis */ \
    else \
    { \
	if (getch ("wantaxis1", "1", &axis1->wantaxis) == 0) \
	    axis1->wantaxis = 1; \
	if (getch ("wantaxis2", "1", &axis2->wantaxis) == 0) \
	   axis2->wantaxis = 1; \
    } \
    if(coord->transp) \
    { \
    wantaxis = axis1->wantaxis; \
    axis1->wantaxis = axis2->wantaxis; \
    axis2->wantaxis = wantaxis; \
    } \
 \
    /* check to see were to put the tics for the axes */ \
    if (getch ("wheretics", "s", wheretics) == 0) \
	strcpy(wheretics," ");	/* if not set by user default to frame */ \
    /* set to structures */ \
    strcpy(axis1->wheretics, wheretics); \
    strcpy(axis2->wheretics, wheretics); \
    /* fetch position of axisor1 */ \
    numaxisor1 = getch ("axisor1", FMT , &axis1->axisor); \
       if (numaxisor1 == 0 ) \
       { \
      if (*axis1->wherelabel == 'b') \
        axis1->axisor = coord->min2;  /* set default position to min2 */ \
       else \
       if (*axis1->wherelabel == 't') \
	axis1->axisor = coord->max2;  /* set default position to max2 */ \
       } \
    numaxisor2 = getch ("axisor2", FMT , &axis2->axisor);  \
     if ( numaxisor2 == 0) \
     { \
       if (*axis2->wherelabel == 'l') \
        axis2->axisor = coord->min1; \
       else \
       if (*axis2->wherelabel == 'r') \
        axis2->axisor = coord->max1; \
     } \
     if (coord->transp) \
      { \
     if ( numaxisor2 != 0 || numaxisor1 != 0) \
     { \
       tempnum = axis1->axisor; \
       axis1->axisor = axis2->axisor; \
       axis2->axisor = tempnum; \
      } \
     }  \
    if (getch ("n1tic", "d", &n1tic) == 0) \
	n1tic = 1; \
    if (getch ("n2tic", "d", &n2tic) == 0) \
	n2tic = 1; \
     if (coord->transp) \
     { \
     axis1->ntic = n2tic; \
     axis2->ntic = n1tic; \
     } \
     else \
     {  \
     axis1->ntic = n1tic; \
     axis2->ntic = n2tic; \
     }  \
    numdxnum = getch ("d1num", FMT , &axis1->dnum); \
    numdynum = getch ("d2num", FMT , &axis2->dnum); \
    if (coord->transp) \
    { \
    if (numdxnum != 0 || numdynum != 0 ) \
    { \
     tempnum = axis2->dnum;  \
     axis2->dnum = axis1->dnum;  \
     axis1->dnum = tempnum;  \
     tempnum = numdxnum; \
     numdxnum = numdynum; \
     numdynum = tempnum; \
    }    \
    } \
	if (numdxnum == 0 ) \
        gl_getscl ## SUF (coord, axis1); \
	if (numdynum == 0 ) \
	gl_getscl ## SUF (coord, axis2); \
  \
    if (axis1->dnum != 0.) \
    { \
       if (coord->min1 < coord->max1) \
	tempmin = coord->min1;  /* set default position to min2 */ \
       else \
	tempmin = coord->max1;  /* set default position to min2 */ \
	for (axis1->num0 = (int) (tempmin / axis1->dnum) * axis1->dnum - axis1->dnum; axis1->num0 < tempmin; axis1->num0 += axis1->dnum); \
    } \
 \
 \
    strcpy(axis2->label,  " "); \
    fetch ("label2", "s", axis2->label); \
    if (axis2->dnum != 0.) \
    { \
       if (coord->min2 < coord->max2) \
	tempmin = coord->min2;  /* set default position to min2 */ \
       else \
	tempmin = coord->max2;  /* set default position to min2 */ \
	for (axis2->num0 = (int) (tempmin / axis2->dnum) * axis2->dnum - axis2->dnum; axis2->num0 < tempmin; axis2->num0 += axis2->dnum); \
    } \
    if (coord->transp) \
    { \
    numo1 = getch ("o1num", FMT , &axis2->num0); \
    numo2 = getch ("o2num", FMT , &axis1->num0); \
    } \
    else \
    { \
    numo1 = getch ("o1num", FMT , &axis1->num0); \
    numo2 = getch ("o2num", FMT , &axis2->num0); \
    } \
    if ( axis1->dnum !=0.) \
    { \
      n1tic = axis1->ntic; \
     if (axis1->ntic == 0) \
    { \
	n1tic = 1; \
    } \
    axis1->dtic = axis1->dnum / n1tic ; \
       if (coord->min1 < coord->max1) \
	tempmin = coord->min1;  /* set default position to min2 */ \
       else \
        tempmin=coord->max1; \
    for (axis1->tic0 = axis1->num0 - axis1->ntic * axis1->dtic; axis1->tic0 < tempmin; axis1->tic0 += axis1->dtic); \
    } \
    if (axis2->dnum !=0.) \
    { \
       if (coord->min2 < coord->max2) \
	tempmin = coord->min2;  /* set default position to min2 */ \
       else \
	tempmin = coord->max2;  /* set default position to min2 */ \
 \
      n2tic = axis2->ntic; \
    if (axis2->ntic == 0) \
    { \
	n2tic = 1; \
    } \
    axis2->dtic = axis2->dnum / n2tic; \
    for (axis2->tic0 = axis2->num0 - axis2->ntic * axis2->dtic; axis2->tic0 < tempmin; axis2->tic0 += axis2->dtic); \
    } \
     \
        \
    if (coord->transp) \
    { \
       strcpy(label, axis1->label); \
       strcpy(axis1->label,axis2->label); \
       strcpy(axis2->label, label); \
    } \
    if( coord->yreverse ) \
        gl_rotate1 ## SUF (&axis1->axisor, coord->min2, coord->max2); \
    if( coord->xreverse ) \
        gl_rotate1 ## SUF (&axis2->axisor, coord->min1, coord->max1); \
    return 0; \
}
A1(double,_g)
A2(double,"g",_g)

A1(float, )
A2(float,"f", )

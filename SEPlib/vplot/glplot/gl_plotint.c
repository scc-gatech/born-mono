/*
*
*
* The routine will fetch and initialize plot and axis parameters.
*
* 
*/
/*
 * Edited Nov 12 1990 by Joe Dellinger
 * 	plotcol=0 is perfectly legitimate, not an error, so allow it.
 */
#include <string.h>
#include "glplot.h"
#define NOT_SET_YET	-1
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_plotint ## SUF (struct plotinfo ## SUF  *plot, struct dashinfo ## SUF  *dash)
#else
#define A1(FTYP,SUF) \
int gl_plotint ## SUF (plot, dash) \
struct plotinfo ## SUF  *plot; \
struct dashinfo ## SUF  *dash;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
int             colnum, i, j, length, k, nplotfat, nsymbolsz, ndashtype; \
char            symbol[NPMAX]; \
    k = 0; \
    for (i = 0; i < NPMAX; i++) \
    { \
	plot->symbol[i] = ' '; \
	plot->symbolsz[i] = 2; \
	plot->col[i] = NOT_SET_YET; \
	plot->fat[i] = 0; \
	dash->dashtype[i] = 0; \
    } \
    ndashtype = getch ("dash", FMT , dash->dashtype);  \
    if (ndashtype) \
    { \
	for (i = ndashtype; i < NPMAX; i++) \
	{ \
	    dash->dashtype[i] = dash->dashtype[(i % ndashtype)]; \
	} \
    } \
    getch ("symbol", "s", plot->symbol); \
    nplotfat = getch ("plotfat", "d", plot->fat); \
    if(nplotfat)  \
    { \
	for (i = nplotfat; i < NPMAX; i++) \
	{ \
	    plot->fat[i] = plot->fat[(i % nplotfat)]; \
	} \
    } \
    nsymbolsz = getch ("symbolsz", "d", plot->symbolsz); \
    if (nsymbolsz) \
    { \
	for (i = nsymbolsz; i < NPMAX; i++) \
	{ \
	    plot->symbolsz[i] = plot->symbolsz[(i % nsymbolsz)]; \
	} \
    } \
    colnum =  getch ("plotcol", "d", plot->col); \
    if (colnum == 0) \
    { \
	for (i = 0; i < NPMAX; i++) \
	{ \
	    plot->col[i] = 6 - (i % 6); \
	} \
    } \
    else \
    { \
        if (colnum > NPMAX) \
           seperr("ENTERED TOO MANY VALUES FOR PLOTCOL\n"); \
	while (plot->col[k] != NOT_SET_YET) \
	    k++; \
	for (i = k; i < NPMAX; i++) \
	{ \
	    plot->col[i] = plot->col[(i % k)]; \
	} \
    } \
    length = strlen (plot->symbol); \
    for (i = length; i < NPMAX; i = i + length) \
    { \
	for (j = 0; j <= length; j++) \
	{ \
	    plot->symbol[i + j] = plot->symbol[j]; \
	} \
    }  \
	return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"f",_g)

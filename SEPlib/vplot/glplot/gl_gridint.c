/*
*	gl_gridint(xgrid, ygrid, grid, g1num, g2num, d1num, d2num, fastplt)
*
*        Input and initialization of variables needed to draw a grid 
*
*
* 	Biondo: 12/1996: Getch gridfat from command line to initialize fat
*
* end self-doc
*/
#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_gridint ## SUF (struct gridinfo ## SUF  *gridin, struct coordinfo ## SUF  *coordinate, \
	   struct axisinfo ## SUF  *axis1, struct axisinfo ## SUF  *axis2)
#else
#define A1(FTYP,SUF) \
int gl_gridint ## SUF (gridin, coordinate, axis1, axis2) \
        struct coordinfo ## SUF  *coordinate; \
	struct gridinfo ## SUF  *gridin; \
	struct axisinfo ## SUF  *axis1; \
	struct axisinfo ## SUF  *axis2;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
    int grid1, i; \
    FTYP tempnum; \
    int tempgrid; \
    int grid2; \
    int grid0; \
    int gridfat; \
    int gridcol; \
    FTYP g1num, g2num; \
    grid0 = 0; \
    grid1 = 0; \
    grid2 = 0; \
		if(coordinate->transp==0){ \
    g1num = axis1->dnum; \
    g2num = axis2->dnum; \
		} \
		else{ \
    g1num = axis2->dnum; \
    g2num = axis1->dnum; \
		} \
    tempgrid = getch ("grid", "1", &grid0); \
    if (tempgrid == 0)  \
    { \
	getch ("grid1", "1", &grid1); \
        gridin->grid1 = grid1; \
	getch ("grid2", "1", &grid2); \
        gridin->grid2 = grid2; \
    } \
   else \
   { \
    if (grid0 == 1) \
    { \
	gridin->grid1 = 1; \
	gridin->grid2 = 1; \
        grid1=1; \
        grid2=1; \
    } \
    else \
    if (grid0 == 0) \
    { \
	gridin->grid1 = 0; \
	gridin->grid2 = 0; \
        grid1=0; \
        grid2=0; \
    } \
   } \
       getch ("g1num", FMT , &g1num); \
       gridin->g1num=g1num; \
       getch ("g2num", FMT , &g2num); \
       gridin->g2num=g2num; \
   if (g1num == 0. ) \
	if (grid1) \
		seperr("g1num is 0 needs to be set"); \
   if (g2num == 0. ) \
	if (grid2) \
		seperr("g2num is 0 needs to be set"); \
   if (getch("gridcol", "d", &gridcol) == 0 ) \
     for ( i = 0; i <= NPMAX; i++ ) \
        gridin->col[i]=axis1->col[i]; \
   else \
     for ( i = 0; i <= NPMAX; i++ ) \
        gridin->col[i]=gridcol; \
   if (getch("gridfat", "d", &gridfat) == 0 ) \
     gridin->fat=1; \
   else \
     gridin->fat=gridfat; \
   if (coordinate->transp) \
   { \
    tempgrid = gridin->grid1;  \
    gridin->grid1 = gridin->grid2; \
    gridin->grid2 = tempgrid; \
   } \
 \
 \
	return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)

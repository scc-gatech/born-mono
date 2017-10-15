#include "cfortran.h"
#include <septravel.h>


#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int fortran_march(int order,float *s,int *b, int *n, float *d, float slow0, float *slow,
  float *ttime)
_XFUNCPROTOEND
#else
int fortran_march(order,s,b,n,d,slow0,slow,ttime)
int *b,*n,order;
float *s,*d,slow0,*slow;
#endif
{
return(fastmarch(order,s[0],s[1],s[2],b[0],b[1],b[2],n[0],n[1],n[2],d[0],d[1],d[2],slow0,slow,ttime));
}
FCALLSCFUN8(INT,fortran_march,FASTMARCH,fastmarch,INT,PFLOAT,PINT,PINT,PFLOAT, FLOAT, PFLOAT, PFLOAT)
FCALLSCFUN8(INT,fortran_march,FASTMARCH_,fastmarch_,INT,PFLOAT,PINT,PINT,PFLOAT, FLOAT, PFLOAT, PFLOAT)

FCALLSCFUN18(INT,hwt_trace_rays,HWT_TRACE_RAYS,hwt_trace_rays,PINT,PFLOAT,PFLOAT,PFLOAT,FLOAT,FLOAT,FLOAT,FLOAT,INT,FLOAT,INT,FLOAT,INT,PFLOAT,PFLOAT,PFLOAT,PFLOAT,PFLOAT)
FCALLSCFUN18(INT,hwt_trace_rays,HWT_TRACE_RAYS_,hwt_trace_rays_,PINT,PFLOAT,PFLOAT,PFLOAT,FLOAT,FLOAT,FLOAT,FLOAT,INT,FLOAT,INT,FLOAT,INT,PFLOAT,PFLOAT,PFLOAT,PFLOAT,PFLOAT)

FCALLSCFUN16(INT,hwt_travel_cube,HWT_TRAVEL_CUBE,hwt_travel_cube,PINT,PFLOAT,PFLOAT,PFLOAT,FLOAT,FLOAT,FLOAT,FLOAT,INT,FLOAT,INT,FLOAT,INT,PFLOAT,INT,PFLOAT)
FCALLSCFUN16(INT,hwt_travel_cube,HWT_TRAVEL_CUBE_,hwt_travel_cube_,PINT,PFLOAT,PFLOAT,PFLOAT,FLOAT,FLOAT,FLOAT,FLOAT,INT,FLOAT,INT,FLOAT,INT,PFLOAT,INT,PFLOAT)
/*  $Id: fortstubs.c,v 1.1.1.1 2004/03/25 06:37:25 cvs Exp $ */

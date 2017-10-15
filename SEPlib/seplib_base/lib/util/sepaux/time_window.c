/*<
time_window

USAGE
int time_window(n,tarray,weight,window)

INPUT PARAMETERS
n        -int       size 

OUTPUT PARAMETERS
tarray   -float(n)  time
weight   -float(n)  weight
window   -char[]    u,r,s,g,w   windows to apply during slant stack

DESCRIPTION
window signal-processing

CATEGORY
Lib:Aux

COMPILE LEVEL
DISTR
*>/



/*    return a 1d window function
			    reference:  Marple, 1987

	KEYWORDS :  window-design signal-processing

*/

#include <sepaux.h>
#include <math.h>
#include<string.h>



#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int time_window( int n, float *tarray, float *weight, char *window )
_XFUNCPROTOEND
#else
int time_window( n, tarray, weight, window )
int n;
float *tarray, *weight;
char window[];
#endif
	{
	int n1,n2,n3,i1,i2,i3,j1,ir;
	float ar[4];
	double pi;

	pi = 4.*atan(((double) (1.)));
	n1 = n;

	if( n1 == 1 )   { weight[0] = 1.; return(0) ; }
	
	for( i1=0 ; i1 < n1 ; i1++ )
       tarray[i1] = ( (float)  ( i1 - (n1-1)/2 ) ) / (n1-1.) ;

	switch( window[0] ) {
	
	case 'u': case 'h':
		  for( i1=0 ; i1< n1 ; i1++ )
		  weight[i1] = 1.;
		  if( (window[0] == 'h')&& (n1 > 4) ) 
			{  weight[0] = 0.33 ; weight[1] = 0.66 ;
			   weight[n1-2] = 0.66 ; weight[n1-1] = 0.33 ; }
		  break;
	case 't':
		  for( i1=0 ; i1< n1 ; i1++ )
		  weight[i1] = 1. - 2*fabs( tarray[i1] );
		  break;
	case 's':
		  for( i1=0 ; i1< n1 ; i1++ )
		  weight[i1] = 0.5*( 1. + cos( 2.* pi *tarray[i1] ));
		  break;
	case 'r':
		  for( i1=0 ; i1 <n1 ; i1++ )
		  weight[i1] = .54 + .46*cos( 2.* pi * tarray[i1] );
		  break;
	case 'g':
		  for( i1=0 ; i1 <n1 ; i1++ )
		  weight[i1] = exp( ((double) (-12.5))*tarray[i1]*tarray[i1] );
		  break;
	case 'w':
		  ar[0] = .3635819;   ar[1] = .4891775;
		  ar[2] = .1365995;   ar[3] = .0106411;
			for (ir=0; ir < n1; ir++) weight[ir]=0.;
		  for( ir=0; ir<4  ; ir++ )
		  for( i1=0; i1<n1 ; i1++ )
		  weight[i1] += ar[ir]*cos( 2.*pi*ir*tarray[i1] );
		  break;
	default:
		 break;

				}
     return(0);
}

/* HACK, HACK, to get round HP preprocessor bug 
 * we really want 
 * FCALLSCSUB4(time_window,TIME_WINDOW,time_window,INT,PFLOAT,PFLOAT,STRING)
 */
static int ftwind( n, tarray, weight, window )
int n;
float *tarray, *weight;
char window[];
{
return time_window( n, tarray, weight, window );
}

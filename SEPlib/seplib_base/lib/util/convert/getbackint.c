/*<
getbackint

USAGE
 int getbackint (n1,floatin,intout)

INPUT PARAMETERS
 n1     - int     number of values to transofrm
 floatin- float*  float in

 OUTPUT PARAMETERS
 intout -int* float output


DESCRIPTION
cast a float to int

CATEGORY 
Lib:Convert

COMPILE LEVEL
DISTR
>*/
#include <sepconvert.h>
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int getbackint(int n1,int *floatin, int *intout)
_XFUNCPROTOEND
#else 
int getbackint(n1,floatin,intout)
int  n1;
int *floatin,*intout;
#endif
{
int i1;
    for(i1=0;i1<n1;i1++){
	*intout=*floatin;
	if(i1<(n1-1)) {floatin++;intout++;}
	}
return 0;
}







/*<
createfloat

USAGE
 int createfloat (n1,intin,floatout)

INPUT PARAMETERS
 n1   - int     number of values to transform
 float- int*  integer in 

 OUTPUT PARAMETERS
 floatout -float* float output


DESCRIPTION
cast an int to a float 

Completely unnecessary as all that is wanted is to "rename" an
int array containing floats. - Stew

CATEGORY
Lib:Convert

COMPILE LEVEL
DISTR
>*/
#include <sepconvert.h>
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int createfloat(int n1, float *intin, float *floatout)
_XFUNCPROTOEND
#else 
int createfloat(n1,intin,floatout)
int  n1;
float *intin;
float *floatout;
#endif
{
int i1;
    while(n1-- > 0) {
	*(floatout++) = *(intin++);
	}
return 0;
}

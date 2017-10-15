/*<
setvalf

USAGE
int setvalf(array,size,val)

INPUT PARAMETERS
array - float*  array to set value to
size  - int     size of array
val   - float   value to set array to

RETURN VALUES 
 0= if successful
-1=if fails for other reasons

DESCRIPTION

Sets an array to a given value

COMPILE LEVEL
DISTR

CATEGORY 
Lib:Vector

>*/
/*
SEE ALSO

AUTHOR

Robert G. Clapp
*/

#include<sepvector.h>
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int setvalf(float *array, int size, float val)
_XFUNCPROTOEND
#else 
int setvalf(array,size,val)
float *array,val;
int  size;
#endif
{
int i1;
    for(i1=0;i1<size;i1++){
	array[i1]=val;
	}
return 0;
}







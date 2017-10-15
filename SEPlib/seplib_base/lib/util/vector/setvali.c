/*<
setvali

usage
int setvali(value,number,setvali)

INPUT PARAMETERS
 value  -int*       value array
 number    -int       number of parameters to try to read
 setvali  -int       default value


DESCRIPTION
set value array of number elemets to setvali

CATEGORY
Lib:Vector

COMPILE LEVEL
DISTR
>*/
#include <sepvector.h>
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int setvali(int *value, int number,  int setvali)
_XFUNCPROTOEND
#else
int setvali(value,number,setvali)
int  number;
int *value;
int setvali;
#endif
{
int i1;

for(i1=0; i1< number; i1++) value[i1] = setvali;
return 0;
}







/*<
parcat


DESCRIPTION
Combines a string with an integer 

INPUT PARAMETERS
parameter - char*  in/out paramerter
number    - int    integer to catinate to parameter

COMMENTS
useful in loops for grabbing params 

CATEGORY
Lib:Aux

COMPILE LEVEL
DISTR

>*/
#include <sepaux.h>
#include <stdio.h>
#include <string.h>
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int parcat(char *parameter, int number,char *pout)
_XFUNCPROTOEND
#else
int parcat(parameter,number,pout)
char *parameter;
int  number;
#endif
{
char temp_ch[256];

sprintf(pout,"%s%d",parameter,number);

return 0;
}







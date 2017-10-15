/*<
compare_c

USAGE
int compare_c(string1,string2)

INPUT PARAMETERS
string1 - char* String 1
string2 - char* String 2

RETURN VALUES:
0 = successful
-1 = not successful
1 = not successful

DESCRIPTION

Compare strings for use in fortran, which does the wrong thing
sometime with Fortran90

CATEGORY
Lib:Aux

COMPILE LEVEL
DISTR

>*/
/*
SEE ALSO

AUTHOR

Robert G. Clapp

*/
#include <sepaux.h>
#include <string.h>

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int compare_c(char *in1, char *in2)
_XFUNCPROTOEND
#else 
int compare_c(in1,in2)
char *in1,*in2;
#endif
{
return strcmp(in1,in2);
}






/*  $Id: compare.c,v 1.1.1.1 2004/03/25 06:37:24 cvs Exp $ */

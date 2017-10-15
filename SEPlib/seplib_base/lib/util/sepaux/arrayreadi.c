/*<
arrayreadi

usage
int arrayreadi(tag,parameter,number,outputi,defaulti)

INPUT PARAMETERS
 tag      - char*     tag of file
 parameter -char*     base text of parameters (e.g. n)
 number    -int       number of parameters to try to read
 defaulti  -int       default value

OUTPUT PARAMETERS
 outputi   -int*      values found

DESCRIPTION
Read  a series of parameters from a tag can save significant code
when dealing with high dimensionality datasets

CATEGORY
Lib:Aux

COMPILE LEVEL
DISTR
>*/
#include<stdio.h>
#include <string.h>
#include <seplib.h>
#include <sepaux.h>
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int arrayreadi(char *tag, char *parameter, int number, int *outputi, int defaulti)
_XFUNCPROTOEND
#else
int arrayreadi(tag,parameter,number,outputi,defaulti)
char *parameter,*tag;
int  number;
int *outputi;
int defaulti;
#endif
{
int i1;
char temp_ch[256];
int ierr;
for(i1=1;i1<=number;i1++){
if(0!=parcat(parameter,i1,temp_ch)) seperr("arrayreadi: trouble running parcat \n");
ierr=fetch(temp_ch,"i",&outputi[i1-1]);
if(ierr==0) outputi[i1-1]=defaulti;
else{ 
	auxpar(temp_ch,"i",&outputi[i1-1],tag);
}
}
return 0;
}







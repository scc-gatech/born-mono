/*<
arrayreadf

usage
int arrayreadf(tag,parameter,number,outputf,defaultf)

INPUT PARAMETERS
 tag      - char*     tag of file
 parameter -char*     base text of parameters (e.g. n)
 number    -int       number of parameters to try to read
 defaultf  -float     default value

OUTPUT PARAMETERS
 outputf   -float*    values found

DESCRIPTION
Read  a series of parameters from a tag can save significant code
when dealing with high dimensionality datasets

CATEGORY
Lib:Aux
>*/
#include<stdio.h>
#include <string.h>
#include <seplib.h>
#include <sepaux.h>
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int arrayreadf(char *tag, char *parameter, int number, float *outputf, float defaultf)
_XFUNCPROTOEND
#else
int arrayreadf(tag,parameter,number,outputf,defaultf)
char *parameter,*tag;
int  number;
float *outputf;
float defaultf;
#endif
{
int i1;
char temp_ch[256];
for(i1=1;i1<=number;i1++){
if(0!=parcat(parameter,i1,temp_ch)) seperr("arrayreadf:trouble running parcat\n");
if(!fetch(temp_ch,"f",&outputf[i1-1])) 
	outputf[i1-1]=defaultf;
else {
	auxpar(temp_ch,"f",&outputf[i1-1],tag);
}
}
return 0;
}







/*<
reed_char_real

USAGE
 int reed_char_real (tag,n1, out)

INPUT PARAMETERS
 tag    - char*   tag to read in from
 n1     - int     number of values to transofrm

 OUTPUT PARAMETERS
 out -float* float output


DESCRIPTION
Read char data convert to real

CATEGORY 
Lib:Convert

COMPILE LEVEL
DISTR
>*/
#include <sepconvert.h>
extern int sreed(const char *, void *, int);
extern int srite(const char *, void *, int);
extern void seperr(const char *, ... );
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int reed_char_real(char *tag, int n1,float *out)
_XFUNCPROTOEND
#else 
int reed_char_real(tag,n1,out)
char *tag;
int  n1;
float *out;
#endif
{
int i1;
unsigned char *input;

input = (unsigned char*) malloc(sizeof(char)*n1);
if(n1!=sreed("in",input,n1)) seperr("trouble reading in data \n");
for(i1=0;i1<n1;i1++) out[i1]=(float )input[i1];

free(input);
return (n1);
}






/*<
rite_real_char

USAGE
 int rite_real_char (tag,n1, in)

INPUT PARAMETERS
 tag    - char*   tag to read in from
 n1     - int     number of values to transofrm
 in     -float* float input

 OUTPUT PARAMETERS


DESCRIPTION
Write char data after converting to real

CATEGORY 
Lib:Convert

COMPILE LEVEL
DISTR
>*/
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int rite_real_char(char *tag, int n1,float *in)
_XFUNCPROTOEND
#else 
int rite_real_char(tag,n1,in)
char *tag;
int  n1;
float *in;
#endif
{
int i1;
unsigned char *out;

out = (unsigned char*) malloc(sizeof(char)*n1);
for(i1=0;i1<n1;i1++) {
	if(in[i1]<0) out[i1]=(unsigned char)0;
	else if(in[i1]>255) out[i1]=(unsigned char)255;
	else out[i1]=(unsigned char)in[i1];
}

if(n1!=srite("out",out,n1)) seperr("trouble writing out data \n");
free(out);
return (n1);
}








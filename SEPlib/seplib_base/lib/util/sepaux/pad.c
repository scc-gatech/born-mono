/*<

pad

Usage:
ierr=pad( arg1, arg2, arg3 )

Return Values




Input Parameters:


Output Parameters:



Description:



>*/ 
/*
-------------------------------------------------

Author: Robert Clapp, ESMB 463, 7230253

Date Created:Fri Sep 22 15:48:56 PDT 2000

Purpose: 

*/	 

#include<string.h>
#include <sepaux.h> 
#ifndef YES
#define YES 1
#endif

#ifndef NO
#define NO 0
#endif
#include <seplib.h>

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int create_trace_ptrs(int extend,int ndim,int *beg,int *nin,int *end, int iout,int n_out, int *ptrs);
int dumb_it(char*,int*, int*,int);
_XFUNCPROTOEND
#else
int create_trace_ptrs();
int dumb_it();
#endif
/*
=head1 NAME

pad_it - Pad an array

=head1 SYNOPSIS

pad_it(input,output,ndim,n,beg,end.extend)

=head1 INPUT PARAMETER

=over 4

=item  input - void*

       Input dataset

=item  ndim - int

       Number of dimensions

=item  n - int[ndim]

       Axes lengths

=item  beg - int[ndim]

       Amount to pad at the begining of each axis

=item  end - int[ndim]

       Amount to pad at the end of each axis

=item  extend- int

       Whether or not to fill with nearest point[1] or zeros [0]

=back

=head1 OUTPUT PARAMETER

=over 4

=item  output - void*

      Output array

=back

=head1 DESCRIPTION

Pads an array. Should work with any dimension.

=head1 LIBRARY

B<sepaux>

=cut
*/

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int  pad_it(char *input, char *output, int ndim, int *n, int *beg, int *end,int esize,int extend)
_XFUNCPROTOEND
#else
int  pad_it(input,output,dim,n,beg,end,esize,extend)
char *input, *output; 
int ndim, *n, *beg, *end,esize,extend_it;
#endif
{
int nhin,nhout,i,zero;

nhin=1; nhout=1;
for(i=0; i < ndim; i++){
	nhin=nhin*n[i];
	nhout=nhout*(n[i]+beg[i]+end[i]);
}
zero=0;

return(pad_portion(input,output,esize,extend,ndim,n,beg,end,0,nhout,&zero,&nhin,dumb_it));



}
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int  dumb_it(char *a,int *b, int *c,int d)
_XFUNCPROTOBEGIN
#else 
int  dumb_it(a,b,c,d)
char *a;
int *b, *c,d;
#endif
{
seperr("internal error:invalid trace pointer \n");
return(1);
}

/*<
pad_portion - internal program for padding a portion of a dataset

usage
int pad_portion(input,output,esize,extend,ndim,nin,beg,end,iout,nout,ib_in,ie_in,grab_input)

INPUT PARAMETERS
 input  -char*     Input

 esize - int      Data esize

  extend -int     1-pad fill with nearest location, 0 - file with zero

  ndim  - int     Number of dimensions in the dataset

  nin   - int*    Dataset dimensions input

  beg   - int*    Amount to pad input

  end   - int*    Amount to pad output

  iout  - int     Current location in output space

  nout  - int     Number of output locations we are doing on this pass

  ib_in - int*    First input location in memory    

  ie_in - int*    Last  output location in memory 

	grab_input -int(char*,int*,int*,int) Function that will  grab next input section
   

OUTPUT PARAMETERS
 output   -char*    Padded output

DESCRIPTION
Pad a portion of a dataset (used by pad_it) and the SEPlib program Pad

CATEGORY
Lib:Aux
>*/



#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int pad_portion(char *input, char *output,int esize,int extend,int ndim,int *nin,int *beg,int *end,int iout,int nout,int *ib_in,int *ie_in,int grab_input(char *,int*,int*,int))
_XFUNCPROTOEND
#else
int pad_portion(input,output,esize,extend,ndim,nin,beg,end,iout,nout,ib_in,ie_in,grab_input)
char *input,*output;
int esize,extend,ndim,*nin,*beg,*end,iout,nout,*ib_in,*ie_in;
int grab_input();
#endif 
{ 
int *ptrs;
char *zeros;
int i,nbig;
int i2out,j,n1in,nlast;



ptrs=(int*)alloc(sizeof(int)*nout);
nbig=esize*(nin[0]+beg[0]+end[0]);
n1in=esize*nin[0];
nlast=esize*(nin[0]-1);


if(extend==NO){ /*make an array of 0s of padding with zeros */
	zeros=(char*)alloc(nbig);
	for(i=0;i < nbig;i++) zeros[i]=(char)0;
}

if(0!=create_trace_ptrs(extend,ndim,beg,nin,end,iout,nout,ptrs))
	seperr("trouble creating trace pts \n");
i2out=0;
for(i=0; i < nout; i++){
	if(ptrs[i]==-1){
		  memcpy((void *)(output+i2out),(const void*)zeros,nbig);
			i2out+=nbig;
	}
	else{
		if(ptrs[i]> *ie_in || ptrs[i] < *ib_in ){
			if(0!=grab_input(input,ib_in,ie_in,ptrs[i])) seperr("trouble reading input");
		}

		/*take care of the begining if not 0 */

		if(beg[0]>0){
			if(extend==YES){
				for(j=0;j<beg[0];j++){
			  	memcpy((void *)(output+i2out),
            (const void*)(input+n1in*(ptrs[i]-*ib_in)),esize);
					i2out+=esize;
				}
			}
			else{
		  	memcpy((void *)(output+i2out),(const void*)zeros,beg[0]*esize);
				i2out+=beg[0]*esize;
			}
		}

		/*fill in the middle */

  	memcpy((void *)(output+i2out),
       (const void*)(input+n1in*(ptrs[i]-*ib_in)),n1in);
		
		i2out+=n1in;

			
		/*take care of the end if not 0 */

		if(end[0]>0){
			if(extend==YES){
				for(j=0;j<end[0];j++){
			  	memcpy((void *)(output+i2out),
            (const void*)(input+nlast+n1in*(ptrs[i]-*ib_in)),esize);
					i2out+=esize;
				}
			}
			else{
		  	memcpy((void *)(output+i2out),(const void*)zeros,end[0]*esize);
				i2out+=end[0]*esize;
			}
		}
	}
}
		

if(extend==NO) free(zeros);
free(ptrs);

return(0);
} 

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int create_trace_ptrs(int extend,int ndim,int *beg,int *nin,int *end, int iout,int n_out, int *ptrs)
_XFUNCPROTOEND
#else
int create_trace_ptrs(extend,ndim,beg,nin,end,iout,n_out,ptrs)
int extend,ndim,*beg,*nin,*end,iout,nout,*ptrs;
#endif
{
int i,zero,j;
int *ibeg,*iend,*nout,*ic;
int ndi,k;

ndi=ndim-1;

ibeg=(int*)alloc(sizeof(int)*ndi);
iend=(int*)alloc(sizeof(int)*ndi);
nout=(int*)alloc(sizeof(int)*ndi);
ic=(int*)alloc(sizeof(int)*ndi);

for(i=1; i < ndim; i++){k=i-1;
	ibeg[k]=beg[i];
	iend[k]=ibeg[k]+nin[i];
	nout[k]=iend[k]+end[i];
}


for(i=0;i < n_out; i++){

	/*convert point to coords */
	h2c(i+iout,nout,ndi,ic);
	
	/*check to see if its input */
	zero=0;
	for(j=0; j < ndi; j++){
		if(ic[j] < 0) return(-1);
		else if(ic[j]<ibeg[j]){
			if(extend==YES) ic[j]=ibeg[j];
			else zero=1;
		}
		else if(ic[j]>=nout[j]) return(1);
		else if(ic[j]>=iend[j]){
			if(extend==YES) ic[j]=iend[j]-1;
			else zero=1;
		}
		ic[j]-=ibeg[j];
	}
	if(zero==1) ptrs[i]=-1;
	else c2h(&ptrs[i],&nin[1],ndi,ic);

}

free(ibeg);free(iend);free(nout);free(ic);

return(0);
}


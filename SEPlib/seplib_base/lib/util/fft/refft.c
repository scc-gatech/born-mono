/*$
=head1 NAME

refft - real fft


=head1 SYNOPSIS

 void refft (x,lx,isign,scale,mode)

=head1 INPUT PARAMETERS

=over 4

=item    x   -complex*     

         input/output array

=item    lx  -int          

         length of real array; must be a power of 2

=item    isign-int         

         sign of sqrt(-1)

=item    scale-float       

         scale factor; sqrt(2./lx) conserves energy

=item    mode -int         

         1 for lx real to lx/2 complex
         2 for lx real to lx/2+1 complex
         -1 for lx/2 complex to lx real
         -2 for lx/2+1 complex to lx real

=back

=head1 DESCRIPTION


     Real fast Fourier transform along one trace.  
     C language subroutine for a one-dimensional real to complex or
     viceversa fast Fourier transform. refft performs transforms
     between elements of a vector. The arguments are as follows:


=head1 SEE ALSO

L<refft>,L<rvfft>,L<cvfft>

=head1 LIBRARY

B<sepfft>

=cut




$*/

/* Four fft subroutines based on the algorithm used in fork (FGDP, p. 12) 
 * and modeled after Clayton's and Ottolini's subroutines
 *********************************************************************
 *								     *
 *		       lx-1					     *
 *	x(k) = scale * sum  [x(j)*exp(isign*2*pi*sqrt(-1)*j*k/lx]    *
 *		       j=0					     *
 *								     *
 *********************************************************************
 *
 *		refft (x,lx,isign,scale,mode)
 *		cefft (x,lx,isign,scale)
 *		rvfft (x,lx,nx,isign,scale,mode)
 *		cvfft (x,lx,nx,isign,scale)
 *
 * See each subroutine below for a description of the arguments.
 * Dave Hale, 3/17/82
 */
#include <sepfft.h>
/*
 * 	refft	lx real to lx/2 (or lx/2+1) complex or vice-versa
 *
 *	x	input/output array
 *	lx	length of real array; must be a power of 2
 *	isign	sign of sqrt(-1)
 *	scale	scale factor; sqrt(2./lx) conserves energy
 *	mode	 1 for lx real to lx/2 complex
 *		 2 for lx real to lx/2+1 complex
 *		-1 for lx/2 complex to lx real
 *		-2 for lx/2+1 complex to lx real
 */
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int refft(register float complex *x,int lx,int isign,float scale,int mode)
_XFUNCPROTOEND
#else
int refft(x,lx,isign,scale,mode)
register float complex *x;
int lx,isign,mode; float scale;
#endif
   {
	register float complex *xp,*xn;
	float real,imag,xsumre,xsumim,xdifre,xdifim;
	double aa,cn,sn,cd,sd,arg;

	if(mode > 0) 				/* real to complex */
	  {
		cefft(x,lx/2,isign,scale);
		real = __real__(*x)+__imag__(*x); imag = __real__(*x)-__imag__(*x);
		__real__(*x) = real;
		if (mode == 1) 			/* pack Nyquist */
			__imag__(*x) = imag;
		else				/* unpack Nyquist */
		  {
			xn = x+lx/2;
			__real__(*xn) = imag;
			__imag__(*xn) = 0.; 
			__imag__(*x) = 0.;
		  }
	  }
	cn = 1.; sn = 0.;			/* initial cosine and sine */
	arg = pi/lx;				/* = pi/lx */
	if (isign < 0) arg = -arg;
	aa = sin(arg);
	cd = 2.*aa*aa; sd = sin(arg+arg);	/* for cosine/sine recursion */
	for (xp = x+1, xn = x+lx/2-1; xp <= xn; xp++, xn--)
	  {
		aa = cd*cn+sd*sn;
		sn += sd*cn-cd*sn;		/* sin(2*arg*(xp-x)) */
		cn -= aa;			/* cos(2*arg*(xp-x)) */
		xsumre = 0.5*(__real__(*xp)+__real__(*xn));
		xsumim = 0.5*(__imag__(*xp)-__imag__(*xn));
		xdifre	= 0.5*(__real__(*xp)-__real__(*xn));
		xdifim = 0.5*(__imag__(*xp)+__imag__(*xn));
		real = sn*xdifre+cn*xdifim;
		imag = sn*xdifim-cn*xdifre;
		__real__(*xp) = xsumre+real;
		__imag__(*xp) = imag+xsumim;
		__real__(*xn) = xsumre-real;
		__imag__(*xn) = imag-xsumim;
	  }
	if(mode < 0) 				/* complex to real */
	  {
		if (mode == -2)			/* Nyquist not packed, */
		  {				/* so pack it */
			xn = x+lx/2;
			__imag__(*x) = __real__(*xn);
			__real__(*xn) = 0.;
		  }
		real = 0.5*(__real__(*x)+__imag__(*x));
		__imag__(*x) = 0.5*(__imag__(*x)-__real__(*x));
		__real__(*x) = real;
		cefft(x,lx/2,isign,scale);
		for(xp=x,xn=x+lx/2; xp<xn; xp++)
			__imag__(*xp) = -__imag__(*xp);
	   }
       return(0);
   }

/*$

=head1 NAME

cefft - complex FFT

=head1 SYNOPSIS

 void cefft (x,lx,isign,scale)

=head1  INPUT PARAMETERS

=over 4

=item  x     - complex*  

       input/output array

=item  lx    - int       

       length of complex array; must be a power of 2

=item  isign - int       

       sign of sqrt(-1)

=item  scale - double    

       scale factor; sqrt(1./lx) conserves energy

=back

=head1 DESCRIPTION

 Complex fast Fourier transform along one trace.  


=head1  COMMENTS

     C language subroutine for a one-dimensional complex fast
     Fourier transform. refft performs transforms between
     elements of a vector The arguments are as follows:

=head1 SEE ALSO

L<refft>,L<rvfft>,L<cvfft>

=head1 LIBRARY

B<sepfft>

=cut

>*/
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
 * Revised: 12/7/87  Stewart A. Levin  
 *   upgraded to use convex veclib V2.0 routines
 */
#include <sepfft.h>
/* 	cefft	lx complex to lx complex
 *
 *	x	input/output array
 *	lx	length of complex array; must be a power of 2
 *	isign	sign of sqrt(-1)
 *	scale	scale factor; sqrt(1./lx) conserves energy
*/
#if defined CONVEX || defined __convex__
static int oldlx = 0;
static float *workbuf = ((float *) NULL);
cefft (x,lx,isign,scale)
float complex *x;
int lx,isign; double scale;
   {
    int log2lx, ix, ierr;
    static int ione = 1;
    float temp = scale;
    unsigned size;
    extern char *alloc(size_t);

/* allocate new work array if needed */
    if(oldlx != lx) {
	log2lx=0;
	for(ix = 1; ix < lx; ix *= 2) log2lx++;
	if(workbuf != ((float *) NULL)) {
	    free(workbuf);
	}
	size = (log2lx-1) * (lx>>1);
	workbuf = (float *) alloc(size*sizeof(float));
	ix = -3;
	c1dfft_(x,&lx,workbuf,&ix,&ierr);
	if(ierr!=0) erexit("bad c1dfft initialization. ierr=%d\n",ierr);
	oldlx = lx;
    }
/* translate fft sign into c1dfft equivalent */
    if(isign<0) {
	ix = 1;
    } else {
	ix = -2;
    }

/* do fft */
    c1dfft_(x,&lx,workbuf,&ix,&ierr);
    if(ierr!=0) erexit("bad c1dfft calculation. ierr=%d\n",ierr);

/* apply scale factor */
    ix = lx<<1;
    if(scale != 1.0) sscal_(&ix,&temp,x,&ione);
   }
#else
static double sintab[]
	   ={
		1.000000000000000e+00, /* sin(pi/2) */
		7.071067811865475e-01,
		3.826834323650897e-01,
		1.950903220161282e-01,
		9.801714032956060e-02,
		4.906767432741801e-02,
		2.454122852291228e-02,
		1.227153828571992e-02,
		6.135884649154475e-03,
		3.067956762965976e-03,
		1.533980186284765e-03,
		7.669903187427045e-04,
		3.834951875713956e-04,
		1.917475973107033e-04,
		9.587379909597734e-05,
		4.793689960306688e-05,
		2.396844980841821e-05,
		1.198422490506970e-05,
		5.992112452642428e-06,
		2.996056226334660e-06,
		1.498028113169011e-06	/* sin(pi/(2**21)) */
	   };

/* 	cefft	lx complex to lx complex
 *
 *	x	input/output array
 *	lx	length of complex array; must be a power of 2
 *	isign	sign of sqrt(-1)
 *	scale	scale factor; sqrt(1./lx) conserves energy
*/
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int cefft (float complex *x,int lx,int isign,float scale)
_XFUNCPROTOEND
#else
int cefft (x,lx,isign,scale)
float complex *x;
int lx,isign; float scale;
#endif
   {
	register float complex *px,*qx;
	float complex *xplx;
	int m,j,k,step;
	float temp,real,imag;
	double cn,sn,cd,sd,dtemp,*psintab;

	/* bit reverse */
	xplx = x+lx;
	for(px=x, j=0; px<xplx; px++, j+=m)
	   {
		if(px < (qx=x+j))
		  {
			temp = __real__(*qx); __real__(*qx) = __real__(*px); __real__(*px) = temp;
			temp = __imag__(*qx); __imag__(*qx) = __imag__(*px); __imag__(*px) = temp;
		  }
		for (m=lx>>1; m>=1 && j>=m; j-=m, m>>=1);
	   }
	/* first butterfly and scaling */
	for(px=x, qx=x+1; px<xplx; px+=2, qx+=2)
	   {
		if (scale != 1.)
		  {
			__real__(*px) *= scale; __imag__(*px) *= scale;
			__real__(*qx) *= scale; __imag__(*qx) *= scale;
		  }
		temp = __real__(*qx); __real__(*qx) = __real__(*px)-temp; __real__(*px) += temp;
		temp = __imag__(*qx); __imag__(*qx) = __imag__(*px)-temp; __imag__(*px) += temp;
	   }
	/* remaining butterflies */
	for (j=2, psintab=sintab; j<lx; j=step)
	   {
		step = j<<1;
		sd = *psintab++;
		if (isign < 0) sd = -sd;
		dtemp = *psintab;
		cd = 2.*dtemp*dtemp;
		sn = 0.;
		cn = 1.;
		for(k=0; k<j; k++)
		   {
			for(px=x+k; px<xplx; px+=step)
			   {
				qx = px+j;
				real = cn*__real__(*qx)-sn*__imag__(*qx);
				imag = sn*__real__(*qx)+cn*__imag__(*qx);
				__real__(*qx) = __real__(*px)-real;
				__imag__(*qx) = __imag__(*px)-imag;
				__real__(*px) += real;
				__imag__(*px) += imag;
			   }
			dtemp = cd*cn+sd*sn;
			sn += sd*cn-cd*sn;
			cn -= dtemp;
		   }
	   }
		return 0;
   }
#endif

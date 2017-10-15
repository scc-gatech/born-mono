
 /*$

=head1 NAME

rvfft -real vector fft

=head1 SYNOPSIS

     void rvfft (x,lx,nx,isign,scale,mode)

=head1 PARAMETERS

=over 4


=item  x      -complex   

       input/output array

=item  lx     -int       

       length of real vectors

=item  nx     -int       

       number of vectors; must be a power of 2

=item  isign  -int       

       sign of sqrt(-1)

=item  scale  -double    

       scale factor; sqrt(2./lx) conserves energy

=item  mode   -int       

       1 for nx real to nx/2 complex
       2 for nx real to nx/2+1 complex
       -1 for nx/2 complex to nx real
       -2 for nx/2+1 complex to nx real

=back

=head1 DESCRIPTION

     Real fast Fourier transform between vectors of an array.  
     Rvfft takes a 2D panel and does a real to complex or 
     viceversa fast fourier transform across the second dimension, 
     of all the time samples. The arguments are as follows:

=head1 SEE ALSO

L<refft>,L<rvfft>,L<cvfft>

=head1 LIBRARY

B<sepfft>

=cut
>*/
/* Four fft subroutines based on the algorithm used in fork (FGDP, p. 12) 
 * and modeled after Clayton's and Ottolini's subroutines
 *********************************************************************
 *                     *
 *           lx-1               *
 *  x(k) = scale * sum  [x(j)*exp(isign*2*pi*sqrt(-1)*j*k/lx]    *
 *           j=0               *
 *                     *
 *********************************************************************
 *
 *    refft (x,lx,isign,scale,mode)
 *    cefft (x,lx,isign,scale)
 *    rvfft (x,lx,nx,isign,scale,mode)
 *    cvfft (x,lx,nx,isign,scale)
 *
 * See each subroutine below for a description of the arguments.
 * Dave Hale, 3/17/82
 */
#include <sepfft.h>

/*   rvfft  nx real vectors to nx/2 (or nx/2+1) complex vectors in core
 *
 *  x  input/output array
 *  lx  length of real vectors
 *  nx  number of vectors; must be a power of 2
 *  isign  sign of sqrt(-1)
 *  scale  scale factor; sqrt(2./nx) conserves energy
 *  mode   1 for nx real to nx/2 complex
 *     2 for nx real to nx/2+1 complex
 *    -1 for nx/2 complex to nx real
 *    -2 for nx/2+1 complex to nx real
 */
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int rvfft (register float complex *x,int lx,int nx,int isign,float scale,int mode)
_XFUNCPROTOEND
#else
int rvfft (x,lx,nx,isign,scale,mode)
register float complex *x;
int lx,nx,isign,mode; float scale;
#endif
  {
  register float complex *xp,*xn,*xpplx;
  int i,j,k,ix,lx2m1;
  float real,imag,xsumre,xsumim,xdifre,xdifim,*xf,*xfplx;
  double aa,cn,sn,cd,sd,arg;

  if(mode > 0)         /* real to complex */
    {
    for (ix=0; ix<nx; ix+=2)  /* weave (interleave) adjacent */
      {        /* real vectors */
      for (i=1, lx2m1=lx*2-1, xf=(float*)x+ix*lx; i<lx2m1; i++)
        {
        for (k=i>>1, j=k+lx*(i-(k<<1)); j<i; 
          k=j>>1, j=k+lx*(j-(k<<1)));
        real = xf[i]; xf[i] = xf[j]; xf[j] = real;
        }
      } 
    cvfft (x,lx,nx/2,isign,scale);
    for (xp=x, xpplx=xp+lx, xn=x+nx/2*lx; xp<xpplx; xp++, xn++)
      {
      real = __real__(*xp)+__imag__(*xp); imag = __real__(*xp)-__imag__(*xp);
      __real__(*xp) = real;
      if (mode == 1)    /* pack Nyquist */
        __imag__(*xp) = imag;
      else      /* unpack Nyquist */
        {
        __real__(*xn) = imag;
        __imag__(*xn) = __imag__(*xp) = 0.;
        }
      }
    }
  cn = 1.; sn = 0.;
  arg = pi/nx;
  if (isign < 0) arg = -arg;
  aa = sin(arg);
  cd = 2.*aa*aa; sd = sin(arg+arg);
  for (ix=1, xp=x+lx; ix<=nx/4; ix++)
      {
    aa = cd*cn+sd*sn;
    sn += sd*cn-cd*sn;
    cn -= aa;
    for (xn=x+(nx/2-ix)*lx, xpplx=xp+lx; xp<xpplx; xp++, xn++)
      {
      xsumre = 0.5*(__real__(*xp)+__real__(*xn));
      xsumim = 0.5*(__imag__(*xp)-__imag__(*xn));
      xdifre  = 0.5*(__real__(*xp)-__real__(*xn));
      xdifim = 0.5*(__imag__(*xp)+__imag__(*xn));
      real = sn*xdifre+cn*xdifim;
      imag = sn*xdifim-cn*xdifre;
      __real__(*xp) = xsumre+real;
      __imag__(*xp) = imag+xsumim;
      __real__(*xn) = xsumre-real;
      __imag__(*xn) = imag-xsumim;
        }
    }
  if(mode < 0)         /* complex to real */
    {
    if (mode == -2)      /* Nyquist not packed, */
      {        /* so pack it */
      for (xp=x, xn=x+nx/2*lx, xpplx=xp+lx; xp<xpplx; xp++, xn++)
        {
        __imag__(*xp) = __real__(*xn);  
        __real__(*xn) = 0.;
        }
      }
    for (xp=x, xpplx=xp+lx; xp<xpplx; xp++)
      {
      real = 0.5*(__real__(*xp)+__imag__(*xp));
      __imag__(*xp) = 0.5*(__imag__(*xp)-__real__(*xp));
      __real__(*xp) = real;
      }
    cvfft (x,lx,nx/2,isign,scale);
    for (ix=0; ix<nx; ix+=2)  /* unweave adjacent */
      {        /* real vectors */
      xf=(float*)x+ix*lx;
      for (i=1, lx2m1=lx*2-1; i<lx2m1; i++)
        {
        for (k=i/lx, j=k+((i-k*lx)<<1); j<i; 
          k=j/lx, j=k+((j-k*lx)<<1));
        real = xf[i]; xf[i] = xf[j]; xf[j] = real;
        }
      for (xf+=lx, xfplx=xf+lx; xf<xfplx; xf++)
        *xf = -*xf;
      } 
     }
     return(0);
   }

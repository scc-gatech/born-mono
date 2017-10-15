/* Supplying compatibility with the C99 standard */
/*
Copyright (C) 2004 University of Texas at Austin

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __cplusplus
/*^*/

#include <math.h>
/*^*/

#include "sergey_rsf.h"

#ifndef _sf_c99_h

#if !defined(__CYGWIN__) && defined(__STDC__) && (__STDC_VERSION__ >= 199901L)
/*^*/

/* The following from C99 - must define for C90 */
#include <complex.h>
#undef I
#ifdef sun
#define I (1.0fi)
#else
#define I _Complex_I
#endif
/*^*/

#else
/*^*/

#define complex  
#define I 0.0
#define csqrtf  sqrtf
#define clogf   logf
#define clog    log
#define cexpf   expf
#define cexp    exp
#define cpowf   powf
#define cabsf   fabsf
#define cabs    fabs
#define ccosf   cosf
#define csinf   sinf
#define ctanf   tanf
#define cacosf  acosf
#define casinf  asinf
#define catanf  atanf
#define ccoshf  coshf
#define csinhf  sinhf
#define ctanhf  tanhf
#define cacoshf acoshf
#define casinhf asinhf
#define catanhf atanhf
#define crealf  sf_crealf
#define cimagf  sf_cimagf
#define conjf   sf_conjf
#define cargf   sf_cargf
#define creal   sf_creal
#define cimag   sf_cimag
#define conj    sf_conj
/*^*/

#endif
/*^*/

#endif

void cprint (float complex c)
/*< print a complex number (for debugging purposes) >*/
{
    sf_warning("%g+%gi",crealf(c),cimagf(c));
}

#if defined(__CYGWIN__) || !defined (__STDC__) || (__STDC_VERSION__ < 199901L)
/*^*/

float sf_crealf(/*@unused@*/ float complex c) 
/*< real part of a complex number >*/
{ 
    sf_warning("No support for complex types!!!\n"
	       "Please use a C99-compliant compiler");
    return c; 
}

float sf_cimagf(/*@unused@*/ float complex c) 
/*< imaginary part of a complex number >*/
{ 
    sf_warning("No support for complex types!!!\n"
	       "Please use a C99-compliant compiler");
    return 0.; 
} 

double sf_creal(/*@unused@*/ double complex c) 
/*< real part of a complex number >*/
{ 
    sf_warning("No support for complex types!!!\n"
	       "Please use a C99-compliant compiler");
    return c; 
}

double sf_cimag(/*@unused@*/ double complex c) 
/*< imaginary part of a complex number >*/
{    
    sf_warning("No support for complex types!!!\n"
	       "Please use a C99-compliant compiler");
    return 0.; 
} 


float sf_cargf(/*@unused@*/ float complex c)
/*< phase of a complex number >*/ 
{ 
    sf_warning("No support for complex types!!!\n"
	       "Please use a C99-compliant compiler");
    return c; 
}

float complex sf_conjf(/*@unused@*/ float complex c)
/*< conjugate of a complex number >*/ 
{ 
    sf_warning("No support for complex types!!!\n"
	       "Please use a C99-compliant compiler");
    return c; 
}

double complex sf_conj(/*@unused@*/ double complex c) 
/*< conjugate of a complex number >*/ 
{ 
    sf_warning("No support for complex types!!!\n"
	       "Please use a C99-compliant compiler");
    return c; 
}

float sqrtf(float x) 
/*< float sqrt >*/
{ return (float) sqrt(x);}

float logf(float x)  
/*< float log >*/
{ return (float) log(x);}

float log10f(float x) 
/*< float log10 >*/
{ return (float) log10(x);}

float expf(float x) 
/*< float exp >*/
{ return (float) exp(x);}

float fabsf(float x) 
/*< float fabs >*/
{ return (float) fabs(x);}

float floorf(float x)
/*< float floor >*/
{ return (float) floor(x);}

float ceilf(float x) 
/*< float ceil >*/
{ return (float) ceil(x);}

float fmodf(float x, float y) 
/*< float fmod >*/
{ return (float) fmod(x,y);}

float cosf(float x) 
/*< float cos >*/
{ return (float) cos(x);}

float sinf(float x) 
/*< float sin >*/
{ return (float) sin(x);}

float tanf(float x) 
/*< float tan >*/
{ return (float) tan(x);}

float acosf(float x) 
/*< float acos >*/
{ return (float) acos(x);}

float asinf(float x) 
/*< float asin >*/
{ return (float) asin(x);}

float atanf(float x) 
/*< float atan >*/
{ return (float) atan(x);}

float atan2f(float x, float y) 
/*< float atan2 >*/
{ return (float) atan2(x,y);}

float coshf(float x) 
/*< float cosh >*/
{ return (float) cosh(x);}

float sinhf(float x) 
/*< float sinh >*/
{ return (float) sinh(x);}

float tanhf(float x) 
/*< float tanh >*/
{ return (float) tanh(x);}

float acoshf(float x) 
/*< float acosh >*/
{ return (float) acosh(x);}

float asinhf(float x) 
/*< float asinh >*/
{ return (float) asinh(x);}

float atanhf(float x) 
/*< float atanh >*/
{ return (float) atanh(x);}

float powf(float x, float y) 
/*< float pow >*/
{ return (float) pow(x,y);}

float hypotf(float x, float y) 
/*< float hypot >*/
{ return (float) hypot(x,y);}

#endif
/*^*/

#endif /* c++ */
/*^*/

/* 	$Id: c99.c 1120 2005-04-15 13:08:40Z fomels $	 */


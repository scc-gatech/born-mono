/*$

=head1 NAME

sclog - complex log


=head1 SYNOPSIS

C<val=complex sclog(a)>

=head1 INPUT PARAMETERS

=over 4

=item a - complex 

     number

=back

=head1 RETURN VALUES

=over 4

=item val -  complex 

      result of complex log

=back


=head1 DESCRIPTION

     Clog returns the principal value of log(a) for an arbitrary
     complex a.  An equivalent formula is
                cmplx(log(cabs(a)),atan2(a.im,a.re)).
     The imaginary part is between -J and J.

=head1  DIAGNOSTICS

     Clog(0) returns the machine equivalent of minus infinity.


=head1 LIBRARY

B<sepmath>

=cut



>*/
#include<sitedef.h>
#include <sepmath.h>
float scabs(complex);

#ifdef _POSIX_SOURCE
#include <float.h>
#endif

#if defined (HAVE_LIMITS_H)
#include<limits.h>
#endif

#ifndef HUGE /* define only if not set elsewhere */
#ifdef FLT_MAX
#define HUGE FLT_MAX
#else
#define HUGE 1e+37
#endif
#endif



#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
complex sclog(complex x)
_XFUNCPROTOEND
#endif

	{
	complex i;
	float f;

	if ((f=scabs2(x)) == 0.0)
		{
		i.re = -HUGE;
		i.im = 0.0;
		}
	     else
		{
		i.re = 0.5*log((double)f);
		i.im = atan2((double)x.im,(double)x.re);
		}
	return (i);
	}

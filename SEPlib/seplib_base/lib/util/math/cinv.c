/*$

=head1 NAME

scinv - Inverse of cmplex number

=head1 SYNOPSIS

C<val=complex scinv(a)>;

=head1 INPUT PARAMETERS

=over 4

=item a - complex  
      
      number

=back

=head1 RETURN VALUES

=over 4

=item val - complex 

      reciprocal

=back
 
=head1  DESCRIPTION

Complex reciprocal
 Cinv returns 1/a.



=head1 LIBRARY

B<sepmath>

=cut


>*/
#include <sepmath.h>
#ifndef HUGE /* define only if not set elsewhere */
#ifdef FLT_MAX
#define HUGE FLT_MAX
#else
#ifdef HUGE_VAL
#define HUGE HUGE_VAL
#endif /* HUGE_VAL */
#endif /* FLT_MAX */
#endif /* HUGE */

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
complex scinv(complex x)
_XFUNCPROTOEND
#endif

	{
	complex i;
	float tmpden;
	tmpden = x.re*x.re + x.im*x.im;
	if (tmpden != 0.0)
		tmpden = 1./tmpden;
	     else
		tmpden = HUGE;
	i.re = x.re * tmpden;
	i.im = - x.im * tmpden;
	return (i);
	}

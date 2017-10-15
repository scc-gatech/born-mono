/*$

=head1  NAME

scsqrt  - Complex square root


=head1 SYNOPSIS

C<val= scsqrt(a)>

=head1  INPUT PARAMETERS

=over 4

=item  a - complex 

       input number

=back

=head1  RETURN VALUES

=over 4

=item val - complex 

      result of sqrt

=back

=head1 DESCRIPTION

    Complex square root
     Csqrt returns sqrt(a) for an arbitrary complex a.  An
     equivalent formula is cspow(a,0.5).


=head1 LIBRARY

B<sepmath>

=cut




>*/
#include <sepmath.h>
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
complex scsqrt(complex x)
#endif

	{
	double tabs, texp;

	tabs = hypot((double)x.re,(double)x.im);
	texp = 0.5*atan2((double)x.im,(double)x.re);
	return ( scmplx((float)(tabs*cos(texp)),(float)(tabs*sin(texp))) );
	}

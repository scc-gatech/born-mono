/*$

=head1 NAME

scexp - complex exponent


=head1 SYNOPSIS

C<val= complex scexp(a)>

=head1 INPUT PARAMETER

=over 4

=item a  - complex  

      number to find the exp of

=back

=head1 RETURN VALUES

=over 4

=item val- complex 

      exp(a)

=back

=head1  DESCRIPTION

     scexp returns exp(a) for an arbitrary complex a.  An
     equivalent formula is csmul(ciexp(a.im),exp(a.re)).

=head1 SEE ALSO

L<csmult>,L<ciexp>


=head1 LIBRARY

B<sepmath>

=cut



>*/
#include <sepmath.h>
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
complex scexp(complex x)
_XFUNCPROTOEND
#endif

	{
	complex i;
	double amp;

	amp = exp((double)(x.re));
	i.re = amp * cos((double)(x.im));
	i.im = amp * sin((double)(x.im));
	return (i);
	}

/*$

=head1 NAME

sciexp - exponent of imaginary number


=head1 SYNOPSIS

C<val=complex sciexp(f)>

=head1 INPUT PARAMETERS

=over 4

=item f - float 

      imagionary number

=back

=head1 RETURN VALUE

=over 4

=item val - complex

      exp(f) of imaginary number

=back


=head1 DESCRIPTION

Ciexp returns exp(i*f) = (cos f, sin f). (For pure imaginary number)


=head1  SEE ALSO

L<cexp>


=head1 LIBRARY

B<sepmath>

=cut



>*/
#include <sepmath.h>
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
complex sciexp(float x)
_XFUNCPROTOEND
#endif
	{
	complex i;
	i.re = cos((double)(x));
	i.im = sin((double)(x));
	return (i);
	}

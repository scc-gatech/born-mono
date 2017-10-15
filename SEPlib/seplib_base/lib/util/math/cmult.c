/*$
=head1 NAME

scmult - complex multiplication

=head1 SYNOPSIS

C<val=complex scmult(a, b)>

=head1 INPUT PARAMETERS

=over 4

=item a,b - complex 

      numbers to multiply

=back

=head1 RETURN VALUES

=over 4

=item val -complex  

      result of multiplication

=back

=head1  DESCRIPTION

 Cmult returns a*b.

=head1 SEE ALSO

L<cdiv>

=head1 LIBRARY

B<sepmath>

=cut



>*/
#include <sepmath.h>
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
complex scmult(complex x, complex  y)
#endif

	{
	complex i;
	i.re = x.re * y.re - x.im * y.im;
	i.im = x.re * y.im + x.im * y.re;
	return (i);
	}

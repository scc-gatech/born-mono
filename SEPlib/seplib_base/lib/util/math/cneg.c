/*$

=head1 NAME

scneg - negative of complex number


=head1 SYNOPSIS 

C<val= scneg(a)>

=head1 INPUT PARAMETERS

=over 4

=item a - complex 

      number to negate

=back

=head1 RETURN VALUES

=over 4

=item val - complex

      the negated number

=back

=head1  DESCRIPTION

Cneg returns the negative of the complex number 



=head1 LIBRARY

B<sepmath>

=cut


>*/
#include <sepmath.h>
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
complex scneg(complex x)
#endif

	{
	complex i;
	i.re = -x.re;
	i.im = -x.im;
	return (i);
	}

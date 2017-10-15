/*$

=head1 NAME

sconjg - complex conjugate

=head1 SYNOPSIS

C<val= sconjg(a)>

=head1 DESCRIPTION

sconjg returns (re a, -im a).

=head1 INPUT PARAMETERS

=over 4

=item a - complex 

      input complex number

=back

=head1 RETURN VALUES

=over 4

=item val - complex 

       conjugate of number

=back


=head1 LIBRARY

B<sepmath>

=cut


>*/
#include <sepmath.h>
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
complex sconjg(complex x)
#endif

	{
	complex i;
	i.re = x.re;
	i.im = - x.im;
	return (i);
	}

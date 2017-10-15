/*$

=head1 NAME

scsub - subtract complex numbers


=head1 SYNOPSIS

C<val= scsub(a, b)>

=head1 INPUT PARAMETERS

=over 4

=item   a,b - complex 

        input complex numbers

=back

=head1  RETURN VALUES

=over 4

=item  val - complex 

       result of a-b

=back

=head1 DESCRIPTION

     Complex subtraction
     Csub returns a-b.

=head1 SEE ALSO

L<cadd>


=head1 LIBRARY

B<sepmath>

=cut



>*/
#include <sepmath.h>
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
complex scsub(complex x,complex  y)
_XFUNCPROTOEND
#endif

	{
	complex i;
	i.re = x.re - y.re;
	i.im = x.im - y.im;
	return (i);
	}

/*$

=head1 NAME

<<<<<<< .mine
 Scadd - complex add
=======
 scadd - complex add
>>>>>>> .r75

=head1 SYNOPSIS

C<val=complex scadd(a, b)>

=head1  INPUT PARAMETERS

=over 4

=item  a,b  -complex   

       complex numbers to add

=back

=head1  RETURN VALUES

=over 4

=item val -  complex 

      complex  the result of addition;

=back

=head1 DESCRIPTION

Cadd returns a+b.

=head1 LIBRARY

B<sepmath>

=cut

>*/
#include <sepmath.h>
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
complex scadd(complex x, complex y)
_XFUNCPROTOEND
#else
complex scadd(x, y)
complex x, y;
#endif

	{
	complex i;
	i.re = x.re + y.re;
	i.im = x.im + y.im;
	return (i);
	}

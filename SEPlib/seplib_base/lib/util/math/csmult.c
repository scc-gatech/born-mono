/*$

=head1 NAME

scsmult - complex multiplication


=head1 SYNOPSIS

C<val= scsmult(c, s)>

=head1 INPUT PARAMETERS

=over 4

=item   c - complex  

        complex input

=item   s -  float   

        float values to multiply by

=back

=head1 RETURN VALUES

=over 4

=item  val- complex 

       result of multiplication

=back

=head1  DESCRIPTION
    Multiply complex number by real scalar
     Csmul returns c*s.


=head1 SEE ALSO

L<cmult>


=head1 LIBRARY

B<sepmath>

=cut



>*/
#include <sepmath.h>
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
complex scsmult(complex x,float  y)
_XFUNCPROTOEND
#endif
	{
	complex i;
	i.re = x.re * y;
	i.im = x.im * y;
	return (i);
	}

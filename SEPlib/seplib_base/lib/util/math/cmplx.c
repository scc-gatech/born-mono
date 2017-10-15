/*$

=head1 NAME

 scmplx - create complex number

=head1 SYNOPSIS

C<val=complex scmplx(re, im)>

=head1 INPUT PARAMETERS

=over 4

=item re - float  

      real portion 

=item im - float  

      imaginary protion

=back

=head1 RETURN VALUES

=over 4

=item  val-complex 

       complex number

=back

=head1  DESCRIPTION

     Cmplx returns a complex number with specified real and ima-
     ginary parts.


=head1 LIBRARY

B<sepmath>

=cut



>*/
#include <sepmath.h>
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
complex scmplx(float x, float y)
_XFUNCPROTOEND
#endif
	
	{
	complex i;
	i.re = x;
	i.im = y;
	return (i);
	}

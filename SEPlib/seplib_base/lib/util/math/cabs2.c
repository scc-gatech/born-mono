/*$

=head1 

Scabs2 - complex absolute value


=head1 SYNOPSIS

C<val= float scabs2(a)>

=head DESCRIPTION

     Scabs2 returns the squared modulus, re**2 + im**2, of the
     complex number a.

=head1 	RETURN VALUE

=over

=item val -  float  

      squared modulus

=back

=head1 	INPUT PARAMTERS

=over4

=item	a  - complex  complex number to find modulus of

=back

=head1 SEE ALSO

L<scabs>, L<sccabs>

=head1 LIBRARY

B<sepmath>


*/
#include <sepmath.h>
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
float scabs2(complex x)
_XFUNCPROTOEND
#else
float scabs2(x)
complex x;
#endif
{
	float i;
	i = x.re*x.re +x.im*x.im;
	return (i);
}

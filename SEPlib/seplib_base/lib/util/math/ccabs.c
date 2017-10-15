/*$

=head1

sccabs - complex modulus


=head1 SYNOPSIS

C<val=float sccabs(a)>

=head1  INPUT PARAMETERS

=over 4

=item a - complex  

      number to find the modulus of

=back

=head1  DESCRIPTION

     sccabs returns the modulus, sqrt(re**2 + im**2), of the com-
     plex number a.

=head1 RETURN VALUES

=over 4

=item val - float 

			modulus of input complex number

=back

=head1 DIAGNOSTICS
     Some care is taken to avoid overflow.

=head1 SEE ALSO

L<scabs>,L<scabs2>


=head1 LIBRARY

B<sepmath>

=cut




>*/
#include <sepmath.h>
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
float sccabs(complex x)
_XFUNCPROTOEND
#else
float sccabs(x)
complex x;
#endif

{
float r;

r=hypot((double)x.re,(double)x.im);

return r;
}

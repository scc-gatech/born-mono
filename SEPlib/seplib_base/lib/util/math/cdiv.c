/*$

=head1 NAME

scdiv  - complex division


=head1 SYNOPSIS

C<val= complex cdiv(a, b)>

=head1 INPUT PARAMETER

=over 4

=item  a,b  - complex 

       number to divide a/b

=back

=head1 RETURN VALUES

=over 4

=item val -  complex  

      result of division

=back

=head1 DESCRIPTION

     Cdiv returns a/b.



=head1 LIBRARY

B<sepmath>

=cut




>*/
#include <sepmath.h>
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
complex scdiv(complex x, complex y)
_XFUNCPROTOEND
#endif

	{

	return (scmult(x,scinv(y)));
	}

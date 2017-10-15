/*$

=head1 NAME

scspow - raise complex number to real power

=head1 SYNOPSIS

C<val= scspow(c,s)>

=head1  INPUT PARAMETERS

=over 4

=item  c  - complex 

       number to raise

=item  s  - real   

       power to raise it

=back
 
=head1  RETURN VALUES

=over 4

=item  val - complex = 

       result of c**s

=back

=head1  DESCRIPTION

     Raise a complex number to a real power
     Cspow returns the principal value of a**s for an arbitrary
     complex a and real exponent s.  An equivalent formula is
     cexp(s*clog(a)).



=head1 SEE ALSO

L<clog>,L<cexp>

=head1 LIBRARY

B<sepmath>

=cut


>*/
#include <sepmath.h>
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
complex scspow(complex z,float s)
_XFUNCPROTOEND
#endif
  {
	double amp,phase;
	amp = pow((double)(z.re*z.re+z.im*z.im),(double)(0.5*s));
	phase = s*atan2(z.im,z.re);
	z.re = amp*cos(phase);
	z.im = amp*sin(phase);
	return(z);
  }

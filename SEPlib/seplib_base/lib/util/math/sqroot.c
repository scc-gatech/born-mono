/*$

=head1 NAME

sqroot - square root

=head1 SYNOPSIS

C<val=sqrt(r,t0,epsilon)>

=head1 INPUT PARAMETERS

=over 4

=item r - real 

      number squared

=item t0 - real

      initial value

=item epsilon - real

      desired precision

=back

=head1 RETURN VALUE

=over 4

=item val - real

      square root

=back

=head1 COMMENTS

Finds the square root of r, starting from t0.

A simple Newton algorithm.
The hope is that with a proper choice of t0, 
it makes less iterations than the system sqrt. 

=head1 DESCRIPTION

"Fast" square root


=head1 LIBRARY

B<sepmath>

=cut


>*/

/* 
 * Copyright (c) 1997 Stanford Exploration Project
 * All Rights Reserved
 *
 * Author: Sergey Fomel
 */

#include <sepmath.h>

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
float sqroot (float r, float t0, float epsilon)
_XFUNCPROTOEND
#else
float sqroot(r, t0, epsilon)
float r, t0,  epsilon;
#endif
{
  float eps;

  if (r < 0.) r = -r;
  eps = t0 - r/t0;
  while (eps > epsilon) {
    eps = t0 - r/t0;
    t0 -= 0.5*eps;
  }
  return t0;
}



!!$=head1 NAME
!!$
!!$irls  - weighting functions
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<ierr=l1(res,weight)>
!!$
!!$C<ierr=cauchy(res,weight)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item res  -  C<real(:)>
!!$
!!$      Residual
!!$
!!$=item weight  -  C<real(:)>
!!$
!!$      Resulting weighting function
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Weighting function to apply to cgmethods (warning stability
!!$is not guaranteed when using these).
!!$
!!$L1 -  weight=1/abs(1+ res/rbar)
!!$CAUCHY- weight=1/sqrt(1+res/rbar**2)
!!$
!!$
!!$=head1 SEE ALSO
!!$
!!$L<solver>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module irls  
  use gee_quantile_mod
  implicit none
  contains
  integer function l1 (res, weight)
    real, dimension (:)  :: res, weight
    real                 :: rbar
    rbar = gee_quantile( int( 0.5*size(res)), abs (res))            
    ! median
    weight = 1. / sqrt( sqrt (1. + (res/rbar)**2))
    l1 = 0
  end function 
  integer function cauchy (res, weight)
    real, dimension (:)  :: res, weight
    real                 :: rbar
    rbar = gee_quantile( int( 0.5*size(res)), abs (res))             
    ! median
    weight = 1. / sqrt (1. + (res/rbar)**2)
    cauchy = 0
  end function 
end module 

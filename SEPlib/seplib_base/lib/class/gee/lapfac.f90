!!$=head1 NAME
!!$
!!$lapfac -  factor a 2-D Laplacian
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<aa= lapfac2(eps,n1,na)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item eps - real
!!$
!!$      small value for approximation
!!$
!!$=item n1  - integer
!!$
!!$      length of n1 axis
!!$
!!$=item na - integer
!!$
!!$      1/2 number of filter coefs for representation
!!$
!!$=back
!!$
!!$=head1 RETURN VALUE
!!$
!!$=over 4
!!$
!!$=item aa    - type(filter)
!!$
!!$      Helix filter
!!$
!!$=back
!!$
!!$=head1  DESCRIPTION
!!$
!!$Create a one sided, minimum phase filter that approximates a laplaciaan
!!$
!!$=head1 SEE ALSO
!!$
!!$L<wilson>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!$=cut
module lapfac  
! Factor 2-D Laplacian.
  use wilson
  implicit none
  contains
  function lapfac2( eps, n1, na)   result (aa)
    type( filter)                       :: aa, lap
    real,                   intent( in) :: eps
    integer,                intent( in) :: n1, na
    integer                             :: i
    real                                :: a0, lap0
    call allocatehelix( lap, 2)         ! laplacian filter
    lap0 = 4. + eps                     ! zero lag coeff.
    lap%lag = (/ 1, n1 /)                ! lag(1)= 1; lag(2)=n1  # one side only
    lap%flt = -1.                        ! flt(1)=-1; flt(2)=-1
    call allocatehelix( aa, 2*na)       ! laplacian derivative
    aa%flt = 0.
! probably done already in allocation.
    do i = 1, na  
      aa%lag( i   ) =      i                ! early lags (first row)
      aa%lag( na+i) = n1 + i - na        ! late lags (second row)
    end do 
    call wilson_init( 10 * n1 )
    call wilson_factor( 20, lap0, lap, a0, aa)
    call wilson_close()
    call deallocatehelix( lap)
  end function 
end module 

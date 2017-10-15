!!$=head1 NAME
!!$
!!$steering  - 2-D steering filters
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call steer_init(hwidth,amp,n1)>
!!$
!!$C<call steering2(slope,aa)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item hwidth - real
!!$
!!$      half-width of triangle
!!$
!!$=item amp    - real
!!$
!!$      amplitude that all coefs sum to
!!$
!!$=item n1     - integer
!!$
!!$      Axis 1 length
!!$
!!$=item slope  - real
!!$
!!$      slope to make filter destroy
!!$
!!$=item aa     - type(filter)
!!$
!!$      Helix filter
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Create a single steering filter
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module steering  
! Bob Clapp's steering filter
  use helix
  implicit none
  integer, private :: n   ! helix period
  real,    private :: dc  ! triangle half-width, filter d.c.
  contains
  subroutine steer_init( amp, n1)
    real,    intent( in) :: amp
    integer, intent( in) :: n1
    dc = amp
    n = n1
  end subroutine 
  subroutine steering2( slope, aa)
    type( filter)     :: aa
    real, intent( in) :: slope
    integer           :: h, nh, ip
    real              :: p0
    nh = size( aa%lag)
    p0 =( nh+1.)/2.
    ip = n - p0 + slope
    do h = 1, nh  
      aa%lag(h) = ip + h
      aa%flt(h) = p0 - 1. - abs (ip + h - n - slope)
    end do 
    where (aa%flt < 0.)
      aa%flt = 0.
end where
    aa%flt = -dc*aa%flt/sum(aa%flt)
  end subroutine 
end module 

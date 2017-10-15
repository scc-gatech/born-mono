!!$=head1 NAME
!!$
!!$steepdip  - Find steep dip decon filter
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<aa=steepn(n,a,d,vel,tgap)>
!!$
!!$=head1 INPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item n -  C<integer(:)>
!!$
!!$      data size
!!$
!!$=item a -  C<integer(:)>
!!$
!!$=item d -  C<real(:)>
!!$
!!$      sampling
!!$
!!$=item vel - real
!!$
!!$      velocity
!!$
!!$=item tgap - real
!!$
!!$      time gap
!!$
!!$=back
!!$
!!$=head1 RETURN VALUE
!!$
!!$=over 4
!!$
!!$=item aa  - type(filter)
!!$
!!$      steep dip filter
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Create steep dip decon filter
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module steepdip  
! Find N-D PEF coef locations for steep dip decon
  use helix
  use cartesian
  implicit none
  contains
  function steepn( n, a, d, vel,tgap) result (aa)
    type( filter)                       :: aa                    
    ! needed by helicon
    integer, dimension (:), intent( in) :: n, a
    real,    dimension (:), intent( in) :: d
    real,                   intent( in) :: vel, tgap 
    integer, dimension (:), allocatable :: lag
    integer, dimension (size (a))       :: c
    real,    dimension (size (a) - 1)   :: x          
    real                                :: t, t0
    integer                             :: i, h, na, nx, it  
    h= 0
    na= product( a)
    nx= size( x)
    it = nx +1
    allocate( lag( na))
    do i = 1, na  
      call line2cart(  a, i, c)
      c (:nx) = c(:nx) - (a(:nx)+1)/2
      c(it) = c(it) - 1
      x = d(:nx)*c(:nx)
      t0 = sqrt (dot_product (x,x)) / vel
      t = c(it)*d(it)
      if ( t < max( t0, tgap)) then
        cycle
      end if
      h = h + 1
      call cart2line( n, c+1, lag(h))
    end do 
    call allocatehelix( aa, h)
    aa%lag = lag( :h)-1
    deallocate( lag)
    aa%flt = 0.0
  end function 
end module 

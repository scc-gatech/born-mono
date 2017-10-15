!!$
!!$=head1 NAME
!!$
!!$nhconest - non stationary  convolution using helix filters, adjoint is filter
!!$
!!$=head1 SYNOPSIS
!!$
!!$Initializer - C<call nhconest_init(x,aa)>
!!$
!!$Operator    - C<ierr=nhconest_lop(adj,add,xx,yy)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item x  - C<real(:)>
!!$
!!$      data
!!$
!!$=item aa - type(filter)
!!$
!!$      helix filter to perform convolution with
!!$
!!$=item adj,add,xx,yy -
!!$
!!$      standard operators parameters
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$  Masked  non-stationary helix convolution, adjoint is the filter$
!!$
!!$=head1 SEE ALSO
!!$
!!$L<nhelix>,L<hconest>,L<nhelicon>,L<npolydiv>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module nhconest  
! Nonstationary convolution, adjoint is the filter.
  use classutil
  use nhelix
  implicit none
  real ,private,  dimension(:), pointer                  :: x 
  type( nfilter), private :: aa 
  integer, private :: nhmax 
  integer,   private                             :: np 
  contains
  subroutine nhconest_init ( x_in,aa_in,nhmax_in )
    real ,   dimension(:), pointer                 :: x_in 
    type( nfilter)    :: aa_in 
    integer    :: nhmax_in 
    x => x_in 
    aa = aa_in 
    nhmax = nhmax_in 
    np = size( aa%hlx)
  end subroutine
  function nhconest_lop ( adj, add, a, y) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: a,y 
    call adjnull (adj,add,a,y )
    call nhconest_lop2(adj,add,a,y )
    stat=0
  end function 
  subroutine nhconest_lop2(adj,add,a,y)
    logical,intent(in) :: adj,add 
    real, dimension ( nhmax, np)  :: a 
    real, dimension (:)  :: y 
    integer                        :: ia, ix, iy, ip
    integer, dimension(:), pointer :: lag
    do iy = 1, size( y)
      if ( aa%mis( iy)) then
        cycle
      end if
      ip = aa%pch( iy)
      lag => aa%hlx( ip)%lag
      do ia = 1, size( lag)
        ix = iy - lag( ia)
        if ( ix < 1) then
          cycle
        end if
        if ( adj) then
          a( ia, ip) =    a( ia, ip) +  y( iy) * x( ix)
        else
          y( iy) =         y( iy) +  a( ia, ip) * x( ix)
        end if
      end do
    end do
  end subroutine 
  subroutine nhconest_close()
  end subroutine 
end module 

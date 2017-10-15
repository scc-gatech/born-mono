!!$
!!$=head1 NAME
!!$
!!$hconest - convolution using helix filters, adjoint is filter
!!$
!!$=head1 SYNOPSIS
!!$
!!$Initializer - C<call hconest_init(x,aa)>
!!$
!!$Operator    - C<ierr=hconest_lop(adj,add,xx,yy)>
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
!!$  Masked helix convolution, adjoint is the filter$
!!$
!!$=head1 SEE ALSO
!!$
!!$L<helix>,L<nhconest>,L<helicon>,L<polydiv>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module hconest  
! masked helix convolution, adjoint is the filter.
  use classutil
  use helix
  implicit none
  real ,private,  dimension (:), pointer  :: x 
  type( filter), private :: aa 
  contains
  subroutine hconest_init ( x_in,aa_in )
    real ,   dimension (:), pointer :: x_in 
    type( filter)    :: aa_in 
    x => x_in 
    aa = aa_in
  end subroutine
  function hconest_lop ( adj, add, a, y) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: a,y 
    call adjnull (adj,add,a,y )
    call hconest_lop2(adj,add,a,y )
    stat=0
  end function 
  subroutine hconest_lop2(adj,add,a,y)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: a 
    real, dimension (:)  :: y 
    integer  ia, ix, iy
    do ia = 1, size( a)
      do iy = 1  + aa%lag( ia), size( y)
        if ( aa%mis( iy)) then
          cycle  
        end if
        ix = iy - aa%lag( ia)
        if ( adj) then
          a( ia) =    a( ia) +  y( iy) * x( ix)
        else
          y( iy) =         y( iy) +  a( ia) * x( ix)
        end if
      end do
    end do
  end subroutine 
  subroutine hconest_close()
  end subroutine 
end module 

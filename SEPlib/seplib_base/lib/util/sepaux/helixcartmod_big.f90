!!$=head1 NAME
!!$
!!$helixcartmod - convert to and from cartesian/helix space
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call helix2cart(nn,i,ii)>
!!$
!!$C<call cart2helix(nn,ii,i)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item n - <int(:)>
!!$
!!$          cartesian axes
!!$
!!$=item ii - <int(:)>
!!$
!!$      cartesian coordinates
!!$
!!$=item i  - int
!!$
!!$      helix coordinate
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$ Convert to and from a helix coordinate system.
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!$
module helixcartmod_big  
! vector to matrix index transform and its inverse
  implicit none
  integer,parameter :: big_int=selected_int_kind(11)
  contains
  subroutine helix2cart( nn, i, ii)
    integer, dimension( :), intent( in) :: nn    
    ! cartesian axes (n1,n2,n3,...)
    integer, dimension( :), intent(out) :: ii    
    ! cartesn coords (i1,i2,i3,...)
    integer(kind=big_int) , intent( in) :: i     
    ! equivalent 1-D linear index
!    integer, dimension( :) :: nn    # cartesian axes (n1,n2,n3,...)
!    integer, dimension( :) :: ii    # cartesn coords (i1,i2,i3,...)
!    integer  :: i     # equivalent 1-D linear index
    integer                             :: axis, n123,j
    n123 = 1
    do axis = 1, size(  nn)
      j= ( i-1)/n123
      ii( axis) = mod( j, nn( axis)) + 1
      n123 = n123 * nn( axis)
    end do
  end subroutine 
  subroutine cart2helix( nn, ii, i)
    integer, dimension( :), intent( in) :: nn, ii
    integer(kind=big_int):: i
!    integer, dimension( :) :: nn, ii
!    integer:: i
    integer                             ::  axis, n123
    n123 = 1
    i = 1
    do axis = 1, size(  nn)
      i = i + ( ii( axis)-1)*n123
      n123 = n123 * nn( axis)
    end do
  end subroutine 
end module 

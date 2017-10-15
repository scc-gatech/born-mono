!!$
!!$=head1 NAME
!!$
!!$cartesian - convert to and from cartesian
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call line2cart(nn,i,ii)>
!!$
!!$C<call cart2line(nn,ii,i)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item nn - C<int(:)>
!!$
!!$      cartesian axes
!!$
!!$=item ii - C<int(:)>
!!$
!!$      cartesian coordinates
!!$
!!$=item i  - int
!!$
!!$      linear index
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$index transform (vector to matrix) and its inverse
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!$
module cartesian  
! index transform (vector to matrix) and its inverse
  implicit none
  contains
  subroutine line2cart( nn, i, ii)
    integer, dimension( :), intent( in) :: nn    
    ! cartesian axes (n1,n2,n3,...)
    integer, dimension( :), intent(out) :: ii    
    ! cartesn coords (i1,i2,i3,...)
    integer               , intent( in) :: i     
    ! equivalent 1-D linear index
    integer                             :: axis, n123
    n123 = 1
    do axis = 1, size(  nn)
      ii( axis) = mod( ( i-1)/n123, nn( axis)) + 1
      n123 = n123 * nn( axis)
    end do
  end subroutine 
  subroutine cart2line( nn, ii, i)
    integer, dimension( :), intent( in) :: nn, ii
    integer                             :: i, axis, n123
    n123 = 1
    i = 1
    do axis = 1, size(  nn)
      i = i + ( ii( axis)-1)*n123
      n123 = n123 * nn( axis)
    end do
  end subroutine 
end module 

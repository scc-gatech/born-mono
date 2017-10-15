!!$=head1 NAME
!!$
!!$regrid - convert a helix filter from one data space to another
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call regridn(nold,nnew,aa)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item nold - C<integer(:)>
!!$
!!$      Old data dimensions
!!$
!!$=item nnew - C<integer(:)>
!!$
!!$      New data dimensions
!!$
!!$=item aa   -  type(filter)
!!$
!!$      Helix filter
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Converts a helix filter from one ata size to another
!!$
!!$=head1 SEE ALSO
!!$
!!$L<cartesian>,L<helix>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module regrid  
! convert a helix filter from one size data to another
  use helix
  use cartesian
  implicit none
  contains
  subroutine regridn( nold, nnew, aa)
    integer, dimension (:), intent (in) :: nold, nnew  
    ! old and new helix grid
    type( filter)                       :: aa
    integer, dimension( size( nold))    :: ii
    integer                             :: i, h0, h1, h
    call cart2line( nold, nold/2, h0)        
    ! lag of any near middle point on nold
    call cart2line( nnew, nold/2, h1)        
    ! lag                          on nnew
    do i = 1, size( aa%lag)
! forall given filter coefficients
      h = aa%lag( i) + h0                ! what is this?
      call line2cart( nold, h, ii)        !
      call cart2line( nnew,    ii, h)        !
      aa%lag( i) = h - h1                ! what is this
    end do
  end subroutine 
end module 

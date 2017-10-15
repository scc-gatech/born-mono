!!$
!!$=head1 NAME
!!$
!!$box  - filter to hypercube
!!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call boxn (nd,center,na,aa,cube)>
!!$
!!$=head1 INPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item nd  - C<int(:)>
!!$
!!$      Input data dimension
!!$
!!$=item center - C<int(:)>
!!$
!!$      Location of the 1 coeficient in box
!!$
!!$=item na  -  C<int(:)>
!!$
!!$      Dimensions of the filter
!!$
!!$=item aa  -
!!$
!!$      C<type(filter)> The filter
!!$
!!$=back
!!$
!!$=head1 OUTPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item cube  -  real(:)
!!$
!!$      Output filter values
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$ Convert helix filter to hypercube: cube(na(1),na(2),...)
!!$
!!$=head1 SEE ALSO
!!$
!!$B<helix>, B<cartesian>, B<helixcart>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!$
module box  
! Convert helix filter to hypercube: cube(na(1),na(2),...)
  use helix
  use cartesian
  implicit none
  contains
  subroutine boxn( nd, center, na, aa, cube)
    integer, dimension (:), intent( in)  :: nd, center, na    
    ! (ndim)
    type( filter),          intent( in)  :: aa
    real,    dimension( :), intent( out) :: cube 
    integer, dimension( size( nd))       :: ii
    integer                              :: j, lag0a, lag0d, id, ia
    cube = 0.
! cube=0
    call cart2line( na, center, lag0a)  
    ! locate the 1.0 in the na_cube.
    cube( lag0a) = 1.                   ! place it.
    call cart2line( nd, center, lag0d)  
    ! locate the 1.0 in the nd_cube.
    do j = 1, size( aa%lag)
! inspect the entire helix
      id = aa%lag( j) + lag0d                ! index = helix_lag + center_d
      call line2cart( nd, id, ii)        ! ii(id) = cartesian indices
      call cart2line( na,     ii, ia)        ! ia(ii) = linear index in aa
      cube( ia) = aa%flt( j)                ! copy the filter coefficient
    end do
  end subroutine 
end module 

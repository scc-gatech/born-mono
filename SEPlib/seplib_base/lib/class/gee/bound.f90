!!$
!!$=head1 NAME
!!$
!!$bound - find the boundaries of a filter on given map
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call boundn(nold,nd,na,aa)>
!!$
!!$=head1 INPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item nold  - C<int(:)>
!!$
!!$      ?????
!!$
!!$=item nd    - C<int(:)>
!!$
!!!$      Size of data
!!$
!!$=item na    - int(:)
!!$
!!$      Size of box surrounding filter
!!$
!!$=item aa    - C<type(filter)>
!!$
!!$      Filter to calculate boundaries for
!!$
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$mark helix filter outputs where input is off data
!!$
!!$=head1 SEE ALSO
!!$
!!$L<cartesian>,L<nbound>,L<helicon>,L<regrid>,L<helix>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!$
!$
!$
module bound  
! mark helix filter outputs where input is off data.
  use cartesian
  use helicon
  use regrid
  implicit none
  contains 
  subroutine boundn ( nold, nd, na, aa)
    integer, dimension( :), intent( in) :: nold, nd, na     ! (ndim)
    type( filter)                       :: aa
    integer, dimension( size( nd))      :: nb, ii
    real,    dimension( :), allocatable :: xx, yy
    integer                             :: iy, my, ib, mb, stat
    nb = nd + 2*na
    mb = product( nb)      ! nb is a bigger space to pad into.
    allocate( xx( mb), yy( mb))                     ! two large spaces, equal size
    xx = 0.                                     !              zeros
    do ib = 1, mb  
! surround the zeros with many ones
      call line2cart( nb, ib, ii)           ! ii( ib)
      if ( any( ii <= na  .or.  ii > nb-na)) then
        xx( ib) = 1. 
      end if
    end do 
    call helicon_init( aa)                     ! give aa pointer to helicon.lop
    call regridn( nold, nb, aa)
    aa%flt = 1.                ! put all 1's in filter
    stat = helicon_lop( .false., .false., xx, yy)        ! apply filter
    call regridn(   nb, nd, aa)
    aa%flt = 0.         ! remake filter for orig data.
    my = product( nd)
    allocate( aa%mis( my))                ! attach missing designation to y_filter
    do iy = 1, my  
! map from unpadded to padded space
      call line2cart( nd, iy, ii )
      call cart2line( nb,     ii+na, ib )            ! ib( iy)
      aa%mis( iy) =             ( yy( ib) > 0.)    
      ! true where inputs missing
    end do 
    deallocate( xx, yy)
  end subroutine 
end module 

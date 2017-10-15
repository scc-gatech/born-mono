!!$
!!$=head1 NAME
!!$
!!$nhelix - module containing allocate and deallocate of a  nhelix filter
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call nallocate(aa,nh,pch)>
!!$
!!$C<call ndeallocate(aa)>
!!$
!!$=head1 INPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item aa - type(helix)
!!$
!!$      Filter
!!$
!!$=item nh - C<int(:)>
!!$
!!$      Number of coefficients for each filter
!!$
!!$=item patch - C<int(:)>
!!$
!!$      For a given model space location the filter that should be applied
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Allocate and deallocation of a nhelix filter.
!!$
!!$=head1 COMMENTS
!!$
!!$type(nfilter) :
!!$
!!$=over 4
!!$
!!$=item hlx - C<type(filter)>
!!$
!!$      filters making up nhelix
!!$
!!$=item mis  - C<real(:)>
!!$
!!$      boundary conditions
!!$
!!$=item pch  - C<real(:)>
!!$
!!$      patches
!!$
!!$=back
!!$
!!$=head1 SEE ALSO
!!$
!!$L<helix>,L<mshelix>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module nhelix  
! Define nonstationary filter type
  use helix
implicit none
  type nfilter  
!  nd  is data length.
    logical,       dimension(:), pointer :: mis  
    ! (nd) boundary conditions
    integer,       dimension(:), pointer :: pch  ! (nd) patches
    type( filter), dimension(:), pointer :: hlx  ! (np) filters
  end type
  contains
  subroutine nallocate( aa, nh, pch)
! allocate a filter
    type( nfilter)                       :: aa
    integer, dimension(:), intent( in)   :: nh, pch  
    integer                              :: ip, np, nd
    np = size( nh)
    allocate( aa%hlx( np))
    do ip = 1, np
      call allocatehelix( aa%hlx( ip), nh( ip))
    end do 
    nd = size( pch)
    allocate( aa%pch( nd)) 
    aa%pch = pch
    nullify( aa%mis)                                   ! set null pointer for mis.
  end subroutine 
  subroutine ndeallocate( aa)
! destroy a filter
    type( nfilter) :: aa
    integer        :: ip
    do ip = 1, size( aa%hlx)
      call deallocatehelix( aa%hlx( ip))
    end do 
    deallocate( aa%hlx, aa%pch)
    if ( associated( aa%mis)) then
! if logicals were allocated
      deallocate( aa%mis)                           ! free them
    end if
  end subroutine 
end module 

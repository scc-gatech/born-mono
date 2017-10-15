!!$
!!$=head1 NAME
!!$
!!$helix - module containing allocate and deallocate of a  helix filter
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call allocatehelix(aa,nh)>
!!$
!!$C<call deallocatehelix(aa)>
!!$
!!$=head1 INPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item aa - type(helix)
!!$
!!$      Filter
!!$
!!$=item nh - integer
!!$
!!$      Number of coefs in filter
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Allocate and deallocation of helix filter.
!!$
!!$=head1 COMMENTS
!!$
!!$type(filter) :
!!$
!!$=over 4
!!$
!!$=item flt - C<real(:)> (nh)
!!$
!!$      filter coefficients
!!$
!!$=item lag - C<real(:)> (nh)
!!$
!!$      filter lags
!!$
!!$=item mis - C<real(:)>  (nd)
!!$
!!$      boundary conditions
!!$
!!$=back
!!$
!!$=head1 SEE ALSO
!!$
!!$L<nhelix>,L<mshelix>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module helix  
! DEFINE helix filter type
    implicit none
  type filter  
    real,    dimension( :), pointer :: flt      
    ! (nh) filter coefficients
    integer, dimension( :), pointer :: lag      ! (nh) filter lags
    logical, dimension( :), pointer :: mis      
    ! (nd) boundary conditions
  end type
  contains
  subroutine allocatehelix( aa, nh )
! allocate a filter
    type( filter) :: aa
    integer       :: nh                                ! count of filter coefs (excl 1)
    allocate( aa%flt( nh), aa%lag( nh))                ! allocate filter and lags.
    nullify( aa%mis)                                ! set null pointer for "mis".
    aa%flt = 0.                                        ! zero filter coef values
  end subroutine 
  subroutine deallocatehelix( aa)
! destroy a filter
    type( filter) :: aa
    deallocate( aa%flt, aa%lag)                        ! free memory
    if ( associated( aa%mis)) then
! if logicals were allocated
      deallocate( aa%mis)                        ! free them
    end if
  end subroutine 
end module 

!!$=head1 NAME
!!$
!!$lopef - estimate pef in patches
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call find_lopef(wall,aa,npatch,nwall,nwind,mask)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item  wall - C<real(:)>
!!$
!!$=item  aa   - type(filter)
!!$
!!$       helix filter
!!$
!!$=item  npatch -C<integer(:)>
!!$
!!$       size of patch
!!$
!!$=item  nwall  -C<integer(:)>
!!$
!!$=item nwind  -C<integer(:)>
!!$
!!$=item mask   -C<real(:) >
!!$
!!$      Mask of known and unknown data
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Estiamte a PEF in patches
!!$
!!$=head1 SEE ALSO
!!$
!!$L<pef>,L<misinput>,L<patch>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module lopef  
! Local PEF estimation in patches.
  use patch                  ! Estimate a vector of filters, one for each patch.
  use misinput
  use pef
  implicit none
  contains
  subroutine find_lopef( wall, aa, npatch, nwall, nwind, mask)
    optional                                 :: mask
    integer,       dimension(:), pointer     :: npatch, nwall, nwind
    real,          dimension(:), intent( in) :: wall, mask
    type( filter), dimension(:)              :: aa
    real,          dimension(:), pointer     :: windata, winmask
    integer                                     :: i, stat
    allocate( windata( product( nwind)))   ! a patch
    if ( present( mask)) then
      allocate( winmask( product( nwind)))   ! missing inputs
    end if
    call patch_init( npatch, nwall, nwind)
    do i = 1, product( npatch)
! do all patches
      stat = patch_lop( .false., .false., wall, windata)      
      ! get a patch
      if ( present( mask)) then
        stat = patch_lop( .false., .false., mask, winmask)
        call find_mask( (winmask /= 0.), aa (i))             
        ! missing data
      end if
      if ( count(.not.aa(i)%mis) > size(aa(i)%lag)) then
! enuf eqns?
        call find_pef( windata, aa(i), niter=size( aa(i)%lag)) 
        ! find PEF
      else if ( i > 1) then
        aa(i)%flt = aa(i-1)%flt                                ! use last PEF
      end if
      call patch_close()
    end do 
    deallocate( windata)
    if ( present( mask)) then
      deallocate( winmask)
    end if
  end subroutine
end module
!       if( size(aa(i)%mis) - count(aa(i)%mis) > size(aa(i)%lag)) # enuf eqns?
  

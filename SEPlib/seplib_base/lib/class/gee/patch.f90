!!$=head1 NAME
!!$
!!$patch - extract and putback patches
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call patch_init(npatch,nwall,nwind)>
!!$
!!$C<ierr=ptch_lop(adj,add,wall,wind)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item npatches  - C<integer(:)>
!!$
!!$      number of patches
!!$
!!$=item nwall     - C<integer(:)>
!!$
!!$=item nwind     - C<integer(:)>
!!$
!!$=item adj,add,wall,wind -
!!$
!!$      Standard operator interface
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Extract and  put back patches
!!$
!!$=head1 SEE ALSO
!!$
!!$L<misinput>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module patch  
! N-dimensional patch extract and putback
  use classutil
  use cartesian
  implicit none
  integer,   private                 :: ipatch 
  integer ,private,  dimension (:), pointer  :: npatch 
  integer ,private,  dimension (:), pointer  :: nwind 
  integer ,private,  dimension (:), pointer  :: nwall 
! overlap where    npatch * nwind > nwall
  contains
  subroutine patch_init ( npatch_in,nwall_in,nwind_in )
    integer ,   dimension (:), pointer :: npatch_in 
    integer ,   dimension (:), pointer :: nwall_in 
    integer ,   dimension (:), pointer :: nwind_in 
    npatch => npatch_in 
    nwall => nwall_in 
    nwind => nwind_in 
    ipatch = 1
  end subroutine
  function patch_lop ( adj, add, wall, wind) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: wall,wind 
    call adjnull (adj,add,wall,wind )
    call patch_lop2(adj,add,wall,wind )
    stat=0
  end function 
  subroutine patch_lop2(adj,add,wall,wind)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: wall 
    real, dimension (:)  :: wind 
    integer, dimension(:),allocatable :: ii, jj             
    ! Ndim=size(npatch)
    integer                            :: i, j, shift
    allocate(ii(size(npatch)),jj(size(npatch)))
    call line2cart(  npatch, ipatch, jj )  
    ! (j1,j2) = (1,1) to (npatch1,npatch2)
! round jj to shift end of last patch flush against the far wall.
    where( npatch.eq.1)
      jj = 1
    elsewhere
      jj = 1.5 +( nwall - nwind)*( jj - 1.)/( npatch - 1.)
    end where 
    call cart2line( nwall,   jj, shift)         
    ! jj is where the patch begins.
    shift =  shift - 1                             !
    do i  = 1, size( wind)
! sweep through a window
      call line2cart(  nwind, i, ii)      
      !   ii(i)   is (i1,i2) in window
      call cart2line(  nwall,    ii, j)   
      ! j(ii)     linear subscript in window
      if ( adj) then
        wall( j + shift) =        wall( j + shift) + wind( i)
      else
        wind( i)         =        wind( i)         + wall( j + shift)
      end if
    end do 
    deallocate(ii,jj)
  end subroutine
  subroutine  patch_close()
    ipatch = ipatch + 1
  end subroutine 
end module 

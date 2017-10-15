!!$
!!$=head1 NAME
!!$
!!$createmshelix - create a multi-scale helix filter
!!$
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<aa=createmshelix(nd,center,gap,jump,na)>
!!$
!!$=head1 INPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item nd      - C<integer(:)>
!!$
!!$      size of data
!!$
!!$=item center  - C<integer(:)>
!!$
!!$      location of the 1 of the filter within na box
!!$
!!$=item jump     - C<integer(:)>
!!$
!!$      stretches
!!$
!!$=item gap     - C<integer(:)>
!!$
!!$      distance along each axis before filter coef
!!$
!!$=item na      - C<integer(:)>
!!$
!!$      size of box arround filter
!!$
!!$=back
!!$
!!$=head1 RETURN VALUE
!!$
!!$=over 4
!!$
!!$=item aa       - C<type(nfilter)>
!!$
!!$      Helix filter
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Create a multi-scale helix filter
!!$
!!$=head1 SEE ALSO
!!$
!!$L<createhelix>,L<mshelix>,L<createnhelix>,L<createmshelix>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!$
!!$
module createmshelixmod  
! Create multiscale helix filter lags and mis
  use mshelix
  use createhelixmod
  use bound
  implicit none
  contains
  function createmshelix( nd, center, gap, jump, na)  result( msaa)
    type( msfilter)                   :: msaa    
    ! needed by mshelicon.
    integer, dimension(:), intent(in) :: nd, na  
    ! data and filter axes
    integer, dimension(:), intent(in) :: center  
    ! normally (na1/2,na2/2,...,1)
    integer, dimension(:), intent(in) :: gap     
    ! normally ( 0,   0,  0,...,0)
    integer, dimension(:), intent(in) :: jump         
    ! jump(ns) stretch scales
    type( filter)                     :: aa
    integer                           :: is, ns, nh, n123 
    aa = createhelix( nd, center, gap, na)
    ns = size( jump)
    nh = size( aa%lag)
    n123 = product( nd)
    call msallocate( msaa, nh, ns)
    do is = 1, ns
      msaa%lag(:,is) = aa%lag(:)*jump(is)         
      ! set lags for expanded scale
    end do 
    call deallocatehelix( aa)
    allocate( msaa%mis( n123, ns))
    do is = 1, ns  
! for all scales
      call onescale( is, msaa, aa)
      nullify( aa%mis)        ! extract a filter
      call boundn( nd, nd, na*jump(is), aa)                ! set up its boundaries
      msaa%mis( :, is) = aa%mis
      deallocate( aa%mis)        ! save them
    end do
  end function 
end module 

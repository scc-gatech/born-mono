!!$=head1 NAME
!!$
!!$createnhelix - create non stationary helix filter
!!$
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<aa=createnhelix(nd,center,gap,na,patch)>
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
!!$=item gap     - C<integer(:)>
!!$
!!$      distance along each axis before filter coef
!!$
!!$=item na      - C<integer(:)>
!!$
!!$      size of box arround filter
!!$
!!$=item patch   - C<integer(:)>
!!$
!!$      what model point a given filter will act on
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
!!$Create a non-stationary helix filter
!!$
!!$=head1 SEE ALSO
!!$
!!$L<createhelix>,L<nhelix>,L<createmshelix>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module createnhelixmod  
! Create non-stationary helix filter lags and mis
  use createhelixmod
  use nhelix
  use nbound
  implicit none
  contains
  function createnhelix( nd, center, gap, na, pch) result (nsaa)
    type( nfilter)                     :: nsaa   ! needed by nhelicon
    integer, dimension(:), intent(in)  :: nd, na 
    ! data and filter axes
    integer, dimension(:), intent(in)  :: center 
    ! normally (na1/2,na2/2,...,1)
    integer, dimension(:), intent(in)  :: gap    
    ! normally ( 0,   0,  0,...,0)
    integer, dimension(:), intent(in)  :: pch         ! (prod(nd)) patching
    type( filter)                      :: aa
    integer                            :: n123, np, ip
    integer, dimension(:), allocatable :: nh
    aa = createhelix( nd, center, gap, na)
    n123 = product( nd)
    np = maxval(pch)
    allocate (nh (np))
    nh = size (aa%lag)
    call nallocate( nsaa, nh, pch)
    do ip = 1, np
      nsaa%hlx( ip)%lag = aa%lag
    end do 
    call deallocatehelix (aa)
    call nboundn(1, nd, na, nsaa)
  end function 
end module 

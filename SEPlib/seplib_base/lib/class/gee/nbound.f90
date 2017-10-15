!!$
!!$=head1 NAME
!!$
!!$nbound - find the boundaries of a multi scale filter on given map
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call nboundn(nold,nd,na,aa)>
!!$
!!$=head1 INPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item nd    - C<int(:)>
!!$
!!$      Size of data
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
!!$mark nhelix filter outputs where input is off data
!!$
!!$=head1 SEE ALSO
!!$
!!$L<cartesian>,L<bound>,L<helicon>,L<regrid>,L<nhelix>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module nbound  
! mark helix filter outputs where input is off data.
  use nhelix
  use bound
  implicit none
  contains 
  subroutine nboundn ( ip, nd, na, aa)
    integer, dimension(:), intent( in) :: nd, na ! (ndim)
    type( nfilter)                     :: aa
    integer, intent (in)               :: ip
    allocate( aa%mis( product (nd)))
    call boundn (nd, nd, na, aa%hlx( ip))
    aa%mis = aa%hlx( ip)%mis
    deallocate( aa%hlx( ip)%mis)
    nullify( aa%hlx( ip)%mis)
  end subroutine 
end module 

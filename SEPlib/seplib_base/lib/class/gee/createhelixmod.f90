!!$=head1 NAME
!!$
!!$createhelix - create a helix filter
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<aa=createhelix(nd,center,gap,na)>
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
!!$=back
!!$
!!$=head1 RETURN VALUE
!!$
!!$=over 4
!!$
!!$=item aa       - C<type(filter)>
!!$
!!$      Helix filter
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Create helix filter
!!$
!!$=head1 SEE ALSO
!!$
!!$L<createnhelix>,L<helix>,L<createmshelix>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module createhelixmod  
! Create helix filter lags and mis
  use helix
  use cartesian
  implicit none
  contains
  function createhelix( nd, center, gap, na)  result( aa)
    type( filter)                     :: aa       
    ! needed by helicon.
    integer, dimension(:), intent(in) :: nd, na   
    ! data and filter axes
    integer, dimension(:), intent(in) :: center   
    ! normally (na1/2,na2/2,...,1)
    integer, dimension(:), intent(in) :: gap      
    ! normally ( 0,   0,  0,...,0)
    integer, dimension( size( nd))    :: ii       ! cartesian indexes
    integer                           :: na123, ia, ndim, nh, lag0a&
      &,lag0d
    integer, dimension(:), allocatable:: lag
    nh= 0
    na123 = product( na)
    ndim = size( nd)
    allocate(  lag( na123 ) )                  ! filter cube size
    call cart2line ( na, center, lag0a)   
    ! lag0a = index pointing to the "1.0"
    do ia = 1+lag0a, na123  
!    ia  is fortran linear index.
      call line2cart( na, ia, ii)        
      ! ii(ia) is fortran array  indices.
      if ( any( ii <= gap)) then
        cycle       ! ignore some locations
      end if
      nh = nh + 1                        ! got another live one.
      call cart2line( nd, ii, lag(nh))          
      ! get its fortran linear index
    end do 
    call cart2line(  nd, center, lag0d)   
    ! lag0d is center shift for nd_cube
    call allocatehelix( aa, nh)                  
    ! nh becomes size of filter on helix.
    aa%lag = lag(1:nh) - lag0d
! lag = fortran_linear_index - center
    aa%flt = 0.0
    deallocate( lag)
  end function 
end module 

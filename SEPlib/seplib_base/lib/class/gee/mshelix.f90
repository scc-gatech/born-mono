module mshelix  
! multiscale helix filter type
  use helix
    implicit none
  type msfilter  
    real,    dimension( :),    pointer :: flt   
    ! (nh) filter coefficients
    integer, dimension( :, :), pointer :: lag   
    ! (nh,ns) filter (lags,scales)
    logical, dimension( :, :), pointer :: mis   
    ! (nd,ns) boundary conditions
  end type
  contains
  subroutine msallocate( msaa, nh, ns)
    type( msfilter)   :: msaa
    integer           :: nh, ns
    allocate( msaa%flt( nh), msaa%lag( nh, ns))
    msaa%flt = 0.
    nullify( msaa%mis)
  end subroutine 
  subroutine msdeallocate( msaa)
    type( msfilter) :: msaa
    deallocate( msaa%flt, msaa%lag)
    if ( associated( msaa%mis)) then
      deallocate( msaa%mis)
    end if
  end subroutine 
  subroutine onescale( i, msaa, aa)
! Extract single-scale filter.
    integer, intent (in) :: i
    type( filter)        :: aa
    type( msfilter)      :: msaa
    aa%flt => msaa%flt
    aa%lag => msaa%lag( :, i)
    if ( associated( msaa%mis)) then
      aa%mis => msaa%mis( :, i)
    else
      nullify( aa%mis)
    end if
  end subroutine 
end module 

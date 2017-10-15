!!$=head1 NAME
!!$
!!$cdoubint - Double causal integration
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call cdoubint_init(nx)>
!!$
!!$C<ierr=cdoubint_lop(adj,add,x,y)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item nx - integer
!!$
!!$      number of data points
!!$
!!$=item adj,add,x,y  -
!!$
!!$      standard operator interface
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Double causal integration
!!$
!!$=head1 SEE ALSO
!!$
!!$L<causint>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module cdoubint         
! double integration
  use classutil
  use causint_mod
  use chain_mod
  implicit none
  real ,private,  dimension (:), allocatable  :: tmp 
  contains
  subroutine cdoubint_init ( nx )
    integer :: nx
    if (.not. allocated(tmp)) then
      allocate(tmp ( nx)) 
    end if
  end subroutine
  function cdoubint_lop ( adj, add, x, y) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: x,y 
    call adjnull (adj,add,x,y )
    call cdoubint_lop2(adj,add,x,y )
    stat=0
  end function 
  subroutine cdoubint_lop2(adj,add,x,y)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: x 
    real, dimension (:)  :: y 
    if (adj) then
      call normal (causint_lop, .true., y, x, tmp)
    else
      call normal (causint_lop, .true., x, y, tmp)
    end if
  end subroutine 
  subroutine cdoubint_close()
    deallocate( tmp )
  end subroutine 
end module 

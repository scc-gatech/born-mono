module omppar_mod
  !! setup for OMP parallel programs

  !! receives
  !! nall = total number of elements
  !! n    = max number of elements loaded in core
  !! nth  = number of threads
  !! returns n

  !! n   should divide nall
  !! nth should divide both nall and n

  implicit none
  integer, parameter, private:: dbl=selected_int_kind(15)
contains

  !----------------------------------------------------------------
  subroutine omppar(nall,n,nth)
    integer(kind=dbl), intent(in)    :: nall
    integer(kind=dbl), intent(inout) :: n
    integer,           intent(inout) :: nth
    integer :: i,k

    !! reset nth to nall if nall is smaller
    if(nth>nall) nth=nall

    do while(nall/nth *nth /= nall)
       nth=nth-1
    end do
    
!    if(nall/nth *nth /= nall) then
!       write(0,*) "nall = ", nall
!       write(0,*) "nth  = ", nth
!       call seperr("cant use nth for the this nall")
!    end if

    do i=1,nall
       if(nall/i  *i   ==nall .and. &
       i/nth*nth ==i    .and. &
       i <= n ) k=i
    end do
    n = k
  end subroutine omppar
  !----------------------------------------------------------------

end module omppar_mod








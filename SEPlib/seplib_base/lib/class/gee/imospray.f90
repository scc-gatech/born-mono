module imospray  
! inverse moveout and spray into a gather.
  use classutil
  use hypotenusei
  implicit none
  real, private :: x0 
  real, private :: dx 
  real, private :: t0 
  real, private :: dt 
  integer, private :: nx 
  integer, private :: nt 
  contains
  subroutine imospray_init ( slow,x0_in,dx_in,t0_in,dt_in,nt_in,nx_in &
    &)
    real    :: x0_in 
    real    :: dx_in 
    real    :: t0_in 
    real    :: dt_in 
    integer    :: nt_in 
    integer    :: nx_in 
    real slow
    x0 = x0_in 
    dx = dx_in 
    t0 = t0_in 
    dt = dt_in 
    nt = nt_in 
    nx = nx_in 
    x0 = x0*slow
    dx = dx*slow
  end subroutine
  function imospray_lop ( adj, add, stack, gather) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: stack,gather 
    call adjnull (adj,add,stack,gather )
    call imospray_lop2(adj,add,stack,gather )
    stat=0
  end function 
  subroutine imospray_lop2(adj,add,stack,gather)
    logical,intent(in) :: adj,add 
    real, dimension (nt)  :: stack 
    real, dimension (nt,nx)  :: gather 
    integer ix, stat
    do ix= 1, nx  
      call hypotenusei_init ( nt, t0, dt, x0 + dx*(ix-1))
      stat = hypotenusei_lop ( adj, .true., stack, gather(:,ix))
    end do 
    call hypotenusei_close ()
  end subroutine 
  subroutine imospray_close()
  end subroutine 
end module 

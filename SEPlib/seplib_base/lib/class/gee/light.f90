module light  
! directional light
  use classutil
  use igrad1_mod
  implicit none
  integer, private :: nx 
  integer, private :: ny 
  real, private :: ax 
  real, private :: ay 
  real ,private,  dimension (:), allocatable  :: x 
  real ,private,  dimension (:), allocatable  :: y 
  contains
  subroutine light_init ( nx_in,ny_in,ax_in,ay_in )
    integer    :: nx_in 
    integer    :: ny_in 
    real    :: ax_in 
    real    :: ay_in 
    nx = nx_in 
    ny = ny_in 
    ax = ax_in 
    ay = ay_in 
    if (.not. allocated(x)) then
      allocate(x ( nx)) 
    end if
    if (.not. allocated(y)) then
      allocate(y ( ny)) 
    end if
  end subroutine
  function light_lop ( adj, add, mod, ruf) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: mod,ruf 
    call adjnull (adj,add,mod,ruf )
    call light_lop2(adj,add,mod,ruf )
    stat=0
  end function 
  subroutine light_lop2(adj,add,mod,ruf)
    logical,intent(in) :: adj,add 
    real, dimension (nx,ny)  :: mod 
    real, dimension (nx,ny)  :: ruf 
    integer ix, iy, stat
    do ix=1,nx  
      if ( adj) then
        y = ay*ruf (ix,:)
      end if
      stat = igrad1_lop (adj, adj, mod(ix,:), y)
      if (.not. adj) then
        ruf (ix,:) = ruf (ix,:) + ay*y
      end if
    end do 
    do iy=1,ny  
      if ( adj) then
        x = ax*ruf (:,iy)
      end if
      stat = igrad1_lop (adj, adj, mod(:,iy), x)
      if (.not. adj) then
        ruf (:,iy) = ruf (:,iy) + ax*x
      end if
    end do
  end subroutine 
  subroutine light_close()
    deallocate( x ,y)
  end subroutine 
end module 

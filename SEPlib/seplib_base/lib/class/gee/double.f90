module double  
! put a double pulse
  use classutil
  implicit none
  integer, private :: nb 
  integer, private :: nx 
  contains
  subroutine double_init ( nb_in,nx_in )
    integer    :: nb_in 
    integer    :: nx_in 
    nb = nb_in 
    nx = nx_in
  end subroutine
  function double_lop ( adj, add, x, y) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: x,y 
    call adjnull (adj,add,x,y )
    call double_lop2(adj,add,x,y )
    stat=0
  end function 
  subroutine double_lop2(adj,add,x,y)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: x 
    real, dimension (:)  :: y 
    if (adj) then
      x (1:nx) = x (1:nx) + (y (1:nx) - y (1 + nb:nx + nb)) /nb
    else
      y (1     :nx     ) = y (1     :nx     ) + x (1:nx) /nb
      y (1 + nb:nx + nb) = y (1 + nb:nx + nb) - x (1:nx) /nb
    end if
  end subroutine 
  subroutine double_close()
  end subroutine 
end module 

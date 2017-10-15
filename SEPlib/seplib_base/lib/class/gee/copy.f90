module copy_mod
! simplest operator possible - identity matrix
  use classutil
implicit none
  contains
  function copy_lop ( adj, add, x, y) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: x,y 
    call adjnull (adj,add,x,y )
    call copy_lop2(adj,add,x,y )
    stat=0
  end function 
  subroutine copy_lop2(adj,add,x,y)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: x 
    real, dimension (:)  :: y 
    if (adj) then
      x =   x + y
    else
      y =        y + x
    end if
  end subroutine 
  subroutine copy_close()
  end subroutine 
end module 

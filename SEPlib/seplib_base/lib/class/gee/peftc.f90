module peftc  
  use classutil
  use tcai1
  use tcaf1
  implicit none
  integer,   private   :: nx 
  contains
  subroutine peftc_init ( aa,yy )
    real, dimension (:), pointer :: aa, yy
    nx = size (yy)
    call tcai1_init (aa)
    call tcaf1_init (yy)
  end subroutine
  function peftc_lop ( adj, add, x, r) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: x,r 
    call adjnull (adj,add,x,r )
    call peftc_lop2(adj,add,x,r )
    stat=0
  end function 
  subroutine peftc_lop2(adj,add,x,r)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: x 
    real, dimension (:)  :: r 
    integer stat1, stat2
    stat1 = tcai1_lop (adj, .true., x(:nx),   r)
    stat2 = tcaf1_lop (adj, .true., x(nx+1:), r)
  end subroutine 
  subroutine peftc_close()
  end subroutine 
end module 

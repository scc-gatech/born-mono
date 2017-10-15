module pefconv  
  use classutil
  use helicon
  use hconest
  implicit none
  integer,   private   :: nx 
  contains
  subroutine pefconv_init ( yy,aa )
    real, dimension( :), pointer :: yy
    type( filter)                :: aa
    call helicon_init( aa)
    call hconest_init( yy, aa)
    nx = size( yy)
  end subroutine
  function pefconv_lop ( adj, add, x, r) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: x,r 
    call adjnull (adj,add,x,r )
    call pefconv_lop2(adj,add,x,r )
    stat=0
  end function 
  subroutine pefconv_lop2(adj,add,x,r)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: x 
    real, dimension (:)  :: r 
    integer stat1, stat2
    stat1 = helicon_lop( adj, .true., x( :nx),   r)
    stat2 = hconest_lop( adj, .true., x( nx+1:), r)
  end subroutine 
  subroutine pefconv_close()
  end subroutine 
end module 

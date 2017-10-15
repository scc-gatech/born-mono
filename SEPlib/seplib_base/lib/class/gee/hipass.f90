module hipass  
  use classutil
  implicit none
  real,   private  :: r 
  real,   private  :: t 
  contains
  subroutine hipass_init ( eps )
    real eps
    r = 1+0.5*eps*eps
    r = r - sqrt(r*r-1.)
  end subroutine
  function hipass_lop ( adj, add, x, y) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: x,y 
    call adjnull (adj,add,x,y )
    call hipass_lop2(adj,add,x,y )
    stat=0
  end function 
  subroutine hipass_lop2(adj,add,x,y)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: x 
    real, dimension (:)  :: y 
    integer i, n
    n = size (x)
    if ( adj) then
      t  = y (n)
      x(n) = x(n) + t
      do i=n-1,1,-1  
        t = y(i) - y(i+1) + r*t
        x(i) = x(i) + t
      end do
    else
      t  = x (1)
      y(1) = y(1) + t
      do i=2,n  
        t = x(i) - x(i-1) + r*t
        y(i) = y(i) + t
      end do
    end if
  end subroutine 
  subroutine hipass_close()
  end subroutine 
end module 

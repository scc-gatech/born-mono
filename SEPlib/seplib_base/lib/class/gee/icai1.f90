module icai1  
! Internal Convolution, Adjoint is the Input.  1-D
  use classutil
  implicit none
  integer, private :: lag 
  real ,private,  dimension (:), pointer  :: bb 
  contains
  subroutine icai1_init ( bb_in,lag_in )
    real ,   dimension (:), pointer :: bb_in 
    integer    :: lag_in 
    bb => bb_in 
    lag = lag_in
  end subroutine
  function icai1_lop ( adj, add, xx, yy) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: xx,yy 
    call adjnull (adj,add,xx,yy )
    call icai1_lop2(adj,add,xx,yy )
    stat=0
  end function 
  subroutine icai1_lop2(adj,add,xx,yy)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: xx 
    real, dimension (:)  :: yy 
    integer  b, x, y 
    do b= 1, size(bb)
      do y= 1+size(bb)-lag, size(yy)-lag+1  
        x= y - b + lag      
        if ( adj) then
          xx(x) =        xx(x) +  yy(y) * bb(b)
        else
          yy(y) =             yy(y) +  xx(x) * bb(b)
        end if
      end do
    end do
  end subroutine 
  subroutine icai1_close()
  end subroutine 
end module 

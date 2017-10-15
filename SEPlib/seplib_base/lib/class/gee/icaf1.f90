module icaf1  
! Internal Convolution, Adjoint is Filter. 1-D
  use classutil
  implicit none
  integer, private :: lag 
  real ,private,  dimension (:), pointer  :: xx 
  contains
  subroutine icaf1_init ( xx_in,lag_in )
    real ,   dimension (:), pointer :: xx_in 
    integer    :: lag_in 
    xx => xx_in 
    lag = lag_in
  end subroutine
  function icaf1_lop ( adj, add, bb, yy) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: bb,yy 
    call adjnull (adj,add,bb,yy )
    call icaf1_lop2(adj,add,bb,yy )
    stat=0
  end function 
  subroutine icaf1_lop2(adj,add,bb,yy)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: bb 
    real, dimension (:)  :: yy 
    integer   x,  b,   y
    do b= 1, size(bb)
      do y= 1+size(bb)-lag, size(yy)-lag+1  
        x= y - b + lag      
        if ( adj) then
          bb(b) =        bb(b) +  yy(y) * xx(x)
        else
          yy(y) =             yy(y) +  bb(b) * xx(x)
        end if
      end do
    end do
  end subroutine 
  subroutine icaf1_close()
  end subroutine 
end module 

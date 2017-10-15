module tcaf1  
  use sep_mod
! Transient Convolution, Adjoint is the Filter, 1-D
  use classutil
  implicit none
  real ,private,  dimension (:), pointer  :: xx 
  contains
  subroutine tcaf1_init ( xx_in )
    real ,   dimension (:), pointer :: xx_in 
    xx => xx_in
  end subroutine
  function tcaf1_lop ( adj, add, bb, yy) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: bb,yy 
    call adjnull (adj,add,bb,yy )
    call tcaf1_lop2(adj,add,bb,yy )
    stat=0
  end function 
  subroutine tcaf1_lop2(adj,add,bb,yy)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: bb 
    real, dimension (:)  :: yy 
    integer        x,   b,   y
    if ( size(yy) < size(xx) + size(bb) - 1 ) then
      call erexit('tcaf')
    end if
    do b= 1, size(bb)
      do x= 1, size(xx)
        y = x + b - 1
        if ( adj) then
          bb(b) =        bb(b) +  yy(y) * xx(x)
        else
          yy(y) =                 yy(y) +  bb(b) * xx(x)
        end if
      end do
    end do
  end subroutine 
  subroutine tcaf1_close()
  end subroutine 
end module 

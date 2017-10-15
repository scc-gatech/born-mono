
! Nearest-neighbor interpolation would do this:  data = model( 1.5 + (t-t0)/dt)
!                             This is likewise but with _linear_ interpolation.
module lint1  
  use classutil
  implicit none
  real, private :: o1 
  real, private :: d1 
  real ,private,  dimension (:), pointer  :: coordinate 
  contains
  subroutine lint1_init ( o1_in,d1_in,coordinate_in )
    real    :: o1_in 
    real    :: d1_in 
    real ,   dimension (:), pointer :: coordinate_in 
    o1 = o1_in 
    d1 = d1_in 
    coordinate => coordinate_in
  end subroutine
  function lint1_lop ( adj, add, mm, dd) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: mm,dd 
    call adjnull (adj,add,mm,dd )
    call lint1_lop2(adj,add,mm,dd )
    stat=0
  end function 
  subroutine lint1_lop2(adj,add,mm,dd)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: mm 
    real, dimension (:)  :: dd 
    integer i, im,  id
    real    f, fx,gx
    do id= 1, size(dd)
      f = (coordinate(id)-o1)/d1
      i=f  
      im= 1+i
      if ( 1<=im .and. im< size(mm)) then
        fx=f-i
        gx= 1.-fx
        if ( adj) then
          mm(im  ) =                        mm(im  ) +  gx * dd(id)
          mm(im+1) =                        mm(im+1) +  fx * dd(id)
        else
          dd(id)   =                        dd(id)   +  gx * mm(im)  +&
            &  fx * mm(im+1)
        end if
      end if
    end do
  end subroutine 
  subroutine lint1_close()
  end subroutine 
end module 

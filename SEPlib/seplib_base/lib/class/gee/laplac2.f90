module laplac2  
! Laplacian operator in 2-D
  use classutil
  use igrad2
  implicit none
  logical,   parameter, private   :: T = .true. 
  logical,   parameter, private   :: F = .false. 
  real ,private,  dimension (:), allocatable  :: tmp 
  contains
  subroutine laplac2_init ( m1,m2 )
    integer m1, m2
    if (.not. allocated(tmp)) then
      allocate(tmp ( m1*m2*2)) 
    end if
    call igrad2_init (m1, m2)
  end subroutine
  function laplac2_lop ( adj, add, x, y) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: x,y 
    call adjnull (adj,add,x,y )
    call laplac2_lop2(adj,add,x,y )
    stat=0
  end function 
  subroutine laplac2_lop2(adj,add,x,y)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: x 
    real, dimension (:)  :: y 
    integer stat1, stat2
    if ( adj) then
      stat1 = igrad2_lop (  F,    F, y, tmp)   ! tmp = grad y
      stat2 = igrad2_lop (  T,  add, x, tmp)   ! x = x + grad' tmp
    else
      stat1 = igrad2_lop (  F,    F, x, tmp)   ! tmp = grad x
      stat2 = igrad2_lop (  T,  add, y, tmp)   ! y = y + grad' tmp
    end if
  end subroutine 
  subroutine laplac2_close()
    deallocate( tmp )
  end subroutine 
end module 

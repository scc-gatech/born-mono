module mask1  
! masking operator
  use classutil
  implicit none
  logical ,private,  dimension( :), pointer  :: m 
  contains
  subroutine mask1_init ( m_in )
    logical ,   dimension( :), pointer :: m_in 
    m => m_in
  end subroutine
  function mask1_lop ( adj, add, x, y) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: x,y 
    call adjnull (adj,add,x,y )
    call mask1_lop2(adj,add,x,y )
    stat=0
  end function 
  subroutine mask1_lop2(adj,add,x,y)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: x 
    real, dimension (:)  :: y 
    if ( adj) then
      where( m)
        x = x + y
end where
    else
!
      where( m)
        y = y + x
end where
    end if
  end subroutine 
  subroutine mask1_close()
  end subroutine 
end module 

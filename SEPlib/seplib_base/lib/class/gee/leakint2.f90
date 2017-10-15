module leakint2  
  use classutil
  use leakint
  implicit none
  integer, private :: n1 
  integer, private :: n2 
  contains
  subroutine leakint2_init ( n1_in,n2_in,rho )
    integer    :: n1_in 
    integer    :: n2_in 
    real, intent (in) :: rho
    n1 = n1_in 
    n2 = n2_in 
    call leakint_init (rho)
  end subroutine
  function leakint2_lop ( adj, add, x, y) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: x,y 
    call adjnull (adj,add,x,y )
    call leakint2_lop2(adj,add,x,y )
    stat=0
  end function 
  subroutine leakint2_lop2(adj,add,x,y)
    logical,intent(in) :: adj,add 
    real, dimension (n1,n2)  :: x 
    real, dimension (n1,n2)  :: y 
    integer :: i2, stat1
    do i2 = 1, n2
      stat1 = leakint_lop (adj, .true., x (:,i2), y (:, i2))
    end do
  end subroutine 
  subroutine leakint2_close()
  end subroutine 
end module 

module leakint  
! leaky integration
  use classutil
  implicit none
  real, private :: rho 
  contains
  subroutine leakint_init ( rho_in )
    real    :: rho_in 
    rho = rho_in
  end subroutine
  function leakint_lop ( adj, add, xx, yy) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: xx,yy 
    call adjnull (adj,add,xx,yy )
    call leakint_lop2(adj,add,xx,yy )
    stat=0
  end function 
  subroutine leakint_lop2(adj,add,xx,yy)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: xx 
    real, dimension (:)  :: yy 
    integer i, n       
    real tt
    n = size (xx)
    tt = 0.
    if ( adj) then
      do i= n, 1, -1  
        tt = rho*tt + yy(i)
        xx(i)  =                xx(i)  +  tt
      end do
    else
      do i= 1, n  
        tt  = rho*tt + xx(i)
        yy(i)  =                yy(i)  +  tt
      end do
    end if
  end subroutine 
  subroutine leakint_close()
  end subroutine 
end module 

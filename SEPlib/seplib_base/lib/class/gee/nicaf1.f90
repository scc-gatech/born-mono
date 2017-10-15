module nicaf1  
! Nonstationary Internal Convolution, Adjoint is the Filter
  use classutil
  implicit none
  real ,private,     dimension (:), pointer  :: yy 
  integer, private :: n 
  integer, private :: na 
  contains
  subroutine nicaf1_init ( yy_in,n_in,na_in )
    real ,      dimension (:), pointer :: yy_in 
    integer    :: n_in 
    integer    :: na_in 
    yy => yy_in 
    n = n_in 
    na = na_in
  end subroutine
  function nicaf1_lop ( adj, add, aa, rr) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: aa,rr 
    call adjnull (adj,add,aa,rr )
    call nicaf1_lop2(adj,add,aa,rr )
    stat=0
  end function 
  subroutine nicaf1_lop2(adj,add,aa,rr)
    logical,intent(in) :: adj,add 
    real, dimension (n,na)  :: aa 
    real, dimension (n)  :: rr 
    integer a, r, y
    do a = 1, na  
      do r = na, n  
        y = r - a + 1 
        if ( adj) then
          aa (r, a) =           aa (r, a) + rr (r)    * yy (y)
        else
          rr (r)    =           rr (r)    + aa (r, a) * yy (y)
        end if
      end do
    end do
  end subroutine 
  subroutine nicaf1_close()
  end subroutine 
end module 

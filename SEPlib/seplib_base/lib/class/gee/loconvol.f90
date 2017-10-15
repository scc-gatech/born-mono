! successive invocations apply successive filters from a vector.
!                      (will fail a dot product test?  Oft used with patching.)
module loconvol  
  use classutil
  use helicon
  implicit none
  integer,   private   :: i 
  type( filter) ,private,  dimension(:), pointer  :: aa 
  contains
  subroutine loconvol_init ( aa_in )
    type( filter) ,   dimension(:), pointer :: aa_in 
    aa => aa_in 
    i = 0
  end subroutine
  function loconvol_lop ( adj, add, xx, yy) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: xx,yy 
    call adjnull (adj,add,xx,yy )
    call loconvol_lop2(adj,add,xx,yy )
    stat=0
  end function 
  subroutine loconvol_lop2(adj,add,xx,yy)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: xx 
    real, dimension (:)  :: yy 
    integer stat1
    i = i + 1
    call helicon_init( aa( i))
    stat1 = helicon_lop( adj, .false., xx, yy)
  end subroutine 
  subroutine loconvol_close()
  end subroutine 
end module 

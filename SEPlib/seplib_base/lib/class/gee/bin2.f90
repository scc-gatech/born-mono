module bin2  
! Data-push binning in 2-D.
  use classutil
  implicit none
  integer, private :: m1 
  integer, private :: m2 
  real, private :: o1 
  real, private :: d1 
  real, private :: o2 
  real, private :: d2 
  real ,private,  dimension (:,:), pointer  :: xy 
  contains
  subroutine bin2_init ( m1_in,m2_in,o1_in,d1_in,o2_in,d2_in,xy_in )
    integer    :: m1_in 
    integer    :: m2_in 
    real    :: o1_in 
    real    :: d1_in 
    real    :: o2_in 
    real    :: d2_in 
    real ,   dimension (:,:), pointer :: xy_in 
    m1 = m1_in 
    m2 = m2_in 
    o1 = o1_in 
    d1 = d1_in 
    o2 = o2_in 
    d2 = d2_in 
    xy => xy_in
  end subroutine
  function bin2_lop ( adj, add, mm, dd) result(stat)
    use classutil
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: mm,dd 
    call adjnull (adj,add,mm,dd )
    call bin2_lop2(adj,add,mm,dd )
    stat=0
  end function 
  subroutine bin2_lop2(adj,add,mm,dd)
    logical,intent(in) :: adj,add 
    real, dimension (m1,m2)  :: mm 
    real, dimension (:)  :: dd 
    integer    i1,i2, id
    do id=1,size(dd)
      i1 = 1.5 + (xy(id,1)-o1)/d1
      i2 = 1.5 + (xy(id,2)-o2)/d2
      if ( 1<=i1 .and. i1<=m1 .and.  1<=i2 .and. i2<=m2   ) then
        if ( adj) then
          mm(i1,i2) = mm(i1,i2) +  dd( id)
        else
          dd( id)   = dd( id)   +  mm(i1,i2)
        end if
      end if
    end do
  end subroutine 
  subroutine bin2_close()
  end subroutine 
end module 

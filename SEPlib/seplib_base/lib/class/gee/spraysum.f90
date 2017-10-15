module spraysum  
 use sep_mod
! Spray or sum over 1, 2, and/or 3-axis.
  use classutil
  implicit none
  integer, private :: n1 
  integer, private :: n2 
  integer, private :: n3 
  integer, private :: m1 
  integer, private :: m2 
  integer, private :: m3 
  contains
  subroutine spraysum_init ( n1_in,n2_in,n3_in,m1_in,m2_in,m3_in )
    integer    :: n1_in 
    integer    :: n2_in 
    integer    :: n3_in 
    integer    :: m1_in 
    integer    :: m2_in 
    integer    :: m3_in 
    n1 = n1_in 
    n2 = n2_in 
    n3 = n3_in 
    m1 = m1_in 
    m2 = m2_in 
    m3 = m3_in
  end subroutine
  function spraysum_lop ( adj, add, xx, yy) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: xx,yy 
    call adjnull (adj,add,xx,yy )
    call spraysum_lop2(adj,add,xx,yy )
    stat=0
  end function 
  subroutine spraysum_lop2(adj,add,xx,yy)
    logical,intent(in) :: adj,add 
    real, dimension (n1,n2,n3)  :: xx 
    real, dimension (m1,m2,m3)  :: yy 
    integer i1,i2,i3,  x1,x2,x3, y1,y2,y3
    if ( n1 .ne. 1  .and.  m1 .ne. 1  .and.  n1 .ne. m1) then
      call erexit('spraysum: n1,m1')
    end if
    if ( n2 .ne. 1  .and.  m2 .ne. 1  .and.  n2 .ne. m2) then
      call erexit('spraysum: n2,m2')
    end if
    if ( n3 .ne. 1  .and.  m3 .ne. 1  .and.  n3 .ne. m3) then
      call erexit('spraysum: n3,m3')
    end if
    do i3= 1, max0(n3,m3)
      x3= min0(i3,n3)
      y3= min0(i3,m3)
      do i2= 1, max0(n2,m2)
        x2= min0(i2,n2)
        y2= min0(i2,m2) 
        do i1= 1, max0(n1,m1)
          x1= min0(i1,n1)
          y1= min0(i1,m1) 
          if ( adj) then
            xx(x1,x2,x3)  =  xx(x1,x2,x3)  +  yy(y1,y2,y3)
          else
            yy(y1,y2,y3)  =       yy(y1,y2,y3)  +  xx(x1,x2,x3)
          end if
        end do
      end do
    end do
  end subroutine 
  subroutine spraysum_close()
  end subroutine 
end module 

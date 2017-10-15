module scaletrace  
  use classutil
  implicit none
  integer,   private                 :: n1 
  integer,   private                 :: n2 
  real ,private,  dimension( :, :), pointer  :: data 
  contains
  subroutine scaletrace_init ( data_in )
    real ,   dimension( :, :), pointer :: data_in 
    data => data_in 
    n1 = size( data, 1)
    n2 = size( data, 2)
  end subroutine
  function scaletrace_lop ( adj, add, scale, sdata) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: scale,sdata 
    call adjnull (adj,add,scale,sdata )
    call scaletrace_lop2(adj,add,scale,sdata )
    stat=0
  end function 
  subroutine scaletrace_lop2(adj,add,scale,sdata)
    logical,intent(in) :: adj,add 
    real, dimension ( n2)  :: scale 
    real, dimension ( n1, n2)  :: sdata 
    integer i1,i2
    do i2= 1, n2
      do i1= 1, n1
        if ( adj) then
          scale(   i2) =                scale(   i2) + sdata(i1,i2) *&
            & data(i1,i2)
        else
          sdata(i1,i2) =                sdata(i1,i2) + scale(   i2) *&
            & data(i1,i2)
        end if
      end do
    end do
  end subroutine 
  subroutine scaletrace_close()
  end subroutine 
end module 

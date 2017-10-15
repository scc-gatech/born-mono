module hypotenusei  
! Inverse normal moveout
  use classutil
  implicit none
  integer, private :: nt 
  integer ,private,  dimension (:), allocatable  :: iz 
  contains
  subroutine hypotenusei_init ( nt_in,t0,dt,xs )
    integer    :: nt_in 
    integer it
    real t0, dt, xs, t, zsquared
    nt = nt_in 
    if (.not. allocated(iz)) then
      allocate(iz ( nt)) 
    end if
    do it= 1, nt  
      t = t0 + dt*(it-1)
      zsquared =  t * t - xs * xs
      if ( zsquared >= 0.) then
        iz (it) = 1.5 + (sqrt( zsquared) - t0) /dt
      else
        iz (it) = 0
      end if
    end do
  end subroutine
  function hypotenusei_lop ( adj, add, zz, tt) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: zz,tt 
    call adjnull (adj,add,zz,tt )
    call hypotenusei_lop2(adj,add,zz,tt )
    stat=0
  end function 
  subroutine hypotenusei_lop2(adj,add,zz,tt)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: zz 
    real, dimension (:)  :: tt 
    integer   it
    do it= 1, nt  
      if ( iz (it) > 0 ) then
        if ( adj) then
          zz( iz(it))  =   zz( iz(it))  +  tt(    it )
        else
          tt(it)       =        tt(it)       +  zz( iz(it))
        end if
      end if
    end do
  end subroutine 
  subroutine hypotenusei_close()
    deallocate( iz )
  end subroutine 
end module 

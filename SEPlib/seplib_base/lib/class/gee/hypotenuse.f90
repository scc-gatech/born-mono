module hypotenuse  
! Normal moveout
  use classutil
  implicit none
  real, private :: t0 
  real, private :: dt 
  real, private :: xs 
  contains
  subroutine hypotenuse_init ( t0_in,dt_in,xs_in )
    real    :: t0_in 
    real    :: dt_in 
    real    :: xs_in 
    t0 = t0_in 
    dt = dt_in 
    xs = xs_in
  end subroutine
  function hypotenuse_lop ( adj, add, zz, tt) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: zz,tt 
    call adjnull (adj,add,zz,tt )
    call hypotenuse_lop2(adj,add,zz,tt )
    stat=0
  end function 
  subroutine hypotenuse_lop2(adj,add,zz,tt)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: zz 
    real, dimension (:)  :: tt 
    integer   it, iz
    real t, zsquared
    do it= 1, size(tt)
      t = t0 + dt*(it-1)
      zsquared =  t * t - xs * xs
      if ( zsquared >= 0.) then
        iz = 1.5 + (sqrt( zsquared) - t0) /dt
        if ( iz > 0 ) then
          if ( adj) then
            zz(iz) =    zz(iz) +  tt(it)
          else
            tt(it) =         tt(it) +  zz(iz)
          end if
        end if
      end if
    end do
  end subroutine 
  subroutine hypotenuse_close()
  end subroutine 
end module 

subroutine imo1( adj, add, xs, t0, dt, nt, zz,     tt )
use classutil
integer          adj, add,             nt,                            &
  &  it, iz
real                            t0, dt,    zz(nt), tt(nt),    t, xs,&
  & zsquared
call adjnull(    adj, add,                 zz,nt,  tt,nt)
do it= 1, nt  
  t = t0 + dt*(it-1)
  zsquared =  t * t - xs * xs
  if ( zsquared >= 0.) then
    iz = 1.5 + (sqrt( zsquared) - t0) /dt
    if ( iz > 0 ) then
      if ( adj .eq. 0 ) then
        tt(it) = tt(it) + zz(iz)
      else
        zz(iz) = zz(iz) + tt(it)
      end if
    end if
  end if
end do 
return
end  

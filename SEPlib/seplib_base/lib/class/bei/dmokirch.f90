subroutine dmokirch( adj, add, velhalf, h, t0,dt,dx, modl,nt,nx, data)
use sep_func_mod
integer ix,iz,it,ib, adj, add,                            nt,nx
real    amp,t,z,b,             velhalf, h, t0,dt,dx, modl(nt,nx),data&
  &(nt,nx)
call adjnull(        adj, add,                       modl,nt*nx, data&
  &,nt*nx)
if ( h .eq. 0) then
  call erexit('h=0')
end if
do ib= -nx, nx  
  b = dx * ib             ! b = midpt separation
  do iz= 2, nt  
    z = t0 + dt * (iz-1)    ! z = zero-offset time
    if ( h**2 <= b**2 ) then
      cycle
    end if
    t= sqrt(  z**2 / (1-b**2/h**2) )
    amp= sqrt(t) * dx/h
    if ( velhalf*abs(b) * t*t > h**2*z) then
      exit
    end if
    it = 1.5 + (t - t0) / dt
    if ( it > nt ) then
      exit
    end if
    do ix= max0(1, 1-ib),  min0(nx, nx-ib)
      if ( adj .eq. 0 ) then
        data(it,ix+ib) = data(it,ix+ib) + modl(iz,ix   ) * amp
      else
        modl(iz,ix   ) = modl(iz,ix   ) + data(it,ix+ib) * amp
      end if
    end do
  end do
end do 
return
end  

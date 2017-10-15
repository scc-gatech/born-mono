! inverse moveout and spray into a gather.
!
subroutine imospray( adj, add, slow, x0,dx, t0,dt, nx,nt, stack, gather&
  &)
use classutil
integer ix,          adj, add,                     nx,nt
real xs,                       slow, x0,dx, t0,dt,    stack(nt), gather&
  &( nt,nx)
call adjnull(        adj, add,                        stack,nt,  gather&
  &, nt*nx)
do ix= 1, nx  
  xs = (x0 + dx * (ix-1)) * slow
  call imo1( adj, 1, xs, t0, dt, nt,            stack,     gather(1,ix&
    &))
end do 
return
end  

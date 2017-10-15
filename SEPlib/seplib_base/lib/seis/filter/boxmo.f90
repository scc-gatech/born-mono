!<
! boxmo
!
! USAGE
! call boxmo(adj,add,t0,dt,dx,nt,slow,antialia,zz,tt)
!
!
! INPUT PARAMETERS
! adj - integer 1-adjoint, 0-forward
! add - integer 1-add to current values, 0 -erase output
! t0  - real    initial time
! dt  - real    sampling in time
! dx  - real    samplin in x
! nt  - integer number of time samples
! slow - real(nt) slowness
! antialias -real antialias???
! zz   - real(nt)
! tt   - real(nt)
!
!
! DESCRIPTION
! Box
!
! CATEGORY
! Lib:Filter
!
! COMPILE LEVEL
! DISTR
!>
subroutine boxmo( adj, add, t0,dt, dx, x, nt,slow,  antialias, zz,    &
  & tt   )
integer it,iz,itp,adj, add,               nt
real t, tp, z, amp,         t0,dt, dx, x, slow(nt), antialias, zz(nt),&
  & tt(nt)
real ss(nt)
ss(1:nt)=0
call adjnull( adj, add,        zz,nt,  tt,nt)
if ( adj .ne. 0) then
  call causint77(     1,   0, nt, ss, tt)
end if
do iz= 2, nt  
  z = t0 + dt*(iz-1)
  t = sqrt( z**2 + (slow(iz)* abs(x)         )**2 )
  it = 1.5 + (t -t0)/dt
  tp= sqrt( z**2 + (slow(iz)*(abs(x)+abs(dx)))**2 )
  tp = t + antialias * (tp - t) + dt
  itp= 1.5 + (tp-t0)/dt
  amp =  sqrt( nt*dt/t) * z/t / (itp - it)
  if ( itp < nt   ) then
    if ( adj .eq. 0 ) then
      ss(it ) = ss(it ) + amp * zz(iz)
      ss(itp) = ss(itp) - amp * zz(iz)
    else
      zz(iz) = zz(iz)  + amp * ss(it )
      zz(iz) = zz(iz)  - amp * ss(itp)
    end if
  end if
end do 
if ( adj .eq. 0) then
  call causint77(     0,  add, nt, ss, tt)
end if
return
end
subroutine boxstack( adj,add,slow,antialias, t0,dt,x0,dx,nt,nx, stack,&
  & gather)
integer adj, add, ix, nx, nt
real x,                slow(nt),antialias, t0,dt,x0,dx, stack(nt), gather&
  &(nt,nx)
call adjnull( adj, add, stack, nt, gather, nt*nx)
do ix= 1, nx  
  x =  x0 + dx * (ix-1)
  call boxmo( adj,1, t0,dt,dx,x,nt, slow,antialias, stack, gather(1,ix&
    &))
end do 
return
end  

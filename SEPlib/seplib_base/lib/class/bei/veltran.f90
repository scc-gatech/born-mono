
! veltran --- velocity transform with anti-aliasing and sqrt(-i omega)
!
subroutine veltran(adj,add,psun,s02,anti,t0,dt,x0,dx,s0,ds,nt,nx,ns,model&
  &,data)
integer it,ix,is,  adj,add,psun,                           nt,nx,ns
real x, s, wt,               s02,anti,t0,dt,x0,dx,s0,ds,model(nt,ns)&
  &,data(nt,nx)
real slow(nt), half(nt,nx)
call null(               half,nt*nx)
call adjnull(      adj,add,                             model,nt*ns,&
  & data,nt*nx)
if ( adj .ne. 0) then
  do ix = 1, nx
    call halfdifa( adj,   0, nt, half(1, ix), data(1, ix) )
  end do
end if
do is= 1, ns  
  s = s0 + (is-1) * ds
  do it=1,nt  
    slow(it) = s
  end do 
  do ix= 1, nx  
    x = x0 + (ix-1) * dx
    if ( psun .eq. 2 ) then
      wt =       abs( s * x)
! vel tran
    else if ( psun .eq. 1 ) then
      wt = sqrt( abs( s * x))
! pseudounitary
    else
      wt = 1.
    end if 
! modeling
    call trimo( adj, 1, t0,dt,dx, x, nt,slow, s02,                    &
      &     wt , anti,  model(1,is),  half(1,ix))
  end do
end do 
if ( adj .eq. 0) then
  do ix = 1, nx
    call halfdifa( adj, add, nt, half(1, ix), data(1, ix) )
  end do
end if
return
end  

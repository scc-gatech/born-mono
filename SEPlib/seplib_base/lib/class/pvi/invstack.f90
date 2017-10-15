! NMO stack by inverse of forward modeling
!
subroutine invstack(  nt,model,nx,gather,rr,t0,x0,dt,dx,slow,niter)
integer it, ix, iter, nt,      nx,                           niter
real    t0,x0,dt,dx,slow, gather(nt,nx), rr(nt,nx), model(nt)
real  dmodel(nt), smodel(nt), dr(nt,nx), sr(nt,nx)
do it= 1, nt
  model(it) = 0.0 
end do 
do it= 1, nt
  do ix= 1, nx
    rr(it,ix) = gather(it,ix)
  end do
end do 
do iter = 0, niter  
  call imospray( 1,0,slow,x0,dx,t0,dt,nx,nt,dmodel,rr)    ! nmo-stack
  call imospray( 0,0,slow,x0,dx,t0,dt,nx,nt,dmodel,dr)    ! modeling
  call cgstep(iter,   nt, model, dmodel, smodel,                          nt&
    &*nx, rr,    dr,     sr)
end do 
return
end  

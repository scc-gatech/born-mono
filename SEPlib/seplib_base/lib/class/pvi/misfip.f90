
! MISFIP --- find MISsing peF and Input data on 1-axis using Prior data.
!
subroutine misfip( nt,tt, na,aa, np,pp,known, niter)
integer nt, na,  ip,np,  npa, nta, nx,nr, iter,niter,  ax, px, qr, tr
real    pp(np), known(np), aa(na)       ! same as in missif()
real    tt(nt)                          
! input: prior training data set.
real   x(np+na),             g(np+na),             s(np+na)
real  rr(np+na-1 +na+nt-1), gg(np+na-1 +na+nt-1), ss(np+na-1 +na+nt-1)
npa= np+na-1
nta= nt+na-1            ! lengths of outputs of filtering
nx = np+na
nr= npa+nta             ! length of unknowns and residuals
px=1
qr=1
ax=1+np
tr=1+npa        ! pointers
call zero( na, aa)
aa(1) = 1.
call copy( np,  pp, x(px))
call copy( na,  aa, x(ax))
do iter= 0, niter  
  call contran( 0, 0, na,aa, np, pp,    rr(qr))
  call contran( 0, 0, na,aa, nt, tt,    rr(tr))  
  ! extend rr with train
  call scaleit( -1.,           nr,        rr    )
  call contran( 1, 0, na,aa, np, g(px), rr(qr))
  call contran( 1, 0, np,pp, na, g(ax), rr(qr))
  call contran( 1, 1, nt,tt, na, g(ax), rr(tr))
  do ip= 1, np  
    if ( known(ip) .ne. 0) then
      g( ip+(px-1)) = 0.
    end if
  end do 
  g( 1 +(ax-1)) = 0.
  call contran( 0, 0, na,aa, np, g(px), gg(qr))
  call contran( 0, 1, np,pp, na, g(ax), gg(qr))
  call contran( 0, 0, nt,tt, na, g(ax), gg(tr))
  call cgstep( iter,  nx, x, g, s,  nr, rr, gg, ss)
  call copy( np, x(px), pp)
  call copy( na, x(ax), aa)
end do 
return
end  

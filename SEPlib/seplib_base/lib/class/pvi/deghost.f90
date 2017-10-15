
! deghost:                   min       |rrtop|  =  | y - bb (contrunc) xx |
!                             x        |rrbot|     | 0 - epsilon  I    xx |
subroutine deghost( eps, nb,bb, n, yy, xx, rr, niter)
integer iter,            nb,    n,             niter
real    bb(nb), yy(n), eps              
! inputs.  typically bb=(1,-2,1)
real    xx(n), rr(n+n)                  ! outputs.
real dx(n), sx(n), dr(n+n), sr(n+n)
call zero( n, xx)
call copy( n, yy, rr(1  ))        ! top half of residual
call zero( n    , rr(1+n))        ! bottom   of residual
do iter= 0, niter  
  call contrunc(1,0,1,nb,bb, n,dx,n,rr)
  call ident(1,1,eps, n,dx,rr(1+n))
  call contrunc(0,0,1,nb,bb, n,dx,n,dr)
  call ident(0,0,eps, n,dx,dr(1+n))
  call cgstep( iter,  n,xx,dx,sx,                           n+n,rr,dr&
    &,sr)
end do 
return
end  

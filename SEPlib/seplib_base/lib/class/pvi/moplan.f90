! mono plane annihilator and its residual
!
subroutine moplan( agc, eps, data,n1,n2, niter, lag1,lag2, gap, aa,a1&
  &,a2, rr)
integer            agc,           n1,n2, niter, lag1,lag2, gap,    a1&
  &,a2
real    dot, lam,       eps, data(n1,n2),            aa(a1,a2),rr(n1*n2&
  &*2+a1*a2)
integer i1, a12, r12, iter
real da( a1,a2),  dr( n1*n2*2+a1*a2)
real sa( a1,a2),  sr( n1*n2*2+a1*a2)
a12 = a1 * a2
r12 = n1 * n2 * 2
call null( rr, r12+a12)
call null( aa, a12)
aa( lag1, lag2 ) = 1.
call null( dr, r12+a12)
call null( da, a12)
call         burg2 ( 0, 0, lag1,lag2, data,n1,n2, aa,a1,a2, rr       )
lam =  eps * sqrt( dot( r12, rr, rr))
call         ident ( 0, 0, lam,     a12,          aa,       rr(r12+1))
call         scaleit (    -1.,    r12+a12,                    rr      &
  & )
do iter= 0, niter  
  call burg2 ( 1, 0, lag1,lag2, data,n1,n2, da,a1,a2, rr       )
  call ident ( 1, 1, lam,     a12,          da,       rr(r12+1))
  do i1= 1, min0( a1, lag1+gap-1)
    da(i1,lag2)= 0.
  end do 
  call burg2 ( 0, 0, lag1,lag2, data,n1,n2, da,a1,a2, dr       )
  call ident ( 0, 0, lam,     a12,          da,       dr(r12+1))
  call cgstep( iter,  a12 , aa,da,sa,                         a12+r12 &
    &, rr,dr,sr )
end do 
if ( agc>0) then
  call scaleit( 1./sqrt(dot(r12,rr,rr)), a1*a2,aa)
end if
call         burg2 ( 0, 0, lag1,lag2, data,n1,n2, aa,a1,a2, rr)
return
end  

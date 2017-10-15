! Find spatial prediction-error filter.
!
subroutine pe2( eps, a1,a2,aa, n1,n2 ,pp, rr, niter, jump)
integer              a1,a2,    n1,n2,         niter, jump
integer i1, iter, midpt, r12, a12
real    aa( a1 , a2),  pp( n1 , n2),  rr( n1 , n2 * 2),  eps
real da( a1, a2),  dr( n1, n2 * 2)
real sa( a1, a2),  sr( n1, n2 * 2)
r12 = n1 * n2
a12 = a1 * a2
call null( aa, a12)
call null( rr, 2 * r12)
call null( da, a12)
call null( dr, 2 * r12)
midpt = (a1+1) / 2
aa( midpt, 1 ) = 1.
call cinjof( 0, 0, jump, n1,n2,pp, a1,a2,aa, rr         )
call ident ( 0, 0, eps,            a12,  aa, rr(1,n2+1) )
call scaleit (       -1.,          2*r12,      rr         )
do iter= 0, niter  
  call cinjof( 1, 0, jump, n1,n2,pp, a1,a2,da, rr         )
  call ident ( 1, 1,  eps,           a12,  da, rr(1,n2+1) )
  do i1= 1, a1  
    da(i1, 1) = 0.
  end do 
  call cinjof( 0, 0, jump, n1,n2,pp, a1,a2,da, dr         )
  call ident ( 0, 0,  eps,           a12,  da, dr(1,n2+1) )
  call cgstep( iter,   a12, aa,da,sa,                            2*r12&
    &, rr,dr,sr )
end do 
return
end  

! lomoplan  ---- LOcal MOno PLane ANnihilate
!
subroutine lomoplan( agc, eps,  w1,w2,data,n1,n2, resid, gap,  aa,a1,a2&
  &,k1,k2)
integer i1,i2,lag1,lag2,agc,w1,w2,         n1,n2,        gap,     a1,a2&
  &,k1,k2
real dot,                 eps,        data(n1,n2),resid(n1,n2),aa(a1,a2&
  &,k1,k2)
integer niter
real pdata(w1,w2), dpres (w1*w2*2+a1*a2), windwt(w1,w2), wallwt(n1,n2)
call null(                                        resid, n1*n2)
niter= a1 + a1/2
lag1 = (a1+1)/2
lag2 = 1
call cinloiwt( a1,a2, lag1,lag2, windwt, w1,w2)
call mkwallwt( k1,k2,            windwt, w1,w2, wallwt, n1,n2)
do i2= 1, k2  
  do i1= 1, k1  
    call patch( 0, 0, i1,i2, k1,k2, data,     n1,n2, pdata, w1,w2)
    call moplan( agc, eps,         pdata,     w1,w2,              niter&
      &, lag1,lag2, gap, aa(1,1,i1,i2),a1,a2, dpres       )
    call diag( 0, 0, windwt, w1*w2, dpres,           dpres       )
    call patch( 1, 1, i1,i2, k1,k2, resid,    n1,n2, dpres, w1,w2)
  end do
end do 
call diag  ( 0, 0, wallwt, n1*n2,       resid, resid )
return
end  

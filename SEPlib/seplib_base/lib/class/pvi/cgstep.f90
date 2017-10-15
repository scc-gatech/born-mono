! A step of conjugate-gradient descent.
!
subroutine cgstep( iter,   n, x, g, s,   m, rr, gg, ss)
use sep_func_mod
integer i,         iter,   n,            m
real    x(n), rr(m)     ! solution, residual
real    g(n), gg(m)     ! gradient, conjugate gradient
real    s(n), ss(m)     ! step,     conjugate step
real dot, sds, gdg, gds, determ, gdr, sdr, alfa, beta
if ( iter .eq. 0 ) then
  do i= 1, n
    s(i) = 0.
  end do 
  do i= 1, m
    ss(i) = 0.
  end do 
  if ( dot(m,gg,gg).eq.0 ) then
    call erexit('cgstep: grad vanishes identically')
  end if
  alfa =  dot(m,gg,rr) / dot(m,gg,gg)
  beta = 0.
else
! search plane by solving 2-by-2
  gdg = dot(m,gg,gg)      !  G . (R - G*alfa - S*beta) = 0
  sds = dot(m,ss,ss)      !  S . (R - G*alfa - S*beta) = 0
  gds = dot(m,gg,ss)
  determ = gdg * sds - gds * gds + (.00001 * (gdg * sds) + 1.e-15)
  gdr = dot(m,gg,rr)
  sdr = dot(m,ss,rr)
  alfa = ( sds * gdr - gds * sdr ) / determ
  beta = (-gds * gdr + gdg * sdr ) / determ
end if 
do i= 1, n                                      ! s = model step
  s(i)  = alfa * g(i)  + beta * s(i)
end do 
do i= 1, m                                      ! ss = conjugate
  ss(i) = alfa * gg(i) + beta * ss(i)
end do 
do i= 1, n                                      ! update solution
  x(i)  =  x(i) +  s(i)
end do 
do i= 1, m                                      ! update residual
  rr(i) = rr(i) - ss(i)
end do 
return
end
real function dot( n, x, y )
integer i, n
real val, x(n), y(n)
val = 0.
do i=1,n   
  val = val + x(i) * y(i)
end do 
dot = val
return
end  

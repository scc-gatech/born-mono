! Half order causal derivative.  OK to equiv(xx,yy)
!
subroutine halfdifa( adj, add, n, xx,    yy  )
use sep_func_mod
integer n2, i,       adj, add, n
real omega,                       xx(n), yy(n)
complex cz, cv(4096)
n2=1
do while (n2<n)
  n2=2*n2
end do 
if ( n2 > 4096) then
  call erexit('halfdif memory')
end if
do i= 1, n2  
  cv(i) = 0.
end do 
do i= 1, n
  if ( adj .eq. 0) then
    cv(i) = xx(i)
  else
    cv(i) = yy(i)
  end if
end do 
call adjnull(       adj, add,    xx,n,  yy,n)
call ftu( +1., n2, cv)
do i= 1, n2  
  omega = (i-1.) * 2.*3.14159265 / n2
  cz = csqrt( 1. - cexp( cmplx( 0., omega)))
  if ( adj .ne. 0) then
    cz = conjg( cz)
  end if
  cv(i) = cv(i) * cz
end do 
call ftu( -1., n2, cv)
do i= 1, n
  if ( adj .eq. 0) then
    yy(i) = yy(i) + cv(i)
  else
    xx(i) = xx(i) + cv(i)
  end if
end do 
return
end  
subroutine halfdif( adj, n, xx, yy )
integer adj, n
real xx(n), yy(n)
integer add
add = 0
call halfdifa( adj, add, n, xx, yy )
return
end

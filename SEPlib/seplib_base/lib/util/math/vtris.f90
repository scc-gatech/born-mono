!<
! vtris
!
! USAGE
! call vtris(n,a,b,c,d,t)
!
! INPUT PARAMETERS
! n   -  integer size
! a   -  real(n) ???
! b   -  real(n) ???
! c   -  real(n) ???
! d   -  real(n) ???
! t   -  real(n) ???
!
! DESCRIPTION
! variable coefficient tridiagonal solver from FGDP.
!
! CATEGORY
! Lib:Math
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine vtris(n, a, b, c, d, t)
use sep_mod
integer n, k
real a(n), b(n), c(n), t(n), d(n), e(4096), f(4096), temp
if (n>4096) then
  call erexit("vtris problem n>4096, edit code")
end if
if ( b(1) .ne. 0. ) then
  e(1) = - a(1) / b(1)
else
  call erexit('vtris divisor b(1) vanishes')
end if 
do k = 2, n-1  
  temp = ( b(k) + c(k-1) * e(k-1) )
  if ( temp .eq. 0. ) then
!                call putch( 'problem in vtris k', 'i', k )
!                call putch( 'problem in vtris n', 'i', n )
!                call putch( 'problem in vtris b(k)', 'f', b(k) )
!                call putch( 'problem in vtris c(k)', 'f', c(k) )
!                call putch( 'problem in vtris c(k-1)', 'f', c(k-1) )
!                call putch( 'problem in vtris e(k-1)', 'f', e(k-1) )
    call erexit(' vtris:  zero divisor')
  else
    e(k) = - a(k) / temp
  end if
end do 
f(1) =  d(1) / b(1)
do k = 2, n  
  temp = ( b(k) + c(k-1) * e(k-1) )
  if ( temp .eq. 0. ) then
!                call putch( 'Problem in vtris k', 'i', k )
!                call putch( 'Problem in vtris n', 'i', n )
!                call putch( 'Problem in vtris b(k)', 'f', b(k) )
!                call putch( 'Problem in vtris c(k)', 'f', c(k) )
!                call putch( 'Problem in vtris c(k-1)', 'f', c(k-1) )
!                call putch( 'Problem in vtris e(k-1)', 'f', e(k-1) )
!                f(k) = 0.
    call erexit(' Vtris:  zero divisor')
  else
    f(k) = ( d(k) - c(k-1) * f(k-1) ) / temp
  end if
end do 
t(n) = f(n)
do k = n-1, 1, -1
  t(k) = e(k) * t(k+1) + f(k)
end do 
return
end
! FGDP:                C(K  ) * T(K-1) + B(K) * T(K) + A(K) * T(K+1) = D(K)
! here:                c(K-1) * T(K-1) + B(K) * T(K) + A(K) * T(K+1) = D(K)
! So for a symmetric matrix:   c(K) = A(K)
  

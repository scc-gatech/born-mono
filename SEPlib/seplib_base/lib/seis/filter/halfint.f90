!<
! halfint
!
! USAGE
! call halfint(conj,inv,n,rho,x,y)
!
! DESCRIPTION
! halfint -- half-order integral filter with conjugate and inverse.
!
! INPUT PARAMETERS
! conj  - integer   =0,causal ; 1=anti-causal
! inv   - integer   =0 half-order integrator =1 differentiator
! n     - integer   size
! rho   - real      rho
! x     - real(x)   model
! y     - real(y)   data
!
! CATEGORY
! Lib:Filter
!
! COMPILE LEVEL
! DISTR
!>
subroutine halfint ( conj, inv, n, rho, x, y )
integer conj, inv, i, n, nn
real rho, x(n), y(n), om
complex, allocatable, dimension(:) :: cx, cf
complex cz

nn = 1
do while ( nn < n )
  nn = nn*2
end do 

allocate(cx(nn),cf(nn))

do i=1,nn
  cx(i) = 0.
end do 
do i=1,n
  if ( conj .eq. inv ) then
    cx(i) = x(i)
  else
    cx(i) = y(i)
  end if
end do 
call fft( nn, cx, +1., 1./sqrt(1.*nn))
do i=1,nn  
  om = (i-1) * 2.*3.14159265/nn
  if ( conj .eq. 1) then
! replace Z by 1/Z
    om = - om
  end if
  cz = cexp(cmplx(0.,om))
  if (inv .eq. 0 ) then
    cf(i) = csqrt( .5 * (1.+rho*cz) / (1.-rho*cz) )
  else
    cf(i) = csqrt( 1.-rho*cz )
  end if
end do 
do i=1,nn
  cx(i) = cx(i) * cf(i)
end do 
call fft( nn, cx, -1., 1./sqrt(1.*nn))
do i=1,n
  if ( conj .eq. inv ) then
    y(i) = cx(i)
  else
    x(i) = cx(i)
  end if
end do 
return
end
subroutine fft( n, c, signi, scale)
! As originally uncoverered by stew somewhere, and adapted by jfc.
!subroutine stock( signi, m, c, work )
!
!         the stockham auto-sort fft
!
! Stew says this is three times faster than FGDP's fork.
! assembly language gives you a factor of two.
! trig tables       gives you a factor of two.
integer i,j,n,ls,ns,a
complex work(n)
complex c(1)        !,work(1)
logical iswap
real scale,signi
do i=1,n
  c(i) = c(i) * scale
end do 
iswap = .false.
ls = 1
a=1
do while (a.eq.1)
  ns = n/(ls+ls)
  if (iswap) then
    call stock1(signi,ls,ns,work,c)
  else
    call stock1(signi,ls,ns,c,work)
  end if 
  iswap = .not. iswap
  ls = ls+ls
  if (ls>=n) then
    exit
  end if
end do 
!
! copy back to input array if we end up swapped around
!
if (iswap) then
  do i = 1,n
    c(i) = work(i)
  end do
end if
return
end
subroutine stock1(signi,ls,ns,c,ch)
integer i,n,ls,j,ns
complex omega,omegk,wyk,c(ns,2,ls),ch(ns,ls,2)
real angle,signi
angle = signi*4.*atan(1.)/float(ls)
omega = cmplx(cos(angle),sin(angle))
if (ls<ns) then
  omegk = 1.
  do i = 1,ls  
    do j = 1,ns  
      wyk = omegk*c(j,2,i)
      ch(j,i,1) = c(j,1,i)+wyk
      ch(j,i,2) = c(j,1,i)-wyk
    end do 
    omegk = omega*omegk
  end do
else
  do j = 1,ns  
    omegk = 1.
    do i = 1,ls  
      wyk = omegk*c(j,2,i)
      ch(j,i,1) = c(j,1,i)+wyk
      ch(j,i,2) = c(j,1,i)-wyk
      omegk = omega*omegk
    end do
  end do
end if 
return
end  

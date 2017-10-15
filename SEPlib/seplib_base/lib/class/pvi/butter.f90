
! Find the numerator and denominator Z-transforms of the Butterworth filter.
!   hilo={1.,-1.} for {low,high}-pass filter
!   cutoff in Nyquist units, i.e. cutoff=1 for (1,-1,1,-1...)
!
subroutine butter( hilo, cutoff, npoly, num, den)
integer npoly, nn, nw, i
real hilo, cutoff, num(npoly), den(npoly), arg, tancut, pi
complex cx(2048)
pi = 3.14159265
nw=2048
nn = npoly - 1
tancut = 2. * tan( cutoff*pi/2. )
do i= 1, nw  
  arg = (2. * pi * (i-1.) / nw) / 2.
  if ( hilo > 0. ) then
! low-pass filter
    cx(i) = (2.*cos(arg)             ) **(2*nn) +(2.*sin(arg) * 2./tancut&
      & ) **(2*nn)
  else
! high-pass filter
    cx(i) = (2.*sin(arg)             ) **(2*nn) +(2.*cos(arg) * tancut&
      &/2. ) **(2*nn)
  end if
end do 
call kolmogoroff( nw, cx)       ! spectral factorization
do i= 1, npoly
  den(i) = cx(i)
end do 
do i= 1, nw                     ! numerator
  cx(i) = (1. + hilo * cexp( cmplx(0., 2.*pi*(i-1.)/nw))) ** nn
end do 
call ftu( -1., nw, cx)
do i= 1, npoly
  num(i) = cx(i)
end do 
return
end  

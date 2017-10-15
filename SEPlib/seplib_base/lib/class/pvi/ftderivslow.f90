subroutine ftderivslow( nyq, t0,dt, ntf, ctt, cdd)
integer nyq, ntf, ie
real t0,dt,f0,df, freq
complex ctt(ntf), cdd(ntf), cmplx
complex cff(ntf)
call slowft( 0, 0, nyq, t0, dt, ntf, ctt, f0, df, ntf, cff)
do ie= 1, ntf  
  freq= f0+(ie-1)*df
  cff(ie) = cff(ie) *  cmplx( 0., - 2. * 3.141549265 * freq )
end do 
if ( nyq > 0 ) then
!  if( omega0 == -pi/dt)
  cff(1) = 0.
end if
call slowft( 1, 0, nyq, t0, dt, ntf, cdd, f0, df, ntf, cff)
return
end  

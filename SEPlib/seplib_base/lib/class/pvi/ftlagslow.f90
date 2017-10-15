subroutine ftlagslow( nyq, lag, t0,dt, n1, ctt)
integer nyq, n1, ie
real lag, t0, dt, f0, df, freq
complex ctt(n1), cexp, cmplx
complex cff(n1)
call slowft( 0, 0, nyq, t0, dt, n1, ctt, f0, df, n1, cff)
do ie= 1, n1  
  freq= f0 + (ie-1)*df
  if ( ie.eq.1  .and.  nyq > 0) then
    cff(1)  = cff(1)  * cos(            2.*3.14159265 * freq * lag )
  else
    cff(ie) = cff(ie) * cexp( cmplx(0., 2.*3.14159265 * freq * lag))
  end if
end do 
call slowft( 1, 0, nyq, t0, dt, n1, ctt, f0, df, n1, cff)
return
end  

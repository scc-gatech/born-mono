!       Convolve, shift, and truncate output.
!
subroutine contrunc( conj, add, lag,  np,pp,  nf,ff,  nq,qq)
integer ns,          conj, add, lag,  np,     nf,     nq
real    pp(np)                          ! input data
real    ff(nf)                          ! filter (output at ff(lag))
real    qq(nq)                          ! filtered data
real ss( np+nf-1)
ns = np + nf - 1
if ( conj .eq. 0 ) then
  call contran( 0, 0, np,pp, nf,ff, ss)
  call advance( 0, add, lag-1, ns,ss, nq,qq)
else
  call advance( 1,   0, lag-1, ns,ss, nq,qq)
  call contran( 1, add, np,pp, nf,ff, ss)
end if 
return
end  

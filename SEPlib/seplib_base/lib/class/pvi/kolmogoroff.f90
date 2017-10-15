subroutine kolmogoroff( n, cx)  ! Spectral factorization.
integer i,              n       ! input:  cx = spectrum
complex cx(n)                   ! output: cx = min phase wavelet
do i= 1, n
  cx(i) = clog( cx(i) )
end do 
call ftu( -1., n, cx)
call scaleit( sqrt(1./n), 2*n, cx)
cx(1    ) = cx(1    ) / 2.
cx(1+n/2) = cx(1+n/2) / 2.
do i= 1+n/2+1, n
  cx(i) = 0.
end do 
call ftu( +1., n, cx)
call scaleit( sqrt(1.*n), 2*n, cx)
do i= 1, n
  cx(i) = cexp( cx(i))
end do 
call ftu( -1., n, cx)
call scaleit( sqrt(1./n), 2*n, cx)
return
end  

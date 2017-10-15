!       convolution:    Y(Z) = X(Z) * B(Z)
!
subroutine convolve( nb, bb, nx, xx, yy )
integer nb      ! number of coefficients in filter
integer nx      ! number of coefficients in input
! number of coefficients in output will be nx+nb-1
real    bb(nb)  ! filter coefficients
real    xx(nx)  ! input trace
real    yy(1)   ! output trace
integer ib, ix, iy, ny
ny = nx + nb -1
call null( yy, ny)
do ib= 1, nb
  do ix= 1, nx
    yy( ix+ib-1) = yy( ix+ib-1) + xx(ix) * bb(ib)
  end do
end do 
return
end  

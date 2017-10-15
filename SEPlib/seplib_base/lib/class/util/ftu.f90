subroutine ftu( signi, nx, cx )
!
 use, intrinsic :: iso_c_binding
!
!use sep_func_mod
!   complex fourier transform with unitary scaling
!
!               1         nx          signi*2*pi*i*(j-1)*(k-1)/nx
!   cx(k)  =  -------- * sum cx(j) * e
!             sqrt(nx)   j=1             for k=1,2,...,nx=2**integer
!
integer nx, i, j, k, m, istep, pad2
real    signi, scale, arg
complex cx(nx), cmplx, cw, cdel, ct
!
 include 'fftw3.f03'
!
type(C_PTR) :: plan
!
if (signi > 0.0) then
  j=1
else
  j=-1
end if 
plan=fftwf_plan_dft_1d(nx,cx,cx,j, FFTW_ESTIMATE)
!
call fftwf_execute_dft(plan,cx,cx)
scale = 1. / sqrt( 1.*nx)
do i= 1, nx
  cx(i) = cx(i) * scale
end do 
!
call fftwf_destroy_plan(plan)
!
return
end  

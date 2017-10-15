!<
! ft1axis
!
! Usage
! call ft1axis(adj,sign1,n1,n2,cx)
!
! Description
! 1D Fourier transform on a 2D data set along the 1-axis
!
! Input Parameters
! adj - integer 1 adjoint, 0 forward
! sign - real  sign for fft
! n1   - integer number of elemnts fast axis
! n2   - integer number of elements second axis
! cx   - complex data
!
! CATEGORY
! Lib:Fft
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine  ft1axis( adj, sign1, n1,n2, cx)
integer i2,          adj,        n1,n2
complex cx(n1,n2)
real sign1
do i2= 1, n2
  call fth( adj, sign1, 1,n1, cx(1,i2))
end do 
return
end  

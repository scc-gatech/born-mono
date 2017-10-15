!<
!ft2axis
!
! Usage
! call ft2axis(adj,sign1,n1,n2,cx)
!
! Description
! 1D Fourier transform on a 2D data set along the 2-axis
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
subroutine  ft2axis( adj, sign2, n1,n2, cx)
integer i1,          adj,        n1,n2
complex cx(n1,n2)
real    sign2
do i1= 1, n1
  call fth( adj, sign2, n1,n2, cx(i1,1))
end do 
return
end  

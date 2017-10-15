!<
!fth
!
!USAGE
!
! call fth(adj,sign,m1,n12,cx)
!
!DESCRIPTION
! FT a vector in a matrix, with first omega = - pi
!
!INPUT PARAMETER
! adj - integer (adjoint=1 , forward =0)
! m1  - integer skip element
! n12 - integer elements along second axis
! cx  - complex  data
!
! CATEGORY
! Lib:Fft
!
! COMPILE LEVEL
! DISTR
!
!
subroutine  fth( adj,sign, m1, n12, cx)
integer i,       adj,      m1, n12
real sign
complex cx(m1,n12)
complex temp(n12)
do i= 1, n12
  temp(i) = cx(1,i)
end do 
if ( adj .eq. 0) then
  do i= 2, n12, 2
    temp(i) =  -temp(i)
  end do 
  call ftu(  sign, n12, temp)
else
  call ftu( -sign, n12, temp)
  do i= 2, n12, 2
    temp(i) =  -temp(i)
  end do
end if 
do i= 1, n12
  cx(1,i) = temp(i)
end do 
return
end  

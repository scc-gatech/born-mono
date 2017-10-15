! FT a vector in a matrix, with first omega = - pi
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

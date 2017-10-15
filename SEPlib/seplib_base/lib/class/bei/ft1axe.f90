! not in use yet.
!
! 1D Fourier transform on a 2D data set along the 1-axis
!
subroutine  ft1axe( adj, add, sign1, n1,n2, cx, cy)
integer i2, nn,     adj, add,        n1,n2
complex                                     cx(n1,n2),  cy(n1,n2)
real sign1
complex                           ct(n1,n2)
nn = 2 * n1 * n2
call adjnull(      adj, add,                cx,nn,      cy,nn )
if ( adj .eq. 0) then
  call copy( nn, cx, ct)
else
  call copy( nn, cy, ct)
end if 
do i2= 1, n2
  call fth( adj, sign1, 1,n1, ct(1,i2))
end do 
if ( adj .eq. 0) then
  call cadd( nn, ct, cy)
else
  call cadd( nn, ct, cx)
end if 
return
end  

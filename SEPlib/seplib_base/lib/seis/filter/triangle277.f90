! triangle
!
! USAGE
! call triangle(rect1,rect2,n1,n2,uu,vv)
!
! INPUT PARAMETERS
! rect1 - integer rectangle width dim=1
! rect2 - integer rectangle width dim=2
! n1 - integer data size faxst axis
! n2 - integer data size axis 2
! uu  - real(n1,n2) input
!
! OUTPUT PARAMETERS
! vv  - real(n1,n2) output
!
!DESCRIPTION
! Smooth by convolving with triangle in two dimensions.
!
! CATEGORY
! Lib:Filter
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine triangle2( rect1, rect2, n1, n2, uu, vv)
integer i1,i2,        rect1, rect2, n1, n2
real uu(n1,n2), vv(n1,n2)
real ss(n1,n2)
do i1= 1, n1
  call triangle( rect2, n1, n2, uu(i1,1), ss(i1,1))
end do 
do i2= 1, n2
  call triangle( rect1,  1, n1, ss(1,i2), vv(1,i2))
end do 
return
end  

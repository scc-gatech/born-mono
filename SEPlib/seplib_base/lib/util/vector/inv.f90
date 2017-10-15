!<
!inv
!
!Usage:
!call inv ( input, output, n1)
!
!Input Parameters:
! input - real(:)  input array
! n1    - integer  numver of elements in input and output
!
!Output Parameters:
! output - real(:)  output array
!
!
!Description:
! Output=1/inpuit
!
! CATEGORY
! Lib:Vector
!
! COMPILE LEVEL
! DISTR
!
!
!>
!-------------------------------------------------
!
!Author: Robert Clapp, ESMB 463, 7230253
!
!Date Created:Tue Nov 11 14:34:59 PST 1997
!
!Purpose:
!
subroutine inv(input,output,n1)
integer n1,i1
real input(n1),output(n1)
do i1=1,n1
  output(i1)=1./input(i1)
end do 
return
end  

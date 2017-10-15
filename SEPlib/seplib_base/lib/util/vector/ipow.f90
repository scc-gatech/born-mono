!<
!ipow
!
!Usage:
!call ipow ( input, factor,output, n1)
!
!Input Parameters:
! input - real(:)  input array
! factor- real     factor to raise input to
! n1    - integer  numver of elements in input and output
!
!Output Parameters:
! output - real(:)  output array
!
!
!Description:
! Output=input**factor
!
! CATEGORY
! Lib:Vector
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
subroutine ipow(input,factor,output,n1)
integer n1,i1,factor
real input(n1),output(n1)
do i1=1,n1
  output(i1)=input(i1)**factor
end do 
return
end  

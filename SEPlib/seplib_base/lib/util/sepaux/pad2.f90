!<
!pad2
!
! Description
! next highest power of 2
!
! Usage
! integer pad2(n)
!
! Input Parameter
! n - integer  input length
!
! Return Values
! n  =  next power of 2
!
! Category
! Lib:Aux
!>
integer function pad2( n )
integer n
pad2 = 1
do while ( pad2 < n )
  pad2 = pad2 * 2 
end do 
return
end  

!<
! icaf3
!
! USAGE
! call icaf3(adj,add,lag1,lag2,lag3,xx,n1,n2,n3,aa,na1,na2,na3,yy)
!
! INPUT PARAMETERS
! adj - integer  1-adjoint, 0-forward
! add - integer  1-add to output, 0-erase output
! lag1- integer  lag along first axis
! lag2- integer  lag along second axis
! lag3- integer  lag along third axis
! xx  - real(n1,n2)  model
! n1   -integer  size of model,data first axis
! n2   -integer  size of model,data second axis
! n3   -integer  size of model,data third axis
! aa   -real(na1,na2) filter
! na1  -integer filter size axis 1
! na2  -integer filter size axis 2
! na3  -integer filter size axis 3
! yy   -real(n1,n2)  data
!
! DESCRIPTION
! Internal Convolution, Adjoint is the Filter. 3-dimensional
!
! CATEGORY
! Lib:Filter
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine icaf3( adj,add, lag1,lag2,lag3, xx,n1,n2,n3,  aa,na1,na2,na3&
  &, yy)
use sep_mod
integer           adj,add, lag1,lag2,lag3,    n1,n2,n3,     na1,na2&
  &,na3
real                                       xx(n1,n2,n3), aa(na1,na2,na3&
  &)
real                                       yy(n1,n2,n3)
integer            x1,x2,x3,       a1,a2,a3,  y1,y2,y3
if ( na1 > n1  .or.  lag1<1 .or. lag1 >na1) then
  call erexit('cvf3()')
end if
if ( na2 > n2  .or.  lag2<1 .or. lag2 >na2) then
  call erexit('cvf3()')
end if
if ( na3 > n3  .or.  lag3<1 .or. lag3 >na3) then
  call erexit('cvf3()')
end if
call adjnull(  adj, add, aa,na1*na2*na3,  yy,n1*n2*n3)
do a3=1,na3  
  do y3= 1+na3-lag3, n3-lag3+1  
    x3= y3 - a3 + lag3
    do a2=1,na2  
      do y2= 1+na2-lag2, n2-lag2+1  
        x2= y2 - a2 + lag2
        do a1=1,na1  
          do y1= 1+na1-lag1, n1-lag1+1  
            x1= y1 - a1 + lag1
            if ( adj .eq. 0 ) then
              yy( y1,y2,y3) = yy( y1,y2,y3) + aa( a1,a2,a3) * xx( x1,x2&
                &,x3)
            else
              aa( a1,a2,a3) = aa( a1,a2,a3) + yy( y1,y2,y3) * xx( x1,x2&
                &,x3)
            end if
          end do
        end do
      end do
    end do
  end do
end do 
return
end  

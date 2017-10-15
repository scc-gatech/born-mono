!<
! icaf2
!
! USAGE
! call icaf2(adj,add,lag1,lag2,xx,n1,n2,aa,na1,na2,yy)
!
! INPUT PARAMETERS
! adj - integer  1-adjoint, 0-forward
! add - integer  1-add to output, 0-erase output
! lag1- integer  lag along first axis
! lag2- integer  lag along second axis
! xx  - real(n1,n2)  model
! n1   -integer  size of model,data first axis
! n2   -integer  size of model,data second axis
! aa   -real(na1,na2) filter
! na1  -integer filter size axis 1
! na2  -integer filter size axis 2
! yy   -real(n1,n2)  data
!
! DESCRIPTION
! Internal Convolution, Adjoint is the Filter. 2-dimensional
!
! CATEGORY
! Lib:Filter
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine icaf2( adj,add, lag1,lag2, xx,n1,n2,  aa,na1,na2,  yy )
integer           adj,add, lag1,lag2,    n1,n2,     na1,na2
real                                  xx(n1,n2), aa(na1,na2), yy(n1,n2&
  &)
integer                                  x1,x2,      a1, a2,     y1,y2
call adjnull(     adj, add,                      aa,na1*na2,  yy,n1*n2&
  &)
do a2=1,na2  
  do y2= 1+na2-lag2, n2-lag2+1  
    x2= y2 - a2 + lag2
    do a1=1,na1  
      do y1= 1+na1-lag1, n1-lag1+1  
        x1= y1 - a1 + lag1
        if ( adj .eq. 0 ) then
          yy( y1,y2) = yy( y1,y2) + aa( a1,a2) * xx( x1,x2)
        else
          aa( a1,a2) = aa( a1,a2) + yy( y1,y2) * xx( x1,x2)
        end if
      end do
    end do
  end do
end do 
return
end  

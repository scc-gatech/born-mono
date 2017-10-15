!<
! tcaf2
!
! USAGE
! call icaf2(adj,add,xx,n1,n2,aa,na1,na2,yy)
!
! INPUT PARAMETERS
! adj - integer  1-adjoint, 0-forward
! add - integer  1-add to output, 0-erase output
! xx  - real(n1,n2)  model
! n1   -integer  size of model first axis
! n2   -integer  size of model second axis
! aa   -real(na1,na2) filter
! na1  -integer filter size axis 1
! na2  -integer filter size axis 2
! yy   -real(n1+na1,n2+na2)  data
!
! DESCRIPTION
! Transient Convolution, Adjoint is the Filter  2-dimensional
!
! CATEGORY
! Lib:Filter
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine tcaf2( adj,add, xx,n1,n2,  aa,m1,m2,  yy )
integer           adj,add,    n1,n2,     m1,m2
real                       xx(n1,n2), aa(m1,m2), yy( n1+m1 , n2+m2)
integer                       x1,x2,     a1,a2,      y1    , y2
call adjnull(     adj, add,           aa,m1*m2,  yy,(n1+m1)*(n2+m2) )
do a2=1,m2  
  do x2=1,n2  
    y2 = x2 + a2 - 1
    do a1=1,m1  
      do x1=1,n1  
        y1 = x1 + a1 - 1
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

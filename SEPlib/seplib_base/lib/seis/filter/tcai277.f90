!<
! tcai2
!
! USAGE
! call tcai2(adj,add,aa,m1,m2,xx,n1,n2,yy)
!
! INPUT PARAMETERS
! adj - integer  1-adjoint, 0-forward
! add - integer  1-add to output, 0-erase output
! aa   -real(na1,na2) filter
! m1  -integer filter size axis 1
! m2  -integer filter size axis 2
! xx  - real(n1,n2)  model
! n1   -integer  size of model first axis
! n2   -integer  size of model second axis
! yy   -real(n1+m1,n2+m2)  data
!
! DESCRIPTION
! Transient Convolution Adjoint is the Input. 2-D.
!
! CATEGORY
! Lib:Filter
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine tcai2( adj,add, aa,m1,m2,  xx,n1,n2,  yy )
integer           adj,add,    m1,m2,     n1,n2
real                       aa(m1,m2), xx(n1,n2), yy( m1+n1 , m2+n2)
integer                       a1,a2,     x1,x2,          y1,y2
call adjnull(     adj,add,            xx,n1*n2,  yy,(m1+n1)*(m2+n2) )
do a2=1,m2  
  do x2=1,n2  
    y2 = x2 + a2 - 1
    do a1=1,m1  
      do x1=1,n1  
        y1 = x1 + a1 - 1
        if ( adj .eq. 0 ) then
          yy(y1,y2) = yy(y1,y2) + xx(x1,x2) * aa(a1,a2)
        else
          xx(x1,x2) = xx(x1,x2) + yy(y1,y2) * aa(a1,a2)
        end if
      end do
    end do
  end do
end do 
return
end  

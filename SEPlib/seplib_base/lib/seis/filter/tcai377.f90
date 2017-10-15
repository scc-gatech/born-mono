!<
! tcai2
!
! USAGE
! call tcai2(adj,add,aa,m1,m2,m3,xx,x1,n1,n2,n3,yy)
!
! INPUT PARAMETERS
! adj - integer  1-adjoint, 0-forward
! add - integer  1-add to output, 0-erase output
! aa   -real(na1,na2) filter
! m1  -integer filter size axis 1
! m2  -integer filter size axis 2
! m3  -integer filter size axis 3
! xx  - real(n1,n2)  model
! n1   -integer  size of model first axis
! n2   -integer  size of model second axis
! n3   -integer  size of model third axis
! yy   -real(n1+m1,n2+m2,n3+m3)  data
!
! DESCRIPTION
! Transient Convolution Adjoint Input 3-dimensional
!
! CATEGORY
! Lib:Filter
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine tcai3(adj,add, aa,m1,m2,m3,  xx,n1,n2,n3,  yy )
integer          adj,add,    m1,m2,m3,     n1,n2,n3
real                      aa(m1,m2,m3), xx(n1,n2,n3), yy( m1+n1, m2+n2&
  &, m3+n3)
integer                      a1,a2,a3,     x1,x2,x3,       y1,    y2, &
  &   y3
call adjnull(     adj,add,              xx,n1*n2*n3, yy,(m1+n1)*(m2+n2&
  &)*(m3+n3))
do a3=1,m3  
  do x3=1,n3  
    y3 = x3 + a3 - 1
    do a2=1,m2  
      do x2=1,n2  
        y2 = x2 + a2 - 1
        do a1=1,m1  
          do x1=1,n1  
            y1 = x1 + a1 - 1
            if ( adj .eq. 0 ) then
              yy(y1,y2,y3) = yy(y1,y2,y3) + xx(x1,x2,x3) * aa(a1,a2,a3&
                &)
            else
              xx(x1,x2,x3) = xx(x1,x2,x3) + yy(y1,y2,y3) * aa(a1,a2,a3&
                &)
            end if
          end do
        end do
      end do
    end do
  end do
end do 
return
end  

!<
! tcai1
!
! USAGE
! call tcai1(adj,add,bb,m,xx,n,yy)
!
! INPUT PARAMETERS
! adj - integer  1-adjoint, 0-forward
! add - integer  1-add to output, 0-erase output
! bb   -real(m) filter
! m   - integer size of filter
! xx  - real(n)  model
! n    -integer  size of model
! yy   -real(n+m)  data
!
! DESCRIPTION
! Transient Convolution Adjoint is the Input 1-D.
!
! CATEGORY
! Lib:Filter
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine tcai1( adj,add, bb,m,  xx,n,  yy )
integer           adj,add,  b,m,   x,n,       y
real                       bb(m), xx(n), yy( m+n)
call adjnull(     adj,add,        xx,n,  yy,(m+n))
do b=1,m  
  do x=1,n  
    y = x + b - 1
    if ( adj .eq. 0 ) then
      yy(y) = yy(y) + xx(x) * bb(b)
    else
      xx(x) = xx(x) + yy(y) * bb(b)
    end if
  end do
end do 
return
end  


! Burg2 --- Burg 2-D conv with (a1,a2) filter (monoplane annihilator if a2=2)
!       output residual partitioned into normal and backward parts.
!       output adjoint to FILTER.
!       output residual(,) aligns with data(,) at filter coef aa(lag1,lag2)
!
subroutine burg2( adj,add, lag1,lag2, data,n1,n2,   aa,a1,a2,  residual&
  &)
use classutil
integer i1,i2,    adj,add, lag1,lag2,      n1,n2,      a1,a2
real                                   data(n1,n2) ,aa(a1,a2), residual&
  &(n1,2*n2)
real                         back(n1,n2)
call adjnull(     adj,add,                          aa,a1*a2,  residual&
  &,n1*n2*2)
do i2= 1, n2  
  do i1= 1, n1  
    back( n1-i1+1, n2-i2+1) = data( i1, i2)
  end do
end do 
call cinlof( adj, 1, lag1,lag2, n1,n2,data, a1,a2,aa, residual        &
  &)
call cinlof( adj, 1, lag1,lag2, n1,n2,back, a1,a2,aa, residual(1,n2+1)&
  &)
return
end  

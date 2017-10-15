!<
! icai1
!
! USAGE
! call icai1(adj,add,lag,xx,n,bb,nb,yy)
!
! INPUT PARAMETERS
! adj - integer  1-adjoint, 0-forward
! add - integer  1-add to output, 0-erase output
! lag - integer  lag
! xx  - real(n)  model
! n    -integer  size of model,data
! bb   -real(nb) size of filter
! yy   -real(n)  data
!
! DESCRIPTION
! Internal Convolution, Adjoint is the Input.  1-dimensional
!
! CATEGORY
! Lib:Filter
!
! COMPILE LEVEL
! DISTR
!>
!
!
subroutine icai1( adj, add, lag, bb,nb,  xx,n,  yy  )
integer           adj, add, lag,  b,nb,   x,n,   y          
! lag=1 means causal
real                             bb(nb), xx(n), yy(n)
call adjnull(     adj, add,              xx,n,  yy,n )
do b= 1, nb  
  do y= 1+nb-lag, n-lag+1  
    x= y - b + lag      
    if ( adj .eq. 0 ) then
      yy(y) = yy(y) + xx(x) * bb(b)
    else
      xx(x) = xx(x) + yy(y) * bb(b)
    end if
  end do
end do 
return
end  

!<
! icaf1
!
! USAGE
! call icaf1(adj,add,lag,xx,n,bb,nb,yy)
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
! Internal Convolution, Adjoint is the Filter. 1-dimensional
!
! CATEGORY
! Lib:Filter
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine icaf1( adj, add, lag, xx,n,  bb,nb,  yy  )
integer           adj, add, lag,  x,n,   b,nb,   y
real                             xx(n), bb(nb), yy(n)
call adjnull(     adj, add,             bb,nb,  yy,n )
do b= 1, nb  
  do y= 1+nb-lag, n-lag+1  
    x= y - b + lag      
    if ( adj .eq. 0 ) then
      yy(y) = yy(y) + bb(b) * xx(x)
    else
      bb(b) = bb(b) + yy(y) * xx(x)
    end if
  end do
end do 
return
end  

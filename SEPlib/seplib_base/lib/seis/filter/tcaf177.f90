!<
! tcaf1
!
! USAGE
! call tcaf1(adj,add,xx,n,bb,nb,yy)
!
! INPUT PARAMETERS
! adj - integer  1-adjoint, 0-forward
! add - integer  1-add to output, 0-erase output
! xx  - real(n)  model
! n    -integer  size of model
! bb   -real(nb) size of filter
! yy   -real(n+nb)  data
!
! DESCRIPTION
! Transient Convolution, Adjoint is the Filter  1-dimensional
!
! CATEGORY
! Lib:Filter
!
! COMPILE LEVEL
! DISTR
!>
!
!
subroutine tcaf1( adj, add,  xx,nx,  bb,nb,  yy  )
integer           adj, add,   x,nx,   b,nb,   y
real                         xx(nx), bb(nb), yy( nx+nb)
call adjnull(     adj, add,          bb,nb,  yy, nx+nb)
do b= 1, nb  
  do x= 1, nx  
    y = x + b - 1
    if ( adj .eq. 0 ) then
      yy(y) = yy(y) + bb(b) * xx(x)
    else
      bb(b) = bb(b) + yy(y) * xx(x)
    end if
  end do
end do 
return
end  

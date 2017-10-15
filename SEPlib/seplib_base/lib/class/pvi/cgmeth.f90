! setup of conjugate gradient descent, minimize  SUM rr(i)**2
!                   nx
! rr(i)  = yy(i) - sum aaa(i,j) * x(j)
!                  j=1
subroutine cgmeth( nx,x, nr,yy,rr, aaa, niter)
integer i, iter,   nx,   nr,            niter
real    x(nx), yy(nr), rr(nr), aaa(nr,nx)
real dx(nx), sx(nx), dr(nr), sr(nr)
do i= 1, nx
  x(i) = 0.
end do 
do i= 1, nr
  rr(i) = yy(i)
end do 
do iter= 0, niter  
  call matmult( 1, aaa, nx,dx, nr,rr)              ! dx= dx(aaa,rr)
  call matmult( 0, aaa, nx,dx, nr,dr)              ! dr= dr(aaa,dx)
  call cgstep( iter, nx, x,dx,sx,                            nr,rr,dr&
    &,sr)                 ! x=x+s; rr=rr-ss
end do 
return
end  

subroutine leak( rho, n, xx, yy)
integer i, n
real xx(n), yy(n), rho
real tt( n)
call     null( tt, n)
tt(1) = xx(1)
do i= 2, n
  tt(i) = rho * tt(i-1) + xx(i)
end do 
call copy( n, tt, yy)
return
end  

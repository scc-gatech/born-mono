! causal integral
!
subroutine causint2( adj, add,        n,xx,  yy   )
integer i,        n, adj, add
real xx(n), yy(n )
real tt( n)
call adjnull(        adj, add,        xx,n,  yy,n )
if ( adj .eq. 0) then
  tt(1) = xx(1) / 2.
  do i= 2, n
    tt(i) = tt(i-1) + (xx(i) + xx(i-1)) / 2.
  end do 
  do i= 1, n
    yy(i) = yy(i) + tt(i)
  end do
else
  tt(n) = yy(n) / 2.
  do i= n, 2, -1
    tt(i-1) = tt(i) + (yy(i) + yy(i-1)) / 2.
  end do 
  do i= 1, n
    xx(i) = xx(i) + tt(i)
  end do
end if 
return
end  

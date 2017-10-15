subroutine ruffen1( adj,    n, xx, yy     )
integer i,          adj,       n
real                        xx(n), yy( n-1)
call adjnull(       adj, 0, xx,n,  yy, n-1)
do i= 1, n-1  
  if ( adj .eq. 0 ) then
    yy(i) = yy(i) + xx(i+1) - xx(i)
  else
    xx(i+1) = xx(i+1) + yy(i)
    xx(i  ) = xx(i  ) - yy(i)
  end if
end do 
return
end  

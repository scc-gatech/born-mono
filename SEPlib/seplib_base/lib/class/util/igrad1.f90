subroutine igrad1( adj, add,  xx,n,  yy   )
integer i,         adj, add,     n
real                          xx(n), yy(n)
call adjnull(      adj, add,  xx,n,  yy,n )
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

! patch ---- copy a patch of information from a wall into a window.
!
subroutine patch(          adj,add, j1,j2, k1,k2, wall, n1,n2, wind, w1&
  &,w2)
use classutil
integer i1,i2,s1,s2,d1,d2, adj,add, j1,j2, k1,k2,       n1,n2,       w1&
  &,w2
real                                              wall( n1,n2),wind( w1&
  &,w2)
call adjnull(              adj,add,               wall, n1*n2, wind, w1&
  &*w2)
if ( k2 .ne. 1) then
  s2 = 1.5 + (n2 - w2) * (j2-1.)/(k2-1.)
else
  s2= 1
end if 
if ( k1 .ne. 1) then
  s1 = 1.5 + (n1 - w1) * (j1-1.)/(k1-1.)
else
  s1= 1
end if 
do i2= 1, w2  
  d2= i2 + s2 - 1
  do i1= 1, w1  
    d1= i1 + s1 - 1
    if ( adj .eq. 0 ) then
      wind( i1,i2) = wind( i1,i2) + wall( d1,d2)
    else
      wall( d1,d2) = wall( d1,d2) + wind( i1,i2)
    end if
  end do
end do 
return
end  

! parcel ---- copy wall of data into k1-by-k2 overlapping windows.
!
subroutine parcel( adj,add, n1,n2, w1,w2, k1,k2, wall, wind)
use classutil
integer j1,j2,     adj,add, n1,n2, w1,w2, k1,k2
real                                   wall( n1,n2),   wind( w1,w2, k1&
  &,k2)
call adjnull(      adj, add,           wall, n1*n2,    wind, w1*w2* k1&
  &*k2 )
do j2= 1, k2  
  do j1= 1, k1  
    call patch( adj, 1, j1,j2, k1,k2, wall, n1,n2, wind(1,1,j1,j2), w1&
      &,w2)
  end do
end do 
return
end  

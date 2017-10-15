! keyword: tridiagonal smoothing on 1-axis or 2-axis
subroutine leaky( distance, m1, n12, uu, vv )
integer i,                  m1, n12
real    distance        ! input:  1. < distance < infinity
real    uu(m1,n12)      ! data in  is the vector (uu( 1, i), i=1,n12)
real    vv(m1,n12)      ! data out is the vector (vv( 1, i), i=1,n12)
real a, b, dc, side
real vecin( n12), vecout( n12)
a  = - (1.-1./distance)
b = 1.+a*a
dc = b+a+a
a = a/dc
b = b/dc
side = a + b
do i= 1,n12  
  vecin(i) = uu(1,i)
end do 
if ( distance<=1..or. n12.eq.1) then
  call copy( n12, vecin, vecout)
else
  call tris( n12, side, a, b, a, side, vecin, vecout)
end if 
do i= 1,n12   
  vv(1,i) = vecout(i)
end do 
return
end  

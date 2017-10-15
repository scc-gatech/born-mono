
! Nearest neighbor interpolation, essentially:   cbx = cb( 1.5 + (x-x0)/dx)
!
subroutine cinterp0( x, nx,x0,dx, cb, cbx)
integer ix,             nx
real    x, x0, dx
complex cb(nx), cbx
ix = 1.5 + (x-x0) / dx
if ( ix < 1 ) then
  cbx = cb( 1)
else if ( ix > nx) then
  cbx = cb( nx)
else
  cbx = cb( ix)
end if 
return
end  

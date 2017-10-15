! signal advance:    y(iy) = x(iy+jump)
!
subroutine advance( adj, add, jump, nx, xx, ny, yy)
use classutil
integer ix, iy,     adj, add, jump, nx,     ny
real                                    xx(nx), yy(ny)
call adjnull( adj, add,                 xx,nx,  yy,ny)
do iy= 1, ny  
  ix = iy + jump
  if (         ix >= 1 ) then
    if ( ix <= nx ) then
      if ( adj .eq. 0 ) then
        yy( iy) = yy( iy) + xx( ix)
      else
        xx( ix) = xx( ix) + yy( iy)
      end if
    end if
  end if
end do 
return
end  

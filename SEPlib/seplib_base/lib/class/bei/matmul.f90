! matrix multiply and its adjoint
!
subroutine matmul( adj, bb,        nx,x,  ny,y)
integer ix, iy,    adj,            nx,    ny
real                    bb(ny,nx), x(nx), y(ny)
call adjnull(      adj, 0,         x,nx,  y,ny)
do ix= 1, nx  
  do iy= 1, ny  
    if ( adj .eq. 0 ) then
      y(iy) = y(iy) + bb(iy,ix) * x(ix)
    else
      x(ix) = x(ix) + bb(iy,ix) * y(iy)
    end if
  end do
end do 
return
end  

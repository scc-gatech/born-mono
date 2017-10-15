subroutine adjnull( adj, add, x, nx,  y, ny )
integer ix, iy,     adj, add,    nx,     ny
real                          x( nx), y( ny )
if ( add .eq. 0 ) then
  if ( adj .eq. 0 ) then
    do iy= 1, ny
      y(iy) = 0.
    end do
  else
    do ix= 1, nx
      x(ix) = 0. 
    end do
  end if
end if
return
end
subroutine ladjnull( adj, add, x, y )
logical adj, add
real x(:), y(:)
if (.NOT. add) then
  if ( .NOT. adj) then
    y = 0.0
  else
    x = 0.0
  end if
end if
return
end  

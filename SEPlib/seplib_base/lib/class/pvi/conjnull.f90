subroutine conjnull( conj, add, x, nx,  y, ny )
integer ix, iy,      conj, add,    nx,     ny
real                            x( nx), y( ny )
if ( add .eq. 0 ) then
  if ( conj .eq. 0 ) then
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

!       Convolve and correlate with no assumptions off end of data.
!
subroutine convin( adj, add, nx, xx, nb, bb, yy)
use sep_func_mod
use classutil
integer ib, iy,ny, adj, add, nx,     nb
real    xx(nx)                  ! input signal
real    bb(nb)                  
! filter      (or output crosscorrelation)
real    yy(nx-nb+1)             
! filtered signal (or second input signal)
ny = nx - nb + 1                ! length of filtered signal
if ( ny < 1 ) then
  call erexit('convin() filter output negative length.')
end if
call adjnull(      adj, add,         bb, nb, yy, ny)
if ( adj .eq. 0 ) then
  do iy= 1, ny  
    do ib= 1, nb  
      yy( iy) = yy( iy) + bb(ib) * xx( iy-ib+nb)
    end do
  end do
else
  do ib= 1, nb  
    do iy= 1, ny  
      bb( ib) = bb( ib) + yy(iy) * xx( iy-ib+nb)
    end do
  end do
end if 
return
end  

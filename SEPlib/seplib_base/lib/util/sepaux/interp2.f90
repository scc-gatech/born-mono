!<
! intrp2
!
! USAGE
! call intrp2( conj, inv, nt, nx, x0, dx, ny, y0, dy, xx, yy)
!
! INPUT PARAMETERS
! conj  -  integer  conj=1 conjugate
! inv   -  integer  inv=1 inverse
! nt    -  integer  number of time samples
! nx    -  integer  number of  samples in x
! x0    -  real     first x value
! dx    -  real     sampling in x
! ny    -  integer  number of samples in y
! y0    -  real     first y value
! dy    -  real     sampling in y
! xx    -  real(nt,nx)     model
! yy    -  real(nt,ny)     data
!
! DESCRIPTION
! Linear interpolation along axis2
!
! CATEGORY
! Lib:Aux
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine intrp2( conj, inv, nt, nx, x0, dx, ny, y0, dy, xx, yy)
use sep_mod
use sep_func_mod
integer conj, inv, it, nt, ix, nx, iy, ny
real x0,dx, y0,dy, xx(nt,nx), yy(nt,ny), x, y, w0, w1, xm
real bb(2048), cc(2048), sp(2048), sp1(2048)
if ( ny > 2048) then
  call erexit('dimension in interp2')
end if
do ix=1,nx  
  bb(ix) = 0.
  cc(ix) = 0.
end do 
do it=1,nt
  if ( conj .eq. inv ) then
    do iy = 1, ny
      yy(it,iy) = 0.
    end do
  else
    do ix = 1, nx
      xx(it,ix) = 0.
    end do
  end if
end do 
do iy = 1, ny  
  y = y0 + (iy-1) * dy
  x = y
  ix = 1 + (x-x0) / dx
  xm = x0 + (ix-1) * dx
  w1 =  (y - xm)  / dx
  w0 = 1. - w1
  if ( 0 < ix  .and.  ix <= nx ) then
    bb(ix  ) = bb(ix  ) + w0 * w0
    cc(ix  ) = cc(ix  ) + w1 * w0
    if ( ix+1 <= nx) then
      bb(ix+1) = bb(ix+1) + w1 * w1
    end if
    if ( conj .eq. inv ) then
      do it=1,nt
        yy(it,iy) = yy(it,iy) + w0 * xx(it,ix  )
      end do 
      if (ix+1 <= nx) then
        do it=1,nt
          yy(it,iy) = yy(it,iy) + w1 * xx(it,ix+1)
        end do
      end if
    else
      do it=1,nt
        xx(it,ix  ) = xx(it,ix  ) + w0*yy(it,iy)
      end do 
      if (ix+1 <= nx) then
        do it=1,nt
          xx(it,ix+1) = xx(it,ix+1) + w1*yy(it,iy)
        end do
      end if
    end if
  end if
end do 
if ( inv .eq. 1 ) then
  do it = 1, nt  
    do ix = 1, nx  
      if ( bb(ix) .eq. 0.0 ) then
        bb(ix) = 1.0
        sp(ix) = 0.0
      else
        sp(ix) = xx(it,ix)
      end if
    end do 
    call vtris(nx,cc,bb,cc,sp,sp1)
    do ix = 1, nx
      xx(it,ix) = sp1(ix)
    end do
  end do
end if
return
end  

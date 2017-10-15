!<
! nmo_neighbor
!
! USAGE
! call nmo_neighbor(adj,add,slow,x,t0,dt,n,zz,tt)>
!
! DESCRIPTION
! Nmo by nearest neighbor for a single trace (adj=1 nmo)
!
! INPUT PARAMETERS
!  adj    -  integer  1=forward, 0=adjoint
!  add    -  integer  1=add to output, 0=erase output
!  slow   -  real(n)  slowness
!  x      -  real     offset
!  t0     -  real     first time
!  dt     -  real     sampling in time
!  n      -  integer  number of time samples
!  zz     -  real(n)  model
!  tt     -  real(n)  data
!
! CATEGORY
! Lib:Velan
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine nmo_neighbor( adj, add, slow,    x, t0, dt, n,zz,  tt )
use sep_mod
use sep_func_mod
integer  it, iz, adj, add,                        n
real  xs, t , z,           slow(n), x, t0, dt, zz(n), tt(n)
call adjnull(    adj, add,                     zz,n,  tt,n)
do iz= 1, n  
  z = t0 + dt*(iz-1)              ! Travel-time depth
  xs= x * slow(iz)
  t = sqrt ( z * z + xs * xs)
  it= 1 + .5 + (t - t0) / dt      ! Round to nearest neighbor.
  if ( it <= n ) then
    if ( adj .eq. 0 ) then
      tt(it) = tt(it) + zz(iz)
    else
      zz(iz) = zz(iz) + tt(it)
    end if
  end if
end do 
return
end
!<
! nmo_neighbor_w
!
! USAGE
! call nmo_neighbor_w(adj,add,slow,x,t0,dt,n,zz,tt)
!
! DESCRIPTION
! Nmo by nearest neighbor for a single trace (adj=1 nmo) with weight function
!
! INPUT PARAMETERS
!  adj    -  integer  1=forward, 0=adjoint
!  add    -  integer  1=add to output, 0=erase output
!  slow   -  real(n)  slowness
!  x      -  real     offset
!  t0     -  real     first time
!  dt     -  real     sampling in time
!  n      -  integer  number of time samples
!  zz     -  real(n)  model
!  tt     -  real(n)  data
!
! CATEGORY
! Lib:Velan
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine nmo_neighbor_w( adj, add, slow,    x, t0, dt, n,zz,  tt )
use sep_mod
use sep_func_mod
integer  it, iz, adj, add,                        n
real  xs, t , z,           slow(n), x, t0, dt, zz(n), tt(n), wt
call adjnull(    adj, add,                     zz,n,  tt,n)
do iz= 1, n  
  z = t0 + dt*(iz-1)
  xs = x * slow(iz)
  t = sqrt ( z * z + xs * xs) + 1.e-20
  wt = z/t * (1./sqrt(t))                 !  weighting function
  it = 1 + .5 + (t - t0) / dt
  if ( it <= n ) then
    if ( adj .eq. 0 ) then
      tt(it) = tt(it) + zz(iz) * wt
    else
      zz(iz) = zz(iz) + tt(it) * wt
    end if
  end if
end do 
return
end
!<
! nmo_2d_i
!
! USAGE
! call nmo_2d_i(tra, inv, slow, t0, dt, x0, dx, n1, n2, tt, zz)
!
! DESCRIPTION
! Nmo by nearest neighbor for a gather
!
! INPUT PARAMETERS
!  tra    -  integer  1=transpose, 0=don't
!  inv    -  integer  1=inverse, 0=forward
!  slow   -  real(n1) slowness
!  t0     -  real     first time
!  dt     -  real     sampling in time
!  x0     -  real     first offset
!  dx     -  real     sampling in space
!  n1     -  integer  number of time samples
!  n2     -  integer  number of offsets
!  zz     -  real(n1,n2)  model
!  tt     -  real(n1,n2)  data
!
! CATEGORY
! Lib:Velan
!
! COMMENTS
! integer arithmetic NMO, for use of interactive programs.
! external arguments are all reals.
!
! NMO, its transpose, inverse, and inverse transpose by nearest neighbor.
! tra=0 inv=0:        zz(iz) = [NMO]                   tt(it)
! tra=1 inv=0:        tt(it) = [NMO transpose]         zz(iz)
! tra=0 inv=1:        tt(it) = [NMO inverse]           zz(iz)
! tra=1 inv=1:        zz(iz) = [NMO inverse transpose] tt(it)
!
! COMPILE LEVEL
! DISTR
!>
!
!
subroutine nmo_2d_i( tra, inv, slow, t0, dt, x0, dx, n1, n2, tt, zz)
use sep_mod
use sep_func_mod
integer  tra, inv, n1, n2, i1, i2, it, it0, ix, ix0, islow(2048)
real slow(n1), t0, dt, x0, dx, x
real tt(n1,n2), zz(n1,n2)
ix0 = .5 + x0 / dx
it0 = .5 + t0 / dt
! UNITS external are usually        sec,        km,        slow = sec/km = ms/meter=.5
! UNITS internal will be        i1,        i2,        slow = 1024 * i1 / i2
! Given:        t =  sqrt( tau**2 +  (x*slow        )**2 )
! Want:                 it = sqrt(itau**2 + ((islow*ix)/1024)**2 )
do i1=1,n1
  islow(i1) = slow(i1) * 1024. * dx / dt
end do 
do i2 = 0, n2-1  
  ix = ix0 + i2 * dx
  call nmo_i( tra, inv, islow, ix, it0, n1, tt(1,ix), zz(1,ix) )
end do 
return
end
!<
! nmo_i
!
! USAGE
! call nmo_i(tra, inv, islow, ix, it0, n1, tt, zz )
!
! DESCRIPTION
! Nmo by nearest neighbor for a trace
!
! INPUT PARAMETERS
!  tra    -  integer  1=transpose, 0=don't
!  inv    -  integer  1=inverse, 0=forward
!  islow  -  real(n1) slowness
!  ix     -  integer  space location
!  it0    -  integer  first itme
!  n1     -  integer  number of time samples
!  tt     -  real(n1)  data
!  zz     -  real(n1)  model
!
! CATEGORY
! Lib:Velan
!
! COMMENTS
! integer arithmetic NMO, for use of interactive programs.
! external arguments are all reals.
!
! NMO, its transpose, inverse, and inverse transpose by nearest neighbor.
! tra=0 inv=0:        zz(iz) = [NMO]                   tt(it)
! tra=1 inv=0:        tt(it) = [NMO transpose]         zz(iz)
! tra=0 inv=1:        tt(it) = [NMO inverse]           zz(iz)
! tra=1 inv=1:        zz(iz) = [NMO inverse transpose] tt(it)
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine nmo_i( tra, inv, islow, ix, it0, n1, tt, zz )
use sep_mod
use sep_func_mod
integer  tra, inv, i1,n1, itab(n1), off, last
integer it0, it, ix, iz, ixs, iarg
integer islow(n1), count(n1)
real tt(n1), zz(n1)
if ( tra .eq. inv ) then
  do iz = 1,n1
    zz(iz) = 0.0
  end do
else
  do i1 = 1,n1
    tt(i1) = 0.0
  end do
end if 
if ( inv > 0 ) then
  do i1 = 1,n1
    count(i1) = 0.0
  end do 
! target space  iz=1,n1
! input space   i1=1,n1
! time                it=1+it0, nt+it0+NMO
!                 ix  physical distance in units of trace spacing.
end if
it = (n1-1) + it0
off = n1 + 1
do iz = n1, 1, -1   
  ixs = ix * islow(iz)
  iarg = (iz+it0) * (iz+it0) + (ixs * ixs) / (1024*1024)
  it = (iarg + it * it) / (it + it)
  i1 =  1 + it - it0
  if ( i1 <= n1  ) then
! Theoretically i1>0
    if ( i1 <=0 ) then
      call erexit('bad i1')
    end if
    count(i1) = count(i1) + 1.
    itab(iz) = i1
  else
    off = iz
  end if
end do 
last = off - 1
if ( tra .eq. 1  .and. inv .eq. 1 ) then
  do i1 = 1, n1
    if ( count(i1) .ne. 0.0 ) then
      tt(i1) = tt(i1) / count(i1)
    end if
  end do
end if
if ( tra .eq. inv ) then
  do iz = 1, last  
    i1 = itab(iz)
    zz(iz) = zz(iz) + tt(i1)
  end do
else
  do iz = 1, last  
    i1 = itab(iz)
    tt(i1) = tt(i1) + zz(iz)
  end do
end if 
if ( tra .eq. 0  .and. inv .eq. 1 ) then
  do i1 = 1, n1
    if ( count(i1) .ne. 0.0 ) then
      tt(i1) = tt(i1) / count(i1)
    end if
  end do
end if
return
end
!<
! lmo_2d_n
!
! USAGE
! call lmo_2d_n(tra, inv, slow, t0, dt, x0, dx, n1, n2, tt, zz)
!
! DESCRIPTION
! Lmo by nearest neighbor for a gather
!
! INPUT PARAMETERS
!  tra    -  integer  1=transpose, 0=don't
!  inv    -  integer  1=inverse, 0=forward
!  slow   -  real(n1) slowness
!  t0     -  real     first time
!  dt     -  real     sampling in time
!  x0     -  real     first offset
!  dx     -  real     sampling in space
!  n1     -  integer  number of time samples
!  n2     -  integer  number of offsets
!  tt     -  real(n1,n2)  data
!  zz     -  real(n1,n2)  model
!
! CATEGORY
! Lib:Velan
!
! COMMENTS
! LMO, its transpose, inverse, and inverse transpose by nearest neighbor.
! tra=0 inv=0:        zz(iz) = [NMO]                   tt(it)
! tra=1 inv=0:        tt(it) = [NMO transpose]         zz(iz)
! tra=0 inv=1:        tt(it) = [NMO inverse]           zz(iz)
! tra=1 inv=1:        zz(iz) = [NMO inverse transpose] tt(it)
! integer arithmetic NMO, for use of interactive programs.
! external arguments are all reals.
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine lmo_2d_n( tra, inv, slow, t0, dt, x0, dx, nt, nx, tt, zz)
use sep_mod
use sep_func_mod
integer  tra, inv, nt, ix, nx
real slow(nt), t0, dt, x0, dx, x, tt(nt,nx), zz(nt,nx)
write (0,*) nt,nx
do ix = 1, nx  
  x = x0 + ix * dx
  call lmo_n( tra, inv, slow, x, t0, dt, nt, tt(1,ix), zz(1,ix) )
end do 
return
end
!<
! lmo_n
!
! USAGE
! call lmo_n(tra, inv, slow, x,t0, dt, nt, tt, zz)
!
! DESCRIPTION
! Lmo by nearest neighbor for a trace
!
! INPUT PARAMETERS
!  tra    -  integer  1=transpose, 0=don't
!  inv    -  integer  1=inverse, 0=forward
!  slow   -  real(nt) slowness
!  x      -  real     offset
!  t0     -  real     first time
!  dt     -  real     sampling in time
!  nt     -  integer  number of time samples
!  tt     -  real(nt)  data
!  zz     -  real(nt)  model
!
! CATEGORY
! Lib:Velan
!
! COMMENTS
! NMO, its transpose, inverse, and inverse transpose by nearest neighbor.
! tra=0 inv=0:        zz(iz) = [NMO]                   tt(it)
! tra=1 inv=0:        tt(it) = [NMO transpose]         zz(iz)
! tra=0 inv=1:        tt(it) = [NMO inverse]           zz(iz)
! tra=1 inv=1:        zz(iz) = [NMO inverse transpose] tt(it)
! integer arithmetic NMO, for use of interactive programs.
! external arguments are all reals.
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine lmo_n( tra, inv, slow, x, t0, dt, nt, tt, zz )
use sep_mod
use sep_func_mod
integer  tra, inv, it, nt, iz, itab(nt), off, last
real slow(nt), t0, dt, tt(nt), zz(nt), count(nt)
real t, x, z, xs, arg
if ( tra .eq. inv ) then
  do iz = 1,nt
    zz(iz) = 0.0
  end do
else
  do it = 1,nt
    tt(it) = 0.0
  end do
end if 
if ( inv > 0 ) then
  do it = 1,nt
    count(it) = 0.0
  end do
end if
z = t0 + nt * dt
t = z
off = nt + 1
do iz = nt, 1, -1   
  xs = x * slow(iz)
  t = sqrt (z * z + xs * xs )
  t = z + xs
  it = 0.5 + (t - t0) / dt
  if ( it <= nt  ) then
    count(it) = count(it) + 1.
    itab(iz) = it
  else
    off = iz
  end if 
  z = z - dt
end do 
last = off - 1
if ( tra .eq. 1  .and. inv .eq. 1 ) then
  do it = 1, nt
    if ( count(it) .ne. 0.0 ) then
      tt(it) = tt(it) / count(it)
    end if
  end do
end if
if ( tra .eq. inv ) then
  do iz = 1, last  
    it = itab(iz)
    zz(iz) = zz(iz) + tt(it)
  end do
else
  do iz = 1, last  
    it = itab(iz)
    tt(it) = tt(it) + zz(iz)
  end do
end if 
if ( tra .eq. 0  .and. inv .eq. 1 ) then
  do it = 1, nt
    if ( count(it) .ne. 0.0 ) then
      tt(it) = tt(it) / count(it)
    end if
  end do
end if
return
end
!<
! nmo_2d_n
!
! USAGE
! call nmo_2d_n(tra, inv, slow, t0, dt, x0, dx, n1, n2, tt, zz)
!
! DESCRIPTION
! Nmo by nearest neighbor for a gather
!
! INPUT PARAMETERS
!  tra    -  integer  1=transpose, 0=don't
!  inv    -  integer  1=inverse, 0=forward
!  slow   -  real(n1) slowness
!  t0     -  real     first time
!  dt     -  real     sampling in time
!  x0     -  real     first offset
!  dx     -  real     sampling in space
!  n1     -  integer  number of time samples
!  n2     -  integer  number of offsets
!  tt     -  real(n1,n2)  data
!  zz     -  real(n1,n2)  model
!
! CATEGORY
! Lib:Velan
!
! COMMENTS
! NMO, its transpose, inverse, and inverse transpose by nearest neighbor.
! tra=0 inv=0:        zz(iz) = [NMO]                   tt(it)
! tra=1 inv=0:        tt(it) = [NMO transpose]         zz(iz)
! tra=0 inv=1:        tt(it) = [NMO inverse]           zz(iz)
! tra=1 inv=1:        zz(iz) = [NMO inverse transpose] tt(it)
! integer arithmetic NMO, for use of interactive programs.
! external arguments are all reals.
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine nmo_2d_n( tra, inv, slow, t0, dt, x0, dx, nt, nx, tt, zz)
use sep_mod
use sep_func_mod
integer  tra, inv, nt, ix, nx
real slow(nt), t0, dt, x0, dx, x, tt(nt,nx), zz(nt,nx)
do ix = 1, nx  
  x = x0 + ix * dx
  call nmo_n( tra, inv, slow, x, t0, dt, nt, tt(1,ix), zz(1,ix) )
end do 
return
end
!<
! nmo_n
!
! USAGE
! call nmo_n(tra, inv, slow, x,t0, dt, nt, tt, zz)
!
! DESCRIPTION
! Nmo by nearest neighbor for a trace
!
! INPUT PARAMETERS
!  tra    -  integer  1=transpose, 0=don't
!  inv    -  integer  1=inverse, 0=forward
!  slow   -  real(nt) slowness
!  x      -  real     offset
!  t0     -  real     first time
!  dt     -  real     sampling in time
!  nt     -  integer  number of time samples
!  tt     -  real(nt)  data
!  zz     -  real(nt)  model
!
! CATEGORY
! Lib:Velan
!
! COMMENTS
! NMO, its transpose, inverse, and inverse transpose by nearest neighbor.
! tra=0 inv=0:        zz(iz) = [NMO]                   tt(it)
! tra=1 inv=0:        tt(it) = [NMO transpose]         zz(iz)
! tra=0 inv=1:        tt(it) = [NMO inverse]           zz(iz)
! tra=1 inv=1:        zz(iz) = [NMO inverse transpose] tt(it)
! integer arithmetic NMO, for use of interactive programs.
! external arguments are all reals.
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine nmo_n( tra, inv, slow, x, t0, dt, nt, tt, zz )
use sep_mod
use sep_func_mod
integer  tra, inv, it, nt, iz, itab(nt), off, last
real slow(nt), t0, dt, tt(nt), zz(nt), count(nt)
real t, x, z, xs, arg
if ( tra .eq. inv ) then
  do iz = 1,nt
    zz(iz) = 0.0
  end do
else
  do it = 1,nt
    tt(it) = 0.0
  end do
end if 
if ( inv > 0 ) then
  do it = 1,nt
    count(it) = 0.0
  end do
end if
z = t0 + nt * dt
t = z
off = nt + 1
do iz = nt, 1, -1   
  xs = x * slow(iz)
  arg = z * z + xs * xs
! next line replaceable by:        t = sqrt ( arg )
  t = (arg + t * t) / (t + t)
  it = 0.5 + (t - t0) / dt
  if ( it <= nt  ) then
    count(it) = count(it) + 1.
    itab(iz) = it
  else
    off = iz
  end if 
  z = z - dt
end do 
last = off - 1
if ( tra .eq. 1  .and. inv .eq. 1 ) then
  do it = 1, nt
    if ( count(it) .ne. 0.0 ) then
      tt(it) = tt(it) / count(it)
    end if
  end do
end if
if ( tra .eq. inv ) then
  do iz = 1, last  
    it = itab(iz)
    zz(iz) = zz(iz) + tt(it)
  end do
else
  do iz = 1, last  
    it = itab(iz)
    tt(it) = tt(it) + zz(iz)
  end do
end if 
if ( tra .eq. 0  .and. inv .eq. 1 ) then
  do it = 1, nt
    if ( count(it) .ne. 0.0 ) then
      tt(it) = tt(it) / count(it)
    end if
  end do
end if
return
end
!<
! nmo_2d_l_tab
!
! USAGE
! call nmo_2d_l_tab(tra, inv, i3, slow, t0, dt, x0, dx, nt, nx, tt, zz, itab, bb, cc, w0, w1 )
!
! DESCRIPTION
! Nmo by linear interpolation for a gather with tabulation (speeds up
! multiple regualar datasets)
!
! INPUT PARAMETERS
!  tra    -  integer  1=transpose, 0=don't
!  inv    -  integer  1=inverse, 0=forward
!  slow   -  real(n1) slowness
!  i3     -  integer  first time needs to (=1 to initalize)
!  t0     -  real     first time
!  dt     -  real     sampling in time
!  x0     -  real     first offset
!  dx     -  real     sampling in space
!  nt     -  integer  number of time samples
!  nx     -  integer  number of offsets
!  tt     -  real(nt,nx)  data
!  zz     -  real(nt,nx)  model
!  itab   -  integer(nt,nx) location table to speed up linear interp
!  bb     -  real(nt,nx)  weights to apply to linear interp
!  cc     -  real(nt,nx)  weights to apply to linear interp
!  w0     -  real(nt,nx)  weights to apply to linear interp
!  w1     -  real(nt,nx)  weights to apply to linear interp
!
! CATEGORY
! Lib:Velan
!
! COMMENTS
! NMO, transpose NMO, and inverse NMO by linear interpolation.
! tra=0 inv=0:        zz(iz) = [NMO]                   tt(it)
! tra=1 inv=0:        tt(it) = [NMO transpose]         zz(iz)
! tra=0 inv=1:        tt(it) = [NMO inverse]           zz(iz)
! tra=1 inv=1:        zz(iz) = [NMO inverse transpose] tt(it)
!
! i3=1 calls for tables to be built. mandatory on first call.
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine nmo_2d_l_tab( tra, inv, i3, slow, t0, dt, x0, dx, nt, nx, tt&
  &, zz,itab, bb, cc, w0, w1 )
use sep_mod
use sep_func_mod
integer  tra, inv, i3, nt, ix, nx, itab(nt,nx)
real slow(nt), t0, dt, x0, dx, x, tt(nt,nx), zz(nt,nx),bb(nt,nx), cc(nt&
  &,nx), w0(nt,nx), w1(nt,nx)
do ix = 1, nx  
  x = x0 + ix * dx
  call nmo_l( tra, inv, i3, slow, x, t0, dt, nt, tt(1,ix), zz(1,ix),itab&
    &(1,ix), bb(1,ix), cc(1,ix), w0(1,ix), w1(1,ix) )
end do 
return
end
!<
! nmo_2d_l
!
! USAGE
! call nmo_2d_l(tra, inv,  slow, t0, dt, x0, dx, nt, nx, tt, zz)
!
! DESCRIPTION
! Nmo by linear interpolation for a gather
!
! INPUT PARAMETERS
!  tra    -  integer  1=transpose, 0=don't
!  inv    -  integer  1=inverse, 0=forward
!  slow   -  real(n1) slowness
!  t0     -  real     first time
!  dt     -  real     sampling in time
!  x0     -  real     first offset
!  dx     -  real     sampling in space
!  nt     -  integer  number of time samples
!  nx     -  integer  number of offsets
!  tt     -  real(nt,nx)  data
!  zz     -  real(nt,nx)  model
!
! CATEGORY
! Lib:Velan
!
! COMMENTS
! NMO, transpose NMO, and inverse NMO by linear interpolation.
! tra=0 inv=0:        zz(iz) = [NMO]                   tt(it)
! tra=1 inv=0:        tt(it) = [NMO transpose]         zz(iz)
! tra=0 inv=1:        tt(it) = [NMO inverse]           zz(iz)
! tra=1 inv=1:        zz(iz) = [NMO inverse transpose] tt(it)
!
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine nmo_2d_l( tra, inv, slow, t0, dt, x0, dx, nt, nx, tt, zz)
use sep_mod
use sep_func_mod
integer  tra, inv, nt, ix, nx, itab(nt)
real slow(nt), t0, dt, x0, dx, x, tt(nt,nx), zz(nt,nx),bb(nt), cc(nt),&
  & w0(nt), w1(nt)
do ix = 1, nx  
  x = x0 + ix * dx
  call nmo_l( tra, inv, 1, slow, x, t0, dt, nt, tt(1,ix), zz(1,ix),itab&
    &, bb, cc, w0, w1 )
end do 
return
end
!<
! nmo_l
!
! USAGE
! call nmo_l(tra, inv, mktab, slow, x, t0, dt, nt, tt, zz, itab, bb, cc, w0, w1 )
!
! DESCRIPTION
! Nmo by linear interpolation for a gather with tabulation (speeds up
! multiple regualar datasets)
!
! INPUT PARAMETERS
!  tra    -  integer  1=transpose, 0=don't
!  inv    -  integer  1=inverse, 0=forward
!  mktab  -  integer  first time needs to (=1 to initalize)
!  slow   -  real(n1) slowness
!  x      -  real     offset
!  t0     -  real     first time
!  dx     -  real     sampling in space
!  nt     -  integer  number of time samples
!  tt     -  real(nt,nx)  data
!  zz     -  real(nt,nx)  model
!  itab   -  integer(nt,nx) location table to speed up linear interp
!  bb     -  real(nt,nx)  weights to apply to linear interp
!  cc     -  real(nt,nx)  weights to apply to linear interp
!  w0     -  real(nt,nx)  weights to apply to linear interp
!  w1     -  real(nt,nx)  weights to apply to linear interp
!
! CATEGORY
! Lib:Velan
!
! COMMENTS
! NMO, transpose NMO, and inverse NMO by linear interpolation.
! tra=0 inv=0:        zz(iz) = [NMO]                   tt(it)
! tra=1 inv=0:        tt(it) = [NMO transpose]         zz(iz)
! tra=0 inv=1:        tt(it) = [NMO inverse]           zz(iz)
! tra=1 inv=1:        zz(iz) = [NMO inverse transpose] tt(it)
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine nmo_l( tra, inv, mktab, slow, x, t0, dt, nt, tt, zz,itab, bb&
  &, cc, w0, w1 )                ! work space
use sep_mod
use sep_func_mod
integer  tra, inv, it, nt, iz, mktab, itab(nt)
real slow(nt), t0, dt, tt(nt), zz(nt),t, x, z, tm, tpart, xs, arg,bb(nt&
  &), cc(nt), w0(nt), w1(nt), ts(nt)
!automatic ts
if ( tra .eq. inv ) then
  do iz = 1, nt
    zz(iz) = 0.
  end do
else
  do it = 1, nt
    tt(it) = 0.
  end do
end if 
do it = 1, nt  
! initialize tridiagonal coefs.
  cc(it) = 0.
  bb(it) = 0.
end do 
if ( mktab .ne. 0 ) then
! tabulate pointers and weights.
  z = t0 + nt * dt
  t = z
  do iz = nt, 1, -1   
    xs = x * slow(iz)
    arg = z * z + xs * xs
! next line replaceable by:        t = sqrt ( arg )
    t = (arg + t * t) / (t + t)
    it = (t - t0) / dt + .00001
    tm = t0 + it * dt
    tpart = t - tm
    w0(iz) = (dt - tpart) / dt
    w1(iz) = 1. - w0(iz)
    itab(iz) = 0
    if ( it+1 <= nt ) then
! interior
      itab(iz)  = it
      bb(it)   = bb(it)   + w0(iz) * w0(iz)
      cc(it)   = cc(it)   + w1(iz) * w0(iz)
      bb(it+1) = bb(it+1) + w1(iz) * w1(iz)
    else if ( it <= nt ) then
! at edge
      itab(iz)  =  - it
      bb(it)   = bb(it)   + w0(iz) * w0(iz)
      cc(it)   = cc(it)   + w1(iz) * w0(iz)
    else
! off end
      itab(iz) = 0
    end if 
    z = z - dt
  end do
end if
do it = 1, nt                        ! avoid destruction of input.
  ts(it) = tt(it)
end do 
if ( tra .eq. 1  .and. inv .eq. 1 ) then
! transpose pseudoinverse
  do it = 1, nt
    if ( bb(it) .eq. 0.0 ) then
      bb(it) = 1.0
      ts(it) = 0.0
    end if
  end do 
  call vtris(nt, cc, bb, cc, ts, ts)   ! vtris allows overlay.
end if
if ( tra .eq. inv ) then
! Operator itself or transpose pseudoinverse
  do iz = 1, nt  
    it = itab(iz)
    if ( it > 0 ) then
      zz(iz) = zz(iz) + w0(iz) * ts(it)
      zz(iz) = zz(iz) + w1(iz) * ts(it+1)
    else if ( it < 0 ) then
      it = -it
      zz(iz) = zz(iz) + w0(iz) * ts(it)
    end if
  end do
else
! transpose or inverse
  do iz = 1, nt  
    it = itab(iz)
    if ( it > 0 ) then
      ts(it)   = ts(it)   + w0(iz) * zz(iz)
      ts(it+1) = ts(it+1) + w1(iz) * zz(iz)
    else if ( it < 0 ) then
      it = -it
      ts(it)   = ts(it)   + w0(iz) * zz(iz)
    end if
  end do
end if 
if ( tra .eq. 0  .and.  inv .eq. 1 ) then
! pseudoinverse
  do it = 1, nt
    if ( bb(it) .eq. 0.0 ) then
      bb(it) = 1.0
      ts(it) = 0.0
    end if
  end do 
  call vtris(nt, cc, bb, cc, ts, tt)
else
  do it = 1, nt
    tt(it) = ts(it)
  end do
end if 
return
end
!<
! lmo_2d_l
!
! USAGE
! call lmo_2d_l(tra, inv,  slow, t0, dt, x0, dx, nt, nx, tt, zz)
!
! DESCRIPTION
! Lmo by linear interpolation for a gather
!
! INPUT PARAMETERS
!  tra    -  integer  1=transpose, 0=don't
!  inv    -  integer  1=inverse, 0=forward
!  slow   -  real(n1) slowness
!  t0     -  real     first time
!  dt     -  real     sampling in time
!  x0     -  real     first offset
!  dx     -  real     sampling in space
!  nt     -  integer  number of time samples
!  nx     -  integer  number of offsets
!  tt     -  real(nt,nx)  data
!  zz     -  real(nt,nx)  model
!
! CATEGORY
! Lib:Velan
!
! COMMENTS
! LMO, transpose LMO, and inverse LMO by linear interpolation.
! tra=0 inv=0:        zz(iz) = [NMO]                   tt(it)
! tra=1 inv=0:        tt(it) = [NMO transpose]         zz(iz)
! tra=0 inv=1:        tt(it) = [NMO inverse]           zz(iz)
! tra=1 inv=1:        zz(iz) = [NMO inverse transpose] tt(it)
!
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine lmo_2d_l( tra, inv, slow, t0, dt, x0, dx, nt, nx, tt, zz)
use sep_mod
integer  tra, inv, nt, ix, nx, itab(nt)
real slow(nt), t0, dt, x0, dx, x, tt(nt,nx), zz(nt,nx),bb(nt), cc(nt),&
  & w0(nt), w1(nt)
do ix = 1, nx  
  x = x0 + ix * dx
  call lmo_l( tra, inv, 1, slow, x, t0, dt, nt, tt(1,ix), zz(1,ix),itab&
    &, bb, cc, w0, w1 )
end do 
return
end
!<
! lmo_l
!
! USAGE
! call lmo_l(tra, inv, mktab, slow, x, t0, dt, nt, tt, zz, itab, bb, cc, w0, w1 )
!
! DESCRIPTION
! Lmo by linear interpolation for a gather with tabulation (speeds up
! multiple regualar datasets)
!
! INPUT PARAMETERS
!  tra    -  integer  1=transpose, 0=don't
!  inv    -  integer  1=inverse, 0=forward
!  mktab  -  integer  first time needs to (=1 to initalize)
!  slow   -  real(n1) slowness
!  x      -  real     offset
!  t0     -  real     first time
!  dx     -  real     sampling in space
!  nt     -  integer  number of time samples
!  tt     -  real(nt,nx)  data
!  zz     -  real(nt,nx)  model
!  itab   -  integer(nt,nx) location table to speed up linear interp
!  bb     -  real(nt,nx)  weights to apply to linear interp
!  cc     -  real(nt,nx)  weights to apply to linear interp
!  w0     -  real(nt,nx)  weights to apply to linear interp
!  w1     -  real(nt,nx)  weights to apply to linear interp
!
! CATEGORY
! Lib:Velan
!
! COMMENTS
! NMO, transpose NMO, and inverse NMO by linear interpolation.
! tra=0 inv=0:        zz(iz) = [NMO]                   tt(it)
! tra=1 inv=0:        tt(it) = [NMO transpose]         zz(iz)
! tra=0 inv=1:        tt(it) = [NMO inverse]           zz(iz)
! tra=1 inv=1:        zz(iz) = [NMO inverse transpose] tt(it)
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine lmo_l( tra, inv, mktab, slow, x, t0, dt, nt, tt, zz,itab, bb&
  &, cc, w0, w1 )                ! work space
use sep_mod
integer  tra, inv, it, nt, iz, mktab, itab(nt)
real slow(nt), t0, dt, tt(nt), zz(nt),t, x, z, tm, tpart, xs, arg,bb(nt&
  &), cc(nt), w0(nt), w1(nt), ts(nt)
!automatic ts
if ( tra .eq. inv ) then
  do iz = 1, nt
    zz(iz) = 0.
  end do
else
  do it = 1, nt
    tt(it) = 0.
  end do
end if 
do it = 1, nt  
! initialize tridiagonal coefs.
  cc(it) = 0.
  bb(it) = 0.
end do 
if ( mktab .ne. 0 ) then
! tabulate pointers and weights.
  z = t0 + nt * dt
  t = z
  do iz = nt, 1, -1   
    xs = x * slow(iz)
    t = z + xs
    it = (t - t0) / dt + .00001
    tm = t0 + it * dt
    tpart = t - tm
    w0(iz) = (dt - tpart) / dt
    w1(iz) = 1. - w0(iz)
    itab(iz) = 0
    if ((it>=1).and.( it+1 <= nt )) then
! interior
      itab(iz)  = it
      bb(it)   = bb(it)   + w0(iz) * w0(iz)
      cc(it)   = cc(it)   + w1(iz) * w0(iz)
      bb(it+1) = bb(it+1) + w1(iz) * w1(iz)
    else if ((it>=1).and.( it <= nt )) then
! at edge
      itab(iz)  =  - it
      bb(it)   = bb(it)   + w0(iz) * w0(iz)
      cc(it)   = cc(it)   + w1(iz) * w0(iz)
    else
! off end
      itab(iz) = 0
    end if 
    z = z - dt
  end do
end if
do it = 1, nt                        ! avoid destruction of input.
  ts(it) = tt(it)
end do 
if ( tra .eq. 1  .and. inv .eq. 1 ) then
! transpose pseudoinverse
  do it = 1, nt
    if ( bb(it) .eq. 0.0 ) then
      bb(it) = 1.0
      ts(it) = 0.0
    end if
  end do 
  call vtris(nt, cc, bb, cc, ts, ts)   ! vtris allows overlay.
end if
if ( tra .eq. inv ) then
! Operator itself or transpose pseudoinverse
  do iz = 1, nt  
    it = itab(iz)
    if ( it > 0 ) then
      zz(iz) = zz(iz) + w0(iz) * ts(it)
      zz(iz) = zz(iz) + w1(iz) * ts(it+1)
    else if ( it < 0 ) then
      it = -it
      zz(iz) = zz(iz) + w0(iz) * ts(it)
    end if
  end do
else
! transpose or inverse
  do iz = 1, nt  
    it = itab(iz)
    if ( it > 0 ) then
      ts(it)   = ts(it)   + w0(iz) * zz(iz)
      ts(it+1) = ts(it+1) + w1(iz) * zz(iz)
    else if ( it < 0 ) then
      it = -it
      ts(it)   = ts(it)   + w0(iz) * zz(iz)
    end if
  end do
end if 
if ( tra .eq. 0  .and.  inv .eq. 1 ) then
! pseudoinverse
  do it = 1, nt
    if ( bb(it) .eq. 0.0 ) then
      bb(it) = 1.0
      ts(it) = 0.0
    end if
  end do 
  call vtris(nt, cc, bb, cc, ts, tt)
else
  do it = 1, nt
    tt(it) = ts(it)
  end do
end if 
return
end
!<
! nmo_2d_unitary
!
! USAGE
! call nmo_2d_unitary(tra, inv, i3, slow, t0, dt, x0, dx, nt, nx, tt, zz, itab, bb, cc, w0, w1 )
!
! DESCRIPTION
! pseudounitary NMO
!
! INPUT PARAMETERS
!  tra    -  integer  1=transpose, 0=don't
!  inv    -  integer  1=inverse, 0=forward
!  slow   -  real(n1) slowness
!  i3     -  integer  first time needs to (=1 to initalize)
!  t0     -  real     first time
!  dt     -  real     sampling in time
!  x0     -  real     first offset
!  dx     -  real     sampling in space
!  nt     -  integer  number of time samples
!  nx     -  integer  number of offsets
!  tt     -  real(nt,nx)  data
!  zz     -  real(nt,nx)  model
!  itab   -  integer(nt,nx) location table to speed up linear interp
!  dd     -  real(nt,nx)  work area?
!  ee     -  real(nt,nx)  work area?
!  w0     -  real(nt,nx)  weights to apply to linear interp
!  w1     -  real(nt,nx)  weights to apply to linear interp
!
! CATEGORY
! Lib:Velan
!
! COMMENTS
! NMO, transpose NMO, and inverse NMO by linear interpolation.
! tra=0 inv=0:        zz(iz) = [NMO]                   tt(it)
! tra=1 inv=0:        tt(it) = [NMO transpose]         zz(iz)
! tra=0 inv=1:        tt(it) = [NMO inverse]           zz(iz)
! tra=1 inv=1:        zz(iz) = [NMO inverse transpose] tt(it)
!
! i3=1 calls for tables to be built. mandatory on first call.
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine unmo_2d( conj, inv, i3, slow, t0, dt, x0, dx, nt, nx, tt, zz&
  &,itab, dd, ee, w0, w1 )
use sep_mod
integer  conj,inv, i3, nt, ix, nx, itab(nt,nx)
real slow(nt), t0, dt, x0, dx, x, tt(nt,nx), zz(nt,nx),dd(nt,nx), ee(nt&
  &,nx), w0(nt,nx), w1(nt,nx)
do ix = 1, nx  
  x = x0 + ix * dx
  call unmo( conj,inv, i3, slow, x, t0, dt, nt, tt(1,ix), zz(1,ix),itab&
    &(1,ix), dd(1,ix), ee(1,ix), w0(1,ix), w1(1,ix) )
end do 
return
end
!<
! unmo
!
! USAGE
! call unmo( conj, inv, mktab, slow, x, t0, dt, nt, tt, zz, itab, dd, ee, w0, w1 )
!
! DESCRIPTION
! pseudounitary NMO
!
! INPUT PARAMETERS
!  conj   -  integer  1=conjugate, 0=don't
!  inv    -  integer  1=inverse, 0=forward
!  mktab  -  integer  first time needs to (=1 to initalize)
!  slow   -  real(n1) slowness
!  x      -  real     offset
!  t0     -  real     first time
!  dt     -  real     sampling in time
!  dx     -  real     sampling in space
!  nt     -  integer  number of time samples
!  tt     -  real(nt,nx)  data
!  zz     -  real(nt,nx)  model
!  itab   -  integer(nt,nx) location table to speed up linear interp
!  dd     -  real(nt,nx)  work area?
!  ee     -  real(nt,nx)  work area?
!  w0     -  real(nt,nx)  weights to apply to linear interp
!  w1     -  real(nt,nx)  weights to apply to linear interp
!
! CATEGORY
! Lib:Velan
!
! COMMENTS
! NMO, transpose NMO, and inverse NMO by linear interpolation.
! tra=0 inv=0:        zz(iz) = [NMO]                   tt(it)
! tra=1 inv=0:        tt(it) = [NMO transpose]         zz(iz)
! tra=0 inv=1:        tt(it) = [NMO inverse]           zz(iz)
! tra=1 inv=1:        zz(iz) = [NMO inverse transpose] tt(it)
!
! i3=1 calls for tables to be built. mandatory on first call.
!
! COMPILE LEVEL
! DISTR
!>
subroutine unmo( conj, inv, mktab, slow, x, t0, dt, nt, tt, zz,itab, dd&
  &, ee, w0, w1 )                ! work space
use sep_mod
integer  conj, inv, it, nt, iz, mktab, itab(nt)
real slow(nt), t0, dt, tt(nt), zz(nt),t, x, z, tm, tpart, xs, arg,w0(nt&
  &), w1(nt), dd(nt), ee(nt),bb(4000), cc(4000), zt(4000)
if ( mktab .eq. 1 ) then
! tabulate pointers and weights.
  do it = 1, nt  
    cc(it) = 0.
    bb(it) = 0.
  end do 
  z = t0 + nt * dt
  t = z
  do iz = nt, 1, -1   
    xs = x * slow(iz)
    arg = z * z + xs * xs
! next line replaceable by:        t = sqrt ( arg )
    t = (arg + t * t) / (t + t)
    it = (t - t0) / dt + .00001
    tm = t0 + it * dt
    tpart = t - tm
    w0(iz) = (dt - tpart) / dt
    w1(iz) = 1. - w0(iz)
    itab(iz) = 0
    if ( it+1 <= nt ) then
! interior
      itab(iz)  = it
      bb(it)   = bb(it)   + w0(iz) * w0(iz)
      cc(it)   = cc(it)   + w1(iz) * w0(iz)
      bb(it+1) = bb(it+1) + w1(iz) * w1(iz)
    else if ( it <= nt ) then
! at edge
      itab(iz)  =  - it
      bb(it)   = bb(it)   + w0(iz) * w0(iz)
      cc(it)   = cc(it)   + w1(iz) * w0(iz)
    else
! off end
      itab(iz) = 0
    end if 
    z = z - dt
  end do 
  dd(1) = sqrt( bb(1) )
  do it = 1, nt-1  
    if ( dd(it) .ne. 0. ) then
      ee(it) = cc(it) / dd(it)
    else
      ee(it) = 0.
    end if 
    dd(it+1) = sqrt( bb(it+1) - ee(it)*ee(it) )
  end do
end if
if ( conj .eq. inv ) then
! Operator itself
  if ( dd(nt) .ne. 0. ) then
    zt(nt) = tt(nt) / dd(nt)
  else
    zt(nt) = 0.
  end if 
  do it = nt-1, 1, -1
    if ( dd(it) .ne. 0. ) then
      zt(it) = (tt(it) - ee(it) * zt(it+1)) / dd(it)
    else
      zt(it) = 0.
    end if
  end do 
  do iz = 1, nt
    zz(iz) = 0.
  end do 
  do iz = 1, nt  
    it = itab(iz)
    if ( it > 0 ) then
      zz(iz) = zz(iz) + w0(iz) * zt(it)
      zz(iz) = zz(iz) + w1(iz) * zt(it+1)
    else if ( it < 0 ) then
      it = -it
      zz(iz) = zz(iz) + w0(iz) * zt(it)
    end if
  end do
else
! pseudoinverse
  do it = 1, nt
    zt(it) = 0.
  end do 
  do iz = 1, nt  
    it = itab(iz)
    if ( it > 0 ) then
      zt(it)   = zt(it)   + w0(iz) * zz(iz)
      zt(it+1) = zt(it+1) + w1(iz) * zz(iz)
    else if ( it < 0 ) then
      it = -it
      zt(it)   = zt(it)   + w0(iz) * zz(iz)
    end if
  end do 
  if ( dd(1) .ne. 0. ) then
    tt(1) = zt(1) / dd(1)
  else
    tt(1) = 0.
  end if 
  do it = 2, nt
    if ( dd(it) .ne. 0. ) then
      tt(it) = (zt(it) - ee(it-1) * tt(it-1)) / dd(it)
    else
      tt(it) = 0.
    end if
  end do
end if 
return
end
!<
! row_nmo_2d
!
! USAGE
! call row_nmo_2d(conj, inv, slow, t0, dt, x0, dx, n1, n2, tt, zz)
!
! DESCRIPTION
!Linear interpolation NMO
! pseudo inverse by inverse of row sum.
!
! INPUT PARAMETERS
!
!  conj   -  integer  1=conjugate, 0=don't
!  inv    -  integer  1=inverse, 0=forward
!  slow   -  real(n1) slowness
!  t0     -  real     first time
!  dt     -  real     sampling in time
!  x0     -  real     first offset
!  dx     -  real     sampling in space
!  n1     -  integer  number of time samples
!  n2     -  integer  number of offsets
!  zz     -  real(n1,n2)  model
!  tt     -  real(n1,n2)  data
!
! CATEGORY
! Lib:Velan
!
! COMMENTS
!
! NMO, its conjugate, inverse, and inverse conjugate by nearest neighbor.
! conj=0 inv=0:        zz(iz) = [NMO]                   tt(it)
! conj=1 inv=0:        tt(it) = [NMO conj]         zz(iz)
! conj=0 inv=1:        tt(it) = [NMO inverse]           zz(iz)
! conj=1 inv=1:        zz(iz) = [NMO inverse conj] tt(it)
!
! COMPILE LEVEL
! DISTR
!>
!
!
subroutine row_nmo_2d( conj, inv, slow, t0, dt, x0, dx, nt, nx, tt, zz&
  &)
use sep_mod
integer  conj, inv, nt, ix, nx, itab(nt)
real slow(nt), t0, dt, x0, dx, x, tt(nt,nx), zz(nt,nx),w0(nt), w1(nt),&
  & zsum(nt)
do ix = 1, nx  
  x = x0 + ix * dx
  call row_nmo( conj, inv, 1, slow, x, t0, dt, nt,tt(1,ix), zz(1,ix),&
    & itab, w0, w1, zsum )
!  call slice('zsum.h',4,nt,1,zsum)
end do 
return
end
!<
! row_nmo
!
! USAGE
! call row_nmo(conj, inv, mktab,slow, x,t0, dt,  nt, tt, zz,itab,w0,w1,zsum)
!
! DESCRIPTION
!Linear interpolation NMO
! pseudo inverse by inverse of row sum.
!
! INPUT PARAMETERS
!  conj   -  integer  1=conjugate, 0=don't
!  inv    -  integer  1=inverse, 0=forward
!  mktab  -  integer  1=first call
!  slow   -  real(n1) slowness
!  x      -  real     offset
!  t0     -  real     first time
!  dt     -  real     sampling in time
!  nt     -  integer  number of time samples
!  tt     -  real(nt)  data
!  zz     -  real(nt)  model
!  itab   -  integer(nt) table
!  w0     -  real(nt)   weights
!  w1     -  real(nt)   weights
!  zsum   -  real(nt)   work area
!
! CATEGORY
! Lib:Velan
!
! COMMENTS
!
! NMO, its conjugate, inverse, and inverse conjugate by nearest neighbor.
! conj=0 inv=0:        zz(iz) = [NMO]                   tt(it)
! conj=1 inv=0:        tt(it) = [NMO conj]         zz(iz)
! conj=0 inv=1:        tt(it) = [NMO inverse]           zz(iz)
! conj=1 inv=1:        zz(iz) = [NMO inverse conj] tt(it)
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine row_nmo( conj, inv, mktab, slow, x, t0, dt, nt, tt, zz,itab&
  &, w0, w1, zsum )
use sep_mod
integer  conj, inv, it, nt, iz, mktab, itab(nt)
real slow(nt), t0, dt, tt(nt), zz(nt),t, x, z, tm, tpart, xs, arg,w0(nt&
  &), w1(nt), zsum(nt), ts(nt)
if ( conj .eq. inv ) then
  do iz = 1, nt
    zz(iz) = 0.
  end do
else
  do it = 1, nt
    tt(it) = 0.
  end do
end if 
do it = 1, nt
  zsum(it) = 0.
end do 
if ( mktab .ne. 0 ) then
! tabulate pointers and weights.
  z = t0 + nt * dt
  t = z
  do iz = nt, 1, -1   
    xs = x * slow(iz)
    arg = z * z + xs * xs
! next line replaceable by:        t = sqrt ( arg )
    t = (arg + t * t) / (t + t)
    it = (t - t0) / dt + .00001
    tm = t0 + it * dt
    tpart = t - tm
    w0(iz) = (dt - tpart) / dt
    w1(iz) = 1. - w0(iz)
    itab(iz) = 0
    if ( it+1 <= nt ) then
! interior
      itab(iz)  = it
      zsum(it  ) = zsum(it  ) + w0(iz)
      zsum(it+1) = zsum(it+1) + w1(iz)
    else if ( it <= nt ) then
! at edge
      itab(iz)  =  - it
      zsum(it  ) = zsum(it  ) + w0(iz)
    else
! off end
      itab(iz) = 0
    end if 
    z = z - dt
  end do
end if
do it = 1, nt                        ! avoid destruction of input.
  ts(it) = tt(it)
end do 
if ( conj .eq. 1  .and. inv .eq. 1 ) then
! transpose pseudoinverse
  do it = 1,nt
    if ( zsum(it) > 0. ) then
      ts(it) = ts(it) / zsum(it)
    end if
  end do
end if
if ( conj .eq. inv ) then
! Operator itself or transpose pseudoinverse
  do iz = 1, nt  
    it = itab(iz)
    if ( it > 0 ) then
      zz(iz) = zz(iz) + w0(iz) * ts(it)
      zz(iz) = zz(iz) + w1(iz) * ts(it+1)
    else if ( it < 0 ) then
      it = -it
      zz(iz) = zz(iz) + w0(iz) * ts(it)
    end if
  end do
else
! transpose or inverse
  do iz = 1, nt  
    it = itab(iz)
    if ( it > 0 ) then
      ts(it)   = ts(it)   + w0(iz) * zz(iz)
      ts(it+1) = ts(it+1) + w1(iz) * zz(iz)
    else if ( it < 0 ) then
      it = -it
      ts(it)   = ts(it)   + w0(iz) * zz(iz)
    end if
  end do
end if 
if ( conj .eq. 0  .and.  inv .eq. 1 ) then
! pseudoinverse
  do it = 1,nt
    if ( zsum(it) > 0. ) then
      tt(it) = ts(it) / zsum(it)
    end if
  end do
else
  do it = 1, nt
    tt(it) = ts(it)
  end do
end if 
return
end
!<
! offset
!
! USAGE
! call offset(conj, inv, slow, nt,t0, dt, nx,x0, dx, ny, y0, dy, xx,yy)
!
! DESCRIPTION
! yy = [interpolation on hyperbolic curves] * xx
!
! INPUT PARAMETERS
!  conj   -  integer  1=conjugate,
!  inv    -  integer  1=inverse, 0=forward
!  slow   -  real(n1) slowness
!  nt     -  integer  number of time samples
!  t0     -  real     first time
!  dt     -  real     sampling in time
!  x0     -  real     first x
!  dx     -  real     sampling in x
!  nx     -  integer  number of samples in x
!  y0     -  real     first y
!  dy     -  real     sampling in y
!  ny     -  integer  number of samples in y
!  xx     -  real(nt,nx) model
!  yy     -  real(nt,nx) data
!
! CATEGORY
! Lib:Velan
!
! COMPILE LEVEL
! DISTR
!>
!
subroutine offset(conj,inv,slow,nt,t0,dt,nx,x0,dx,ny,y0,dy,xx,yy)
use sep_mod
integer conj,inv, it, nt, ix, nx, iy, ny, jt, kt, iz, iter
real t0,dt, x0,dx, y0,dy, xx(nt,nx), yy(nt,ny), slow(nt),x, y, w0, w1,&
  & xm, part, arg, xs, dtodx, z, t
if ( inv.eq.1 ) then
  call erexit('program offset not smart enough to do inverse')
end if
do it=1,nt
  if ( conj .eq. 0 ) then
    do iy = 1, ny
      yy(it,iy) = 0.
    end do
  else
    do ix = 1, nx
      xx(it,ix) = 0.
    end do
  end if
end do 
do it=1,nt  
  t = t0 + it * dt
  iz = it
  z = t0 + it * dt
  do iy=1,ny  
    y = y0 + iy * dy
    x = y
    ix = (x - x0) / dx     +     .5
    xm = x0 + ix * dx
    part = x - xm
    w1 =  part / dx
    w0 = 1. - w1
    do iter=1,1  
      xs = x * slow(iz)
      arg = t*t - xs*xs
      if ( z .ne. 0.0 ) then
        z = (arg + z*z) / (z + z)
      end if
      iz = (z - t0) / dt
      if ( iz < 1 ) then
        iz = 1
      end if
      if ( iz > nt ) then
        iz = nt
      end if
    end do 
    dtodx = xs * slow(iz) / (t * dt + 1.e-10)
    jt = it - (   part) * dtodx + .5
    kt = it + (dx-part) * dtodx + .5
    if ( 0 < ix .and. ix+1 <= nx  .and.  0 < jt .and.   jt <= nt  .and.&
      &  0 < kt .and.   kt <= nt   ) then
      if ( conj .eq. 0 ) then
        yy(it,iy) = yy(it,iy) + w0 * xx(jt,ix  )
        yy(it,iy) = yy(it,iy) + w1 * xx(kt,ix+1)
      else
        xx(jt,ix  ) = xx(jt,ix  ) + w0 * yy(it,iy)
        xx(kt,ix+1) = xx(kt,ix+1) + w1 * yy(it,iy)
      end if
    end if
  end do
end do 
return
end
!<
! radial
!
! USAGE
! call radial(conj, inv, t0, dt,x0, dx, u0,du, nt, nx, nu, xx, uu,count)
!
! DESCRIPTION
! Make radial traces
!
! INPUT PARAMETERS
!  conj   -  integer  1=conjugate,
!  inv    -  integer  1=inverse, 0=forward
!  t0     -  real     first time
!  dt     -  real     sampling in time
!  x0     -  real     first x
!  dx     -  real     sampling in x
!  u0     -  real     first u
!  du     -  real     sampling in u
!  nt     -  integer  number of time samples
!  y0     -  real     first y
!  nx     -  integer  number of samples in x
!  nu     -  integer  number of samples in u
!  xx     -  real(nt,nx) model
!  uu     -  real(nt,nu) u
!  count  -  real(nt,nx) count
!
! CATEGORY
! Lib:Velan
!
! COMPILE LEVEL
! DISTR
!>
!
!
subroutine radial(conj, inv, t0, dt, x0, dx, u0, du , nt, nx, nu, xx,&
  & uu, count)
use sep_mod
integer nt, nx, nu, nz, conj,inv,it, ix, iu, iz
real xx(nt,nx), uu(nt,nu), count(nt,nx),t0, x0,  dt, dx,z0, u0,  dz, du&
  &,t, x, z, u, tanang, cosi
z0 = t0   ! removed moveout
dz = dt   ! removed moveout
nz = nt
if ( conj .eq. inv ) then
  do iz=1,nz
    do iu=1,nu
      uu   (iz,iu) = 0.
    end do
  end do
else
  do it=1,nt
    do ix=1,nx  
      xx   (it,ix) = 0.
      count(it,ix) = 0.
    end do
  end do
end if 
do iu=1,nu  
  u = u0 + iu*du
! tanang = u * slow(iu)
! cosi =  sqrt( 1. + tanang * tanang )
  cosi = 1.   ! removed moveout
  do iz=1,nz  
    z = z0 + iz*dz
    x = u * z
    ix = (x - x0) / dx + .5
    t = z * cosi
    it = (t - t0) / dt + .5
    if ( 0 < ix  .and.  ix <= nx  .and.  0 < it  .and.  it <= nt )&
      & then
      if ( conj .eq. inv ) then
        uu(iz,iu) = uu(iz,iu) + xx(it,ix)
      else
        xx(it,ix) = xx(it,ix) + uu(iz,iu)
        count(it,ix) = count(it,ix) + 1.
      end if
    end if
  end do
end do 
if ( inv .eq. 1 ) then
  do ix=1,nx
    do it=1,nt
      if ( count(it,ix) > 0.0 ) then
        xx(it,ix) = xx(it,ix) / count (it,ix)
      end if
    end do
  end do
end if
return
end
! Radial traces with normal moveout.
! z in this program is like traveltime depth tau in real life.
subroutine radnmo( conj,inv, slow, t0, dt, x0, dx, z0, dz, u0, du,nt,&
  & nx, nu, xx, uu, count)
use sep_mod
integer nt, nx, nu, nz, conj,inv,it, ix, iu, iz
real xx(nt,nx), uu(nt,nu), count(nt,nx), slow(nt),t0, x0,  dt, dx,z0,&
  & u0,  dz, du,t, x, z, u, tanang, cosi
nz = nt
if ( conj .eq. inv ) then
  do iz=1,nz
    do iu=1,nu
      uu   (iz,iu) = 0.
    end do
  end do
else
  do it=1,nt
    do ix=1,nx  
      xx   (it,ix) = 0.
      count(it,ix) = 0.
    end do
  end do
end if 
do iu=1,nu  
  u = u0 + iu*du
  tanang = u * slow(iu)
  cosi =  sqrt( 1. + tanang * tanang )
  do iz=1,nz  
    z = z0 + iz*dz
    x = u * z
    ix = (x - x0) / dx + .5
    t = z * cosi
    it = (t - t0) / dt + .5
    if ( 0 < ix  .and.  ix <= nx  .and.  0 < it  .and.  it <= nt )&
      & then
      if ( conj .eq. inv ) then
        uu(iz,iu) = uu(iz,iu) + xx(it,ix)
      else
        xx(it,ix) = xx(it,ix) + uu(iz,iu)
        count(it,ix) = count(it,ix) + 1.
      end if
    end if
  end do
end do 
if ( inv .eq. 1 ) then
  do ix=1,nx
    do it=1,nt
      if ( count(it,ix) > 0.0 ) then
        xx(it,ix) = xx(it,ix) / count (it,ix)
      end if
    end do
  end do
end if
return
end
! Determine the grid for radial traces
subroutine radgrd(conj,inv,nt,nx,t0,dt,x0,dx,slow,nz,nu,z0,dz,u0,du)
use sep_mod
integer  nt, nx, nz, nu, iter, conj,inv, junk,ierr
real    t0, x0,  dt, dx,    slow,z0, u0,  dz, du,tmax, uritee, ulefte,&
  & umax, zmax,xl, xr, cl, cr
character*40 labelx, labelu
if ( conj .eq. inv ) then
  if (1.ne.hetch("label2","s",labelx)) then
    call seperr("couldn't get label2")
  end if
  if (1.ne.hetch("o1","f",t0)) then
    t0=0.
  end if
  if (1.ne.hetch("d1","f",dt)) then
    call seperr("couldn't get d1")
  end if
  if (1.ne.hetch("o2","f",x0)) then
    call seperr("couldn't get o2")
  end if
  if (1.ne.hetch("d2","f",dx)) then
    call seperr("couldn't get d2")
  end if
! compute defaults and check for overrides.
  tmax = t0 + nt*dt
!ulefte = (x0+ 1*dx) / sqrt( (tmax)**2 - ((x0+ 1*dx)*slow)**2 )
!uritee = (x0+nx*dx) / sqrt( (tmax)**2 - ((x0+nx*dx)*slow)**2 )
!umax = amax1( abs(ulefte), abs(uritee) )
!z0 = (nz*(t0+dt) - zmax) / (nz-1)
!dz = ( - (t0+dt) + zmax) / (nz-1)
  umax = 1.5 * .7071   ! go to 45 degree angles.
  zmax = tmax / sqrt( (1. + umax*umax*slow*slow ) )
  z0 = t0
  junk = tetch('o1','r',z0)
  dz = (zmax - z0 ) / nz
  junk = tetch('d1','r',dz)
  du = dx / zmax
  junk = tetch('d2','r',du)
! match time cutoffs on both sides = xleft/sinleft = xrite/sinrite
  cr = 1.
  cl = 1.
  do iter=1,5  
    xl = x0 + 1*dx
    xr = x0 + nx*dx
    u0 = (xl*cr*nu*du - xr*cl*du)/(xr*cl-xl*cr)
    cr = 1. / sqrt( 1. + ((u0+nu*du)*slow)**2 )
    cl = 1. / sqrt( 1. + ((u0+ 1*du)*slow)**2 )
  end do 
  junk = tetch('o2','r',u0)
  labelu =  'radial angle, velocity'//CHAR(0)
  junk = tetch('label2','s',labelu)
  ierr= putch( 'label2' , 's', labelu )
  ierr= putch("o1","f",z0)
  ierr= putch("d1","f",dz)
  ierr= putch("o2","f",u0)
  ierr= putch("d2","f",du)
else
  ierr=  hetch("label2","s",labelu)
  z0=0
  ierr=  hetch("o1","f",z0 )
  ierr=  hetch("d1","f",dz)
  ierr=  hetch("o2","f",u0)
  ierr=  hetch("d2","f",du)
! Nobody would ever start out in radial space and
! I am too lazy to compute defaults, so I'll insist parameters are there.
  if (1.ne.hetch("label2","s",labelx)) then
    call seperr("couldn't get label2")
  end if
  if (1.ne.hetch("o1","f",t0)) then
    t0=z0
  end if
  if (1.ne.hetch("d1","f",dt)) then
    dt=dz
  end if
  if (1.ne.hetch("o2","f",x0)) then
    call seperr("couldn't get o2")
  end if
  if (1.ne.hetch("d2","f",dx)) then
    call seperr("couldn't get d2")
  end if
end if 
ierr= putch( 'label2' , 's', 'radial angle, velocity' )
return
end
subroutine adjnull(    adj, add,                     zz,n1,  tt,n2)
use sep_mod
integer adj,add
real zz(n1),tt(n2)
if (add .eq.0) then
  if (adj.eq.0) then
    tt=0
  else
    zz=0.
  end if
end if
return
end  

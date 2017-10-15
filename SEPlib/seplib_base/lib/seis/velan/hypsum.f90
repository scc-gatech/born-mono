!<
! hypsumtab
!
! USAGE
! call hypsumtab ( t0,dt, x0,dx, v0,dv, nt,nx,nv, itab, count)
!
! INPUT PARAMETERS
! t0    -   real      first time
! dt    -   real      sanpling in time
! x0    -   real      first x
! dx    -   real      sampliing in x
! v0    -   real      first velocity
! dv    -   real      sampling in velocity
! nt    -   integer   number of time samples
! nx    -   integer   number of offsets
! nv    -   integer   number of velocities
! itab  -   integer(nx,nv,nt)  work area
! count -   real(nx,nv,nt)  work area
!
! DESCRIPTION
! rudimentary velocity spectrum program
!
! COMMENTS
! pseudo-unitary
! nearest neighbor.
! square root in inner loop of table builder.
!
! CATEGORY
! Lib:Velan
!
! COMPILE LEVEL
! DISTR
!>
!  table building
!subroutine hypsumtab ( t0,dt, x0,dx, v0,dv, nt,nx,nv, itab, ctx, czv)
subroutine hypsumtab ( t0,dt, x0,dx, v0,dv, nt,nx,nv, itab, count)
integer it,iz,ix,iv, nt,nx,nv
integer itab (nx,nv,nt)
real        t0,x0,v0, dt,dx,dv, t,z, x,v, xov, arg,count(nx,nv,nt)
!        ctx(nt,nx), czv(nt,nv)
!do it=1,nt
!        do ix=1,nx
!                ctx(it,ix) = 0.0
!do iz=1,nt
!        do iv=1,nv
!                czv(iz,iv) = 0.0
do it=1,nt
  do iv=1,nv
    do ix=1,nx
      count(ix,iv,it) = 0.
    end do
  end do
end do 
do iz=1,nt  
  z = t0 + (iz-1) * dt
  z = max( z, dt)
  do iv =1,nv  
    v = v0 + (iv-1) * dv
    do ix=1,nx  
      x = x0 + (ix-1) * dx
      xov = x*v        ! v is slowness!
      arg = z*z + xov*xov
      t = sqrt(arg)
      it = 1 + (t-t0) / dt
!                        ctx(it,ix) = ctx(it,ix) + 1.0
!                        czv(iz,iv) = czv(iz,iv) + 1.0
      if ( it <= nt ) then
        itab(ix,iv,iz) = it
        count(ix,iv,it) = count(ix,iv,it) + 1.
      else
        itab(ix,iv,iz) = -1
      end if
    end do
  end do
end do 
!do it=1,nt {
!        t = t0 + (it-1) * dt + dt
!        do ix=1,nx {
!                x = x0 + (ix-1) * dx
!                if( ctx(it,ix) != 0. )
!                        ctx(it,ix) = sqrt(x/t)/ sqrt( ctx (it,ix) )
!                }
!        }
!do iz=1,nt
!        do iv=1,nv  {
!                v = v0 + (iv-1) * dv
!                if( czv(iz,iv) != 0. )
!                        czv(iz,iv) = sqrt(v) / sqrt( czv(iz,iv))
!                }
do it=1,nt
  do ix=1,nx
    do iv=1,nv   
      x = x0 + (ix-1) * dx
      v = v0 + (iv-1) * dv
      t = t0 + (it-1) * dt + dt
      if ( count(ix,iv,it) .ne. 0. ) then
        count(ix,iv,it)=sqrt( (v*x/t) / count(ix,iv,it))
      end if
    end do 
!do it=nt-10,nt
!        do ix=1,nx
!                do iv=1,nv
!                        count(ix,iv,it) = 0.
  end do
end do 
return
end
! transformation assuming tables exist.
! WARNING one of (tx,zv) will be created, the other one DESTROYED.
!subroutine hypsum ( conj,inv, nt,nx,nv, itab, ctx, czv, tx, zv)
subroutine hypsum ( conj,inv, nt,nx,nv, itab, count, tx, zv)
integer conj, inv,  it,iz,ix,iv,  nt,nx,nv
real         tx(nt,nx),  zv(nt,nv), rho,count(nx,nv,nt)
!        ctx(nt,nx), czv(nt,nv)
integer itab (nx,nv,nt)
rho = 1. - 2./nt
if ( conj .eq. inv) then
  do iz=1,nt
    do iv=1,nv
      zv(iz,iv) = 0.
    end do
  end do
else
  do it=1,nt
    do ix=1,nx
      tx(it,ix) = 0.
    end do
  end do
end if 
if ( conj .eq. inv ) then
!        do it = 1, nt
!                do ix=1,nx
!                        tx(it,ix) = tx(it,ix) * ctx(it,ix)
  do iz = 1, nt
    do ix=1,nx
      do iv=1,nv 
        it = itab(ix,iv,iz)
        if ( it > 0 ) then
          zv(iz,iv) = zv(iz,iv) +tx(it,ix) * count(ix,iv,it)
        end if
      end do 
!        do iz = 1,nt
!                do iv=1,nv
!                        zv(iz,iv) = zv(iz,iv) * czv(iz,iv)
    end do
  end do 
  do iv=1,nv
    call halfint ( 1, 1, nt, rho, zv(1,iv), zv(1,iv) )
  end do
else
  do iv=1,nv
    call halfint ( 0, 1, nt, rho, zv(1,iv), zv(1,iv) )
  end do 
!        do iz = 1,nt
!                do iv=1,nv
!                        zv(iz,iv) = zv(iz,iv) * czv(iz,iv)
  do iz = 1, nt  
    do iv=1,nv  
      do ix=1,nx 
        it = itab(ix,iv,iz)
        if ( it > 0 ) then
          tx(it,ix) = tx(it,ix) +zv(iz,iv) * count(ix,iv,it)
        end if
      end do
    end do
  end do 
!        do it = 1, nt
!                do ix=1,nx
!                        tx(it,ix) = tx(it,ix) * ctx(it,ix)
end if 
return
end
! nmo1.r has an alternate way to handle time truncation.
  

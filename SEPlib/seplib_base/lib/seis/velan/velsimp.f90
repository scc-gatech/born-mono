!!$
!!$=head1 NAME
!!$
!!$velsimp - simple velocity transfrom
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call velsimp( adj,add, t0,dt,x0,dx,s0,ds, nt,nx,ns, modl, data)>
!!$
!!$=head1 INPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item adj   - int
!!$
!!$      0- (slow->offset), 1- (offset->slow)
!!$
!!$=item add   - int
!!$
!!$      0- erase output, 1 save output
!!$
!!$=item t0    - real
!!$
!!$      Initial time
!!$
!!$=item dt    - real
!!$
!!$      Sampling in time
!!$
!!$=item x0    - real
!!$
!!$      First offset
!!$
!!$=item dx    - real
!!$
!!$      Offset sampling
!!$
!!$=item s0    - real
!!$
!!$      First slowness
!!$
!!$=item ds    - real
!!$
!!$      Sampling in slowness
!!$
!!$=item nt    - int
!!$
!!$      Number of time samples
!!$
!!$=item nx    - int
!!$
!!$      Number of offset
!!$
!!$=item ns    - int
!!$
!!$      Number of slownesses
!!$
!!$=item modl  - real(nt,ns)
!!$
!!$      time,slowness panel
!!$
!!$=item data - real(nt,nx)
!!$
!!$      time,offset panel
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$A fortran77 program to convert from time,slowness to time, offset
!!$
!!$
!!$=head1 LIBRARY
!!$
!!$B<sepvelanf>
!!$
!!$=cut
!!!$
!$
! velsimp --- simple velocity transform
!
subroutine velsimp( adj,add, t0,dt,x0,dx,s0,ds, nt,nx,ns, modl, data)
integer it,ix,is,   adj,add,                    nt,nx,ns,             &
  & iz,nz
real x,s,sx, t,z, z0,dz,wt,   t0,dt,x0,dx,s0,ds, modl(nt,ns),data(nt,nx&
  &)
call adjnull(       adj,add,                     modl,nt*ns, data,nt*nx&
  &)
nz= nt
z0=t0
dz= dt
! z is travel time depth
do is= 1, ns  
  s = s0 + (is-1) * ds
  do ix= 1, nx  
    x = x0 + (ix-1) * dx
    do iz= 2, nz  
      z = z0 + (iz-1) * dz
      sx =  abs( s * x)  
      t  = sqrt( z * z + sx * sx)
      it = 1.5 + (t - t0) / dt
      if ( it <= nt) then
        wt= (z/t) / sqrt( t)
        if ( adj .eq. 0 ) then
          data(it,ix) = data(it,ix) + modl(iz,is) * sx * wt
        else
          modl(iz,is) = modl(iz,is) + data(it,ix) * sx * wt
        end if
      end if
    end do
  end do
end do 
return
end  

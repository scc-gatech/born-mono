!<
! kirchfast
!
! USAGE
! call kirchfast( adj, add, vrms,     t0,dt,dx, modl,nt,nx, data)
!
! DESCRIPTION
! Kirchhoff migration and diffraction.  (greased lightning)
!
! INPUT PARAMETERS
! adj       - integer   forward=0, adjoint=1
! add       - integer   add=1 (add to output), add=0 (erase output)
! vrms      - real(nt)  RMS velocity
! t0        - real      first sample time
! dt        - real      sampling in time
! dx        - real      sampling in x
! modl      - real(nt,nx)  model
! nt        - integer   number of time samples
! nx        - integer   number of samples in x
! data      - real(nt,nx)  data
!
! CATEGORY
! Lib:Image
!
! COMPILE LEVEL
! DISTR
!
!>
!
subroutine kirchfast( adj, add, vrms,     t0,dt,dx, modl,nt,nx, data)
integer ix,iz,it,ib,  adj, add,                          nt,nx
real    amp,t,z,b,              vrms(nt), t0,dt,dx, modl(nt,nx),data(nt&
  &,nx)
call adjnull(         adj, add,                     modl,nt*nx, data,nt&
  &*nx)
do ib= -nx, nx  
  b = dx * ib                     ! b = offset
  do iz= 2, nt  
    z = t0 + dt * (iz-1)            ! z = travel-time depth
    t = sqrt( z**2  +  (b*2/vrms(iz))**2 )
    it = 1.5 + (t - t0) / dt
    if ( it > nt ) then
      exit
    end if
    amp = (z / t)  *  sqrt( nt*dt / t )
    do ix= max0(1, 1-ib),  min0(nx, nx-ib)
      if ( adj .eq. 0 ) then
        data(it,ix+ib)=data(it,ix+ib)+modl(iz,ix   )*amp
      else
        modl(iz,ix   )=modl(iz,ix   )+data(it,ix+ib)*amp
      end if
    end do
  end do
end do 
return
end  

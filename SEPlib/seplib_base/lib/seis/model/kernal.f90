module kernal_mod
use extrapolation_types
use fftw_flags_mod
use sep

implicit none
real,private :: dx_sq,dz_sq,dt_sq,dtz,dtx,pi,scale ! dtz is for boundary condition  dtz=dt/dz
integer,private :: nx,nz,nop
integer,private::iz_sou
real,private,allocatable :: lap(:,:)
character(len=128):: oper
integer :: it_middle, it_minor1,it_plus1
real :: c0x,c1x,c2x,c0z,c1z,c2z
complex,pointer :: temp_x(:), temp_z(:), temp(:,:)
real,private,allocatable :: q_x(:,:,:,:),q_z(:,:,:,:) ! for A-boundary condition
integer :: ib_middle,ib_minor1,ib_plus1,b_min,b_max ! for A-boundary
integer :: nbtx,nbtz    ! n of taper boundary 
real,private,allocatable :: xtaper(:),ztaper(:)  ! taper coefficient
real,pointer    :: kx_sq(:), kz_sq(:)
integer*8,private,save :: planF,planI

contains
!---------------------------------------------------------------------------
subroutine kernal_init(extrapolation)
type(extrapolation_type):: extrapolation
real ::dx,dz,dt
real :: c0,c1,c2
real :: kx,kz
integer :: ik
integer :: iz,ix
integer, external :: sep_thread_num
real :: xtapco,ztapco

pi=atan(1.)*4.
oper=extrapolation%oper

it_middle=2; it_minor1=1; it_plus1=3
ib_middle=2; ib_minor1=1; ib_plus1=3; b_min=1; b_max=2

if (oper(1:2) /= 'fd' .and. oper(1:3) /= 'fft') then
  write(0,*) "extrapolate_oper must be fd or fft"
end if

nx=extrapolation%nx
dx=extrapolation%dx
nz=extrapolation%nz
dz=extrapolation%dz
dt=extrapolation%dt
nop=extrapolation%nop
iz_sou=extrapolation%iz_sou

allocate(lap(nz,nx))

if (oper(1:2) == 'fd' ) then 
  !*********** parameter for finite difference*********
  dx_sq=dx*dx
  dz_sq=dz*dz
  c0=-30./12.
  c1=+16./12.
  c2=-1./12.
  c0x=c0/dx_sq
  c1x=c1/dx_sq
  c2x=c2/dx_sq
  c0z=c0/dz_sq
  c1z=c1/dz_sq
  c2z=c2/dz_sq
  !****************************************************
else  
  !******* paparemter for fft of laplacian************
  allocate(temp_x(nx),temp_z(nz),temp(nz,nx))
  allocate(kx_sq(nx),kz_sq(nz))
  do ik=1,nx
    kx=(-1.0+(ik-1)*2.0/nx)*3.1415926/dx
    kx_sq(ik)=kx*kx 
  end do
  do ik=1,nz
    kz=(-1.0+(ik-1)*2.0/nz)*3.1415926/dz
    kz_sq(ik)=kz*kz
  end do
  call klinemap(kx_sq,nx,dx,0); kx_sq=kx_sq**2
  call klinemap(kz_sq,nz,dz,0); kz_sq=kz_sq**2

  ! boundary coeff
  nbtx=extrapolation%nbtx
  nbtz=extrapolation%nbtz
  ztapco=extrapolation%ztapco
  xtapco=extrapolation%xtapco

  allocate(ztaper(nbtz),xtaper(nbtx))
  ! boundary coeff
  nbtx=extrapolation%nbtx
  nbtz=extrapolation%nbtz
  ztapco=extrapolation%ztapco
  xtapco=extrapolation%xtapco

  allocate(ztaper(nbtz),xtaper(nbtx))
  do iz=1,nbtz
    ztaper(iz)=exp(-ztapco*iz*iz)
  end do
  do ix=1,nbtx
    xtaper(ix)=exp(-xtapco*ix*ix)
  end do
  !****************************************************
end if
dt_sq=dt*dt

!********************** for A-boundary condition (check boundary_fd())**** 
dtz=dt/dz; dtx=dt/dx
! here for q_x and q_z, 3 mean three level: it_plus1, it_middle, it_minor1
! and 2 mean two side:  x_max and x_min  or z_max and z_min
allocate(q_x(extrapolation%nz,nop,3,2),q_z(extrapolation%nx,nop,3,2) )
q_x=0.0; q_z=0.0
!*************************************************************************

!INITIALIZE THE FFT
call sfftw_plan_dft(planF,2,(/nz,nx/),temp(1,1),temp(1,1), FFTW_FORWARD,FFTW_PATIENT)
call sfftw_plan_dft(planI,2,(/nz,nx/),temp(1,1),temp(1,1), FFTW_BACKWARD,FFTW_PATIENT)
scale=sqrt(1./(nx*nz))



end subroutine kernal_init


!--------------------------one step of extrapolation-----------------
logical function extrapolate_onestep(force_block,wave_block,slow_block)
real :: force_block(:),wave_block(:,:,:),slow_block(:,:)
integer, external :: sep_thread_num
integer :: ix,iz

wave_block(it_minor1,:,:)=wave_block(it_middle,:,:)
wave_block(it_middle,:,:)=wave_block(it_plus1,:,:)
! it_middle, it_minor1, it_plus1,nx,nz,nop,dt_sq, is intialized in kernal_init()
! laplacian P is computed in kernal_laplacian_fd or fft
if (oper(1:2) == 'fd' ) then ! finite difference
  call kernal_laplacian_fd(wave_block(it_middle,:,:),slow_block) ! computing laplacian
  wave_block(it_plus1,1+nop:nz-nop,1+nop:nx-nop)=    &
   2*wave_block(it_middle,1+nop:nz-nop,1+nop:nx-nop) &
    -wave_block(it_minor1,1+nop:nz-nop,1+nop:nx-nop) &
    +dt_sq*lap(1+nop:nz-nop,1+nop:nx-nop)
  call boundary_fd(wave_block,slow_block) 
else             
!if(sep_thread_num()==3)  write(0,*) "CHECK 1",sum(wave_block),sum(lap)
  !************************ FFt for laplacian ***************************************
  call kernal_laplacian_fft(wave_block(it_middle,:,:),slow_block) !computing laplacian
  !**********************************************************************************
!if(sep_thread_num()==3)  write(0,*) "CHECK 3",sum(wave_block),sum(lap)
  call boundary_taper(wave_block)
!if(sep_thread_num()==3)  write(0,*) "CHECK 4",sum(wave_block),sum(lap)
  !**** solve  \frac{\partial^2}{\partial t^2} P=Laplacian P ************************
  wave_block(it_plus1,:,:)=2*wave_block(it_middle,:,:)-wave_block(it_minor1,:,:)+dt_sq*lap(:,:)
!if(sep_thread_num()==3)  write(0,*) "CHECK 5",sum(wave_block),sum(lap)
  call boundary_fd(wave_block,slow_block)
!if(sep_thread_num()==3)  write(0,*) "CHECK 6",sum(wave_block),sum(lap)
end if
!**********************add source**********************************************
wave_block(it_plus1,iz_sou,:)=wave_block(it_plus1,iz_sou,:)+dt_sq*force_block(:)
wave_block(it_plus1,iz_sou+1,:)=wave_block(it_plus1,iz_sou+1,:)+dt_sq*0.5*force_block(:)
wave_block(it_plus1,iz_sou-1,:)=wave_block(it_plus1,iz_sou-1,:)+dt_sq*0.5*force_block(:)

extrapolate_onestep=.true.
return 
end function extrapolate_onestep
!-----------------------------------------------------------------------
!------------------------laplacian by finite difference-----------------
subroutine kernal_laplacian_fd(wave_block,slow_block)
real :: wave_block(:,:),slow_block(:,:)
real :: vel_sq,lap_x,lap_z
integer ix,iz
lap=0.
do ix=1+nop,nx-nop
  do iz=1+nop,nz-nop
      vel_sq=1.0/(slow_block(iz,ix)*slow_block(iz,ix))
      !******** \frac{\partial^2}{\partial x^2}P **********
      lap_x=c0x*wave_block(iz,ix)+ &
            c1x*(wave_block(iz,ix+1)+wave_block(iz,ix-1))+&
            c2x*(wave_block(iz,ix+2)+wave_block(iz,ix-2))
      !******** \frac{\partial^2}{]partial z^2}P **********
      lap_z=c0z*wave_block(iz,ix)+&
            c1z*(wave_block(iz+1,ix)+wave_block(iz-1,ix))+&
            c2z*(wave_block(iz+2,ix)+wave_block(iz-2,ix))
      lap(iz,ix)=vel_sq*(lap_x+lap_z)
  end do 
end do
end subroutine kernal_laplacian_fd
!-------------------------------------------------------------------------

!------------------------laplacian by FFT--------------------------------
subroutine kernal_laplacian_fft(wave_block,slow_block)
real :: wave_block(:,:),slow_block(:,:)
integer ::iz,ix
integer, external :: sep_thread_num
real:: vel_sq, der2 
lap=0.
!*************** fft for x-direction**************

temp=wave_block
call sfftw_execute(planF)

do ix=1,nx
  temp(:,ix)=temp(:,ix)*(-(kx_sq(ix)+kz_sq(:)))*scale
end do
call sfftw_execute(planI)
temp=temp*scale
!if(sep_thread_num()==3) call srite("junk1.H",real(temp),size(temp)*4)
!if(sep_thread_num()==3) call srite("junk5.H",real(temp),size(temp)*4)


!do iz=1,nz
!  temp_x(:)=cmplx(wave_block(iz,:))
!  call segfft1d(temp_x,nx,1)
!  temp(iz,:)=temp_x(:)
!
!end do
!*************************************************
!
!do ix=1,nx
!  !********************fft of z-dirction**********
!  temp_z(:)=temp(:,ix)
!  call segfft1d(temp_z,nz,1)
!  !*********** 2d second laplacian****************
!  temp_z(:)=temp_z(:)*(-(kx_sq(ix)+kz_sq(:)))
!  !***********************************************
!  !********** inverse fft of z-direction *********
!  call segfft1d(temp_z,nz,-1)
!  temp(:,ix)=temp_z(:)
!  !***********************************************
!end do
!!***************** inverse fft of x-direction*****
!do iz=1,nz
!  temp_x(:)=temp(iz,:)
!  call segfft1d(temp_x,nx,-1)
!  temp(iz,:)=temp_x(:)
!end do
!!*************************************************
do ix=1,nx
  do iz=1,nz
    vel_sq=1.0/(slow_block(iz,ix)*slow_block(iz,ix))
    lap(iz,ix)=vel_sq*real(temp(iz,ix))
  end do
end do
!!if(sep_thread_num()==3) call srite("junk3.H",real(lap),size(temp)*4)

end subroutine kernal_laplacian_fft
!-----------------------------------------------------------------------

!subroutine kernal_laplacian_fft1(wave_block,slow_block)
!real :: wave_block(:,:),slow_block(:,:)
!integer ::iz,ix
!real:: vel_sq, der2 
!lap=0.
!!*************** fft for x-direction**************
!do iz=1,nz
!  temp_x1(:)=cmplx(wave_block(iz,:))
!  call segfft1d(temp_x1,nx,1)
!  temp1(iz,:)=temp_x1(:)
!end do
!!*************************************************
!
!do ix=1,nx
!  !********************fft of z-dirction**********
!  temp_z1(:)=temp1(:,ix)
!  call segfft1d(temp_z1,nz,1)
!  !*********** 2d second laplacian****************
!  temp_z1(:)=temp_z1(:)*(-(kx_sq(ix)+kz_sq(:)))
!  !***********************************************
!  !********** inverse fft of z-direction *********
!  call segfft1d(temp_z1,nz,-1)
!  temp1(:,ix)=temp_z1(:)
!  !***********************************************
!end do
!!***************** inverse fft of x-direction*****
!do iz=1,nz
!  temp_x1(:)=temp1(iz,:)
!  call segfft1d(temp_x1,nx,-1)
!  temp1(iz,:)=temp_x1(:)
!end do
!!*************************************************
!do ix=1,nx
!  do iz=1,nz
!    vel_sq=1.0/(slow_block(iz,ix)*slow_block(iz,ix))
!    lap(iz,ix)=vel_sq*real(temp1(iz,ix))
!  end do
!end do
!end subroutine kernal_laplacian_fft1
!-----------------------------------------------------------------------

! kernal_auxwave_init is for boundary_fd --------------
subroutine kernal_auxwave_init()
   q_z=0.0;q_x=0.0
end subroutine kernal_auxwave_init
!------------------------------------------------------

subroutine boundary_taper(wave_block)
real ::wave_block(:,:,:)
integer iz,ix,it
integer, external :: sep_thread_num



!************************* min_x and max_x **************************************
do iz=1,nz
  do it=it_minor1,it_middle
    wave_block(it,iz,1:nbtx)=wave_block(it,iz,1:nbtx)*xtaper(nbtx:1:-1)
    wave_block(it,iz,nx-nbtx+1:nx)=wave_block(it,iz,nx-nbtx+1:nx)*xtaper(1:nbtx:+1)
  end do
  lap(iz,1:nbtx)=lap(iz,1:nbtx)*xtaper(nbtx:1:-1)  
  lap(iz,nx-nbtx+1:nx)=lap(iz,nx-nbtx+1:nx)*xtaper(1:nbtx:+1)

end do

!*************************** min_z and max_z**************************************
do ix=1+nbtx,nx-nbtx
  do it=it_minor1,it_middle
    wave_block(it,1:nbtz,ix)=wave_block(it,1:nbtz,ix)*ztaper(nbtz:1:-1)
    wave_block(it,nz-nbtz+1:nz,ix)=wave_block(it,nz-nbtz+1:nz,ix)*ztaper(1:nbtz:+1)
  end do
  lap(1:nbtz,ix)=lap(1:nbtz,ix)*ztaper(nbtz:1:-1)
  lap(nz-nbtz+1:nz,ix)=lap(nz-nbtz+1:nz,ix)*ztaper(1:nbtz:+1)
end do



end subroutine boundary_taper


!----------------------------------------------------------------------
subroutine boundary_fd(wave_block,slow_block)
real :: wave_block(:,:,:),slow_block(:,:)
integer iop,ix,iz
real vel,delp,bndz0,bndx0
! dtz,it_plus1, it_middle is intialized in kernal_init

!******************z-direction A-boundary***************
q_z(:,:,ib_minor1,:)=q_z(:,:,ib_middle,:)
q_z(:,:,ib_middle,:)=q_z(:,:,ib_plus1,:)
q_z(1,:,ib_plus1,:)=0.0; q_z(nx,:,ib_plus1,:)=0.0
do iop=1,nop
  do ix=2,nx-1
    vel=1.0/slow_block(iop,ix)
    delp=vel*dtx
    q_z(ix,iop,ib_plus1,b_min)=  &
   +2.0*q_z(ix,iop,ib_middle,b_min) &
   -    q_z(ix,iop,ib_minor1,b_min) &
   -    0.5*0.4761*delp*delp*             (&
   +    wave_block(it_middle,iop,ix+1)& 
   -2.0*wave_block(it_middle,iop,ix  )&
   +    wave_block(it_middle,iop,ix-1)&
   +    wave_block(it_middle,iop+1,ix+1) &
   -2.0*wave_block(it_middle,iop+1,ix  ) &
   +    wave_block(it_middle,iop+1,ix-1)  ) &
   +    0.3741*delp*delp*                 ( &
   +    q_z(ix+1,iop,ib_middle,b_min) &
   -2.0*q_z(ix,  iop,ib_middle,b_min) &
   +    q_z(ix-1,iop,ib_middle,b_min)     ) 
  end do
end do
do iop=1,nop
  do ix=2,nx-1
    vel=1.0/slow_block(nz-nop+iop,ix)
    delp=vel*dtx
    q_z(ix,iop,ib_plus1,b_max)=  &
   +2.0*q_z(ix,iop,ib_middle,b_max) &
   -    q_z(ix,iop,ib_minor1,b_max) &
   -    0.5*0.4761*delp*delp*                    (&
   +    wave_block(it_middle,nz-nop+iop,ix+1)  &
   -2.0*wave_block(it_middle,nz-nop+iop,ix  )  &
   +    wave_block(it_middle,nz-nop+iop,ix-1)  &
   +    wave_block(it_middle,nz-nop+iop-1,ix+1)&
   -2.0*wave_block(it_middle,nz-nop+iop-1,ix  )&
   +    wave_block(it_middle,nz-nop+iop-1,ix-1)  )&
   +0.3741*delp*delp*                            (&
   +    q_z(ix+1,iop,ib_middle,b_max)&
   -2.0*q_z(ix  ,iop,ib_middle,b_max)&
   +    q_z(ix-1,iop,ib_middle,b_max)            )
  end do
end do
do iop=nop,1,-1
  do ix=1,nx
    vel=1.0/slow_block(iop,ix)    
    delp=vel*dtz
    bndz0=(1.-delp)/(1.+delp)
    wave_block(it_plus1,iop,ix)= wave_block(it_middle,iop+1,ix)&
     +bndz0*(wave_block(it_middle,iop,ix)-wave_block(it_plus1,iop+1,ix)) &
     +2.0/(1+delp)*(q_z(ix,iop,ib_middle,b_min)-q_z(ix,iop,ib_plus1,b_min))  
  end do
  do ix=1,nx
    vel=1.0/slow_block(nz-iop+1,ix)
    delp=vel*dtz
    bndz0=(1.-delp)/(1.+delp)
    wave_block(it_plus1,nz-iop+1,ix)= wave_block(it_middle,nz-iop,ix)&
     +bndz0*(wave_block(it_middle,nz-iop+1,ix)-wave_block(it_plus1,nz-iop,ix))&
     +2.0/(1+delp)*(q_z(ix,nop-iop+1,ib_middle,b_max)-q_z(ix,nop-iop+1,ib_plus1,b_max))
  end do
end do
!*************************************************************
!*******************88 x-direction A-boundary*****************
q_x(:,:,ib_minor1,:)=q_x(:,:,ib_middle,:)
q_x(:,:,ib_middle,:)=q_x(:,:,ib_plus1 ,:)
q_x(1,:,ib_plus1, :)=0.0
q_x(nz,:,ib_plus1,:)=0.0

do iop=1,nop
  do iz=2,nz-1
    vel=1.0/slow_block(iz,iop)
    delp=vel*dtz
    q_x(iz,iop,ib_plus1,b_min)=      &
   +2.0*q_x(iz,iop,ib_middle,b_min)  &
   -    q_x(iz,iop,ib_minor1,b_min)  &
   -    0.5*0.4761*delp*delp*                          (&
   +    wave_block(it_middle,iz+1,iop) &
   -2.0*wave_block(it_middle,iz  ,iop) &
   +    wave_block(it_middle,iz-1,iop) &
   +    wave_block(it_middle,iz+1,iop+1)  &
   -2.0*wave_block(it_middle,iz  ,iop+1)  &
   +    wave_block(it_middle,iz-1,iop+1)               )&
   +    0.3741*delp*delp*                              (&
   +    q_x(iz+1,iop,ib_middle,b_min)&
   -2.0*q_x(iz  ,iop,ib_middle,b_min)&
   +    q_x(iz-1,iop,ib_middle,b_min)                  )     
  end do
end do


do iop=1,nop
  do iz=2,nz-1
    vel=1.0/slow_block(iz,nx-nop+iop)
    delp=vel*dtz
    q_x(iz,iop,ib_plus1,b_max)=     &
   +2.0*q_x(iz,iop,ib_middle,b_max) &
   -    q_x(iz,iop,ib_minor1,b_max) &
   -    0.5*0.4761*delp*delp*                          (&
   +    wave_block(it_middle,iz+1,nx-nop+iop) &
   -2.0*wave_block(it_middle,iz  ,nx-nop+iop) &
   +    wave_block(it_middle,iz-1,nx-nop+iop) &
   +    wave_block(it_middle,iz+1,nx-nop+iop-1) &
   -2.0*wave_block(it_middle,iz  ,nx-nop+iop-1) &
   +    wave_block(it_middle,iz-1,nx-nop+iop-1)        )&
   +    0.3741*delp*delp*                              (&
   +    q_x(iz+1,iop,ib_middle,b_max) &
   -2.0*q_x(iz  ,iop,ib_middle,b_max) &
   +    q_x(iz-1,iop,ib_middle,b_max)                  )
  end do
end do

do iop=nop,1,-1
  do iz=1+nop,nz-nop
    vel=1.0/slow_block(iz,iop)
    delp=vel*dtx
    bndx0=(1.0-delp)/(1.0+delp)
    wave_block(it_plus1,iz,iop)=wave_block(it_middle,iz,iop+1)&
     +bndx0*(wave_block(it_middle,iz,iop)-wave_block(it_plus1,iz,iop+1) )&
     +2.0/(1+delp)*(q_x(iz,iop,ib_middle,b_min)-q_x(iz,iop,ib_plus1,b_min))
  end do
  do iz=1+nop,nz-nop
    vel=1.0/slow_block(iz,nx-iop+1)
    delp=vel*dtx
    bndx0=(1.0-delp)/(1.0+delp)
    wave_block(it_plus1,iz,nx-iop+1)=wave_block(it_middle,iz,nx-iop) &
     +bndx0*(wave_block(it_middle,iz,nx-iop+1)-wave_block(it_plus1,iz,nx-iop))&
     +2.0/(1+delp)*(q_x(iz,nop-iop+1,ib_middle,b_max)-q_x(iz,nop-iop+1,ib_plus1,b_max))
  end do
end do
end subroutine boundary_fd

!-----------------------------------------------------------------------
subroutine kernal_clean()
deallocate(lap)
if (oper(1:3) == 'fft' ) then
  deallocate(kx_sq,kz_sq,temp_x,temp_z,temp)
  deallocate(q_x,q_z)
  deallocate(xtaper,ztaper)
end if
end subroutine kernal_clean
!-----------------------------------------------------------------------
  subroutine klinemap(k,n,d,shift)
    real, dimension(:), pointer :: k
    integer, intent(in) :: n,shift
    real,    intent(in) :: d
    integer             :: i,m
                                                                                                    
k=0.
    if(n/=1) then
       m = n/2
       do i=1,m
          k(i+shift  ) =    i-1
          k(i+m+shift) = -m+i-1
       end do
                                                                                                    
    else
       k=0. !! make sure k=0 if n=1
    end if
                                                                                                    
    k = k / (n*d)      !! 1/length
    k = 2 * pi * k !! radians
  end subroutine klinemap


end module 

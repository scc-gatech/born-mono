module kirmod_mod
  use sep3d_struct_mod
  use sep_mod
  implicit none

  !parameters
  integer, parameter, private :: REFL_Z=1, REFL_X=2, REFL_Y=3, REFL_ISO=4
  integer, parameter, private :: REFL_RHO=4, REFL_LAMBDA=5, REFL_MU=6
  integer, parameter, private :: SOU_X=1, SOU_Y=2, SOU_Z=3 
  integer, parameter, private :: REC_X=4, REC_Y=5, REC_Z=6
  real, parameter, private :: eps=1e-6

  !green's function variables
  complex, dimension (:), pointer, private, save :: green, ang_diff, ang_surf
  logical, dimension (:), pointer, private, save :: mask
  integer, private, save :: nz_green, noff_green, n_green
  real, private, save :: off_green_max,z_green_max, idoff_green, idz_green
  real, private, save :: off0_green, z0_green, t0_data
  !other
  integer, public, save :: conv_wave, mode, spow, rpow
  integer, private, save :: ntr_block, nt_data, n_block
  real, private, save :: dt_data
  real, dimension (:,:), pointer, private, save :: block_traces
  real, dimension (:,:), pointer, private, save :: block_headers

  integer :: nthreads

contains

  subroutine init_modeling ()

    if (getch ('convolve_wave', 'd', conv_wave) == 0) conv_wave=1
    if (getch ('mode', 'd', mode) == 0) mode=1
    if (getch ('spow', 'd', spow) == 0) spow=1
    if (getch ('rpow', 'd', rpow) == 0) rpow=1

    write(0,*) '   mode=',mode
    write(0,*) '   conv_wave=',conv_wave
    write(0,*) '   spow=',spow
    write(0,*) '   rpow=',rpow

  end subroutine init_modeling

  subroutine read_green ()
    real, allocatable, dimension(:) :: dummy1, dummy2
    integer ::  i, stat

    if (auxpar('n1','d',nz_green,   'Green')==0) call seperr('Err:nz_green')
    if (auxpar('n2','d',noff_green, 'Green')==0) call seperr('Err:noff_green')
    if (auxpar('d1','f',idz_green,  'Green')==0) call seperr('Err:idz_green')
    if (auxpar('d2','f',idoff_green,'Green')==0) call seperr('Err:idoff_green')
    if (auxpar('o1','f',z0_green,   'Green')==0) call seperr('Err:z0_green')
    if (auxpar('o2','f',off0_green, 'Green')==0) call seperr('Err:off0_green')

    idz_green=1/idz_green
    idoff_green=1/idoff_green
    n_green = nz_green*noff_green
    off_green_max=(noff_green-1)/idoff_green + off0_green
    z_green_max=(nz_green-1)/idz_green + z0_green

    allocate (green(n_green), dummy1(n_green), dummy2(n_green))
    stat=sreed('Green',dummy1,n_green*4)
    if (stat /= n_green*4) call seperr("trouble reading green table times ")
    stat=sreed('Green',dummy2,n_green*4)
    if (stat /= n_green*4) call seperr("trouble reading green table amplitudes ")
    green = cmplx(dummy1, dummy2)
write(0,*) mode

    if (mode == 2) then
write(0,*) n_green
       allocate (ang_diff(n_green), ang_surf(n_green))
write(0,*) n_green
       if (sreed('Green',dummy1,n_green*4)==0) call seperr('Err:Ang_diff')
       if (sreed('Green',dummy2,n_green*4)==0) call seperr('Err:Ang_diff')
       ang_diff = cmplx(dummy1, dummy2)
       if (sreed('Green',dummy1,n_green*4)==0) call seperr('Err:Ang_surf')
       if (sreed('Green',dummy2,n_green*4)==0) call seperr('Err:Ang_surf')
       ang_surf = cmplx(dummy1, dummy2)
    end if
    deallocate (dummy1, dummy2)

  end subroutine read_green

  subroutine alloc_block (n_trace)

    integer :: i,  n_trace
!$  integer, external  :: omp_get_num_threads

    nthreads = 1
!$OMP PARALLEL
!$      nthreads = omp_get_num_threads() 
!$OMP END PARALLEL

    write(0,*) 'nthreads = ',nthreads

    if (getch('ntr_block', 'd', ntr_block) == 0) then
       ntr_block=n_trace
       n_block=1
    else
       if (n_trace.lt.ntr_block) then
          ntr_block=n_trace
          write(0,*) 'Overiding par: setting ntr_block=n_trace=',n_trace
       end if
       if (mod (n_trace, ntr_block) /= 0) then
          write(0,*) 'ntr_block=', ntr_block,'     n_trace=',n_trace
          call seperr ('Error (or rather bug): Currently need ntr_block to be a factor of n_trace')
       end if
       n_block=n_trace/ntr_block
    end if
    write(0,*) '   n_trace=',n_trace
    write(0,*) '   ntr_block=',ntr_block
    write(0,*) '   n_block=',n_block
    
    if (getch('dt_data', 'f', dt_data) == 0) call seperr ('Need dt_data')
    if (getch('nt_data', 'd', nt_data) == 0) call seperr ('Need nt_data')
    if (getch('t0_data', 'f', t0_data) == 0) t0_data=0.

    allocate(block_traces(nt_data,ntr_block),block_headers(6,ntr_block))

  end subroutine alloc_block

  subroutine kir_modeling (refl, headers, wavelet)
    implicit none

    type (sep3d) :: refl, headers, wavelet
    integer :: i_block, i,  itr_block, jtr_block

    integer :: nwind,fwind
    real, dimension (:), pointer :: dummy
		real,  allocatable :: wave(:),ref_pts(:,:)
		integer ::  tempi,ierr

    if (conv_wave==1) then 
       if (dt_data /= wavelet%d(1)) &
       call seperr ('Need dt_data=dt_wavelet')
    end if

    allocate(wave(wavelet%n(1)))
    tempi=sreed("Wavelet",wave,size(wave)*4)
    allocate(ref_pts(refl%n(1),refl%n(2)),stat=ierr)
    if(ierr.ne.0) call erexit("trouble allocating ref_pts")
    
    tempi=sreed("Reflector",ref_pts,size(ref_pts)*4)

		
    if (headers%ndims .ne.2) call seperr ('Need dim(headers)=2')

    allocate (mask(ntr_block),dummy(ntr_block))

    trace_block: do i_block = 1, n_block
       write(0,*) 'Modeling block number ',i_block,' out of ',n_block

       nwind=ntr_block; 
       fwind=(i_block-1)*ntr_block

       call sep3d_grab_headers("in",headers,tempi,fwind=(/fwind/),nwind=(/nwind/))


       ! get_key_values == get_char_real_key
       call sep3d_grab_key_vals(headers,"s_x",dummy)
       block_headers(SOU_X,:)=dummy
       call sep3d_grab_key_vals(headers,"s_y",dummy)
       block_headers(SOU_Y,:)=dummy
       call sep3d_grab_key_vals(headers,"s_elev",dummy)
       block_headers(SOU_Z,:)=dummy
       call sep3d_grab_key_vals(headers,"g_x",dummy)
       block_headers(REC_X,:)=dummy
       call sep3d_grab_key_vals(headers,"g_y",dummy)
       block_headers(REC_Y,:)=dummy
       call sep3d_grab_key_vals(headers,"g_elev",dummy)
       block_headers(REC_Z,:)=dummy

       block_traces(:,:) = 0

       if (mode == 1) then
          write(0,*) "   Calling isotropic-scattering subroutine..."
          call kinematic (refl,ref_pts)
       else
          write(0,*) "   Calling Rayleigh-scattering subroutine..."
          call dynamic (refl,ref_pts)
       end if

       if (conv_wave == 1) then
          write(0,*) "   Convolving block with wavelet..."
          call conv_wavelet (wavelet,wave)
       end if

       write(0,*) "   Writing block..."
       ierr= srite ('out',block_traces(:,:),ntr_block*nt_data*4)

    end do trace_block

    deallocate(dummy)

    if (conv_wave == 1) then
       t0_data = t0_data + wavelet%o(1)
    end if
    if (putch ('d1','f',dt_data) /= 0) call seperr('Error writing to history')
    if (putch ('o1','f',t0_data) /= 0) call seperr('Error writing to history')
    if (putch ('n1','d',nt_data) /= 0) call seperr('Error writing to history')
    if (putch ('esize','d',4) /= 0) call seperr('Error writing to history')
    write(0,*) "All done"

  end subroutine kir_modeling

  !------------------------------------!
  ! Modeling with isotropic scattering !
  !------------------------------------!

  subroutine kinematic (refl,ref_pts)
    implicit none

    type (sep3d) :: refl
    real :: ref_pts(:,:)

    real :: sou_off_tr, weight_off, z_tr, weight_z, weight_t
    real :: t_tr, amp_tr, rec_off_tr, weight_amp
    complex :: green_00, green_01, green_10, green_11, t_amp_tr
    integer :: index_off, index_z, index_green, index_t, ir_ref, i

       !for each trace within the block
!$OMP PARALLEL DO                                       &
!$OMP&    PRIVATE(sou_off_tr,weight_off,z_tr,weight_z,weight_t,t_tr) &
!$OMP&    PRIVATE(amp_tr,rec_off_tr,weight_amp,green_00,green_01) &
!$OMP&    PRIVATE(green_10,green_11,t_amp_tr,index_off,index_z,index_green) &
!$OMP&    PRIVATE(index_t,i,ir_ref)
       trace: do i = 1, ntr_block !loop over the output

          !for each point reflector in the model
          model: do ir_ref = 1, refl%n(2) !looping over the number of reflect


          !----------------------------!
          ! downgoing Green's function !
          !----------------------------!

          sou_off_tr=sqrt((ref_pts(REFL_X,ir_ref)-block_headers(SOU_X,i))**2+ &
                          (ref_pts(REFL_Y,ir_ref)-block_headers(SOU_Y,i))**2)
          !calculate the distance from the reflector position to the source

          mask(i)=sou_off_tr .gt. off_green_max

          weight_off=((sou_off_tr-off0_green)*idoff_green)+1
          index_off=merge(1,int(weight_off),mask(i))
          weight_off=weight_off-index_off

          z_tr=(ref_pts(REFL_Z,ir_ref) - block_headers(SOU_Z,i))

          weight_z=((z_tr-z0_green)*idz_green)+1
          index_z=int(weight_z)
          weight_z=weight_z-index_z

          index_green=index_z + (index_off-1)*nz_green
          green_00 = green(index_green)
          index_green=index_z + (index_off-1)*nz_green+1
          green_10 = green(index_green)
          index_green=index_z + (index_off)*nz_green 
          green_01 = green(index_green)
          index_green=index_z + (index_off)*nz_green +1
          green_11 = green(index_green)

          t_amp_tr=green_00*(1.-weight_z)*(1.-weight_off)+ &
          green_10*(weight_z)*(1.-weight_off)+ &
          green_01*(1.-weight_z)*(weight_off)+ &
          green_11*(weight_z)*(weight_off)

          t_tr=real(t_amp_tr)
          amp_tr=aimag(t_amp_tr)

          !--------------------------!
          ! upgoing Green's function !
          !--------------------------!

          rec_off_tr=sqrt((ref_pts(REFL_X,ir_ref)-block_headers(REC_X,i))**2+ &
                          (ref_pts(REFL_Y,ir_ref)-block_headers(REC_Y,i))**2)

          mask(i)=mask(i).or.(rec_off_tr .gt. off_green_max)
          weight_off=((rec_off_tr-off0_green)*idoff_green)+1
          index_off=merge(1,int(weight_off),mask(i))
          weight_off=weight_off-index_off

          z_tr=(ref_pts(REFL_Z,ir_ref) - block_headers(REC_Z,i))

          weight_z=((z_tr-z0_green)*idz_green)+1
          index_z=int(weight_z)
          weight_z=weight_z-index_z

          index_green=index_z + (index_off-1)*nz_green
          green_00 = green(index_green)
          index_green=index_z + (index_off-1)*nz_green+1
          green_10 = green(index_green)
          index_green=index_z + (index_off)*nz_green 
          green_01 = green(index_green)
          index_green=index_z + (index_off)*nz_green +1
          green_11 = green(index_green)

          t_amp_tr=green_00*(1.-weight_z)*(1.-weight_off)+ &
          green_10*(weight_z)*(1.-weight_off)+ &
          green_01*(1.-weight_z)*(weight_off)+ &
          green_11*(weight_z)*(weight_off)

          t_tr=t_tr+real(t_amp_tr)
          amp_tr=amp_tr*aimag(t_amp_tr)



          weight_t=((t_tr-t0_data)/dt_data)+1
          weight_t=min(weight_t,float(nt_data-1))

          index_t=int(weight_t)
          weight_t=weight_t-index_t

          !--------------------------------!
          ! calculate scattered amplitudes !
          !--------------------------------!

          weight_amp = amp_tr * ref_pts(REFL_ISO,ir_ref)


          block_traces(index_t,i) = block_traces(index_t,i) &
          + weight_amp*(1.-weight_t)

          block_traces(index_t+1,i) = block_traces(index_t+1,i) &
          + weight_amp*weight_t

       end do model
    end do trace
!$OMP END PARALLEL DO                                      

    if (any(mask)) then
       write(0,*) 'WARNING: need larger Greens function'
    end if

  end subroutine kinematic

  !-----------------------------------!
  ! Modeling with Rayleigh scattering !
  !-----------------------------------!
  subroutine dynamic (refl,ref_pts)
    implicit none

    type (sep3d) :: refl
		real :: ref_pts(:,:)

    real :: sou_off_tr, weight_off, z_tr, weight_z, weight_t
    real :: t_tr, amp_tr, sin_b1, cos_b1, cos_theta_rec, cos_theta
    real :: rec_off_tr, sin_b2, cos_b2, cos_phi, cos_theta_sq, weight_amp
    complex :: green_00, ang_diff_00, ang_surf_00 
    complex :: green_01, ang_diff_01, ang_surf_01 
    complex :: green_10, ang_diff_10, ang_surf_10 
    complex :: green_11, ang_diff_11, ang_surf_11
    complex :: t_amp_tr, ang_diff_tr, ang_surf_tr
    integer :: index_off, index_z, index_green, index_t, ir_ref, i



!$OMP PARALLEL DO                                       &
!$OMP&    PRIVATE(sou_off_tr, weight_off, z_tr, weight_z, weight_t) &
!$OMP&    PRIVATE(t_tr, amp_tr, sin_b1, cos_b1, cos_theta_rec, cos_theta) &
!$OMP&    PRIVATE(rec_off_tr,sin_b2,cos_b2,cos_phi,cos_theta_sq, weight_amp) &
!$OMP&    PRIVATE(green_00, ang_diff_00, ang_surf_00) &
!$OMP&    PRIVATE(green_01, ang_diff_01, ang_surf_01) &
!$OMP&    PRIVATE(green_10, ang_diff_10, ang_surf_10) &
!$OMP&    PRIVATE(green_11, ang_diff_11, ang_surf_11) &
!$OMP&    PRIVATE(t_amp_tr, ang_diff_tr, ang_surf_tr) &
!$OMP&    PRIVATE(index_off, index_z, index_green, index_t, ir_ref, i)

    !for each trace within the block
    trace: do i = 1, ntr_block

       !for each point reflector in the model
       model: do ir_ref = 1, refl%n(2)

          !----------------------------!
          ! downgoing Green's function !
          !----------------------------!

          sou_off_tr=sqrt((ref_pts(REFL_X,ir_ref) - block_headers(SOU_X,i))**2 + &
          (ref_pts(REFL_Y,ir_ref) - block_headers(SOU_Y,i))**2)

          mask(i)=sou_off_tr .gt. off_green_max

          weight_off=((sou_off_tr-off0_green)*idoff_green)+1
          index_off=merge(1,int(weight_off),mask(i))
          weight_off=weight_off-index_off

          z_tr=(ref_pts(REFL_Z,ir_ref) - block_headers(SOU_Z,i))

          weight_z=((z_tr-z0_green)*idz_green)+1
          index_z=int(weight_z)
          weight_z=weight_z-index_z

          index_green=index_z + (index_off-1)*nz_green
          green_00 = green(index_green)
          ang_diff_00 = ang_diff(index_green)
          ang_surf_00 = ang_surf(index_green)
          index_green=index_z + (index_off-1)*nz_green+1
          green_10 = green(index_green)
          ang_diff_10 = ang_diff(index_green)
          ang_surf_10 = ang_surf(index_green)
          index_green=index_z + (index_off)*nz_green 
          green_01 = green(index_green)
          ang_diff_01 = ang_diff(index_green)
          ang_surf_01 = ang_surf(index_green)
          index_green=index_z + (index_off)*nz_green +1
          green_11 = green(index_green)
          ang_diff_11 = ang_diff(index_green)
          ang_surf_11 = ang_surf(index_green)

          t_amp_tr=green_00*(1.-weight_z)*(1.-weight_off)+ &
          green_10*(weight_z)*(1.-weight_off)+ &
          green_01*(1.-weight_z)*(weight_off)+ &
          green_11*(weight_z)*(weight_off)
          ang_diff_tr=ang_diff_00*(1.-weight_z)*(1.-weight_off)+ &
          ang_diff_10*(weight_z)*(1.-weight_off)+ &
          ang_diff_01*(1.-weight_z)*(weight_off)+ &
          ang_diff_11*(weight_z)*(weight_off)
          ang_surf_tr=ang_surf_00*(1.-weight_z)*(1.-weight_off)+ &
          ang_surf_10*(weight_z)*(1.-weight_off)+ &
          ang_surf_01*(1.-weight_z)*(weight_off)+ &
          ang_surf_11*(weight_z)*(weight_off)

          t_tr=real(t_amp_tr)
          amp_tr=aimag(t_amp_tr)

          sin_b1=real(ang_diff_tr)
          cos_b1=aimag(ang_diff_tr)
          cos_theta_rec=aimag(ang_surf_tr)
          !cos_theta_rec=aimag(ang_surf_tr)**spow

          !--------------------------!
          ! upgoing Green's function !
          !--------------------------!

          rec_off_tr=sqrt((ref_pts(REFL_X,ir_ref) - block_headers(REC_X,i))**2 + &
          (ref_pts(REFL_Y,ir_ref) - block_headers(REC_Y,i))**2)

          mask(i)=mask(i).or.(rec_off_tr .gt. off_green_max)
          weight_off=((rec_off_tr-off0_green)*idoff_green)+1
          index_off=merge(1,int(weight_off),mask(i))
          weight_off=weight_off-index_off

          z_tr=(ref_pts(REFL_Z,ir_ref) - block_headers(REC_Z,i))

          weight_z=((z_tr-z0_green)*idz_green)+1
          index_z=int(weight_z)
          weight_z=weight_z-index_z

          index_green=index_z + (index_off-1)*nz_green
          green_00 = green(index_green)
          ang_diff_00 = ang_diff(index_green)
          ang_surf_00 = ang_surf(index_green)
          index_green=index_z + (index_off-1)*nz_green+1
          green_10 = green(index_green)
          ang_diff_10 = ang_diff(index_green)
          ang_surf_10 = ang_surf(index_green)
          index_green=index_z + (index_off)*nz_green 
          green_01 = green(index_green)
          ang_diff_01 = ang_diff(index_green)
          ang_surf_01 = ang_surf(index_green)
          index_green=index_z + (index_off)*nz_green +1
          green_11 = green(index_green)
          ang_diff_11 = ang_diff(index_green)
          ang_surf_11 = ang_surf(index_green)

          t_amp_tr=green_00*(1.-weight_z)*(1.-weight_off)+ &
          green_10*(weight_z)*(1.-weight_off)+ &
          green_01*(1.-weight_z)*(weight_off)+ &
          green_11*(weight_z)*(weight_off)
          ang_diff_tr=ang_diff_00*(1.-weight_z)*(1.-weight_off)+ &
          ang_diff_10*(weight_z)*(1.-weight_off)+ &
          ang_diff_01*(1.-weight_z)*(weight_off)+ &
          ang_diff_11*(weight_z)*(weight_off)
          ang_surf_tr=ang_surf_00*(1.-weight_z)*(1.-weight_off)+ &
          ang_surf_10*(weight_z)*(1.-weight_off)+ &
          ang_surf_01*(1.-weight_z)*(weight_off)+ &
          ang_surf_11*(weight_z)*(weight_off)

          t_tr=t_tr+real(t_amp_tr)

          amp_tr=amp_tr*aimag(t_amp_tr)
          sin_b2=real(ang_diff_tr)
          cos_b2=aimag(ang_diff_tr)
          cos_theta_rec=cos_theta_rec*aimag(ang_surf_tr)

          weight_t=((t_tr-t0_data)/dt_data)+1
          weight_t=min(weight_t,float(nt_data-1))

          index_t=int(weight_t)
          weight_t=weight_t-index_t

          !--------------------------------!
          ! calculate scattered amplitudes !
          !--------------------------------!

          cos_phi = ((block_headers(SOU_X,i) - ref_pts(REFL_X,ir_ref))  &
                   * (block_headers(REC_X,i) - ref_pts(REFL_X,ir_ref))  &
                   + (block_headers(REC_Y,i) - ref_pts(REFL_Y,ir_ref))  &
                   * (block_headers(SOU_Y,i) - ref_pts(REFL_Y,ir_ref))) & 
                                    / (sou_off_tr * rec_off_tr + eps)

          cos_theta = sin_b1*sin_b2*cos_phi + cos_b1*cos_b2
          cos_theta_sq = cos_theta*cos_theta

          weight_amp = amp_tr * &
          (ref_pts(REFL_RHO,ir_ref)*cos_theta_rec*cos_theta &
          -ref_pts(REFL_LAMBDA,ir_ref)*cos_theta_rec &
          -2*ref_pts(REFL_MU,ir_ref)*cos_theta_rec*cos_theta_sq)

          block_traces(index_t,i) = block_traces(index_t,i) &
          + weight_amp*(1.-weight_t)

          block_traces(index_t+1,i) = block_traces(index_t+1,i) &
          + weight_amp*weight_t

       end do model
    end do trace
!$OMP END PARALLEL DO
    if (any(mask)) then
       write(0,*) 'WARNING: need larger Greens function'
    end if

  end subroutine dynamic

  !---------------------------------------!
  ! Convolve wavelet with data            !
  ! (internal convolution in time domain) !
  !---------------------------------------!

  subroutine conv_wavelet (wavelet,wave)

    type (sep3d) :: wavelet
    real, dimension (:) :: wave
    real, dimension (:,:), pointer :: yy

    integer :: j, itr_block, i_data, i_wavelet, n_wavelet, ithread
!$  integer, external  :: omp_get_thread_num

    ithread=1
    allocate (yy(nt_data,nthreads))
    n_wavelet = wavelet%n(1)

!$OMP PARALLEL DO PRIVATE(itr_block,i_data,i_wavelet,j,ithread) 
    do itr_block = 1, ntr_block
!$     ithread=omp_get_thread_num()+1
       do i_data = 1, nt_data
          yy(i_data,ithread)=block_traces(i_data,itr_block)
          block_traces(i_data,itr_block) = 0.
       end do
       do i_data = 1, nt_data
          do i_wavelet = 1, n_wavelet
             j = i_data - i_wavelet +1
             if (j.lt.1) cycle
             block_traces(i_data,itr_block) =  block_traces(i_data,itr_block) + yy(j,ithread) * wave(i_wavelet)
          end do
       end do
    end do
!$OMP END PARALLEL DO

    deallocate(yy)

  end subroutine conv_wavelet
end module kirmod_mod




















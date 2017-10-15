module amo_types_mod

type amo_param_type 
  integer,dimension(3):: n
  real,   dimension(3):: o,d
  integer             :: n1_cmpxcube
  logical             :: amo,fft_xy_forw,fft_xy_back,fk_filter
  real                      :: hx_inp,hy_inp,hx_out,hy_out
  real                      :: v_min,f_max 
  real                      :: t_min_log
end type amo_param_type 


end module amo_types_mod

module fft_types
  integer, parameter, private :: dbl=selected_real_kind(10,50)
  type fft_type 
   integer   :: ierr,ndim
   integer(kind=dbl),pointer   :: plan_forw(:),plan_back(:)
   real :: scale
   character(len=30)   :: label
  end type fft_type
end module fft_types


module fft_data_mod
  use fftwflags
  use fft_types
  use fftw_sep_fake_mod
  integer, parameter, private :: dbl=selected_real_kind(10,50)
  complex,  allocatable :: tempc2(:,:,:),tempc1(:,:),tempcr(:,:)
  real,  allocatable :: temprc(:,:)
 

contains

  ! ----------------------------------------------------------------------
  function  init_fft_1d(n1,nth,fast_init) result (fft_id)
    type (fft_type) fft_id
    integer n1,stat,flag,nth,ith
    logical, optional :: fast_init


    if(present(fast_init)) then
       if(fast_init) then
          flag=ior(ior(fFTW_ESTIMATE,fFTW_USE_WISDOM),fFTW_THREADSAFE)
       else
          flag=ior(ior(fFTW_MEASURE,fFTW_USE_WISDOM),fFTW_THREADSAFE)
       end if
    else
       flag=ior(ior(fFTW_ESTIMATE,fFTW_USE_WISDOM),fFTW_THREADSAFE)
    end if
    fft_id%scale=sqrt(1./(n1))
     allocate(fft_id%plan_back(nth))
     allocate(fft_id%plan_forw(nth))
     allocate(tempc1(n1,nth))

!    call fftw_f77_create_plan(fft_id%plan_forw,n1,fFTW_FORWARD ,flag)
!    call fftw_f77_create_plan(fft_id%plan_back,n1,fFTW_BACKWARD,flag)
   do ith=1,nth
    call sfftw_plan_dft(fft_id%plan_forw(ith),1,(/n1/),tempc1(1,ith),&
       tempc1(1,ith),fFTW_FORWARD ,flag)
    call sfftw_plan_dft(fft_id%plan_back(ith),1,(/n1/),tempc1(1,ith),&   
      tempc1(1,ith),fFTW_BACKWARD,flag)
  end do
    if((fft_id%plan_forw(1) == 0 ).or.(fft_id%plan_back(1) == 0)) then
       fft_id%ierr=1
    else
       fft_id%ierr=0
    end if
    fft_id%ndim=1

  end function init_fft_1d
  ! ----------------------------------------------------------------------
  function init_rfft_1d(n1,nth,create) result (fft_id)
    type(fft_type)     :: fft_id
    integer, intent(in):: n1,nth
    integer            :: stat, flag,ith
    logical            :: create

    flag=ior(ior(fFTW_ESTIMATE,fFTW_USE_WISDOM),fFTW_THREADSAFE)
    fft_id%scale=sqrt(1./(n1))
    if(create) then
      allocate(temprc(n1,nth),tempcr(n1/2+1,nth))
    end if
!    call rfftw_f77_create_plan(fft_id%plan_forw,n1,fFTW_REAL_TO_COMPLEX,flag)
!    call rfftw_f77_create_plan(fft_id%plan_back,n1,fFTW_COMPLEX_TO_REAL,flag)
    allocate(fft_id%plan_forw(nth))
    allocate(fft_id%plan_back(nth))
do ith=1,nth
   fft_id%label="CORRECT"
    call sfftw_plan_dft_c2r(fft_id%plan_forw(ith),1,(/n1/),tempcr(1,ith),temprc(1,ith),&
     fFTW_REAL_TO_COMPLEX,flag)
    call sfftw_plan_dft_c2r(fft_id%plan_back(ith),1,(/n1/),tempcr(1,ith),temprc(1,ith),&
     fFTW_COMPLEX_TO_REAL,flag)
end do
    if((fft_id%plan_forw(1) == 0 ).or.(fft_id%plan_back(1) == 0)) then
       fft_id%ierr=1
    else
       fft_id%ierr=0
    end if
    fft_id%ndim=1
  end function init_rfft_1d
  ! ----------------------------------------------------------------------
  function  init_fft_2d(n1,n2,nth,fast_init) result (fft_id)
    type (fft_type) fft_id
    integer n1,n2,stat,flag,nth,ith
    logical, optional :: fast_init

    if(present(fast_init)) then
       if(fast_init) then
          flag=ior(ior(fFTW_ESTIMATE,fFTW_USE_WISDOM),fFTW_THREADSAFE)
       else
          flag=ior(ior(fFTW_MEASURE,fFTW_USE_WISDOM),fFTW_THREADSAFE)
       end if
    else
       flag=ior(ior(fFTW_ESTIMATE,fFTW_USE_WISDOM),fFTW_THREADSAFE)
    end if

   allocate(fft_id%plan_forw(nth))
   allocate(fft_id%plan_back(nth))
    fft_id%scale=sqrt(1./(n1*n2))
    allocate(tempc2(n1,n2,nth))
!    call fftw2d_f77_create_plan(fft_id%plan_forw,n1,n2,fFTW_FORWARD ,flag)
!    call fftw2d_f77_create_plan(fft_id%plan_back,n1,n2,fFTW_BACKWARD,flag)
do ith=1,nth
    call  sfftw_plan_dft(fft_id%plan_forw(ith),2,(/n1,n2/),&
      tempc2(1,1,ith),tempc2(1,1,ith),fFTW_FORWARD ,flag)
    call sfftw_plan_dft(fft_id%plan_back(ith),2,(/n1,n2/),&
      tempc2(1,1,ith),tempc2(1,1,ith),fFTW_BACKWARD,flag)
end do

    if((fft_id%plan_forw(1) ==0 ).or.(fft_id%plan_back(1) == 0)) then
       fft_id%ierr=1
    else
       fft_id%ierr=0
    end if

    fft_id%ndim=2
  end function init_fft_2d

  !----------------------------------------------------------------------
  subroutine fft_1d_data(fft_id,sign,n1,data_vec,ith)
    type (fft_type) fft_id
    integer :: sign, n1,ith
    complex, dimension (:) :: data_vec
 
    tempc1(1:n1,ith)=data_vec*fft_id%scale
    if(sign == fFTW_FORWARD) then
       call sfftw_execute(fft_id%plan_forw(ith))
    else
       call sfftw_execute(fft_id%plan_back(ith))
    end if
    return        
    data_vec(1:n1)=tempc1(1:n1,ith)
  end subroutine fft_1d_data
  !----------------------------------------------------------------------
  subroutine rfft_1d_data(fft_id,sign,n,real_vec,cmpx_vec,ith)
  ! No side effects. cmpx_vec has both zero freq and Nyq
 
    type(fft_type),            intent(in   ) :: fft_id
    integer,                   intent(in   ) :: sign, n,ith
    real,    dimension(n    )                :: real_vec
    complex, dimension(n/2+1), intent(inout) :: cmpx_vec
    integer                                  :: i

    if(mod(n,2)==1) then
       write(0,*) "Error! rfft_1d_data needs a even n. Now, n =",n
    end if
    
    if(sign == fFTW_REAL_TO_COMPLEX) then
       temprc(1:n,ith)=real_vec(1:n)
       call sfftw_execute(fft_id%plan_forw(ith))
!       call rfftw_f77_one(fft_id%plan_forw, real_vec, temp_vec)
       cmpx_vec(    1)=cmplx(temprc(    1,ith),0.) ! zero freq
       cmpx_vec(n/2+1)=cmplx(temprc(n/2+1,ith),0.) ! Nyquist
       do i=2,n/2
             cmpx_vec(i)=cmplx(temprc(i,ith),-temprc(n+2-i,ith))
       end do
       cmpx_vec=cmpx_vec*fft_id%scale
    else
       temprc(:n/2+1,ith)=real(cmpx_vec(1:n/2+1))
       do i=0,n/2-2
          temprc(n-i,ith)=-aimag(cmpx_vec(i+2))
       end do
!       call rfftw_f77_one(fft_id%plan_back, temp_vec, real_vec)
       call sfftw_execute(fft_id%plan_back(ith))
       real_vec(1:n)=temprc(1:n,ith)*fft_id%scale
    end if
    return        
  end subroutine rfft_1d_data 
  !----------------------------------------------------------------------
  subroutine fft_2d_data(fft_id,sign,n1,n2,data_slice,ith)
    type (fft_type) fft_id
    integer :: n1,n2,ith
    integer :: sign
    complex, dimension (:,:) :: data_slice

    tempc2(:,:,ith)=data_slice*fft_id%scale

    if(sign == fFTW_FORWARD) then
!       call fftwnd_f77_one(fft_id%plan_forw, temp_2d,data_slice)
       call sfftw_execute(fft_id%plan_forw(ith))
    else
!       call fftwnd_f77_one(fft_id%plan_back, temp_2d,data_slice)
       call sfftw_execute(fft_id%plan_back(ith))
    end if

    data_slice=tempc2(:,:,ith)
    return        
  end subroutine fft_2d_data
  !----------------------------------------------------------------------
  subroutine clean_fft(fft_id) 
    type (fft_type) fft_id
    integer ierr,i

 do i=1,size(fft_id%plan_forw) 
    select case (fft_id%ndim)
    case (1)
!       call fftw_f77_destroy_plan(fft_id%plan_forw)
!       call fftw_f77_destroy_plan(fft_id%plan_back)
       call sfftw_destroy_plan(fft_id%plan_forw(i))
       call sfftw_destroy_plan(fft_id%plan_back(i))
    case (2)
!       call fftwnd_f77_destroy_plan(fft_id%plan_forw)
!       call fftwnd_f77_destroy_plan(fft_id%plan_back)
       call sfftw_destroy_plan(fft_id%plan_forw(i))
       call sfftw_destroy_plan(fft_id%plan_back(i))
    case (3)
!       call fftwnd_f77_destroy_plan(fft_id%plan_forw)
!       call fftwnd_f77_destroy_plan(fft_id%plan_back)
       call sfftw_destroy_plan(fft_id%plan_forw(i))
       call sfftw_destroy_plan(fft_id%plan_back(i))
    case default
       write(0,*)'fft_id%ndim=',fft_id%ndim
       call seperr('Not supported fft_id%ndim')
    end select
 end do
       deallocate(fft_id%plan_forw)
       deallocate(fft_id%plan_back)
    return        
  end subroutine clean_fft
!-----------------------------------------------------
  subroutine clean_rfft(fft_id) 
    type (fft_type) fft_id
    integer ierr,ith

    select case (fft_id%ndim)
    case (1)
!       call rfftw_f77_destroy_plan(fft_id%plan_forw)
!       call rfftw_f77_destroy_plan(fft_id%plan_back)
      do ith=1,size(fft_id%plan_forw)
         call sfftw_destroy_plan(fft_id%plan_forw(ith))
         call sfftw_destroy_plan(fft_id%plan_back(ith))
      end do
       deallocate(fft_id%plan_forw)
       deallocate(fft_id%plan_back)
    case default
       write(0,*)'fft_id%ndim=',fft_id%ndim
       call seperr('Not supported fft_id%ndim')
    end select
    return        
  end subroutine clean_rfft
end module fft_data_mod









!
!
!
!
!
!
!



module amo_logstretch_mod
! Ioan Vlad, Biondo Biondi, August 2001
  use fft_data_mod
  use amo_types_mod
  use fft_types
  use sep
  use fftwflags

  implicit none
  private
  public::init_amo_logstretch,amo_logstretch,clean_amo_logstretch

  integer                      :: n1, n2, n3, n1p, ilim, n_threads
  real                         :: o1, o2, o3, d1, d2, d3, d1s, epsilon
  real,dimension(:),allocatable:: os, kx, ky, tau, taureg, kfilt_max
  type(fft_type)               :: fft_1d_id,fft_2d_id,rfft_1d_id1,rfft_1d_id2

contains
  !------------------------------------------------------------- 
  subroutine init_amo_logstretch(param_amo)
    ! Purpose: compute only once some variables needed by amo_logstretch
    ! Everything alocated here is deallocated in clean_amo_logstretch

    type(amo_param_type), intent(inout) :: param_amo
    integer                             :: i1
    n_threads  = 1

    call calc_amo_dim(param_amo)

    ! More readable notations for dimensions
    n1=param_amo%n(1); n2=param_amo%n(2); n3=param_amo%n(3);
    o1=param_amo%o(1); o2=param_amo%o(2); o3=param_amo%o(3);
    d1=param_amo%d(1); d2=param_amo%d(2); d3=param_amo%d(3);

    ! Construct the irregularly sampled tau axis vector
    allocate(tau(n1-ilim))
    do i1=1,n1-ilim
       tau(i1)=log((o1+(ilim+i1-1)*d1)/param_amo%t_min_log) 
    end do

    ! Construct a regular tau coordinate vector for the stretch domain
    allocate(taureg(n1p))
    do i1=1,n1p
       taureg(i1)=tau(1)+(i1-1)*d1s
    end do

    ! Define the new axes in the log-stretch fft domain
    allocate(os(n1p/2+1),kx(n2),ky(n3),kfilt_max(n1/2+1)) 
    kx=build_fspace(d2,n2); 
    ky=build_fspace(d3,n3)
    do i1=1,n1p/2+1! omega for AMO 
       os(i1)=(2.*acos(-1.)*real(i1-1))/(d1s*real(n1p)) 
    end do
    do i1=1,n1/2+1 ! first line is omega for fk filtering
       kfilt_max(i1)=(2.*acos(-1.)*real(i1-1))/(d1 *real(n1))
       kfilt_max(i1)=abs(2*kfilt_max(i1)/param_amo%v_min) 
    end do

    ! Epsilon=Epsilon_0*(n2*d2*n3*d3)/(n2_0*d2_0*n3_0*d3_0)
    ! Or epsilon=param_amo%v_min_decay, read from parameter file in 
    ! the function calc_amo_dim or in the main program. If you do that,
    ! you need to edit the amo_param_type, to add %v_min_decay
    epsilon=1.*n2*d2*n3*d3/(256.*0.024*128.*0.024)

    ! Initialize FFTs. Components dellocated in clean_amo_logstretch
    fft_1d_id  =init_fft_1d (n1p ,n_threads  )
    fft_2d_id  =init_fft_2d (n2 ,n3,n_threads)
    rfft_1d_id1=init_rfft_1d(n1p  ,n_threads,.true. )
    rfft_1d_id2=init_rfft_1d(n1   ,n_threads,.false. )

  end subroutine init_amo_logstretch
  !-------------------------------------------------------------
  subroutine amo_logstretch(param_amo, realcube, cmpxcube)

    ! Purpose: do AMO using : "An accurate formulation of log-stretch dip moveout
    ! in the frequency-wavenumber domain", by Zhou B., Mason I., Greenhalgh A.,
    ! Geophysics 61, 815-821.
    
    ! Explanation of inputs and outputs:
    ! AMO is done as a transformation of the cmpxcube 
    ! from the offset (param_amo%hx_inp,param_amo%hy_inp) 
    !   to the offset (param_amo%hx_out,param_amo%hy_out).
    ! If the data is not log-stretched and FFT-ed, realcube is the input
    ! param_amo%v_min is minimum velocity for (f,k) filtering
    ! param_amo%t_min_log is the cutoff time for log stretch
    
    ! This subroutine needs to have the global variables and some things in 
    ! param_amo calculated by a preceding call to init_amo_logstretch, and some
    ! arrays deallocated by a call to clean_amo_logstretch, after the job is done

    type(amo_param_type)                  , intent(in   ) :: param_amo
    real,    dimension(n1          ,n2,n3), intent(inout) :: realcube
    complex, dimension(ilim+n1p/2+1,n2,n3), intent(inout) :: cmpxcube

    if(param_amo%fft_xy_forw) call p2t(realcube , cmpxcube, .true.)
    if(param_amo%amo        ) call t2t(param_amo, cmpxcube)
    if(param_amo%fft_xy_back) then 
       call t2p(realcube, cmpxcube, .true.)
       if((param_amo%fk_filter).and.(param_amo%v_min/=0.)) then 
          call fkfilter(realcube)
       end if
    end if

  end subroutine amo_logstretch
  !-------------------------------------------------------------
  subroutine clean_amo_logstretch

    deallocate(tau, taureg, os, kx, ky, kfilt_max)
    call clean_fft(fft_1d_id)
    call clean_fft(fft_2d_id)
    call clean_rfft(rfft_1d_id1)
    call clean_rfft(rfft_1d_id2)

  end subroutine clean_amo_logstretch
  !-------------------------------------------------------------
  subroutine p2t(realcube, cmpxcube, stretchflag)

    real,    dimension(:,:,:), intent(in   ) :: realcube
    complex, dimension(:,:,:), intent(  out) :: cmpxcube
    logical,                   intent(in   ) :: stretchflag
    ! log-stretch+ffts: stretchflag = .true.
    ! ffts only (for f-k filtering): stretchflag=.false.
    integer                              :: i1,i2,i3,i_thread
    real,    dimension(:,:), allocatable :: omp_slice1,omp_slice2
    complex, dimension(:,:), allocatable :: omp_slice3
    type(fft_type)                       :: rfft_id

    logstretch_fw:if(stretchflag) then ! size(cmpxcube)=(ilim+n1p/2+1,n2,n3)
       rfft_id=rfft_1d_id1
       allocate(omp_slice1(n1p    ,n_threads))
       allocate(omp_slice2(n1-ilim,n_threads)) 
       allocate(omp_slice3(n1p/2+1,n_threads))
       do i3=1,n3
          i_thread=1
          do i2=1,n2
             do i1=1,n1-ilim 
                omp_slice2(i1,i_thread)=realcube(ilim+i1,i2,i3)
             end do
             call omp_insulator3(n1-ilim,tau,omp_slice2,n1p,taureg,omp_slice1,i_thread)
             call omp_insulator2(rfft_id,fFTW_REAL_TO_COMPLEX,n1p,omp_slice1,omp_slice3,i_thread)
             do i1=ilim+1,ilim+n1p/2+1
                cmpxcube(i1,i2,i3)=omp_slice3(i1-ilim,i_thread)
             end do
          end do
       end do
       cmpxcube(:ilim,:,:)=cmplx(realcube(:ilim,:,:),0.)
       deallocate(omp_slice1,omp_slice2,omp_slice3)
       call omp_fft_2d(cmpxcube,ilim+1,ilim+n1p/2+1,fft_2d_id,fFTW_BACKWARD)
    else              ! cmpxcube has dimensions (n1/2+1,n2,n3)
       rfft_id=rfft_1d_id2
       allocate(omp_slice1(n1    ,n_threads))
       allocate(omp_slice3(n1/2+1,n_threads))
       do i3=1,n3
          i_thread=1
          do i2=1,n2
             do i1=1,n1
                omp_slice1(i1,i_thread)=realcube(i1,i2,i3)
             end do
             call omp_insulator2(rfft_id,fFTW_REAL_TO_COMPLEX,n1,omp_slice1,omp_slice3,i_thread)
             do i1=1,n1/2+1
                cmpxcube(i1,i2,i3)=omp_slice3(i1,i_thread)
             end do
          end do
       end do
       deallocate(omp_slice1,omp_slice3)
       call omp_fft_2d(cmpxcube,1,n1/2+1,fft_2d_id,fFTW_BACKWARD)
   end if logstretch_fw

  end subroutine p2t
  !-------------------------------------------------------------
  subroutine t2t(param_amo, cmpxcube)
    ! Arguments:
    type(amo_param_type)                  , intent(in   ) :: param_amo
    complex, dimension(ilim+n1p/2+1,n2,n3), intent(inout) :: cmpxcube
    ! Internal variables:
    integer                   :: i1, i2, i3, i_thread
    real, dimension(n_threads):: fi_inp, fi_out, skh

    do i_thread=1,n_threads
    do i3=i_thread,n3,n_threads
    do i2=1,n2
    do i1=1,n1p/2+1

       skh(i_thread)=kx(i2)*param_amo%hx_inp+ky(i3)*param_amo%hy_inp
       if (skh(i_thread)==0) then
          fi_inp(i_thread)=0.
       else if (os(i1)==0) then
          fi_inp(i_thread)=skh(i_thread)
       else 
          skh(i_thread)=sqrt(1.+(2.*skh(i_thread)/os(i1))**2.)
          fi_inp(i_thread)=0.5*os(i1)*(skh(i_thread)-1.-log(0.5*(skh(i_thread)+1.)))
       end if

       skh(i_thread)=kx(i2)*param_amo%hx_out+ky(i3)*param_amo%hy_out
       if (skh(i_thread)==0) then
          fi_out(i_thread)=0.
       else if (os(i1)==0) then
          fi_out(i_thread)=skh(i_thread)
       else 
          skh(i_thread)=sqrt(1.+(2.*skh(i_thread)/os(i1))**2.)
          fi_out(i_thread)=0.5*os(i1)*(skh(i_thread)-1.-log(0.5*(skh(i_thread)+1.)))
       end if

       cmpxcube(ilim+i1,i2,i3)=cmpxcube(ilim+i1,i2,i3)*&
       exp(cmplx(0.,fi_inp(i_thread)-fi_out(i_thread)))

    end do
    end do
    end do
    end do

  end subroutine t2t
  !-------------------------------------------------------------
  subroutine t2p(realcube, cmpxcube, stretchflag)

    real   , dimension(:,:,:), intent(  out) :: realcube
    complex, dimension(:,:,:), intent(in   ) :: cmpxcube
    logical,                   intent(in   ) :: stretchflag
    ! log-stretch+ffts: stretchflag = .true.
    ! ffts only (for f-k filtering): stretchflag=.false.
    type(fft_type)                         :: rfft_id
    integer                                :: i1,i2,i3,i_thread
    real,    dimension(:,:  ), allocatable :: omp_slice1,omp_slice2
    complex, dimension(:,:,:), allocatable :: temp_cmpx
    complex, dimension(:,:  ), allocatable :: omp_slice3

    logstretch_bk:if(stretchflag) then 
       rfft_id=rfft_1d_id1
       if(ilim>0) realcube(:ilim,:,:)=real(cmpxcube(:ilim,:,:))
       allocate(temp_cmpx(n1p/2+1,n2,n3))
       temp_cmpx=cmpxcube((ilim+1):,:,:)
       call omp_fft_2d(temp_cmpx,1,n1p/2+1,fft_2d_id,fFTW_FORWARD)
       allocate(omp_slice1(n1p    ,n_threads))
       allocate(omp_slice2(n1-ilim,n_threads)) 
       allocate(omp_slice3(n1p/2+1,n_threads))
       do i3=1,n3
          i_thread=1
          do i2=1,n2
             do i1=1,n1p/2+1
                omp_slice3(i1,i_thread)=temp_cmpx(i1,i2,i3)
             end do
             call omp_insulator2(rfft_id,fFTW_COMPLEX_TO_REAL,n1p,omp_slice1,omp_slice3,i_thread)
             call omp_insulator3(n1p,taureg,omp_slice1,n1-ilim,tau,omp_slice2,i_thread)
             do i1=1,n1-ilim
                realcube(ilim+i1,i2,i3)=omp_slice2(i1,i_thread)
             end do
          end do
       end do
       deallocate(temp_cmpx,omp_slice1,omp_slice2,omp_slice3)
    else           
       rfft_id=rfft_1d_id2
       allocate(temp_cmpx(n1/2+1,n2,n3))
       temp_cmpx=cmpxcube
       call omp_fft_2d(temp_cmpx,1,n1/2+1,fft_2d_id,fFTW_FORWARD)
       allocate(omp_slice1(n1    ,n_threads))
       allocate(omp_slice3(n1/2+1,n_threads))
       do i3=1,n3
          i_thread=1
          do i2=1,n2
             do i1=1,n1/2+1
                omp_slice3(i1,i_thread)=temp_cmpx(i1,i2,i3)
             end do
             call omp_insulator2(rfft_id,fFTW_COMPLEX_TO_REAL,n1,omp_slice1,omp_slice3,i_thread)
             do i1=1,n1
                realcube(i1,i2,i3)=omp_slice1(i1,i_thread)
             end do
          end do
       end do
       deallocate(temp_cmpx,omp_slice1,omp_slice3)
   end if logstretch_bk

  end subroutine t2p
  !-------------------------------------------------------------
  subroutine fkfilter(realcube)

    real,   dimension(n1    ,n2,n3), intent(inout) :: realcube
    complex,dimension(n1/2+1,n2,n3)                :: temp_cmpx

    integer                             :: i1, i2, i3, i_thread
    real,dimension(n_threads)           :: kfilt_k

    call p2t(realcube, temp_cmpx, .false.)

    !$OMP PARALLEL DEFAULT (SHARED), PRIVATE (i1, i2, i3, i_thread, kfilt_k)
    !$OMP DO
    do i_thread=1,n_threads
       do i3=i_thread,n3,n_threads
          do i2=1,n2
             kfilt_k(i_thread)=sqrt(kx(i2)**2+ky(i3)**2) 
             do i1=1,n1/2+1
                if(kfilt_k(i_thread)>kfilt_max(i1)) &
                temp_cmpx(i1,i2,i3)=temp_cmpx(i1,i2,i3)*&
                exp(-epsilon*(kfilt_k(i_thread)-kfilt_max(i1))**2)
             end do
          end do 
       end do
    end do
    !$OMP END PARALLEL
    call t2p(realcube, temp_cmpx, .false.)

  end subroutine fkfilter
  !-------------------------------------------------------------
  function linterp(nn1,coord1,ord1,nn2,coord2) result(ord2)
    ! calculates ord2 by interpolation. No extrapolation!!
    ! the vectors are ordered in ascending order

    integer,              intent(in) :: nn1,nn2
    integer                          :: i1,i2
    real, dimension(nn1), intent(in) :: coord1, ord1
    real, dimension(nn2), intent(in) :: coord2
    real, dimension(nn2)             :: ord2

    i1=1
    do i2=1,nn2 ! loop over output space
       do
          if (i1==nn1-1) exit
          if (coord1(i1+1)>coord2(i2)) exit
          i1=i1+1
       end do
       ord2(i2)=((coord1(i1+1)-coord2(i2))*ord1(i1  )+&
                 (coord2(i2  )-coord1(i1))*ord1(i1+1))/&
                 (coord1(i1+1)-coord1(i1))
    end do
  end function linterp
  !-------------------------------------------------------------
  function build_fspace(sampling_interval, N) result(omega)
    ! N must be even!!! It is the number of elements in vector to be fft-ed
    ! sampling_interval is d1, d2, d3,etc

    integer, intent(in) :: N
    real,    intent(in) :: sampling_interval
    real                :: dom
    integer             :: i
    real, dimension(N)  :: omega

    if(mod(N,2)==1) then
       write(0,*) " =============ERROR! ERROR! ERROR!============= "
       write(0,*) "The function build_fspace needs the (t,mx,my) cube"
       write(0,*) "to have even dimension values AFTER time cutoff"
       write(0,*) "for log stretch! The number", N, "is not even!"
    end if

    dom=1./(sampling_interval*real(N))

    do i=1,N/2+1
       omega(i)=dom*real(i-1)
    end do

    do i=N/2+2,N
       omega(i)=dom*real(i-N/2-1)-omega(N/2+1)
    end do
    omega=omega*2.*acos(-1.) ! acos(-1.) = pi
  end function build_fspace
  !--------------------------------------------------------------
  subroutine calc_amo_dim(param_amo)

    type(amo_param_type), intent(inout) :: param_amo
    integer                             :: pow2
    real                                :: distau

    ! Data from times lower than the cutoff time (lid) is "left at home".
    nonzero_cutoff_time: if(param_amo%t_min_log==0.) then
       write(0,*) "Error in the function calc_amo_dim:"
       write(0,*) "Cutoff time is zero! It will be set to o1"
       if(param_amo%o(1)>0.) then
          param_amo%t_min_log=param_amo%o(1)
       else
          write(0,*) "Error! o1 is not strictly positive either."
          write(0,*) "Cutoff time set to d1=",param_amo%d(1)
          param_amo%t_min_log=param_amo%d(1)
       end if
    end if nonzero_cutoff_time
    exist_lid:  if (param_amo%t_min_log>param_amo%o(1)) then
       ilim=1+(param_amo%t_min_log-param_amo%o(1))/param_amo%d(1) !largest index to be "left at home"
    else ! if there is no lid because cutoff time<o1
       ilim=0
    end if exist_lid

    ! To avoid aliasing, delta tau min = log(1+(delta t max)/tmax)
    d1s=log((param_amo%o(1)+param_amo%d(1)*(param_amo%n(1)-1))/&
     (param_amo%o(1)+param_amo%d(1)*(param_amo%n(1)-1)-0.5/param_amo%f_max))
    if(d1s==0.) write(0,*) "Error in calc_amo_dim:d1s=0. Division by zero"
    distau=log((param_amo%o(1)+(param_amo%n(1)-1)*param_amo%d(1))/(param_amo%o(1)+ilim*param_amo%d(1)))
    n1p=ceiling(1+distau/d1s)
    pow2=1.
    goto_closest_pow_of_2: do
       pow2=pow2*2
       if(pow2>=n1p) then
          n1p=pow2
          exit
       end if
    end do goto_closest_pow_of_2
    if(n1p<param_amo%n(1)) n1p=param_amo%n(1)
    d1s=distau/(n1p-1) !for perfect invertibility of logstretch
    param_amo%n1_cmpxcube=n1p/2+1+ilim
  end subroutine calc_amo_dim
!--------------------------------------------------------------
  subroutine omp_fft_2d(cmpxcube,startindex,endindex,fft_id,sign)

    complex, dimension(:,:,:), intent(inout):: cmpxcube
    integer                  , intent(in   ):: startindex,endindex,sign
    type(fft_type)           , intent(in   ):: fft_id
    integer                                 :: i1, i2, i3, i_thread
    complex, dimension(n2,n3,n_threads)     :: omp_slice

    do i_thread=1,n_threads
       do i1=(startindex-1)+i_thread,endindex,n_threads
          do i2=1,n2
             do i3=1,n3
                omp_slice(i2,i3,i_thread)=cmpxcube(i1,i2,i3)
             end do
          end do
          call omp_insulator1(fft_id,sign,n2,n3,i_thread,omp_slice)
          do i2=1,n2
             do i3=1,n3
                cmpxcube(i1,i2,i3)=omp_slice(i2,i3,i_thread)
             end do
          end do
       end do
    end do
  end subroutine omp_fft_2d
!--------------------------------------------------------------
  subroutine omp_insulator1(fft_id,sign,na,nb,i_thread,omp_slice)

    type(fft_type)                     ,intent(in   )::fft_id
    integer                            ,intent(in   )::sign,na,nb,i_thread
    complex, dimension(n2,n3,n_threads),intent(inout)::omp_slice

    call fft_2d_data(fft_id,sign,na,nb,omp_slice(:,:,i_thread),i_thread)

  end subroutine omp_insulator1
!--------------------------------------------------------------
  subroutine omp_insulator2(rfft_id,sign,na,romp_slice,comp_slice,i_thread)

    type(fft_type)        ,intent(in   )::rfft_id
    integer               ,intent(in   )::sign,na,i_thread
    real   ,dimension(:,:),intent(inout)::romp_slice
    complex,dimension(:,:),intent(inout)::comp_slice

    call rfft_1d_data(rfft_id               ,&
                      sign                  ,&
                      na                    ,&
                      romp_slice(:,i_thread),&
                      comp_slice(:,i_thread),i_thread )

  end subroutine omp_insulator2
!--------------------------------------------------------------
  subroutine omp_insulator3(nn1,coord1,ord1,nn2,coord2,ord2,i_thread)

    integer                       , intent(in   ) :: nn1,nn2,i_thread
    real, dimension(nn1          ), intent(in   ) :: coord1
    real, dimension(nn1,n_threads), intent(in   ) :: ord1
    real, dimension(nn2          ), intent(in   ) :: coord2
    real, dimension(nn2,n_threads), intent(  out) :: ord2

    ord2(:,i_thread)=linterp(nn1,coord1,ord1(:,i_thread),nn2,coord2)

  end subroutine omp_insulator3
!--------------------------------------------------------------
end module amo_logstretch_mod








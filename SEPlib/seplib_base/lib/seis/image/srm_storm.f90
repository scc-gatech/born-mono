module storm_mod
  use utilities_mod
  use pctrist_mod
  use sep_mod

  implicit none
  integer, parameter, private:: dbl=selected_int_kind(15)

  real,    dimension(:),   allocatable, private :: kzi,khxi,khyi,kmxi,kmyi
  complex, dimension(:,:), allocatable, private :: model, data
  real,    dimension(:,:), allocatable, private :: str

  real,    private :: eps
  logical, private :: push

  integer, private :: jz
contains
  !----------------------------------------------------------------
  subroutine storm_init(nth,az,ahx,ahy,amx,amy,eps_in,push_in,scale)
    integer,     intent(in) :: nth
    type(paxis), intent(in) :: az,ahx,ahy,amx,amy
    real,        intent(in) :: eps_in
    logical,     intent(in) :: push_in
    logical,     intent(in) :: scale
    type(paxis)             :: akz

    integer :: iz,ihx,ihy,imx,imy
    real    :: pi
    real    :: oz,dz
    integer :: stat

    push = push_in
    eps  = eps_in

    pi = acos(-1.)

    allocate( data(nth,az%n), stat=stat)
    if(stat /=0) call seperr("cant allocate data")
    allocate(model(nth,az%n), stat=stat)
    if(stat /=0) call seperr("cant allocate model")
    allocate(  str(nth,az%n), stat=stat)
    if(stat /=0) call seperr("cant allocate str")

    allocate( kzi( az%n), stat=stat)
    if(stat /=0) call seperr("cant allocate kzi")
    allocate(khxi(ahx%n), stat=stat);khxi=0
    if(stat /=0) call seperr("cant allocate khxi")
    allocate(khyi(ahy%n), stat=stat);khyi=0
    if(stat /=0) call seperr("cant allocate khyi")
    allocate(kmxi(amx%n), stat=stat);kmxi=0
    if(stat /=0) call seperr("cant allocate kmxi")
    allocate(kmyi(amy%n), stat=stat);kmyi=0
    if(stat /=0) call seperr("cant allocate kmyi")

    !! kz
    do iz = 1,az%n
       kzi(iz) = (-az%n/2+iz-1)
    end do
    kzi = 2*pi* kzi / (az%n*az%d)
    !
    oz = - pi/ az%d
    dz = 2*pi/(az%n*az%d)

    jz = az%n/2
    !
    akz%o = kzi(1)
    akz%d = 2*pi / (az%n*az%d)
    akz%n = az%n



    !! khx
    do ihx = 1,ahx%n/2
       khxi(ihx        ) = (         ihx-1)
       khxi(ihx+ahx%n/2) = (-ahx%n/2+ihx-1)
    end do
    khxi = 2*pi* khxi / (ahx%n*ahx%d)

    !! khy
    do ihy = 1,ahy%n/2
       khyi(ihy        ) = (         ihy-1)
       khyi(ihy+ahy%n/2) = (-ahy%n/2+ihy-1)
    end do


    kmyi = 2*pi* khyi / (ahy%n*ahy%d)

    !! kmx
    do imx = 1,amx%n/2
       kmxi(imx        ) = (         imx-1)
       kmxi(imx+amx%n/2) = (-amx%n/2+imx-1)
    end do
    kmxi = 2*pi* kmxi / (amx%n*amx%d)

    !! kmy
    do imy = 1,amy%n/2
       kmyi(imy        ) = (         imy-1)
       kmyi(imy+amy%n/2) = (-amy%n/2+imy-1)
    end do
    kmyi = 2*pi* kmyi / (amy%n*amy%d)

    call pctrist_init(nth,akz,akz,push,eps)

  end subroutine storm_init
  !----------------------------------------------------------------
  subroutine storm_srm_compute(ith,isl,f,ratio,az,ahx,ahy,imx,imy)
    integer,           intent(in) :: ith
    integer(kind=dbl), intent(in) :: isl
    real,        intent(in) :: ratio
    integer,     intent(in) ::    imx,imy
    type(paxis), intent(in) :: az,ahx,ahy
    integer                 :: iz,ihx,ihy
    complex, dimension(:,:,:,:) :: f

    real    :: ro
    real    :: kz,khx,khy,kmx,kmy
    real    :: mks,mkg
    real    :: kzh,kzm
    real    :: zzs,zzg

    kmx = kmxi(imx)
    kmy = kmyi(imy)

    if(push) then
       ro =   ratio
    else
       ro = 1/ratio
    end if

    do ihy = 1, ahy%n;      khy = khyi(ihy)
       do ihx = 1, ahx%n;   khx = khxi(ihx)
          do iz = 1, az%n;  kz  = kzi (iz )
             !! dispersion relation - doc in SEP100

             mks=sqrt( (kmx-khx)**2 + (kmy-khy)**2 ) !! | ks |
             mkg=sqrt( (kmx+khx)**2 + (kmy+khy)**2 ) !! | kg |

             kzh = (4*kz**2 + (mkg-mks)**2)/4
             kzm = (4*kz**2 + (mkg+mks)**2)/4 

             zzs = ro**2 * kzh*kzm - kz**2 * mks**2
             zzg = ro**2 * kzh*kzm - kz**2 * mkg**2
             str(ith,iz) = az%o
             if (zzs>0..and. zzg>0.) &
             str(ith,iz)=0.5/kz*(sqrt(zzs)+sqrt(zzg))
          end do
          call pctrist_define(ith,str)

          data(ith,1   :jz   )=f(1+jz:jz+jz,ihx,ihy,isl)
          data(ith,1+jz:jz+jz)=f(1   :jz   ,ihx,ihy,isl)

          call pctrist_apply(ith,model,data)

          f(1   :jz   ,ihx,ihy,isl)=model(ith,1+jz:jz+jz)
          f(1+jz:jz+jz,ihx,ihy,isl)=model(ith,1   :jz   )

       end do
    end do

  end subroutine storm_srm_compute



  !----------------------------------------------------------------
  subroutine storm_stolt_compute(ith,isl,f,vel,az,ahx,ahy,imx,imy)
    integer,           intent(in) :: ith
    integer(kind=dbl), intent(in) :: isl
    real,        intent(in) :: vel
    integer,     intent(in) ::    imx,imy
    type(paxis), intent(in) :: az,ahx,ahy
    integer                 :: iz,ihx,ihy
    complex, dimension(:,:,:,:) :: f

    real    :: ro
    real    :: kw,khx,khy,kmx,kmy
    real    :: mks,mkg
    real    :: kzh,kzm
    real    :: zzs,zzg,q

    kmx = kmxi(imx)
    kmy = kmyi(imy)


    do ihy = 1, ahy%n;      khy = khyi(ihy)
       do ihx = 1, ahx%n;   khx = khxi(ihx)
          do iz = 1, az%n;  kw  = kzi (iz )
             !! dispersion relation - doc in SEP100
if(abs(kw)>0.0001) then
    if(push) then
       ro =   kw/vel
    else
       ro = vel/kw
    end if

             mks=sqrt( (kmx-khx)**2 + (kmy-khy)**2 ) !! | ks |
             mkg=sqrt( (kmx+khx)**2 + (kmy+khy)**2 ) !! | kg |


             zzs = ro**2  -  mks**2
             zzg = ro**2  -  mkg**2

             str(ith,iz) = az%o
             if (zzs>0..and. zzg>0.) &
             str(ith,iz)=kw/abs(kw)*(sqrt(zzs)+sqrt(zzg))
!             str(ith,iz)=-0.5*(sqrt(zzs)+sqrt(zzg))
            data(ith,iz)=abs(str(ith,iz)/kw)
write(0,*) "CHECK",kmx,kw,str(ith,iz),data(ith,iz)
else 
end if
data(ith,iz)=1.
          end do
          call pctrist_define(ith,str)
  

          data(ith,1   :jz   )=f(1+jz:jz+jz,ihx,ihy,isl)* data(ith, 1+jz:jz+jz  )
          data(ith,1+jz:jz+jz)=f(1   :jz   ,ihx,ihy,isl)*  data(ith,1:1+jz)

          call pctrist_apply(ith,model,data)

          f(1   :jz   ,ihx,ihy,isl)=model(ith,1+jz:jz+jz)
          f(1+jz:jz+jz,ihx,ihy,isl)=model(ith,1   :jz   )

       end do
    end do

  end subroutine storm_stolt_compute




  !----------------------------------------------------------------
  subroutine storm_close()
    integer :: stat
    call pctrist_close()

    deallocate(kzi,  stat=stat)
    if(stat /=0) call seperr("cant deallocate kzi")
    deallocate(khxi, stat=stat)
    if(stat /=0) call seperr("cant deallocate khxi")
    deallocate(khyi, stat=stat)
    if(stat /=0) call seperr("cant deallocate khyi")
    deallocate(kmxi, stat=stat)
    if(stat /=0) call seperr("cant deallocate kmxi")
    deallocate(kmyi, stat=stat)
    if(stat /=0) call seperr("cant deallocate kmyi")

    deallocate(data,  stat=stat)
    if(stat /=0) call seperr("cant deallocate data")
    deallocate(model, stat=stat)
    if(stat /=0) call seperr("cant deallocate model")
    deallocate(str,   stat=stat)
    if(stat /=0) call seperr("cant deallocate str")

  end subroutine storm_close
  !----------------------------------------------------------------
end module storm_mod













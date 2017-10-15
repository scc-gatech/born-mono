module oc_filter_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================
  use oc_global_mod
  use oc_file_mod
  implicit none

  !----------------------------------------------------------------
  type rfilter
     real,    dimension( :), pointer :: flt
     integer, dimension( :), pointer :: lag
  end type rfilter

  type cfilter
     complex, dimension( :), pointer :: flt
     integer, dimension( :), pointer :: lag
  end type cfilter
  !----------------------------------------------------------------

  !----------------------------------------------------------------
  interface oc_allocatehelix
     module procedure R_allocatehelix
     module procedure C_allocatehelix
  end interface

  interface oc_deallocatehelix
     module procedure R_deallocatehelix
     module procedure C_deallocatehelix
  end interface

  interface oc_regrid
     module procedure R_regrid
     module procedure C_regrid
  end interface

  interface oc_buildfilter
     module procedure R_buildfilter
     module procedure C_buildfilter
  end interface

  interface oc_printfilter
     module procedure R_printfilter
     module procedure C_printfilter
  end interface
  !----------------------------------------------------------------

contains
  !----------------------------------------------------------------
  subroutine R_allocatehelix(aa, nh)
    type(rfilter), intent(inout)  :: aa
    integer,       intent(in)     :: nh
    allocate(aa%flt(nh), aa%lag(nh))
    aa%flt = 0.
    aa%lag = 0
  end subroutine R_allocatehelix

  subroutine C_allocatehelix(aa, nh)
    type(cfilter), intent(inout)  :: aa
    integer,       intent(in)     :: nh
    allocate(aa%flt(nh), aa%lag(nh))
    aa%flt = (0.,0.)
    aa%lag = 0
  end subroutine C_allocatehelix
  !----------------------------------------------------------------

  !----------------------------------------------------------------
  subroutine R_deallocatehelix(aa)
    type(rfilter)     :: aa
    deallocate(aa%flt, aa%lag)
  end subroutine R_deallocatehelix

  subroutine C_deallocatehelix(aa)
    type(cfilter)     :: aa
    deallocate(aa%flt, aa%lag)
  end subroutine C_deallocatehelix
  !----------------------------------------------------------------

  !----------------------------------------------------------------
  subroutine l2c(nn, ii, i)
    !! converts linear (i) to Cartessian (ii) coordinates
    integer, dimension(:), intent(in) :: nn
    integer, dimension(:), intent(out):: ii
    integer,               intent(in) ::  i
    integer                           :: n,k
    n=1
    do k = 1, size(nn)
       ii(k) = mod((i-1)/n, nn(k)) + 1
       n = n * nn(k)
    end do
    if (any(ii>nn)) call seperr("l2c: ii>nn")
  end subroutine l2c
  !----------------------------------------------------------------
  subroutine c2l(nn, ii, i)
    !! converts Cartessian (ii) to linear (i) coordinates
    integer, dimension(:), intent(in) :: nn
    integer, dimension(:), intent(in) :: ii
    integer,               intent(out)::  i
    integer                           :: n,k

    i=1
    n=1
    do k = 1, size(nn)
       i = i + (ii(k)-1)*n
       n = n * nn(k)
    end do
    if ((i>n)) call seperr("c2l: i>n")
  end subroutine c2l
  !----------------------------------------------------------------

  !----------------------------------------------------------------
  subroutine R_regrid(nold, nnew, aa)
    integer, dimension(:), intent(in)    :: nold, nnew
    type(rfilter),         intent(inout) :: aa
    integer, dimension(size(nold))       :: ii
    integer                              :: i, h0, h1, h

    call c2l(nold,nold/2+1,h0)
    call c2l(nnew,nold/2+1,h1)

    do i = 1, size(aa%lag)
       h = h0  +   aa%lag(i)
       call l2c(nold,ii,h) !! ii <-nold-  h
       call c2l(nnew,ii,h) !! ii  -nnew-> h 
       aa%lag(i)= h - h1
    end do
  end subroutine R_regrid
  !----------------------------------------------------------------
  subroutine C_regrid(nold, nnew, aa)
    integer, dimension(:), intent(in)    :: nold, nnew
    type(cfilter),         intent(inout) :: aa
    integer, dimension(size(nold))       :: ii
    integer                              :: i, h0, h1, h

    call c2l(nold,nold/2+1,h0)
    call c2l(nnew,nold/2+1,h1)

    do i = 1, size(aa%lag)
       h = h0  +   aa%lag(i)
       call l2c(nold,ii,h) !! ii <-nold-  h
       call c2l(nnew,ii,h) !! ii  -nnew-> h 
       aa%lag(i)= h - h1
    end do
  end subroutine C_regrid
  !----------------------------------------------------------------  
  subroutine R_buildfilter(ff,dbox,fbox,nf)
    type(rfilter),         intent(inout) :: ff
    integer,               intent(in)    :: nf
    integer, dimension(:), intent(in)    :: dbox  !   data box
    integer, dimension(:), intent(in)    :: fbox  ! filter box
    integer, dimension(:), allocatable   :: lbox  !  local box
    integer                              :: i

    allocate(lbox(size(fbox)))
    lbox=1
    do i=1,size(dbox)
       if(fbox(i)>dbox(i)) then
          lbox(i)=dbox(i)
       else
          lbox(i)=fbox(i)
       end if
       lbox(i) = min(lbox(i),64)
    end do
    lbox(1)=2*nf+1

    call oc_allocatehelix(ff,(product(lbox)-1)/2)
    do i=1,(product(lbox)-1)/2
       ff%lag(i)=i
    end do

    call oc_regrid(lbox,dbox,ff)

    deallocate(lbox)
  end subroutine R_buildfilter
  !----------------------------------------------------------------
  subroutine C_buildfilter(ff,dbox,fbox,nf)
    type(cfilter),         intent(inout) :: ff
    integer,               intent(in)    :: nf
    integer, dimension(:), intent(in)    :: dbox  !   data box
    integer, dimension(:), intent(in)    :: fbox  ! filter box
    integer, dimension(:), allocatable   :: lbox  !  local box
    integer                              :: i

    allocate(lbox(size(fbox)))
    lbox=1
    do i=1,size(dbox)
       if(fbox(i)>dbox(i)) then
          lbox(i)=dbox(i)
       else
          lbox(i)=fbox(i)
       end if
       lbox(i) = min(lbox(i),64)
    end do
    lbox(1)=2*nf+1

    call oc_allocatehelix(ff,(product(lbox)-1)/2)
    do i=1,(product(lbox)-1)/2
       ff%lag(i)=i
    end do

    call oc_regrid(lbox,dbox,ff)

    deallocate(lbox)
  end subroutine C_buildfilter
  !----------------------------------------------------------------
  subroutine R_printfilter(ff,nf)
    type(rfilter),        intent(in)  :: ff
    integer,              intent(in)  :: nf
    integer                           :: if,nk,ik
    real, dimension(:),   allocatable :: vff
    real, dimension(:,:), allocatable :: flt

    allocate(vff(nf+1+size(ff%lag)))

    vff=0.
    vff(nf+1)=1.
    vff(nf+1+1:)=ff%flt

    nk = (nf+1+size(ff%lag)) / (2*nf+1)
    allocate(flt(2*nf+1,nk))

    do ik=1,nk !! put filter on a matrix
       do if=1,2*nf+1
          flt(if,ik) = vff(if+ (ik-1)*(2*nf+1))
       end do
    end do

    select case(nk)
    case(1)
       write(0,*) "================="
       do if=1,2*nf+1
          write(0,'(1f8.3)') flt(if,:)
       end do
       write(0,*) "================="
    case(2)
       write(0,*) "================="
       do if=1,2*nf+1
          write(0,'(2f8.3)') flt(if,:)
       end do
       write(0,*) "================="
    case(5)
       write(0,*) "========================================="   
       do if=1,2*nf+1
          write(0,'(5f8.3)') flt(if,:)
       end do
       write(0,*) "========================================="
    end select

    deallocate(vff,flt)
  end subroutine R_printfilter
  !----------------------------------------------------------------
  subroutine C_printfilter(ff,nf)
    type(cfilter),           intent(in)  :: ff
    integer,                 intent(in)  :: nf
    integer                              :: if,nk,ik
    complex, dimension(:),   allocatable :: vff
    complex, dimension(:,:), allocatable :: flt

    allocate(vff(nf+1+size(ff%lag)))

    vff=0.
    vff(nf+1)=1.
    vff(nf+1+1:)=ff%flt

    nk = (nf+1+size(ff%lag)) / (2*nf+1)
    allocate(flt(2*nf+1,nk))

    do ik=1,nk !! put filter on a matrix
       do if=1,2*nf+1
          flt(if,ik) = vff(if+ (ik-1)*(2*nf+1))
       end do
    end do

    select case(nk)
    case(1)
       write(0,*) "REAL"
       write(0,*) "================="
       do if=1,2*nf+1
          write(0,'(1f8.3)') real(flt(if,:))
       end do
       write(0,*) "================="

       write(0,*) "IMAGINARY"
       write(0,*) "================="
       do if=1,2*nf+1
          write(0,'(1f8.3)') real((0.,1.)*flt(if,:))
       end do
       write(0,*) "================="

    case(2)
       write(0,*) "REAL"
       write(0,*) "================="
       do if=1,2*nf+1
          write(0,'(2f8.3)') real(flt(if,:))
       end do
       write(0,*) "================="

       write(0,*) "IMAGINARY"
       write(0,*) "================="
       do if=1,2*nf+1
          write(0,'(2f8.3)') real((0.,1.)*flt(if,:))
       end do
       write(0,*) "================="

    case(5)
       write(0,*) "REAL"
       write(0,*) "========================================="   
       do if=1,2*nf+1
          write(0,'(5f8.3)') real(flt(if,:))
       end do
       write(0,*) "========================================="

       write(0,*) "IMAGINARY"
       write(0,*) "========================================="   
       do if=1,2*nf+1
          write(0,'(5f8.3)') real((0.,1.)*flt(if,:))
       end do
       write(0,*) "========================================="
    end select

    deallocate(vff,flt)
  end subroutine C_printfilter
  !----------------------------------------------------------------
  subroutine oc_autoco2helix(aa,dbox,fdat)
    !! takes a filter in a box and puts it on a helix
    type(rfilter),            intent(inout) :: aa
    integer, dimension(:),    intent(in)    :: dbox
    real,    dimension(:,:,:),intent(in)    :: fdat
    integer, dimension(:),    allocatable   :: fbox
    character(len=128)                      :: x_
    integer                                 :: ns,nd,na
    integer                                 :: is,id,ia
    integer                                 :: i0,i
    integer, dimension(:),    allocatable   :: rfbox, rdbox
    real,    dimension(:),    allocatable   :: fvec

    allocate(fbox(3))
    fbox(1)=size(fdat,1)
    fbox(2)=size(fdat,2)
    fbox(3)=size(fdat,3)

    !-------------------------
    ns=size (dbox  )
    nd=count(dbox>1)      !! number of dimensions with n>1

    allocate(rfbox(nd),rdbox(nd))
    rfbox=1;
    rdbox=1;
    !-------------------------
    id=1;                 !! retains just n>1
    do is=1,ns
       if(fbox(is)>1) then
          rfbox(id)=fbox(is)
          rdbox(id)=dbox(is)
          id=id+1
       end if
    end do
    !-------------------------
    na=(product(rfbox)-1)/2
    call oc_allocatehelix(aa,2*na)     !! autocorrelation helix
    !-------------------------  

    !-------------------------  
    !! FILTER COEFFICIENTS
    allocate(fvec(product(rfbox)))
    fvec= pack(fdat,T)
    fvec=fvec/fvec(1+na)            !! scale to 1
    aa%flt(1   :  na)=fvec(1     :1+  na)
    aa%flt(1+na:2*na)=fvec(1+na+1:1+2*na)
    deallocate(fvec)
    !-------------------------  

    !------------------------- 
    !! FILTER LAGS
    call c2l(rfbox,rfbox/2+1,i0)    !! center absolute coord - line coords
    do ia=1,na
       aa%lag(   ia)=ia-i0
       aa%lag(na+ia)=ia
    end do
    call oc_regrid(rfbox,rdbox,aa)     !! relative coord - data coords
    !-------------------------  
  end subroutine oc_autoco2helix
  !----------------------------------------------------------------
  subroutine oc_printautoco(ff,nf)
    type(rfilter),        intent(in)  :: ff
    integer,              intent(in)  :: nf
    integer                           :: if,nk,ik
    real, dimension(:),   allocatable :: vff
    real, dimension(:,:), allocatable :: flt

    allocate(vff(1+size(ff%lag)))

    vff=0.
    vff(1:size(ff%lag)/2     )=ff%flt(1:size(ff%lag)/2   )
    vff(  size(ff%lag)/2+1   )=1.
    vff(  size(ff%lag)/2+1+1:)=ff%flt(  size(ff%lag)/2+1:)

    nk = (1+size(ff%lag)) / (2*nf+1)
    allocate(flt(2*nf+1,nk))

    do ik=1,nk !! put autoco on a matrix
       do if=1,2*nf+1
          flt(if,ik) = vff(if+ (ik-1)*(2*nf+1))
       end do
    end do

    select case(nk)
    case(3)
       write(0,*) "================="
       do if=1,2*nf+1
          write(0,'(3f8.3)') flt(if,:)
       end do
       write(0,*) "================="
    case(9)
       write(0,*) "========================================="   
       do if=1,2*nf+1
          write(0,'(5f8.3)') flt(if,:)
       end do
       write(0,*) "========================================="
    end select

    deallocate(vff,flt)
  end subroutine oc_printautoco
  !----------------------------------------------------------------
end module oc_filter_mod

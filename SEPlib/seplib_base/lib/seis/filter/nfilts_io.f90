!<
!io_nfilt
!
!Usage:
!call io_nfilt (arg1, arg2 arg3)
!
!Input Parameters:
!
!
!Output Parameters:
!
!
!
!Description:
!
!
!
!>
!-------------------------------------------------
!
!Author: Robert Clapp, ESMB 463, 7230253
!
!Date Created:Fri Mar 17 12:15:28 PST 2000
!
!Purpose:
!
!
module io_nfilt 
  use nhelix
  use regrid
  use sep
    implicit none
   integer, private :: ierr
  interface read_filt 
    module procedure read_filt_f,read_filt_n
  end interface 
  contains
  logical function rite_nfilt(tag,filt,n,o,d)
    character(len=*)  :: tag
    type(nfilter) :: filt
    integer :: n(:)
    integer, allocatable :: tempi(:)
    real, allocatable :: tempr(:)
    real    :: o(:),d(:)
    integer :: ndim ,nc,nout(9),i1,i
    rite_nfilt=.false.
    nout=1
    ndim=size(n)
    if (product(n).ne.size(filt%pch)) then
      write(0,*) "n=",n," size=",size(filt%pch)
      return
    end if
    call to_history("nfilt",size(filt%hlx),tag)
    call to_history("ndim",ndim,tag)
    call to_history("od",o,tag)
    call to_history("dd",d,tag)
    call to_history("nd",n,tag)
    nc=size(filt%pch)
    allocate(tempi(nc))
    tempi=filt%pch
    ierr= srite2(tag,tempi,size(tempi)*4,"xdr_int")
    deallocate(tempi)
    do  i1=1,size(filt%hlx)
      nc=                nc+2*size(filt%hlx(i1)%lag)+1
      ierr= srite2(tag,size(filt%hlx(i1)%flt),4,"xdr_int")
      allocate(tempi(size(filt%hlx(i1)%lag)),tempr(size(filt%hlx(i1)%lag))&
        &)
      tempr=filt%hlx(i1)%flt
      tempi=filt%hlx(i1)%lag
      ierr= srite2(tag,tempr,4*size(filt%hlx(i1)%flt),"xdr_float")        
      ierr= srite2(tag,tempi,4*size(filt%hlx(i1)%lag),"xdr_int")        
      deallocate(tempi,tempr)
    end do 
    if (associated(filt%mis)) then
      i=1
    else
      i=0
    end if 
    ierr= srite(tag,i,4)
    nc= nc+1
    if (i.eq.1) then
      allocate(tempi(size(filt%mis)))
      tempi=0
      where(filt%mis)
        tempi=1.
end where
      ierr= srite(tag,tempi,size(filt%mis)*4)
      deallocate(tempi)
    else
      nullify(filt%mis)
    end if 
    call to_history("n1",nc,tag)
    call to_history("n2",1,tag)
    call to_history("n3",1,tag)
    call to_history("n4",1,tag)
    call to_history("n5",1,tag)
    call to_history("n6",1,tag)
    call to_history("n7",1,tag)
    call to_history("n8",1,tag)
    rite_nfilt=.true.
  end function 
  logical function read_nfilt(tag,filt)
    character(len=*) :: tag
    type(nfilter) :: filt
    integer, allocatable  ::  n(:)
    integer :: ndim,nt,nfilt,i
    integer, allocatable :: tempi(:)
    real, allocatable :: tempr(:)
    read_nfilt=.false.
    if (associated(filt%hlx)) then
      call ndeallocate(filt)
    end if
    call from_aux(tag,"nfilt",nfilt)
    call from_aux(tag,"ndim",ndim)
    allocate(n(ndim))
    call from_aux(tag,"nd",n)
    allocate(tempi(product(n)))
    allocate(filt%pch(product(n)))
    ierr= sreed2(tag,tempi,size(filt%pch)*4,"xdr_int")
    if (any(tempi < 1)) then
      write(0,*) "invalid patch size",minval(tempi),maxval(tempi)
      return
    end if
    filt%pch=tempi
    deallocate(tempi)
    allocate(filt%hlx(nfilt))
    do i=1,nfilt 
      ierr= sreed2(tag,nt,4,"xdr_int")
      allocate(tempi(nt),tempr(nt))
      call allocatehelix(filt%hlx(i),nt)
      ierr= sreed2(tag,tempr,nt*4,"xdr_float")
      ierr= sreed2(tag,tempi,nt*4,"xdr_int")
      filt%hlx(i)%lag=tempi
      filt%hlx(i)%flt=tempr
      deallocate(tempi,tempr)
    end do 
    ierr= sreed(tag,i,4)
    if (i.eq.1) then
      allocate(tempi(product(n)))
      ierr= sreed(tag,tempi,4*size(tempI))
      allocate(filt%mis(size(tempi)))
      filt%mis=.false.
      where(tempi.ne.0)
        filt%mis=.true.
end where
      deallocate(tempi)
    end if
    read_nfilt=.true.
  end function 
  logical function rite_filt(tag,filt,n)
    character(len=*) :: tag
    type(filter) :: filt
    integer :: n1,i
    integer :: n(:)
    integer, allocatable :: m(:)
    real :: a0
    integer,external:: auxputch 
    rite_filt=.false.
    call to_history("n1",size(filt%lag))
    call to_history ("n", n,tag)
    call to_history("lag",filt%lag,tag)
    ierr= srite(tag,filt%flt,size(filt%lag)*4)
    rite_filt=.true.
  end function 
  logical function filt_to_nfilt(filt,nfilt,n)
    type(filter) :: filt
    type(nfilter) :: nfilt
    integer :: n(:)
    filt_to_nfilt=.false.
    allocate(nfilt%pch(product(n)))
    nfilt%pch=1
    allocate(nfilt%hlx(1))
    allocate(nfilt%hlx(1)%flt(size(filt%flt)))
    nfilt%hlx(1)%flt=filt%flt
    allocate(nfilt%hlx(1)%lag(size(filt%lag)))
    nfilt%hlx(1)%lag=filt%lag
    nullify(nfilt%mis)
    filt_to_nfilt=.true.
  end function 
  logical function read_filt_n(tag,nfilt,n)
    character(len=*) :: tag
    type(filter) :: filt
    type(nfilter) :: nfilt
    integer :: n(:)
    read_filt_n=.false.
    if (.not. read_filt_f(tag,filt,n)) then
      write(0,*) "trouble reading filter"
      return
    end if
    if (.not. filt_to_nfilt(filt,nfilt,n)) then
      write(0,*) "trouble converting filter to nfilter"
      return
    end if
    call deallocatehelix(filt)
    read_filt_n=.true.
  end function 
  logical function read_filt_f(tag,filt,n)
    character(len=*) :: tag
    type(filter) :: filt
    integer :: n1,i
    integer :: n(:)
    integer, allocatable :: m(:)
    real :: a0
!   integer,external:: auxpar 
    allocate(m(size(n)))
    read_filt_f=.false.
    call from_aux (tag, "a0", a0, 1.)
    call from_aux (tag, "n1", n1)
    call from_aux (tag, "n", m)
    allocate(filt%lag(n1),filt%flt(n1))
    i=auxpar("lag","d",filt%lag,tag)
    if (n1.ne.i) then
      write(0,*) "number of lags not equal number of coef",i,n1
      return
    end if
    ierr= sreed(tag,filt%flt,n1*4)
    filt%flt=filt%flt/a0
    call regridn (m, n, filt)
    deallocate(m)
    read_filt_f=.true.
    nullify(filt%mis)
  end function 
end module 

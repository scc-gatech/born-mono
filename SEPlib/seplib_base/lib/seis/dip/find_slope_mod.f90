!
! This is Sergey Fomel's dip finder
!
!  and Jon Claerbout Puck
!
!
!
module find_xz_dip_mod 
  use tri_smooth_mod
  use grad3_dip_mod
  use allpass_3d
  use allder_3d
  use divide
  implicit none
  contains
  logical function find_puck_dip_fast(field,nwind,dip,coh)
    real,intent(in)     :: field(:,:,:)
    integer,intent(in)  :: nwind(3)
    real,intent(out)    :: dip(:,:,:),coh(:,:,:)
    integer             :: b(3),e(3),i1,i2,i3,npct,i
    real                :: ipct
    real, pointer       :: dx(:,:,:),dt(:,:,:),xt(:,:,:),tt(:,:,:),xx(:&
      &,:,:)
    integer             :: ierr,n1,n2,n3
    find_puck_dip_fast=.false.
    n1=size(field,1)
    n2=size(field,2)
    n3=size(field,3)
    allocate(dx(n1,n2,n3),dt(n1,n2,n3),xt(n1,n2,n3),stat=ierr)
    if (ierr.ne.0) then
      write(0,*) "trouble allocating dx and dt"
      return
    end if
    call wavekill(1.,0.,field,dx)
    call wavekill(0.,1.,field,dt)
    xt=dt*dx
    tt=>dt
    tt=dt*dt
    xx=>dx
    xx=dx*dx
    call tri_smooth(xx,nwind)
    call tri_smooth(tt,nwind)
    call tri_smooth(xt,nwind)
    dip=-xt/tt
    coh=sqrt((xt/tt)*(xt/xx))
    deallocate(dx,dt,xt)
    find_puck_dip_fast=.true.
  end function 
  logical function find_puck_dip_slow(field,nwind,dip,coh,verb)
    real,intent(in)     :: field(:,:,:)
    integer,intent(in)  :: nwind(3)
    logical, intent(in) :: verb
    real,intent(out)    :: dip(:,:,:),coh(:,:,:)
    integer             :: b(3),e(3),i1,i2,i3,npct,i
    real                :: ipct
    find_puck_dip_slow=.false.
    npct=size(field,2)*size(field,3)
    ipct=.0001
    i=1
    do i3=1,size(field,3)
      b(3)=max(1,i3-nwind(3))
      e(3)=min(size(field,3),i3+nwind(3))
      do i2=1,size(field,2)
        b(2)=max(1,i2-nwind(2))
        e(2)=min(size(field,2),i2+nwind(2))
        i=     i+1
        if (real(i)/real(npct) > ipct .and. verb) then
          ipct=ipct+.05
          write(0,*) "Finished ",nint(real(ipct)/real(npct)*100.), "%"
        end if
        do i1=1,size(field,1)
          b(1)=max(1,i1-nwind(1))
          e(1)=min(size(field,1),i1+nwind(1))
          call find_puck_dips(field(b(1):e(1),b(2):e(2),b(3):e(3)),dip&
            &(i1,i2,i3),coh(i1,i2,i3))
        end do
      end do
    end do 
    find_puck_dip_slow=.true.
  end function 
  subroutine wavekill( aa, pp,    uu,  vv )
    integer i1,i2,                      n1,n2,n3,i3
    real aa, pp, s11, s12, s21, s22, uu(:,:,:), vv(:,:,:)
    s11 = -aa-pp
    s12 = aa-pp
    s21 = -aa+pp
    s22 = aa+pp
    vv=0
    n1=size(uu,1)
    n2=size(uu,2)
    n3=size(uu,3)
    do i3=1, n3 
      do i1= 1, n1-1  
! vv is one point shorter than uu on both axes.
        do i2= 1, n2-1  
          vv(i1,i2,i3) =        vv(i1,i2,i3) +  uu(i1  ,i2,i3) * s11 +&
            & uu(i1  ,i2+1,i3) * s12 +uu(i1+1,i2,i3) * s21 + uu(i1+1,i2+1,i3) * s22
        end do
      end do
    end do 
    vv(n1,:,:)=vv(n1-1,:,:)
    vv(:,n2,:)=vv(:,n2-1,:)
  end subroutine 
  subroutine find_puck_dips(field,dip,coh)
    real :: field(:,:,:)
    real :: dip,coh
    real :: dx(size(field,1),size(field,2),size(field,3))
    real :: dt(size(field,1),size(field,2),size(field,3))
    real :: xx,xt,tt
    call wavekill(1.,0.,field,dx)
    call wavekill(0.,1.,field,dt)
    xx=1e-30+sum(dx*dx)
    xt=1e-30+sum(dx*dt)
    tt=1e-30+sum(dt*dt)
    coh=sqrt((xt/tt)*(xt/xx))
    dip=-xt/tt
  end subroutine 
  logical function find_sergey_slow(n,field,niter,nl_iter,eps,dip,verb&
    &)
    real,intent(in) :: field(:)
    logical, intent(in) :: verb
    real :: eps,mean
    real,pointer ::  dip(:)
    integer, intent(in) :: niter,nl_iter
    integer :: n(3),ierr,n123,ndim,iter
    real, allocatable  :: u1(:),u2(:),dp(:)
    find_sergey_slow=.false.
    n123=product(n)
    allocate(u1(n123),u2(n123),dp(n123),stat=ierr)
    if (ierr.ne.0) then
      write(0,*) "trouble allocating temp arrays"
      return
    end if
    call grad3_init(n)
    call  allpass_3d_init(n(1),n(2),n(3),dip)
    call  allder_3d_init(n(1),n(2),n(3),dip)
    ndim=0
    if (n(1) >1) then
      ndim= ndim+1
    end if
    if (n(2) >1) then
      ndim= ndim+1
    end if
    if (n(3) >1) then
      ndim= ndim+1
    end if
    do iter=1,nl_iter 
      if (verb) then
        write(0,*) "working on non-linear iteration",iter
      end if
      ierr=allpass_3d_lop(.false.,.false.,field,u2)
      ierr=allder_3d_lop(.false.,.false.,field,u1)
      mean=sqrt(sum(u1*u1))
      u1=u1/mean
      u2=u2/mean
      call divide_apply_cg(.false.,n123,u2,u1,dp,eps,niter,grad3_op,ndim&
        &*n123)
      dip=dip+dp
    end do 
    deallocate(dp,u1,u2)
    call allpass_3d_close()
    call allder_3d_close()
    call grad3_close()
    find_sergey_slow=.true.
  end function 
  logical function find_sergey_fast(n,field,niter,eps1,eps2,dip,verb)
    logical, intent(in) :: verb
    real,pointer :: field(:)
    real :: eps1,eps2,mean
    real,pointer ::  dip(:)
    integer, intent(in) :: niter
    integer :: n(3),ierr,n123,ndim,iter
    real, allocatable  :: u1(:),u2(:),dp(:)
    find_sergey_fast=.false.
    n123=product(n)
    allocate(u1(n123),u2(n123),dp(n123),stat=ierr)
    if (ierr.ne.0) then
      write(0,*) "trouble allocating temp arrays"
      return
    end if
    call  allpass_3d_init(n(1),n(2),n(3),dip)
    call  allder_3d_init(n(1),n(2),n(3),dip)
    dip=0.
    do iter=1,niter 
      if (verb) then
        write(0,*) "working on non-linear iteration",iter
      end if
      ierr=allpass_3d_lop(.false.,.false.,field,u2)
      ierr=allder_3d_lop(.false.,.false.,field,u1)
      mean=sqrt(sum(u1*u1))
      u1=u1/mean
      u2=u2/mean
      call divide_apply_tri(n,u2,u1,dp,eps1,eps2)
      dip=dip+dp
    end do 
    deallocate(dp,u1,u2)
    call allpass_3d_close()
    call allder_3d_close()
    find_sergey_fast=.true.
  end function 
end module 

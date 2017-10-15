module ztrans_mod
  use sep
 

  implicit none
contains

  subroutine z_trans( n1,n2,n3,eps_t,eps_h,ztrans)
    integer		     :: n1,n2,n3,stat1,div,mirror_len
    integer                  :: it,i1,i2,i3,ir,ix,iy,taper_len,dist

    complex		     :: Ztrans1, Ztrans2,Ztrans3,Ztrans4,Ztrans5,Ztrans6
    complex                  :: Ztrans7,SumZtrans,kern1,kern2,kern3,kern4,kern5,kern6
    complex                  :: kern7
    real		     :: delx,dely,delz,kx,ky,kz,kernval1
    real                     ::eps_t,eps_h
    double precision         :: pi_var
    real, allocatable        :: rt_adj(:),FrequRangeX(:), FrequRangeY(:)
    real, pointer            :: rt(:)
    real, allocatable        :: FrequRangeZ(:)
    real, allocatable        :: Outphase(:,:,:), rt_adj_3D(:,:,:),rt_adj_tap(:,:,:),diverg(:,:,:),rt_adj_sub(:,:,:)
    real, allocatable        ::subxy(:,:,:),subx(:,:,:),suby(:,:,:)
    complex                  ::ztrans(:,:,:)
    complex, allocatable     :: OutY(:,:,:),rt_adj_cmp3D(:,:,:)
    real,    dimension  (2)  :: dot1,dot2

!    type(fft_type)           :: fft_3d_id

    ! CREATE PI VARIABLE
    pi_var=acos(-1.)


    delx=1.
    dely=1.
    delz=1.
 
    allocate(FrequRangeX(n1),FrequRangeY(n2), FrequRangeZ(n3))
  
    ! DEFINE FREQUENCY RANGES FOR EACH DIMENSION
    do i1=1,(n1/2+1)
       FrequRangeX(i1)=(1./n1)*(i1-1)
    end do
    do i1=n1/2+2,n1  
       FrequRangeX(i1)=-1*FrequRangeX(n1/2)+(1./n1)*(i1-n1/2-2)
    end do
    do i2=1,(n2/2+1)
       FrequRangeY(i2)=(1./n2)*(i2-1)
    end do
    do i2=n2/2+2,n2  
       FrequRangeY(i2)=-1*FrequRangeY(n2/2)+(1./n2)*(i2-n2/2-2)
    end do
    do i3=1,(n3/2+1)
       FrequRangeZ(i3)=(1./n3)*(i3-1)
    end do
    do i3=n3/2+2,n3  
       FrequRangeZ(i3)=-1*FrequRangeZ(n3/2)+(1./n3)*(i3-n3/2-2)
    end do

    if (n1==1) then
       eps_t=0.
    endif

    ! COMPUTE Z TRANSFORM
    do i1=1, n1 
       do i2=1,n2  
          do i3=1,n3  
             kx=FrequRangeX(i1) 
             ky=FrequRangeY(i2) 
             kz=FrequRangeZ(i3) 
             kern1=cmplx(0.,-kx*delx*2.*pi_var)
             Ztrans1= -eps_t*exp(kern1) 
             kern2=cmplx(0., -ky*dely*2.*pi_var )
             Ztrans2= -exp(kern2) 
             kern3=cmplx(0., -kz*delz*2.*pi_var )
             Ztrans3= -eps_h*exp(kern3) 
             Ztrans4= 2.+2.*eps_h +2.*eps_t
             kern5=cmplx(0.,kx*delx*2.*pi_var)
             Ztrans5= -eps_t*exp(kern5)
             kern6=cmplx(0.,ky*dely*2.*pi_var)
             Ztrans6= -exp(kern6)
             kern7=cmplx(0.,kz*delz*2.*pi_var)
             Ztrans7= -eps_h*exp(kern7)
             SumZtrans= Ztrans1 + Ztrans2 + Ztrans3 + Ztrans4 + Ztrans5 +Ztrans6&
                  &+Ztrans7 

             if (SumZtrans/=0.) then
                ztrans(i1,i2,i3)=SumZtrans
             else
                ztrans(i1,i2,i3)=0.
             end if
          end do
       end do
    end do
    deallocate(FrequRangeX,FrequRangeY,FrequRangeZ)


  end subroutine z_trans
  subroutine costrans( n1,n2,n3,eps_t,eps_h,cos_trans)
    integer		     :: n1,n2,n3,stat1,div,mirror_len
    integer                  :: it,i1,i2,i3,ir,ix,iy,taper_len,dist

    !    complex		     :: Ztrans1, Ztrans2,Ztrans3,Ztrans4,Ztrans5,Ztrans6
    real		     :: cos_trans1, cos_trans2,cos_trans3,cos_trans4,cos_trans5
    real                     :: cos_trans6,cos_trans7,Sum_cos_trans
    complex                  :: Ztrans7,SumZtrans,kern1,kern2,kern3,kern4,kern5,kern6
    complex                  :: kern7
    real		     :: delx,dely,delz,kx,ky,kz,kernval1
    real                     ::eps_t,eps_h
    double precision         :: pi_var
    real, allocatable        :: rt_adj(:),FrequRangeX(:), FrequRangeY(:)
    real, pointer            :: rt(:)
    real, allocatable        :: FrequRangeZ(:)
    real, allocatable        :: Outphase(:,:,:), rt_adj_3D(:,:,:),rt_adj_tap(:,:,:),diverg(:,:,:),rt_adj_sub(:,:,:)
    real, allocatable        ::subxy(:,:,:),subx(:,:,:),suby(:,:,:)
    real                     ::cos_trans(:,:,:)
    complex, allocatable     :: OutY(:,:,:),rt_adj_cmp3D(:,:,:)
    real,    dimension  (2)  :: dot1,dot2

!    type(fft_type)           :: fft_3d_id

    ! CREATE PI VARIABLE
    pi_var=acos(-1.)


    delx=1.
    dely=1.
    delz=1.

    allocate(FrequRangeX(n1),FrequRangeY(n2), FrequRangeZ(n3))

    ! DEFINE FREQUENCY RANGES FOR EACH DIMENSION
    do i1=1,(n1/2+1)
       FrequRangeX(i1)=(1./n1)*(i1-1)
    end do
    do i1=n1/2+2,n1  
       FrequRangeX(i1)=-1*FrequRangeX(n1/2)+(1./n1)*(i1-n1/2-2)
    end do
    do i2=1,(n2/2+1)
       FrequRangeY(i2)=(1./n2)*(i2-1)
    end do
    do i2=n2/2+2,n2  
       FrequRangeY(i2)=-1*FrequRangeY(n2/2)+(1./n2)*(i2-n2/2-2)
    end do
    do i3=1,(n3/2+1)
       FrequRangeZ(i3)=(1./n3)*(i3-1)
    end do
    do i3=n3/2+2,n3  
       FrequRangeZ(i3)=-1*FrequRangeZ(n3/2)+(1./n3)*(i3-n3/2-2)
    end do


    if (n1==1) then
       eps_t=0.
    endif

    ! COMPUTE Z TRANSFORM
    do i1=1, n1 
       do i2=1,n2  
          do i3=1,n3  
             kx=FrequRangeX(i1) 
             ky=FrequRangeY(i2) 
             kz=FrequRangeZ(i3)             
             cos_trans1=-eps_t*cos(-kx*delx*2.*pi_var)
             cos_trans2=-cos(-ky*dely*2.*pi_var)         
             cos_trans3=-eps_h*cos(-kz*delz*2.*pi_var)
             cos_trans4=2. +2.*eps_h +2.*eps_t
             cos_trans5=-eps_t*cos(kx*delx*2.*pi_var)
             cos_trans6=-cos(ky*dely*2.*pi_var)
             cos_trans7=-eps_h*cos(kz*delz*2.*pi_var)

             Sum_cos_trans= cos_trans1 + cos_trans2 + cos_trans3 + cos_trans4 + cos_trans5 +cos_trans6&
                  &+cos_trans7 
 
!Sum_cos_trans=2-2*cos(pi_var*ky)

             if (Sum_cos_trans/=0.) then
                cos_trans(i1,i2,i3)=Sum_cos_trans
             else
                cos_trans(i1,i2,i3)=0.
             end if
          end do
       end do
    end do
    deallocate(FrequRangeX,FrequRangeY,FrequRangeZ)


  end subroutine costrans

end module ztrans_mod

module dipinteg_effic
  use sep 

  implicit none
contains


  subroutine p_at_tau2D(tau,p,p_new,n1,n2,ref_2)
    integer		       :: n1,n2,n3,tau_1,tau_2,ref_2
    integer                 :: it,i1,i2,i3,ir,ix,iy,taper_len,dist
    real		       :: tau(:),upwt,downwt,p_tau_x,p_tau_y,dtau_dx,dtau_dy
    real                    :: shift_var,p(:),p_new(:)
    real, allocatable       ::tau_3D(:,:,:),px_3D(:,:,:),py_3D(:,:,:)
    real, allocatable       ::px_3D_new(:,:,:),py_3D_new(:,:,:)

    n3=1
    allocate(tau_3D(n1,n2,n3),px_3D(n1,n2,n3))!,py_3D(n1,n2,n3))
    allocate(px_3D_new(n1,n2,n3)) !,py_3D_new(n1,n2,n3))

    tau_3D=reshape(tau,(/n1,n2,n3/))
    px_3D=reshape(p(n1*n2*n3+1:n1*n2*n3*2),(/n1,n2,n3/))
 
    p_new=0.
    px_3D_new=0.
 
    ! FOR EACH POINT IN TAU SAMPLE THE PX FIELD 
    do it=1,n1
!       do ix=1,n2-1
       do ix=1,n2
          !   do iy=1,n3-1
          iy=1
          shift_var=tau_3D(it,ix,iy)-tau_3D(it,ref_2,1)
          tau_1=floor(it+shift_var)
          tau_2=ceiling(it+shift_var)
          upwt=it+shift_var-tau_1
          downwt=1.-upwt
          if (tau_1>=1.AND.tau_2<=n1) then
             p_tau_x=downwt*px_3D(tau_1,ix,iy)+upwt*px_3D(tau_2,ix,iy)

             px_3D_new(it,ix,iy)=p_tau_x !-dtau_dx
    
          end if
       

       end do
    end do
    p_new(n1*n2*n3+1:n1*n2*n3*2)=reshape(px_3D_new,(/n1*n2*n3/))
   
    deallocate(tau_3D,px_3D)
    deallocate(px_3D_new)
  end subroutine p_at_tau2D

  subroutine p_at_tau(tau,p,p_new,n1,n2,n3,ref_2,ref_3)
    integer		       :: n1,n2,n3,tau_1,tau_2,ref_2,ref_3
    integer                 :: it,i1,i2,i3,ir,ix,iy,taper_len,dist
    real		       :: tau(:),upwt,downwt,p_tau_x,p_tau_y,dtau_dx,dtau_dy
    real                    :: shift_var,p(:),p_new(:)
    real, allocatable       ::tau_3D(:,:,:),px_3D(:,:,:),py_3D(:,:,:)
    real, allocatable       ::px_3D_new(:,:,:),py_3D_new(:,:,:)
 
    allocate(tau_3D(n1,n2,n3),px_3D(n1,n2,n3),py_3D(n1,n2,n3))
    allocate(px_3D_new(n1,n2,n3),py_3D_new(n1,n2,n3))

    tau_3D=reshape(tau,(/n1,n2,n3/))
    px_3D=reshape(p(n1*n2*n3+1:n1*n2*n3*2),(/n1,n2,n3/))
    py_3D=reshape(p(n1*n2*n3*2+1:3*n1*n2*n3),(/n1,n2,n3/))
    p_new=0.
    px_3D_new=0.
    py_3D_new=0.
    ! FOR EACH POINT IN TAU SAMPLE THE PX FIELD
    do it=1,n1
       do ix=1,n2-1
          do iy=1,n3-1
             shift_var=tau_3D(it,ix,iy)-tau_3D(it,ref_2,ref_3)
             tau_1=floor(it+shift_var)
             tau_2=ceiling(it+shift_var)
             upwt=it+shift_var-tau_1
             downwt=1.-upwt
             if (tau_1>=1.AND.tau_2<=n1) then
                p_tau_x=downwt*px_3D(tau_1,ix,iy)+upwt*px_3D(tau_2,ix,iy)
                p_tau_y=downwt*py_3D(tau_1,ix,iy)+upwt*py_3D(tau_2,ix,iy)

                px_3D_new(it,ix,iy)=p_tau_x !-dtau_dx
                py_3D_new(it,ix,iy)=p_tau_y !-dtau_dy
             end if
       
          end do
       end do
    end do
    p_new(n1*n2*n3+1:n1*n2*n3*2)=reshape(px_3D_new,(/n1*n2*n3/))
    p_new(n1*n2*n3*2+1:n1*n2*n3*3)=reshape(py_3D_new,(/n1*n2*n3/))
    deallocate(tau_3D,px_3D,py_3D)
    deallocate(px_3D_new,py_3D_new)
  end subroutine p_at_tau


end module dipinteg_effic

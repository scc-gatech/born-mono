!
!
! Lloyd's algorthm for velocity selection
!
! Reference velocity selection by a generalized Lloyd method(Clapp 2002)
!
module lloyd_mod 
  use gee_quantile_mod
  use sep
  implicit none
  real, private, save :: min_r_pct,min_s_pct,minme,maxme,slow_own
  real, private :: bound2(1000)
  integer,private,save :: ndiv,niter_over
  logical,private,save :: perc_start
  contains
!INIT FUNCTION
  subroutine init_lloyd()
!MINIMUM ALLOWED PCT OF ALL SLOWNESS BEFORE GROUP IS DESTROYED
    call from_param("min_region_pct",min_r_pct,1.)
    min_r_pct=min_r_pct/100.
!MINIMUM ALLOWED DEVIATION BETWEEN CENTROIDS
    call from_param("min_slow_dev",min_s_pct,6.)
    min_s_pct=min_s_pct/100.
    call from_param("perc_start",perc_start,.false.)
    call from_param("vel_own",slow_own,-1.)
    slow_own=1./slow_own
!NUMBER OF ITERATIONS
    call from_param("niter_lloyd",niter_over,-1)
  end subroutine 
  subroutine median_init(array,nn,near)
    integer :: nn,ib,i
    real :: array(:),near(:)
    do ib=1,nn 
      i=real(ib)*real(size(array))/real(nn+1)
      near(ib)=gee_quantile(i,array)
    end do
  end subroutine 
  subroutine mean_init(nar,array,nn,near)
    integer :: nn,ib,i,nuse,nar,nlast,ifirst
    real :: array(:),near(:),tempa(nar)
    real :: mymax,mymin,delta
    tempa=array
    ifirst=1
    mymax=maxval(tempa)
    mymin=mymax
    if (slow_own>0.) then
      where(tempa <= slow_own)
        tempa=mymax
end where
      near(1)=slow_own
      ifirst=  ifirst+1
      do i=1,size(tempa)
        if (tempa(i) > slow_own .and. tempa(i) < mymin) then
          mymin=tempa(i)
        end if
      end do
    else
      mymin=minval(tempa)
    end if 
    delta=(mymax-mymin)/(nn-ifirst+1)
    do i=ifirst,nn 
      near(i)=mymin+(i-.5)*delta
    end do
  end subroutine 
  integer function lloyd_go(array,near,nn,bound,err,nlast)
    real :: array(:),near(:),bound(:)
    integer :: niter
    integer :: ib,i,nn,nn1,nn2
    real :: smin,smax,ds,dist_old,dist_near,distort_local(nn)
    real :: junk(nn),err,try1(nn),try2(nn),bound1(nn),bound2(nn),err2,err1,a
    complex :: j(nn)
    integer :: block(size(array)),region_count(nn),nsmall,nl,ireg
    integer, optional :: nlast

!initial guess
    minme=minval(array)
    maxme=maxval(array)
    min_r_pct=min_r_pct*size(array)
    junk=0
    nl=0
    if(present(nlast)) nl=nlast
    if(nl<2) then
    if (perc_start) then
      call median_init(array,nn,near)
    else
      call mean_init(size(array),array,nn,near)
    end if 
    ndiv=nn
   else
     ndiv=nlast
   end if
    if (niter_over.eq.-1) then
      niter=30
    else
      niter=niter_over
    end if 
    if (niter<1) then
      call find_bound(near,bound)
    end if
!dist_old= compute_distort(array,block,near)
    do i=1,niter 
      junk=i
      call find_bound(near,bound)
      call partition_space(array,bound,block)
!      write(0,*) "CHECKL",i,ndiv
      call compute_center(array,block,near,region_count)
      dist_near=compute_distort(array,block,near,distort_local,region_count,err)
      if (i< niter/2.) then
!        write(0,*) "REGION IN",near(1:ndiv)
        call change_regions(array,block,near,bound,distort_local&
          &,region_count)
         if(i< niter/4) then
            do while(ndiv < size(near))
               call random_number(a)
              ireg=min(ndiv,max(1,ceiling(a*real(ndiv))))
!              write(0,*) "CHEC1",ndiv,ireg
              call add_region(9999,ireg,array,block,near,bound&
                &,distort_local,region_count)
               ndiv=ndiv+1
             end do
         end if
!        write(0,*) "REGION OUT",near(1:ndiv)
      end if
!      write(0,*) "CHECK2",i,ndiv
    end do
    min_r_pct=min_r_pct/size(array)
    err=err/(sum(array)/size(array))*100.
    lloyd_go=ndiv
  end function 
  subroutine change_regions(array,block,near,bound,distort_local&
    &,region_count)
    real :: array(:),near(:),bound(:),distort_local(:)
    integer :: block(:),i,region_count(:)
!Get rid of regions with low count
    call rid_small(array,block,near,bound,distort_local,region_count) 
!Get rid of regions with close centers
    call rid_close(array,block,near,bound,distort_local,region_count)
  end subroutine 
  subroutine rid_close(array,block,near,bound,distort_local,region_count&
    &)
    real :: array(:),near(:),bound(:),distort_local(:),maxd,val
    integer :: block(:),i,region_count(:),ireg
    maxd=1000.
    ireg=-1
    do i=1,ndiv-1 
      val=(near(i+1) -near(i))/near(i)
      if (val< min_s_pct .and. val < maxd) then
        ireg=i
        if (distort_local(i+1)< distort_local(i)) then
          ireg=i+1
        end if
        maxd=val
      end if
    end do 
    if (ireg.ne.-1) then
      call replace_region(ireg,array,block,near,bound,distort_local&
        &,region_count)
    end if
  end subroutine 
  subroutine delete_region(ireg_del,array,block,near,bound,distort_local&
    &,region_count)
    integer :: ireg_del
    real :: array(:),near(:),bound(:),distort_local(:),minr,maxr
    integer  :: ireg_split,ib(1)
    integer :: block(:),i,region_count(:)
    if (ireg_del.ne.ndiv) then
      near(ireg_del: ndiv-1)=near(ireg_del+1:) !shift down the center
      bound(ireg_del: ndiv-1)=bound(ireg_del+1:) 
      !shift down the region boundaries
    end if
  end subroutine 
  subroutine replace_region(ireg_del,array,block,near,bound,distort_local&
    &,region_count)
    integer :: ireg_del
    real :: array(:),near(:),bound(:),distort_local(:),minr,maxr
    integer  :: ireg_split,ib(1),iold_reg
    integer :: block(:),i,region_count(:)
    real :: pct_close,maxd
    maxd=0.
    ireg_split=-1
    do i=1,ndiv 
      pct_close=1000.
      if (i >1) then
        pct_close=(near(i)-near(i-1))/near(i)
      end if
      if (i<ndiv) then
        pct_close=max(pct_close,(near(i+1)-near(i))/near(i+1))
      end if
      if (pct_close > min_s_pct) then
        if (distort_local(i)> maxd .and. i.ne.ireg_del .and. region_count(i)> min_r_pct &
          &) then
          maxd=distort_local(i)
          ireg_split=i
        end if
      end if
    end do 
!DON'T ADD A POINT IF THE MAX DEVIATION REGION'S DEVIATION
! IS REALLY  SMALL
    if (sqrt(maxd)/near(1) <  min_s_pct/4.) then
      ireg_split=-1
    end if
!DELETE REGION IF ITS CLOSE
    pct_close=10000.
    call delete_region(ireg_del,array,block,near,bound,distort_local&
      &,region_count)
    if (ireg_split>0 ) then
      call add_region(ireg_del,ireg_split,array,block,near,bound&
        &,distort_local,region_count)
    else
      ndiv=   ndiv-1
    end if
  end subroutine 
  subroutine add_region(ireg_del,ireg_split,array,block,near,bound&
    &,distort_local,region_count)
    integer :: ireg_del
    real :: array(:),near(:),bound(:),distort_local(:),minr,maxr
    integer  :: ireg_split,ib(1)
    integer :: block(:),i,region_count(:)
    real :: dist_near,err
    if (ireg_split > ireg_del) then
      ireg_split= ireg_split-1
    end if
    near(ireg_split+2:ndiv)=near(ireg_split+1:ndiv-1)
    if (ireg_split.eq.1) then
      minr=minme
    else
      minr=bound(ireg_split-1)
    end if 
    if (ireg_split.eq.ndiv-1) then
      maxr=maxme
    else
      maxr=bound(ireg_split)
    end if 
    near(ireg_split)=2./3.*minr +1./3. * maxr
    near(ireg_split+1)=1./3.*minr +2./3. * maxr
    call find_bound(near,bound)
    call partition_space(array,bound,block)
    call compute_center(array,block,near,region_count)
    dist_near=compute_distort(array,block,near,distort_local,region_count,err)
  end subroutine 
  subroutine rid_small(array,block,near,bound,distort_local,region_count&
    &)
    real :: array(:),near(:),bound(:),distort_local(:)
    integer :: block(:),i,region_count(:)
    do i=1,ndiv 
      if (region_count(i) < min_r_pct) then
        call replace_region(i,array,block,near,bound,distort_local&
          &,region_count)
      end if
    end do
  end subroutine 
  subroutine compute_center(array,block,near,ncount)
    real :: array(:),near(:),temp1,temp2
    integer :: block(:),i,ia,ncount(:),i_f
    real :: low,high
    ncount=0
    near=0
    do i=1,size(array)
      near(block(i))=  near(block(i))+array(i)
      ncount(block(i))=  ncount(block(i))+1
    end do 
    where(ncount>0)
      near=near/ncount
end where
    do i=1,ndiv 
!if the region has 0 points
      if (ncount(i).eq.0) then
!look for the first region with more than 0 points
        if (i.eq.1) then
          i_f=2
          do while (near(i_f).eq.0.)
            i_f= i_f+1
          end do 
          near(1)=near(i_f)
        else if (i.eq.ndiv) then
          i_f=ndiv-1
          do while (near(i_f).eq.0.)
            i_f= i_f-1
          end do 
          near(ndiv)=near(i_f)
        end if
        low=near(max(1,i-1))
        i_f=i+1
        do while (i_f < ndiv .and. near(i_f).eq.0. )
          i_f= i_f+1
        end do 
        if (i_f.ne.ndiv) then
          high=near(i_f)
        else
          if (near(ndiv).ne.0.) then
            high=near(i_f)
          else
            high=low
          end if
        end if 
        near(i)=0.5*(high+low)
      end if
    end do
  end subroutine 
!subroutine partition_space(array,bound,block){
!real :: array(:)
!real :: bound(:)
!integer  :: block(:),i,ib
!
!
!do i=1,size(array){
! block(i)=1
! while(array(i)>= bound(block(i))){
!    block(i)+=1;
!  }
!
!
!}
!
!if(run%m_ith==0) write(0,*) "SAMPLED",block(1::117)
!
!}
  subroutine partition_space(array,bound,block)
    real :: array(:)
    real :: bound(:)
    integer  :: block(:),i,ib,ilast,in,ix
    bound2(1)=0.
    bound2(2:size(bound)+1)=bound(1:size(bound))
! block(1)=1
! while(array(i)>= bound(block(1))){
!    block(1)+=1;
!  }
!  ilast=block(1)
    do i=1,size(array)
! if(array(i) < bound2(ilast) || array(i) >= bound2(ilast+1)){
      block(i)=1
      do while (array(i)>= bound(block(i)))
        block(i)=      block(i)+1
      end do 
! }
! block(i)=ilast
    end do
  end subroutine 
  real function compute_distort(array,block,near,distort_local,nn,total_distort)
    real :: array(:),distort_local(:)
    integer :: block(:) ,nn(:)
    real :: near(:),total_distort
    integer :: i
    compute_distort=0.
    distort_local=0.
    do i=1,size(array)
      distort_local(block(i))=    distort_local(block(i))+(array(i)-near(blo&
        &ck(i)))**2
    end do 
    where(nn>1)
      distort_local=distort_local/nn
   end where
   total_distort=sum(distort_local)
  end function 
  subroutine find_bound(near,bound)
    real :: near(:),bound(:)
    integer :: i
    do i=1,ndiv-1
      bound(i)=near(i+1)+near(i)
    end do 
    bound=bound/2.
    bound(ndiv)=1000000.
  end subroutine 
end module 

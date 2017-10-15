module oc_polydiv_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================
  use oc_global_mod
  use oc_filter_mod
  use oc_adjnull_mod
  implicit none

  type(rfilter),     private :: ocraa
  type(cfilter),     private :: occaa
  logical,           private :: cflag
  integer,           private :: ocmaxmem

  interface oc_polydiv_init
     module procedure R_polydiv_init
     module procedure C_polydiv_init
  end interface

  interface polydiv
     module procedure  R_polydiv
     module procedure  C_polydiv
  end interface

contains

  !----------------------------------------------------------------
  subroutine R_polydiv_init(raa,maxmem)
    type(rfilter), intent(in) :: raa
    integer,       optional   :: maxmem

    ocraa = raa 
    ocmaxmem=0; if(present(maxmem)) ocmaxmem=maxmem
    cflag = F

    call oc_allocatehelix(occaa,size(ocraa%lag))
    occaa%lag = ocraa%lag
    occaa%flt = ocraa%flt

  end subroutine R_polydiv_init
  !----------------------------
  subroutine C_polydiv_init(caa,maxmem)
    type(cfilter), intent(in) :: caa 
    integer,       optional   :: maxmem

    occaa = caa 
    ocmaxmem=0; if(present(maxmem)) ocmaxmem=maxmem
    cflag = T

    call oc_allocatehelix(ocraa,size(occaa%lag))
    ocraa%lag =      occaa%lag
    ocraa%flt = real(occaa%flt)

  end subroutine C_polydiv_init
  !----------------------------------------------------------------

  !----------------------------------------------------------------
  integer function R_polydiv(adj,add, xx,yy) result(stat)
    logical,            intent(in)    :: adj,add 
    real, dimension(:), intent(inout) :: xx,yy
    integer                           :: ix,iy,ia
    real, dimension(:), allocatable   :: tt
    integer                           :: nd

    nd=size(xx)

    call adjnull(adj,add,xx,yy)

    allocate(tt(nd))
    tt = 0.

    if(adj) then
       do ix= nd,1,-1  
          tt(ix) = yy(ix)
          do ia = 1, size(ocraa%lag)
             iy = ix + ocraa%lag(ia)
             if (iy > nd) then
                cycle
             end if
             tt(ix) = tt(ix) - ocraa%flt(ia) * tt(iy)
          end do
       end do
       xx = xx + tt
    else
       do iy= 1,nd  
          tt(iy) = xx(iy)
          do ia = 1, size(ocraa%lag)
             ix = iy - ocraa%lag(ia)
             if(ix < 1) then
                cycle
             end if
             tt(iy) = tt(iy) - ocraa%flt(ia) * tt(ix)
          end do
       end do
       yy = yy + tt
    end if

    deallocate(tt)
    stat=0
  end function R_polydiv
  !----------------------------
  integer function C_polydiv(adj,add, xx,yy) result(stat)
    logical,               intent(in)    :: adj,add 
    complex, dimension(:), intent(inout) :: xx,yy
    integer                              :: ix,iy,ia
    complex, dimension(:), allocatable   :: tt
    integer                              :: nd

    nd=size(xx)

    call adjnull(adj,add,xx,yy)

    allocate(tt(nd))
    tt = 0.

    if(adj) then
       do ix= nd,1,-1  
          tt(ix) = yy(ix)
          do ia = 1, size(occaa%lag)
             iy = ix + occaa%lag(ia)
             if (iy > nd) then
                cycle
             end if
             if(cflag) then
                tt(ix) = tt(ix) - conjg(occaa%flt(ia)) * tt(iy)
             else
                tt(ix) = tt(ix) -       ocraa%flt(ia)  * tt(iy)
             end if
          end do
       end do
       xx = xx + tt
    else
       do iy= 1,nd  
          tt(iy) = xx(iy)
          do ia = 1, size(occaa%lag)
             ix = iy - occaa%lag(ia)
             if(ix < 1) then
                cycle
             end if
             if(cflag) then
                tt(iy) = tt(iy) -       occaa%flt(ia) * tt(ix)
             else
                tt(iy) = tt(iy) -       ocraa%flt(ia) * tt(ix)
             end if
          end do
       end do
       yy = yy + tt
    end if

    deallocate(tt)
    stat=0
  end function C_polydiv
  !----------------------------------------------------------------
  integer function oc_polydiv(adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(stat)
    logical,               intent(in)  :: adj,add 
    character(len=128),    intent(in)  :: x_,yy_
    integer, external,     optional    :: op1,op2,op3,op4,op5,op6,op7,op8,op9
    type(fileinfo)                     :: filex, filey, file
    real,    dimension(:), allocatable :: rxx, ryy, rzz, rtt
    complex, dimension(:), allocatable :: cxx, cyy, czz, ctt
    integer                            :: ix,iy,ia,i

    call oc_adjnull(adj,add,x_,yy_)

    call oc_allocatefile(filex, x_,  ocmaxmem); call sseek(x_, 0,0)
    call oc_allocatefile(filey, yy_, ocmaxmem); call sseek(yy_,0,0)
    call oc_checksimilarity(filex, filey, "oc_polydiv")
    call oc_deallocatefile(filex); call sseek(x_, 0,0)
    call oc_deallocatefile(filey); call sseek(yy_,0,0)
    call oc_allocatefile(file, x_, ocmaxmem)

    if (file%esize /=8) then !! REAL
       if(maxval(ocraa%lag)>file%bloe) call seperr("maxlag > block elements")

       allocate(rxx(2*file%bloe),ryy(2*file%bloe),rzz(2*file%bloe),rtt(file%bloe))
       rzz=0.

       if(adj) then ! adjoint operator
          !! read first block
          stat=sseek_block(yy_,file%blon-1,file%blob,0)
          call sreed(yy_,rtt,file%blob)
          ryy(1+file%bloe:2*file%bloe)=rtt

          stat=sseek_block(x_, file%blon-1,file%blob,0)
          call sreed(x_, rtt,file%blob)
          rxx(1+file%bloe:2*file%bloe)=rtt

          !! polydiv on the first block
          do ix=2*file%bloe,1+file%bloe,-1
             rzz(ix) = ryy(ix)
             do ia=1,size(ocraa%lag)
                iy=ix + ocraa%lag(ia)
                if(iy>2*file%bloe) cycle
                rzz(ix) = rzz(ix) - ocraa%flt(ia) * rzz(iy)
             end do
             rxx(ix) = rxx(ix) + rzz(ix)
          end do

          !! output first block in case it is the only one
          stat=sseek_block(x_,file%blon-1,file%blob,0)
          call srite(x_,rxx(1+file%bloe:2*file%bloe),file%blob)

          !! loop over the other blocks
          do i=file%blon-1,1,-1
             !! read the lower block; put it below the lower one
             stat=sseek_block(yy_,i-1,file%blob,0)
             call sreed(yy_,rtt,file%blob)
             ryy(1:file%bloe)=rtt

             stat=sseek_block(x_, i-1,file%blob,0)
             call sreed(x_,rtt,file%blob)
             rxx(1:file%bloe)=rtt

             !! polydiv on the lower block
             do ix=file%bloe,1,-1
                rzz(ix) = ryy(ix)
                do ia=1,size(ocraa%lag)
                   iy=ix + ocraa%lag(ia)
                   rzz(ix) = rzz(ix) - ocraa%flt(ia) * rzz(iy)
                end do
                rxx(ix) = rxx(ix) + rzz(ix)
             end do

             !! write out both blocks - overwrite the previous upper write 
             stat=sseek_block(x_,i-1,file%blob,0)
             call srite(x_,rxx,2*file%blob)

             !! shift the upper to the lower blocks
             rxx(1+file%bloe:2*file%bloe) = rxx(1:file%bloe)
             ryy(1+file%bloe:2*file%bloe) = ryy(1:file%bloe)
             rzz(1+file%bloe:2*file%bloe) = rzz(1:file%bloe)
          end do
       else         ! forward operator
          !! read first block
          stat=sseek_block(yy_,0,file%blob,0)
          call sreed(yy_,rtt,file%blob)
          ryy(1:file%bloe)=rtt

          stat=sseek_block(x_, 0,file%blob,0)
          call sreed(x_, rtt,file%blob)
          rxx(1:file%bloe)=rtt

          !! polydiv on the first block
          do iy=1, file%bloe
             rzz(iy) = rxx(iy)
             do ia=1,size(ocraa%lag)
                ix=iy - ocraa%lag(ia)
                if(ix<1) cycle
                rzz(iy) = rzz(iy) - ocraa%flt(ia) * rzz(ix)
             end do
             ryy(iy) = ryy(iy) + rzz(iy)
          end do

          !! output first block in case it is the only one
          stat=sseek_block(yy_,0,file%blob,0)
          call srite(yy_,ryy(1:file%bloe),file%blob)

          !! loop over the other blocks
          do i=2,file%blon
             !! read the upper block; put it above the lower one
             stat=sseek_block(yy_,i-1,file%blob,0)
             call sreed(yy_,rtt,file%blob)
             ryy(1+file%bloe:2*file%bloe)=rtt

             stat=sseek_block(x_, i-1,file%blob,0)
             call sreed(x_, rtt,file%blob)
             rxx(1+file%bloe:2*file%bloe)=rtt

             !! polydiv on the upper block
             do iy=1+file%bloe, 2*file%bloe
                rzz(iy) = rxx(iy)
                do ia=1,size(ocraa%lag)
                   ix=iy - ocraa%lag(ia)
                   rzz(iy) = rzz(iy) - ocraa%flt(ia) * rzz(ix)
                end do
                ryy(iy) = ryy(iy) + rzz(iy)
             end do

             !! write out both blocks - overwrite the previous lower write 
             stat=sseek_block(yy_,i-2,file%blob,0)
             call srite(yy_,ryy,2*file%blob)

             !! shift the upper to the lower blocks
             rxx(1:file%bloe) = rxx(1+file%bloe:2*file%bloe)
             ryy(1:file%bloe) = ryy(1+file%bloe:2*file%bloe)
             rzz(1:file%bloe) = rzz(1+file%bloe:2*file%bloe)
          end do

       end if
       deallocate(rxx,ryy,rzz,rtt)
    else                     !! COMPLEX
       if(maxval(occaa%lag)>file%bloe) call seperr("maxlag > block elements")

       allocate(cxx(2*file%bloe),cyy(2*file%bloe),czz(2*file%bloe),ctt(file%bloe))
       czz=(0.,0.)

       if(adj) then ! adjoint operator
          !! read first block
          stat=sseek_block(yy_,file%blon-1,file%blob,0)
          call sreed(yy_,ctt,file%blob)
          cyy(1+file%bloe:2*file%bloe)=ctt

          stat=sseek_block(x_, file%blon-1,file%blob,0)
          call sreed(x_, ctt,file%blob)
          cxx(1+file%bloe:2*file%bloe)=ctt

          !! polydiv on the first block
          do ix=2*file%bloe,1+file%bloe,-1
             czz(ix) = cyy(ix)
             do ia=1,size(occaa%lag)
                iy=ix + occaa%lag(ia)
                if(iy>2*file%bloe) cycle
                if(cflag) then
                   czz(ix) = czz(ix) - conjg(occaa%flt(ia)) * czz(iy)
                else
                   czz(ix) = czz(ix) -       ocraa%flt(ia)  * czz(iy)
                end if
             end do
             cxx(ix) = cxx(ix) + czz(ix)
          end do

          !! output first block in case it is the only one
          stat=sseek_block(x_, file%blon-1,file%blob,0)
          call srite(x_, cxx(1+file%bloe:2*file%bloe),file%blob)

          !! loop over the other blocks
          do i=file%blon-1,1,-1
             !! read the lower block; put it below the lower one
             stat=sseek_block(yy_,i-1,file%blob,0)
             call sreed(yy_,ctt,file%blob)
             cyy(1:file%bloe)=ctt

             stat=sseek_block(x_, i-1,file%blob,0)
             call sreed(x_, ctt,file%blob)
             cxx(1:file%bloe)=ctt

             !! polydiv on the lower block
             do ix=file%bloe,1,-1
                czz(ix) = cyy(ix)
                do ia=1,size(occaa%lag)
                   iy=ix + occaa%lag(ia)
                   if(cflag) then
                      czz(ix) = czz(ix) - conjg(occaa%flt(ia)) * czz(iy)
                   else
                      czz(ix) = czz(ix) -       ocraa%flt(ia)  * czz(iy)
                   end if
                end do
                cxx(ix) = cxx(ix) + czz(ix)
             end do

             !! write out both blocks - overwrite the previous upper write 
             stat=sseek_block(x_, i-1,file%blob,0)
             call srite(x_, cxx,2*file%blob)

             !! shift the upper to the lower blocks
             cxx(1+file%bloe:2*file%bloe) = cxx(1:file%bloe)
             cyy(1+file%bloe:2*file%bloe) = cyy(1:file%bloe)
             czz(1+file%bloe:2*file%bloe) = czz(1:file%bloe)
          end do
       else         ! forward operator
          !! read first block
          stat=sseek_block(yy_,0,file%blob,0)
          call sreed(yy_,ctt,file%blob)
          cyy(1:file%bloe)=ctt

          stat=sseek_block(x_, 0,file%blob,0)
          call sreed(x_, ctt,file%blob)
          cxx(1:file%bloe)=ctt

          !! polydiv on the first block
          do iy=1, file%bloe
             czz(iy) = cxx(iy)
             do ia=1,size(occaa%lag)
                ix=iy - occaa%lag(ia)
                if(ix<1) cycle
                if(cflag) then
                   czz(iy) = czz(iy) - occaa%flt(ia) * czz(ix)
                else
                   czz(iy) = czz(iy) - ocraa%flt(ia) * czz(ix)
                end if
             end do
             cyy(iy) = cyy(iy) + czz(iy)
          end do

          !! output first block in case it is the only one
          stat=sseek_block(yy_,0,file%blob,0)
          call srite(yy_,cyy(1:file%bloe),file%blob)

          !! loop over the other blocks
          do i=2,file%blon
             !! read the upper block; put it above the lower one
             stat=sseek_block(yy_,i-1,file%blob,0)
             call sreed(yy_,ctt,file%blob)
             cyy(1+file%bloe:2*file%bloe)=ctt

             stat=sseek_block(x_, i-1,file%blob,0)
             call sreed(x_, ctt,file%blob)
             cxx(1+file%bloe:2*file%bloe)=ctt

             !! polydiv on the upper block
             do iy=1+file%bloe, 2*file%bloe
                czz(iy) = cxx(iy)
                do ia=1,size(occaa%lag)
                   ix=iy - occaa%lag(ia)
                   if(cflag) then
                      czz(iy) = czz(iy) - occaa%flt(ia) * czz(ix)
                   else
                      czz(iy) = czz(iy) - ocraa%flt(ia) * czz(ix)
                   end if
                end do
                cyy(iy) = cyy(iy) + czz(iy)
             end do

             !! write out both blocks - overwrite the previous lower write 
             stat=sseek_block(yy_,i-2,file%blob,0)
             call srite(yy_,cyy,2*file%blob)

             !! shift the upper to the lower blocks
             cxx(1:file%bloe) = cxx(1+file%bloe:2*file%bloe)
             cyy(1:file%bloe) = cyy(1+file%bloe:2*file%bloe)
             czz(1:file%bloe) = czz(1+file%bloe:2*file%bloe)
          end do
       end if
       deallocate(cxx,cyy,czz,ctt)
    end if
    call sseek(x_, 0,0)
    call sseek(yy_,0,0)
    stat = 0
  end function oc_polydiv
  !----------------------------------------------------------------
end module oc_polydiv_mod

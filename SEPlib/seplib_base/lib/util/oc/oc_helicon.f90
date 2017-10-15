module oc_helicon_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================
  use oc_global_mod
  use oc_filter_mod
  use oc_adjnull_mod
  use oc_scale_mod
  implicit none

  type(rfilter), private :: ocraa
  type(cfilter), private :: occaa
  logical,       private :: cflag
  integer,       private :: ocmaxmem

  interface oc_helicon_init
     module procedure R_helicon_init
     module procedure C_helicon_init
  end interface

  interface helicon
     module procedure  R_helicon
     module procedure  C_helicon
  end interface

contains
  !----------------------------------------------------------------
  subroutine R_helicon_init(raa, maxmem)
    type(rfilter), intent(in) :: raa 
    integer,       optional   :: maxmem

    ocraa = raa
    ocmaxmem=0; if(present(maxmem)) ocmaxmem=maxmem
    cflag = F

    call oc_allocatehelix(occaa,size(ocraa%lag))
    occaa%lag = ocraa%lag
    occaa%flt = ocraa%flt

  end subroutine R_helicon_init
  !----------------------------
  subroutine C_helicon_init(caa, maxmem)
    type(cfilter), intent(in) :: caa 
    integer,       optional   :: maxmem

    occaa = caa
    ocmaxmem=0; if(present(maxmem)) ocmaxmem=maxmem
    cflag = T

    call oc_allocatehelix(ocraa,size(occaa%lag))
    ocraa%lag =      occaa%lag
    ocraa%flt = real(occaa%flt)

  end subroutine C_helicon_init
  !----------------------------------------------------------------

  !----------------------------------------------------------------
  integer function R_helicon(adj,add, x,yy) result(stat)
    logical,            intent(in)    :: adj,add 
    real, dimension(:), intent(inout) :: x,yy 
    integer                           :: ix,iy,ia

    call adjnull(adj,add,x,yy)

    if(adj) then
       x  = x + yy
    else
       yy = yy + x
    end if

    do ia =1,size(ocraa%lag)
       do iy = 1  + ocraa%lag(ia), size(yy)
          ix = iy - ocraa%lag(ia)
          if(adj) then
             x(ix)  = x(ix) + yy(iy) * ocraa%flt(ia)
          else
             yy(iy) = yy(iy) + x(ix) * ocraa%flt(ia)
          end if
       end do
    end do

    stat=0
  end function R_helicon
  !----------------------------
  integer function C_helicon(adj,add, x,yy) result(stat)
    logical,               intent(in)    :: adj,add 
    complex, dimension(:), intent(inout) :: x,yy 
    integer                              :: ix,iy,ia

    call adjnull(adj,add,x,yy)

    if(adj) then
       x  = x + yy
    else
       yy = yy + x
    end if

    do ia =1,size(occaa%lag)
       do iy = 1  + occaa%lag(ia), size(yy)
          ix = iy - occaa%lag(ia)
          if(cflag) then
             if(adj) then
                x(ix)  = x(ix) + yy(iy) * conjg(occaa%flt(ia))
             else
                yy(iy) = yy(iy) + x(ix) *       occaa%flt(ia)
             end if
          else
             if(adj) then
                x(ix)  = x(ix) + yy(iy) *       ocraa%flt(ia)
             else
                yy(iy) = yy(iy) + x(ix) *       ocraa%flt(ia)
             end if
          end if
       end do
    end do

    stat=0
  end function C_helicon
  !----------------------------------------------------------------
  integer function oc_helicon(adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(stat)
    logical,            intent(in)     :: adj,add 
    character(len=128), intent(in)     :: x_,yy_
    integer, external,  optional       :: op1,op2,op3,op4,op5,op6,op7,op8,op9
    type(fileinfo)                     :: filex, filey, file
    real,    dimension(:), allocatable :: rxx, ryy, rtt
    complex, dimension(:), allocatable :: cxx, cyy, ctt
    integer                            :: ix,iy,ia,i

    call oc_adjnull(adj,add,x_,yy_)

    call oc_allocatefile(filex, x_,  ocmaxmem); call sseek(x_, 0,0)
    call oc_allocatefile(filey, yy_, ocmaxmem); call sseek(yy_,0,0)
    call oc_checksimilarity(filex, filey, "oc_helicon")
    call oc_deallocatefile(filex); call sseek(x_, 0,0)
    call oc_deallocatefile(filey); call sseek(yy_,0,0)
    call oc_allocatefile(file, x_, ocmaxmem)

    if (file%esize /=8) then !! REAL
       if(maxval(ocraa%lag)>file%bloe) call seperr("maxlag > block elements")

       call oc_scale_init(1.0,ocmaxmem) !! zero lag
       stat = oc_scale(adj,add,x_,yy_)

       allocate(rxx(2*file%bloe),ryy(2*file%bloe),rtt(file%bloe))

       !! first block
       call sreed(yy_,rtt,file%blob); ryy(1:file%bloe)=rtt
       call sreed(x_, rtt,file%blob); rxx(1:file%bloe)=rtt

       do ia = 1, size(ocraa%lag)
          do iy = 1  + ocraa%lag(ia), file%bloe
             ix = iy - ocraa%lag(ia)
             if(adj) then
                rxx(ix) = rxx(ix) + ryy(iy) * ocraa%flt(ia)
             else
                ryy(iy) = ryy(iy) + rxx(ix) * ocraa%flt(ia)
             end if
          end do
       end do

       if(file%blon==1) then !! write if only one block
          if(adj) then
             stat=sseek_block(x_,0,file%blob,0)
             call srite(x_, rxx(1:file%bloe),file%blob)
          else
             stat=sseek_block(yy_,0,file%blob,0)
             call srite(yy_,ryy(1:file%bloe),file%blob)
          end if
       end if

       do i=2,file%blon !! continue with the other blocks
          call sreed(yy_,rtt,file%blob); ryy(1+file%bloe:2*file%bloe)=rtt
          call sreed(x_, rtt,file%blob); rxx(1+file%bloe:2*file%bloe)=rtt

          do ia = 1, size(ocraa%lag)
             do iy = 1+file%bloe, 2*file%bloe
                ix = iy - ocraa%lag(ia)
                if(adj) then
                   rxx(ix) = rxx(ix) + ryy(iy) * ocraa%flt(ia)
                else
                   ryy(iy) = ryy(iy) + rxx(ix) * ocraa%flt(ia)
                end if
             end do
          end do

          if(adj) then
             stat=sseek_block(x_,i-2,file%blob,0)
             call srite(x_, rxx,2*file%blob)
          else
             stat=sseek_block(yy_,i-2,file%blob,0)
             call srite(yy_,ryy,2*file%blob)
          end if

          rxx(1:file%bloe) = rxx(1+file%bloe:2*file%bloe)
          ryy(1:file%bloe) = ryy(1+file%bloe:2*file%bloe)
       end do
       deallocate(rxx,ryy,rtt)
    else                     !! COMPLEX
       if(maxval(occaa%lag)>file%bloe) call seperr("maxlag > block elements")

       call oc_scale_init(1.0,ocmaxmem) !! zero lag
       stat = oc_scale(adj,add,x_,yy_)

       allocate(cxx(2*file%bloe),cyy(2*file%bloe),ctt(file%bloe))

       !! first block
       call sreed(yy_,ctt,file%blob); cyy(1:file%bloe)=ctt
       call sreed(x_, ctt,file%blob); cxx(1:file%bloe)=ctt

       do ia = 1, size(occaa%lag)
          do iy = 1  + occaa%lag(ia), file%bloe
             ix = iy - occaa%lag(ia)
             if(cflag) then
                if(adj) then
                   cxx(ix) = cxx(ix) + cyy(iy) * conjg(occaa%flt(ia))
                else
                   cyy(iy) = cyy(iy) + cxx(ix) *       occaa%flt(ia)
                end if
             else
                if(adj) then
                   cxx(ix) = cxx(ix) + cyy(iy) *       ocraa%flt(ia)
                else
                   cyy(iy) = cyy(iy) + cxx(ix) *       ocraa%flt(ia)
                end if
             end if
          end do
       end do

       if(file%blon==1) then !! write if only one block
          if(adj) then
             stat=sseek_block(x_,0,file%blob,0)
             call srite(x_, cxx(1:file%bloe),file%blob)
          else
             stat=sseek_block(yy_,0,file%blob,0)
             call srite(yy_,cyy(1:file%bloe),file%blob)
          end if
       end if

       do i=2,file%blon !! continue with the other blocks
          call sreed(yy_,ctt,file%blob); cyy(1+file%bloe:2*file%bloe)=ctt
          call sreed(x_, ctt,file%blob); cxx(1+file%bloe:2*file%bloe)=ctt

          do ia = 1, size(occaa%lag)
             do iy = 1+file%bloe, 2*file%bloe
                ix = iy - occaa%lag(ia)
                if(cflag) then
                   if(adj) then
                      cxx(ix) = cxx(ix) + cyy(iy) * conjg(occaa%flt(ia))
                   else
                      cyy(iy) = cyy(iy) + cxx(ix) *       occaa%flt(ia)
                   end if
                else
                   if(adj) then
                      cxx(ix) = cxx(ix) + cyy(iy) *       ocraa%flt(ia)
                   else
                      cyy(iy) = cyy(iy) + cxx(ix) *       ocraa%flt(ia)
                   end if
                end if
             end do
          end do

          if(adj) then
             stat=sseek_block(x_,i-2,file%blob,0)
             call srite(x_, cxx,2*file%blob)
          else
             stat=sseek_block(yy_,i-2,file%blob,0)
             call srite(yy_,cyy,2*file%blob)
          end if

          cxx(1:file%bloe) = cxx(1+file%bloe:2*file%bloe)
          cyy(1:file%bloe) = cyy(1+file%bloe:2*file%bloe)
       end do
       deallocate(cxx,cyy,ctt)
    end if
    call sseek(x_, 0,0)
    call sseek(yy_,0,0)
    stat = 0
  end function oc_helicon
  !----------------------------------------------------------------
end module oc_helicon_mod

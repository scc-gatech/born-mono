module oc_weight_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================
  use sep
  use oc_file_mod
  use oc_adjnull_mod
  implicit none

  character(len=128), private :: w_
  integer,            private :: maxmem

contains

  !----------------------------------------------------------------
  subroutine oc_weight_init( w__in, maxmem_in)
    character(len=128), intent(in) :: w__in
    integer,            optional   :: maxmem_in
    w_ = w__in
    maxmem=0; if(present(maxmem_in)) maxmem=maxmem_in
  end subroutine oc_weight_init
  !----------------------------------------------------------------
  integer function oc_weight(adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(stat)
    logical,               intent(in)  :: adj,add
    character(len=128),    intent(in)  :: x_,yy_
    integer, external,     optional    :: op1,op2,op3,op4,op5,op6,op7,op8,op9
    type(fileinfo)                     :: filew,filex,filey
    integer                            :: j
    real,    dimension(:), allocatable :: rblovx, rblovy, rblovw
    complex, dimension(:), allocatable :: cblovx, cblovy, cblovw

    call oc_adjnull(adj,add,x_,yy_)

    call oc_allocatefile(filew, w_,  maxmem); call sseek(w_, 0,0)
    call oc_allocatefile(filex, x_,  maxmem); call sseek(x_, 0,0)
    call oc_allocatefile(filey, yy_, maxmem); call sseek(yy_,0,0)
    call oc_checksimilarity(filex,filey, "oc_weight")
    call oc_checkspace(filew,filex,"oc_weight")

    adjoint: if(adj) then  !! x=w y
       if(filex%esize /=8) then !! REAL
          allocate(rblovy(filex%bloe),rblovx(filex%bloe))
          do j=1,filex%blon
             call sreed(yy_, rblovy, filey%blob)

             rblovx = rZERO
             if(add) then
                if(x_.eq.yy_) stat=sseek_block(x_,j-1,filex%blob,0)
                call sreed(x_, rblovx, filey%blob)
                stat=sseek_block(x_,j-1,filey%blob,0)
             end if

             allocate(rblovw(filew%bloe))
             call sreed(w_, rblovw, filew%blob)                
             rblovx = rblovx + rblovy * rblovw                    !! R = R R 
             deallocate(rblovw)

             if(x_.eq.yy_) stat=sseek_block(x_,j-1,filex%blob,0)
             call sep_write(rblovx, x_)
          end do
          deallocate(rblovy,rblovx)
       else                     !! COMPLEX
          allocate(cblovy(filex%bloe),cblovx(filex%bloe))
          do j=1,filex%blon
             call sreed(yy_, cblovy, filey%blob)

             cblovx = cZERO
             if(add) then
                if(x_.eq.yy_) stat=sseek_block(x_,j-1,filex%blob,0)
                call sreed(x_, cblovx, filex%blob)
                stat=sseek_block(x_,j-1,filey%blob,0)
             end if

             if(filew%esize /=8) then
                allocate(rblovw(filew%bloe))
                call sreed(w_, rblovw, filew%blob)                
                cblovx = cblovx + cblovy * cmplx(rblovw)          !! C = C R
                deallocate(rblovw)
             else
                allocate(cblovw(filew%bloe))
                call sreed(w_, cblovw, filew%blob)                
                cblovx = cblovx + cblovy * conjg(cblovw)          !! C = C C
                deallocate(cblovw)
             end if

             if(x_.eq.yy_) stat=sseek_block(x_,j-1,filex%blob,0)
             call sep_write(cblovx, x_)
          end do
          deallocate(cblovy,cblovx)
       end if
    else                   !! y=w x
       if(filey%esize /=8) then !! REAL
          allocate(rblovx(filex%bloe),rblovy(filex%bloe))
          do j=1,filey%blon
             call sreed(x_, rblovx, filex%blob)

             rblovy = rZERO
             if(add) then
                if(yy_.eq.x_) stat=sseek_block(yy_,j-1,filey%blob,0)
                call sreed(yy_, rblovy, filey%blob)
                stat=sseek_block(yy_,j-1,filey%blob,0)
             end if

             allocate(rblovw(filew%bloe))
             call sreed(w_, rblovw, filew%blob)                
             rblovy = rblovy + rblovx * rblovw                    !! R = R R 
             deallocate(rblovw)

             if(yy_.eq.x_) stat=sseek_block(yy_,j-1,filey%blob,0)
             call sep_write(rblovy, yy_)
          end do
          deallocate(rblovx,rblovy)
       else                     !! COMPLEX
          allocate(cblovx(filex%bloe),cblovy(filex%bloe))
          do j=1,filey%blon
             call sreed(x_, cblovx, filex%blob)

             cblovy = cZERO
             if(add) then
                if(yy_.eq.x_) stat=sseek_block(yy_,j-1,filey%blob,0)
                call sreed(yy_, cblovy, filey%blob)
                stat=sseek_block(yy_,j-1,filey%blob,0)
             end if

             if(filew%esize /=8) then
                allocate(rblovw(filew%bloe))
                call sreed(w_, rblovw, filew%blob)                
                cblovy = cblovy + cblovx * cmplx(rblovw)          !! C = C R
                deallocate(rblovw)
             else
                allocate(cblovw(filew%bloe))
                call sreed(w_, cblovw, filew%blob)                
                cblovy = cblovy + cblovx *       cblovw           !! C = C C
                deallocate(cblovw)
             end if

             if(yy_.eq.x_) stat=sseek_block(yy_,j-1,filey%blob,0)
             call sep_write(cblovy, yy_)
          end do
          deallocate(cblovx,cblovy)
       end if
    end if adjoint

    call oc_deallocatefile(filey); call sseek(w_, 0,0)
    call oc_deallocatefile(filex); call sseek(x_, 0,0)
    call oc_deallocatefile(filew); call sseek(yy_,0,0)
    stat = 0
  end function oc_weight

end module oc_weight_mod

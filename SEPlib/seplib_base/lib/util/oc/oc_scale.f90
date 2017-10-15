module oc_scale_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================
  use sep
  use oc_global_mod
  use oc_file_mod
  use oc_adjnull_mod
  implicit none

  integer, private                     :: maxmem

  real,                        private :: R_eps
  complex,                     private :: C_eps
  logical,                     private :: cflag   !! flag for complex 

  interface oc_scale_init
     module procedure R_scale_init
     module procedure C_scale_init
  end interface

contains
  !----------------------------------------------------------------
  subroutine R_scale_init(R_eps_in,maxmem_in)
    real,    intent(in) :: R_eps_in
    integer, optional   :: maxmem_in
    R_eps=R_eps_in
    maxmem=0; if(present(maxmem_in)) maxmem=maxmem_in
    cflag=F
  end subroutine R_scale_init
  !----------------------------------------------------------------
  subroutine C_scale_init(C_eps_in,maxmem_in)
    complex, intent(in) :: C_eps_in
    integer, optional   :: maxmem_in
    C_eps=     C_eps_in
    R_eps=real(C_eps)
    maxmem=0; if(present(maxmem_in)) maxmem=maxmem_in
    cflag=T
  end subroutine C_scale_init
  !----------------------------------------------------------------
  integer function oc_scale(adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(stat)
    logical,                intent(in)  :: adj,add
    character(len=128)       :: x_,yy_
    integer, external,      optional    :: op1,op2,op3,op4,op5,op6,op7,op8,op9
    type(fileinfo)                      :: filex, filey
    real,    dimension (:), allocatable :: rx, ry
    complex, dimension (:), allocatable :: cx, cy
    integer                             :: j

    call oc_adjnull(adj,add,x_,yy_)

    call oc_allocatefile(filex, x_,  maxmem); call sseek(x_, 0,0)
    call oc_allocatefile(filey, yy_, maxmem); call sseek(yy_,0,0)
    call oc_checksimilarity(filex,filey, "oc_scale")

    if (filex%esize /=8) then !! REAL
       allocate(rx(filex%bloe),ry(filey%bloe))
       do j=1,filex%blon
          if (adj) then !! x=y
             call sreed(yy_, ry, filey%blob)
             call sreed(x_,  rx, filex%blob)
             stat=sseek_block(x_, j-1,filex%blob,0)
             rx = rx + R_eps*ry
             call sep_write(rx, x_ )
          else          !! y=x
             call sreed(x_,  rx, filex%blob)
             call sreed(yy_, ry, filey%blob)
             stat=sseek_block(yy_,j-1,filey%blob,0)
             ry = ry + R_eps*rx
             call sep_write(ry, yy_)
          end if
       end do
       deallocate(rx,ry)
    else                      !! COMPLEX
       allocate(cx(filex%bloe),cy(filey%bloe))
       do j=1,filex%blon
          if (adj) then !! x=y
             call sreed(yy_, cy, filey%blob)
             call sreed(x_,  cx, filex%blob)
             stat=sseek_block(x_, j-1,filex%blob,0)
             if(cflag) then
                cx = cx + conjg(C_eps)*cy
             else
                cx = cx +       R_eps *cy
             end if
             call sep_write(cx, x_ )
          else          !! y=x
             call sreed(x_,  cx, filex%blob)
             call sreed(yy_, cy, filey%blob)
             stat=sseek_block(yy_,j-1,filey%blob,0)
             if(cflag) then
                cy = cy +       C_eps *cx
             else
                cy = cy +       R_eps *cx
             end if
             call sep_write(cy, yy_)
          end if
       end do
       deallocate(cx,cy)
    end if

    call sseek(x_, 0,0)
    call sseek(yy_,0,0)
    stat = 0
  end function oc_scale
  !----------------------------------------------------------------
end module oc_scale_mod

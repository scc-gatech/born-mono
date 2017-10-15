module oc_matmult_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================
  use sep
  use oc_global_mod
  use oc_file_mod
  implicit none

  complex, dimension(:,:), pointer :: cbb
  real,    dimension(:,:), pointer :: rbb
contains
  !----------------------------------------------------------------
  subroutine oc_matmult_init( bb_)
    character(len=128)           :: bb_
    integer                      :: n1,n2
    integer                      :: esize

    call from_aux(trim(bb_),"n1",n1)
    call from_aux(trim(bb_),"n2",n2) 
    call from_aux(trim(bb_),'esize',esize)

    if (esize /=8) then !! REAL 
       allocate(rbb(n1,n2))
       call sreed(bb_, rbb, n1*n2*esize)
    else
       allocate(cbb(n1,n2))
       call sreed(bb_, cbb, n1*n2*esize)
    end if

  end subroutine oc_matmult_init
  !----------------------------------------------------------------
  integer function oc_matmult( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) &
  result(stat)
    integer, external, optional         :: op1,op2,op3,op4,op5,op6,op7,op8,op9
    logical, intent (in)                :: adj, add
    character(len=128)                  :: x_,yy_
    integer                             :: n1x,nx, n1y,ny, ix,iy
    integer                             :: esize
    integer                             :: nd,id
    character (len=10)                  :: buffer

    complex, dimension (:), allocatable :: cx,cy
    real,    dimension (:), allocatable :: rx,ry

    nd = sep_dimension (x_)

    call sseek(x_, 0,0)
    call sseek(yy_,0,0)

    call from_aux(trim(x_),"esize",esize)

    nx=1
    do id=1,nd
       write (buffer, "(a,i1)" ) 'n',id
       call from_aux(trim(x_ ),buffer,n1x)
       nx=nx*n1x
    end do

    ny=1
    do id=1,nd
       write (buffer, "(a,i1)" ) 'n',id
       call from_aux(trim(yy_),buffer,n1y)
       ny=ny*n1y
    end do

    if (esize /=8) then !! REAL
       allocate(rx(nx))
       allocate(ry(ny)) 
       if (adj) then
          rx=real(rZERO); if (add) call sreed(x_, rx,nx*esize)
          call sreed(yy_,ry,ny*esize)
       else
          call sreed(x_, rx,nx*esize)
          ry=real(rZERO); if (add) call sreed(yy_,ry,ny*esize)
       end if

       do ix= 1, nx
          do iy= 1, ny
             if( adj) then
                rx(ix) = rx(ix) + rbb(iy,ix) * ry(iy)
             else
                ry(iy) = ry(iy) + rbb(iy,ix) * rx(ix)
             end if
          end do
       end do

       call sseek(x_, 0,0)
       call sseek(yy_,0,0)
       if(adj) then
          call sep_write(rx,x_ )
       else
          call sep_write(ry,yy_)
       end if

       deallocate(rx)
       deallocate(ry)
    else !!COMPLEX
       allocate(cx(nx))
       allocate(cy(ny)) 
       if (adj) then
          cx=cmplx(cZERO); if (add) call sreed(x_, cx,nx*esize)
          call sreed(yy_,cy,ny*esize)
       else
          call sreed(x_, cx,nx*esize)
          cy=cmplx(cZERO); if (add) call sreed(yy_,cy,ny*esize)
       end if

       do ix= 1, nx
          do iy= 1, ny
             if( adj) then
                cx(ix) = cx(ix) + conjg(cbb(iy,ix)) * cy(iy)
             else
                cy(iy) = cy(iy) +       cbb(iy,ix)  * cx(ix)
             end if
          end do
       end do

       call sseek(x_, 0,0)
       call sseek(yy_,0,0)
       if(adj) then
          call sep_write(cx,x_ )
       else
          call sep_write(cy,yy_)
       end if

       deallocate(cx)
       deallocate(cy)
    end if

    call sseek(x_, 0,0)
    call sseek(yy_,0,0)
    stat = 0 
  end function oc_matmult
  !----------------------------------------------------------------
  subroutine oc_matmult_close(bb_)
    character(len=128)     :: bb_
    integer :: esize

    call from_aux(trim(bb_),'esize',esize)

    if (esize /=8) then !! REAL 
       deallocate(rbb)
    else
       deallocate(cbb)
    end if
  end subroutine oc_matmult_close
  !----------------------------------------------------------------
end module oc_matmult_mod

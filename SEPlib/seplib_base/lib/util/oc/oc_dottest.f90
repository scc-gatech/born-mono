!!$=head1 NAME
!!$
!!$oc_dottest_init - initialize out of core dottest
!!$
!!$
!!$=head1 SYNOPSIS
!!$
!!$ call oc_dottest_init(no_add,adj_first,maxmem)
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$
!!$=item no_add - logical, optional
!!$
!!$ Whether or not to do add test 
!!$
!!$=item adj_first - logical, optional
!!$
!!$ Whether or not to  perform adjoint first 
!!$
!!$=item maxmem - integer, optional
!!$
!!$Maximum memory to use for blocking operation
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$ Initialize out of core dottest
!!$
!!$=head1 SEE ALSO
!!$
!!$L<oc_dottest>
!!$
!!$=head1 LIBRARY
!!$
!!$
!!$sepocf90
!!$
!!$=cut
!!$
!!$>



!!$=head1 NAME
!!$
!!$oc_dottest -  Out of core dottest
!!$
!!$
!!$=head1 SYNOPSIS
!!$
!!$call oc_dottest (oper,x,y,op1,op2,op3,op4,op5,op6,op7,op8,op9)
!!$
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$
!!$
!!$=item op1,op2...,op9  integer, optional - optional operators
!!$
!!$=item oper - Standard oc operator
!!$
!!$=item x,y - tags (model and data)
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$ Perform out of core dottest
!!$
!!$=head1 SEE ALSO
!!$
!!$L<oc_dottest_init>
!!$
!!$=head1 LIBRARY
!!$
!!$
!!$sepocf90
!!$
!!$=cut
!!$
!!$

module oc_dottest_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================
  use oc_global_mod
  use oc_file_mod
  use oc_filealgebra_mod

  implicit none

  logical, private :: ocno_add, ocadj_first
  integer, private :: ocmaxmem
contains
  !----------------------------------------------------------------  
  subroutine oc_dottest_init(no_add,adj_first,maxmem)
    logical, optional :: no_add, adj_first
    integer, optional :: maxmem

    ocno_add=F;   if(present(no_add))       ocno_add=no_add
    ocadj_first=F;if(present(adj_first)) ocadj_first=adj_first
    ocmaxmem = 0; if(present(maxmem))       ocmaxmem=maxmem
  end subroutine oc_dottest_init
  !----------------------------------------------------------------
  subroutine oc_dottest(oper, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9)
    character(len=128), intent(in) :: x_, yy_
    integer, external,  optional   :: op1,op2,op3,op4,op5,op6,op7,op8,op9
    integer                        :: stat
    integer                        :: xesize, yesize
    character(len=128)             :: x0_, y0_
    character(len=128)             :: x1_, y1_
    character(len=128)             :: name
    complex(kind=db)               :: cdpx, cdpyy
    real(kind=db)                  :: rdpx, rdpyy
    logical,dimension(2)           :: opor
    logical                        :: add, loop, do_add

    interface !!--------------------------------------------------------
       integer function oper(adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional    :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical,            intent(in) :: adj, add
         character(len=128), intent(in) :: x_, yy_
       end function oper
    end interface!!-----------------------------------------------------

    opor = (/ F,T /)
    if(ocadj_first) opor =.not. opor

    do_add=T
    if(ocno_add) do_add=.not. ocno_add

    loop = T
    add  = F

    name='_x0'; x0_ = oc_clone(T, x_ ,name, ocmaxmem)
    name='_x1'; x1_ = oc_clone(T, x_ ,name, ocmaxmem)

    name='_y0'; y0_ = oc_clone(T, yy_,name, ocmaxmem)
    name='_y1'; y1_ = oc_clone(T, yy_,name, ocmaxmem)

    call from_aux( x0_, "esize", xesize)
    call from_aux( y0_, "esize", yesize)
    if(xesize /= yesize) call seperr('cant run DP test from real to complex')

    if(opor(1)) then
       call oc_random(y0_, 1.0, ocmaxmem)
       call oc_random(x1_, 1.0, ocmaxmem)
    else
       call oc_random(x0_, 1.0, ocmaxmem)
       call oc_random(y1_, 1.0, ocmaxmem)
    end if

    do while (loop)
       stat = oper( opor(1), add, x0_, y0_, op1,op2,op3,op4,op5,op6,op7,op8,op9)
       stat = oper( opor(2), add, x1_, y1_, op1,op2,op3,op4,op5,op6,op7,op8,op9)

       if (xesize /=8) then !! REAL
          rdpx = oc_rdp((/x1_/) , (/x0_/), ocmaxmem) 
          rdpyy= oc_rdp((/y1_/),  (/y0_/), ocmaxmem)

          write(0,*) '---------------------------------------------------'
          write(0,'( a6," ",f20.6)')" x0.x1",    rdpx
          write(0,'( a6," ",f20.6)')" y0.y1",    rdpyy
          write(0,'( a5,l2, f20.6)')" add=",add, rdpx-rdpyy
          write(0,*) '---------------------------------------------------'
       else                !! COMPLEX          
          cdpx = oc_cdp((/x1_/), (/x0_/), ocmaxmem) 
          cdpyy= oc_cdp((/y1_/), (/y0_/), ocmaxmem)

          write(0,*) '---------------------------------------------------'
          write(0,'( a6," ",f20.6,sp,f20.6,"(i)")')" x0.x1",    real(cdpx ),aimag(cdpx )
          write(0,'( a6," ",f20.6,sp,f20.6,"(i)")')" y0.y1",    real(cdpyy),aimag(cdpyy)
          write(0,'( a5,l2, f20.6,sp,f20.6,"(i)")')" add=",add,real(cdpx)-real(cdpyy),aimag(cdpx)-aimag(cdpyy)
          write(0,*) '---------------------------------------------------'
       end if

       if ((.not. add) .and. do_add) then
          add=T
       else
          loop=F
       end if
    end do

  end subroutine oc_dottest
  !----------------------------------------------------------------
end module oc_dottest_mod

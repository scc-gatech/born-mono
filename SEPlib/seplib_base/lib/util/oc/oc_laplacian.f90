module oc_laplacian_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================
  use oc_global_mod
  use oc_file_mod
  use oc_filter_mod
  use oc_wilson_mod
  implicit none

  type(cfilter),    private :: aa
  integer,          private :: ocmaxmem

contains
  !----------------------------------------------------------------
  subroutine oc_laplacian_init(t_,nf,niter,eps,maxmem)
    character(len=128), intent(in) :: t_
    real,               optional   :: eps
    integer,            optional   :: nf,niter,maxmem
    type(fileinfo)                 :: file

    call oc_allocatefile(file, t_, ocmaxmem); call sseek(t_,0,0)
    ocmaxmem = 0; if(present(maxmem)) ocmaxmem=maxmem
    if(.not.present(eps)) eps=0.0

    call oc_buildfilter(aa,file%n,(/3,3,3/),nf)

    call oc_laplacian_factor(aa,t_,nf,niter,eps,ocmaxmem)
    call oc_deallocatefile(file)
  end subroutine oc_laplacian_init
  !----------------------------------------------------------------
  subroutine oc_laplacian_factor(bb,t_,nf,niter,eps,ocmaxmem)
    type(cfilter),             intent(inout) :: bb
    character(len=128),        intent(in)    :: t_
    real,                      intent(in)    :: eps
    integer,                   optional      :: nf,niter,ocmaxmem
    type(fileinfo)                           :: file
    integer, dimension(:),     allocatable   :: dbox
    real,    dimension(:,:,:), allocatable   :: laplacian
    type(rfilter)                            :: ss

    call oc_allocatefile(file,t_,ocmaxmem)
    allocate(dbox(file%nd))

    select case(file%nd)
    case(1)
       allocate(laplacian(3,1,1))
       laplacian=0.
       laplacian(2,1,1)= 2. + eps
       laplacian(1,1,1)=-1.; laplacian(3,1,1)=-1.;

       dbox=(/ min(64,file%n(1)) ,1 ,1 /)
       call oc_autoco2helix(ss,dbox,laplacian)
       call oc_buildfilter (bb,dbox,(/3,1,1/),nf)
       call oc_wilfac_init(2,niter)
       call oc_wilfac(ss,bb)
       call oc_regrid( dbox,file%n,bb)

       deallocate(laplacian)
    case(2)
       allocate(laplacian(3,3,1))
       laplacian=0.
       laplacian(2,2,1)= 4. + eps
       laplacian(1,2,1)=-1.; laplacian(3,2,1)=-1.
       laplacian(2,1,1)=-1.; laplacian(2,3,1)=-1.

       dbox=(/ min(64,file%n(1)) ,min(64,file%n(2)) ,1 /)
       call oc_autoco2helix(ss,dbox,laplacian)
       call oc_buildfilter (bb,dbox,(/3,3,1/),nf)
       call oc_wilfac_init(2,niter)
       call oc_wilfac(ss,bb)
       call oc_regrid( dbox,file%n,bb)

       deallocate(laplacian)
    case(3)
       allocate(laplacian(3,3,3))
       laplacian=0.
       laplacian(2,2,2)= 6. + eps
       laplacian(1,2,2)=-1.; laplacian(3,2,2)=-1.
       laplacian(2,1,2)=-1.; laplacian(2,3,2)=-1.
       laplacian(2,2,1)=-1.; laplacian(2,2,3)=-1.

       dbox=(/ min(64,file%n(1)) ,min(64,file%n(2)) ,min(8,file%n(3)) /)
       call oc_autoco2helix(ss,dbox,laplacian)
       call oc_buildfilter (bb,dbox,(/3,3,3/),nf)
       call oc_wilfac_init(2,niter)
       call oc_wilfac(ss,bb)
       call oc_regrid( dbox,file%n,bb)

       deallocate(laplacian)
    end select

    deallocate(dbox)
    call oc_deallocatefile(file)
  end subroutine oc_laplacian_factor
  !----------------------------------------------------------------
  integer function oc_laplacian(adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(stat)
    logical,            intent(in) :: adj,add 
    character(len=128), intent(in) :: x_,yy_
    integer, external,  optional   :: op1,op2,op3,op4,op5,op6,op7,op8,op9
    character(len=128)             :: t_,name
    type(fileinfo)                 :: filex,filey

    call oc_adjnull(adj,add,x_,yy_)

    call oc_allocatefile(filex, x_,  ocmaxmem); call sseek(x_, 0,0)
    call oc_allocatefile(filey, yy_, ocmaxmem); call sseek(yy_,0,0)
    call oc_checksimilarity(filex,filey, "oc_laplacian")

    name='_t.laplacian'; t_ = oc_clone(T, x_,name,ocmaxmem)
    call oc_assign(t_,0.,ocmaxmem)
    call oc_helicon_init(aa, ocmaxmem)

    if(adj) then
       stat = oc_helicon(F,  F,yy_,t_)
       stat = oc_helicon(T,add,x_ ,t_)
    else
       stat = oc_helicon(F,  F,x_, t_)
       stat = oc_helicon(T,add,yy_,t_)
    end if

    call oc_deallocatefile(filex); call sseek(x_, 0,0)
    call oc_deallocatefile(filey); call sseek(yy_,0,0)
    stat = 0
  end function oc_laplacian
  !----------------------------------------------------------------
  integer function oc_ilaplacian(adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(stat)
    logical,            intent(in) :: adj,add 
    character(len=128), intent(in) :: x_,yy_
    character(len=128)             :: t_,name
    integer, external,  optional   :: op1,op2,op3,op4,op5,op6,op7,op8,op9
    type(fileinfo)                 :: filex,filey

    call oc_adjnull(adj,add,x_,yy_)

    call oc_allocatefile(filex, x_,  ocmaxmem); call sseek(x_, 0,0)
    call oc_allocatefile(filey, yy_, ocmaxmem); call sseek(yy_,0,0)
    call oc_checksimilarity(filex,filey, "oc_ilaplacian")

    name='_t.laplacian'; t_ = oc_clone(T, x_,name,ocmaxmem)
    call oc_assign(t_, 0.    ,ocmaxmem)

    call oc_polydiv_init(aa, ocmaxmem)
    if(adj) then
       stat = oc_polydiv(T,  F,t_,yy_)
       stat = oc_polydiv(F,add,t_,x_ )
    else
       stat = oc_polydiv(T,  F,t_,x_ )
       stat = oc_polydiv(F,add,t_,yy_)
    end if

    call oc_deallocatefile(filex); call sseek(x_, 0,0)
    call oc_deallocatefile(filey); call sseek(yy_,0,0)
    stat = 0
  end function oc_ilaplacian
  !----------------------------------------------------------------
  integer function oc_hderivative(adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(stat)
    logical,            intent(in) :: adj,add 
    character(len=128), intent(in) :: x_,yy_
    integer, external,  optional   :: op1,op2,op3,op4,op5,op6,op7,op8,op9
    type(fileinfo)                 :: filex,filey

    call oc_adjnull(adj,add,x_,yy_)

    call oc_allocatefile(filex, x_,  ocmaxmem); call sseek(x_, 0,0)
    call oc_allocatefile(filey, yy_, ocmaxmem); call sseek(yy_,0,0)
    call oc_checksimilarity(filex,filey, "oc_hderivative")

    call oc_helicon_init(aa, ocmaxmem)
    if(adj) then
       stat = oc_helicon(T,add,x_,yy_)
    else
       stat = oc_helicon(F,add,x_,yy_)
    end if

    call oc_deallocatefile(filex); call sseek(x_, 0,0)
    call oc_deallocatefile(filey); call sseek(yy_,0,0)
    stat = 0
  end function oc_hderivative
  !----------------------------------------------------------------
  integer function oc_ihderivative(adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(stat)
    logical,            intent(in) :: adj,add 
    character(len=128), intent(in) :: x_,yy_
    integer, external,  optional   :: op1,op2,op3,op4,op5,op6,op7,op8,op9
    type(fileinfo)                 :: filex,filey

    call oc_adjnull(adj,add,x_,yy_)

    call oc_allocatefile(filex, x_,  ocmaxmem); call sseek(x_, 0,0)
    call oc_allocatefile(filey, yy_, ocmaxmem); call sseek(yy_,0,0)
    call oc_checksimilarity(filex,filey, "oc_ihderivative")

    call oc_polydiv_init(aa, ocmaxmem)
    if(adj) then
       stat = oc_polydiv(T,add,x_,yy_)
    else
       stat = oc_polydiv(F,add,x_,yy_)
    end if

    call oc_deallocatefile(filex); call sseek(x_, 0,0)
    call oc_deallocatefile(filey); call sseek(yy_,0,0)
    stat = 0
  end function oc_ihderivative
  !----------------------------------------------------------------
end module oc_laplacian_mod

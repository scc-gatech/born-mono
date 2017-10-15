module oc_solver_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================
  use oc_global_mod
  use oc_tags_mod
  use oc_file_mod
  use oc_filealgebra_mod
  implicit none

  integer,                  private :: ocniter,ocmaxmem
  logical,                  private :: ocverb, ocmmovie, ocdmovie

contains
  !! L = linear         operator
  !! S = solver

  !----------------------------------------------------------------
  subroutine oc_solver_init(niter,maxmem,verb,mmovie,dmovie)
    integer, optional    :: niter,maxmem
    logical, optional    :: verb, mmovie, dmovie
    ocniter=10;     if(present(niter))     ocniter=niter
    ocmaxmem=0;     if(present(maxmem))   ocmaxmem=maxmem
    ocverb=F;       if(present(verb))       ocverb=verb
    ocmmovie=F;     if(present(mmovie))   ocmmovie=mmovie
    ocdmovie=F;     if(present(dmovie))   ocdmovie=dmovie
  end subroutine oc_solver_init
  !----------------------------------------------------------------
  subroutine oc_solver( L,S, x_,yy_, x0_,res_,op1,op2,op3,op4,op5,op6,op7,op8,op9)
    character(len=128), optional  :: x0_,res_
    character(len=128)            ::  x_, yy_
    integer, external,  optional  :: op1,op2,op3,op4,op5,op6,op7,op8,op9

    type(fileinfo)                :: file
    character(len=128)            ::      rd_
    character(len=128)            ::  g_, gd_
    character(len=128)            ::  s_, sd_
    character(len=128)            :: mm_, md_ !! model/data movie
    character(len=128)            ::  name

    integer                       :: i, stat, esize
    logical                       :: forget
    real(kind=db), dimension(1)            :: res
    real, dimension(1)            :: res_ss

    interface
       integer function L( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function L
       integer function S( forget, x_,g_,rr_,gg_,s_,ss_,maxmem) result(st)
         integer, optional                :: maxmem
         logical                          :: forget
         character(len=128)               ::  x_,  g_,  s_
         character(len=128), dimension(:) :: rr_, gg_, ss_
       end function S
    end interface

    call oc_allocatefile(file,x_,ocmaxmem)
    name=SS_Rd;    rd_ = oc_clone(F, yy_,name, ocmaxmem) 

    name=SS_g;      g_ = oc_clone(F,  x_,name, ocmaxmem)
    name=SS_Gd;    gd_ = oc_clone(F, yy_,name, ocmaxmem) 

    name=SS_s;      s_ = oc_clone(F,  x_,name, ocmaxmem) 
    name=SS_Sd;    sd_ = oc_clone(F, yy_,name, ocmaxmem)

    if(ocmmovie) then
       name=SS_Mmovie; mm_ = oc_clone(F,  x_,name, ocmaxmem)
       call oc_adddim(mm_, ocniter)
    end if
    if(ocdmovie) then
       name=SS_Dmovie; md_ = oc_clone(F, yy_,name, ocmaxmem)
       call oc_adddim(md_, ocniter)
    end if

    if( present(res_)) then
       call oc_shapeheader(res_,4,(/ocniter/),(/0./),(/1./))
    end if

    call oc_linear(rd_, (/yy_/), (/-rONE/), ocmaxmem)
    if( present(x0_)) then 
       stat = L( F,T, x0_,rd_, op1,op2,op3,op4,op5,op6,op7,op8,op9)   !! Rd = L x0 -d
       call oc_linear(x_, (/x0_/), (/rONE/), ocmaxmem)
    else 
       call oc_assign(x_ , real(rZERO), ocmaxmem)       !! x0 = 0
    end if

    forget = T
    do i = 1, ocniter
       if(ocverb) call message('iteration',i)
       if(ocverb) call message('g=L* Rd')
       stat = L( T, F, g_, rd_, op1,op2,op3,op4,op5,op6,op7,op8,op9)
       if(ocverb) call message('Gd=L g')
       stat = L( F, F, g_, gd_, op1,op2,op3,op4,op5,op6,op7,op8,op9)
       if(ocverb) call message('descend')
       stat = S( forget, x_,g_,(/rd_/),(/gd_/),s_,(/sd_/),ocmaxmem)
       !----------------------------------------------------------------
       if(ocmmovie) then
          if(ocverb) call message('mmovie append')
          call oc_append(mm_,  x_)
       end if
       if(ocdmovie) then
          if(ocverb) call message('dmovie append')
          call oc_append(md_, rd_)
       end if
       !----------------------------------------------------------------
       if(present(res_)) then
          res = oc_norm((/rd_/), ocmaxmem)
          write(0,*)'res=',res
          res_ss=res
          if( present(res_)) call sep_write(res_ss, res_)
       end if
       !----------------------------------------------------------------
       if(stat ==1) then
          if( present(res_)) call oc_shapeheader(res_,4,(/i/),(/0./),(/1./))
          call message('DESCENT STOPPED')
          exit !! can't descend anymore
       end if
       !----------------------------------------------------------------       
       forget = F !! forget=false except for i=1
    end do
  end subroutine oc_solver
  !----------------------------------------------------------------
end module oc_solver_mod










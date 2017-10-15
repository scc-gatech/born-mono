module oc_solverpre_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================
  use oc_global_mod
  use oc_tags_mod
  use oc_utils_mod
  use oc_file_mod
  use oc_filealgebra_mod
  use oc_combine_mod
  implicit none

  integer,                    private :: ocniter,ocmaxmem
  real,                       private :: oceps
  logical,                    private :: ocverb, ocmmovie, ocdmovie

contains
  !! L = linear          operator
  !! P = preconditioning operator
  !! W = weighting       operator
  !! S = step

  !----------------------------------------------------------------
  subroutine oc_solverpre_init(niter,eps,maxmem,verb,mmovie,dmovie)
    integer, optional       :: niter,       maxmem
    real,    optional       :: eps
    logical, optional       :: verb, mmovie, dmovie
    ocniter=10;     if(present(niter))     ocniter=  niter
    ocmaxmem=0;     if(present(maxmem))   ocmaxmem= maxmem
    oceps=0.01;     if(present(eps))         oceps=    eps
    ocverb=F;       if(present(verb))       ocverb=   verb
    ocmmovie=F;     if(present(mmovie))   ocmmovie= mmovie
    ocdmovie=F;     if(present(dmovie))   ocdmovie= dmovie
  end subroutine oc_solverpre_init
  !----------------------------------------------------------------
  subroutine oc_solverpre( L,P,S, x_,yy_, npre, W ,k_,p0_,res_,op1,op2,op3,op4,op5,op6,op7,op8,op9)
    optional                                 :: W, k_,p0_,res_
    character(len=128),    intent(in)        ::    k_,p0_,res_
    character(len=128),    intent(in)        :: x_, yy_
    integer, external,     optional          :: op1,op2,op3,op4,op5,op6,op7,op8,op9
    integer, dimension(:), intent(in)        :: npre

    type(fileinfo)     :: file
    character(len=128) ::    rd_,rm_ !! residual
    character(len=128) :: g_,gd_,gm_ !! gradient and conjugate gradient
    character(len=128) :: s_,sd_,sm_ !! previous step
    character(len=128) ::mm_,md_     !! model/data movie
    character(len=128) :: p_,name
    integer            :: i, stat
    logical            :: forget
    real, dimension(1) :: res

    interface
       integer function L( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function L
       integer function P( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function P
       integer function W( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function W
       integer function S( forget,x_,g_,rr_,gg_,s_,ss_,maxmem) result(st)
         integer, optional                :: maxmem
         logical                          :: forget
         character(len=128)               ::  x_,  g_,  s_
         character(len=128), dimension(:) :: rr_, gg_, ss_
       end function S
    end interface

    call oc_allocatefile(file,x_,ocmaxmem)

    name=PS_p;       p_ = oc_allocate(F, name, file%esize, npre) !! preconditioned variable

    name=PS_Rm;     rm_ = oc_allocate(F, name, file%esize, npre) !! model residual
    name=PS_Rd;     rd_ = oc_clone(F, yy_,name, ocmaxmem) !! data  residual

    name=PS_g;       g_ = oc_clone(F,  x_,name, ocmaxmem)
    name=PS_Gd;     gd_ = oc_clone(F, yy_,name, ocmaxmem) !! data  conj grad
    name=PS_Gm;     gm_ = oc_allocate(F, name, file%esize, npre) !! model conj grad

    name=PS_s;       s_ = oc_clone(F,  x_,name, ocmaxmem)
    name=PS_Sd;     sd_ = oc_clone(F, yy_,name, ocmaxmem) !! data  previous step
    name=PS_Sm;     sm_ = oc_allocate(F, name, file%esize, npre) !! model previous step

    if(ocmmovie) then

       name=PS_Mmovie; mm_ = oc_clone(F,  x_,name, ocmaxmem)
       call oc_adddim(mm_, ocniter)
    end if
    if(ocdmovie) then
       name=PS_Dmovie; md_ = oc_clone(F, yy_,name, ocmaxmem)
       call oc_adddim(md_, ocniter)
    end if

    if( present(res_)) then
       call oc_shapeheader(res_,4,(/ocniter/),(/0./),(/1./))
    end if

    call oc_assign( rm_, real(rZERO), ocmaxmem)                          !! Rm =   0
    call oc_assign( sm_, real(rZERO), ocmaxmem)                          !! Sm =   0
    call oc_assign( gm_, real(rZERO), ocmaxmem)                          !! Gm =   0

    call oc_linear( rd_, (/yy_/), (/-rONE/), ocmaxmem)                   !! Rd = - d
    if(present(W)) stat = W( F,F, rd_,rd_,op1,op2,op3,op4,op5,op6,op7,op8,op9)         !! Rd = -Wd

    if(present(p0_)) then                                              !! Rd = WLP p0
       if(present(W)) then                                             !! Rm = eI  p0
          call oc_column(W,L,P,oceps, F,T, p0_,rd_,rm_, ocmaxmem,op1,op2,op3,op4,op5,op6,op7,op8,op9)
       else
          call oc_column(  L,P,oceps, F,T, p0_,rd_,rm_, ocmaxmem,op1,op2,op3,op4,op5,op6,op7,op8,op9)
       end if
       call oc_linear( p_, (/p0_/),(/rONE/), ocmaxmem)                   !! p  = p0
    else
       call oc_assign( p_ , real(rZERO), ocmaxmem)                       !! p  =  0
    end if

    forget=T; if(present(p0_)) forget=F
    do i = 1, ocniter
       if(ocverb) call message('iteration',i)
       !----------------------------------------------------------------
       if(ocverb) call message('g= P*L*W* Rd + eI Rm')
       if(present(W)) then
          call oc_column(W,L,P,oceps, T,F, g_,rd_,rm_, ocmaxmem,op1,op2,op3,op4,op5,op6,op7,op8,op9)
       else
          call oc_column(  L,P,oceps, T,F, g_,rd_,rm_, ocmaxmem,op1,op2,op3,op4,op5,op6,op7,op8,op9)
       end if
       !----------------------------------------------------------------
       if(present(k_)) then
          call oc_mask(k_,g_,ocmaxmem) !! don't change known
       end if
       !----------------------------------------------------------------
       if(ocverb) call message('Gd= W L P g; Gm= eI g')
       if(present(W)) then
          call oc_column(W,L,P,oceps, F,F, g_,gd_,gm_, ocmaxmem,op1,op2,op3,op4,op5,op6,op7,op8,op9)
       else
          call oc_column(  L,P,oceps, F,F, g_,gd_,gm_, ocmaxmem,op1,op2,op3,op4,op5,op6,op7,op8,op9)
       end if
       !----------------------------------------------------------------       
       if(ocverb) call message('descend')
       stat = S(forget,p_,g_,(/rd_,rm_/),(/gd_,gm_/),s_,(/sd_,sm_/),ocmaxmem)
       !----------------------------------------------------------------
       if(ocverb) call message('x = P p')
       stat = P( F,F, p_,x_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! x = P p
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
          if( present(res_)) call sep_write(res, res_)
       end if
       !----------------------------------------------------------------
       if(stat ==1) then
          if( present(res_)) call oc_shapeheader(res_,4,(/i/),(/0./),(/1./))
          call message('DESCENT STOPPED')
          exit !! can't descend anymore
       end if
       !----------------------------------------------------------------
       forget = F !! forget=false except for i=1
    end do !! end iterations

  end subroutine oc_solverpre
  !----------------------------------------------------------------
end module oc_solverpre_mod




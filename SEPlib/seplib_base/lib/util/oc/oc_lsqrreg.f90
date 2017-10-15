module oc_lsqrreg_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================
  use oc_global_mod
  use oc_tags_mod
  use oc_file_mod
  use oc_filealgebra_mod
  use oc_combine_mod
  implicit none

  integer,                  private :: ocniter,ocmaxmem
  logical,                  private :: ocverb, ocmovie
  real,                     private :: oceps

contains
  !----------------------------------------------------------------  
  subroutine oc_lsqrreg_init(niter,eps,maxmem,verb,movie)
    integer, optional :: niter,maxmem
    real,    optional :: eps
    logical, optional :: verb, movie

    ocniter=10;     if(present(niter))     ocniter=niter
    ocmaxmem=0;     if(present(maxmem))   ocmaxmem=maxmem
    oceps=0.01;     if(present(eps))         oceps=eps
    ocverb=F;       if(present(verb))       ocverb=verb
    ocmovie=F;      if(present(movie))     ocmovie=movie
  end subroutine oc_lsqrreg_init
  !----------------------------------------------------------------
  subroutine oc_lsqrreg( L,A, x_,yy_,nreg,W,op1,op2,op3,op4,op5,op6,op7,op8,op9)
    character(len=128),    intent(in) ::  x_, yy_
    integer, dimension(:), intent(in) :: nreg
    integer, external,     optional   :: op1,op2,op3,op4,op5,op6,op7,op8,op9
    optional                          :: W

    type(fileinfo)                    :: file
    character(len=128)                :: name
    character(len=128)                :: ud_,um_, v_, w_, m_
    integer                           :: stat,i
    real(kind=db)                     :: alfa,beta
    real(kind=db)                     :: phibar, rhobar, teta,phi,c,s,rho
    real(kind=db)                     :: t1,t2

    interface
       integer function L( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function L
       integer function A( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function A
       integer function W( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function W
    end interface

    call oc_allocatefile(file,x_,ocmaxmem)

    name=RL_Ud; ud_ = oc_clone(T, yy_,name, ocmaxmem)
    name=RL_Um; um_ = oc_allocate(T, name, file%esize, nreg)
    name=RL_v;  v_  = oc_clone(T,  x_,name, ocmaxmem)
    name=RL_w;  w_  = oc_clone(T,  x_,name, ocmaxmem)

    if(ocmovie) then
       name=RL_Mmovie; m_ = oc_clone(F, x_,name, ocmaxmem)
       call oc_adddim( m_, ocniter)
    end if

    call oc_assign(x_,real(rZERO),ocmaxmem)                      !! x=0

    call oc_linear(ud_,(/yy_/),(/rONE/),ocmaxmem)                !! Ud=D
    if(present(W)) stat = W( F,F, ud_,ud_,op1,op2,op3,op4,op5,op6,op7,op8,op9) !! Ud = -WD
    call oc_assign(um_,real(rZERO),ocmaxmem)                     !! Um=0
    call oc_normalize((/ud_,um_/),beta,ocmaxmem)                 !! U=U / |U|; b=|U|

    if(ocverb) call message('v= L*W* Ud + eA* Um')
    if(present(W)) then
       call oc_column(W,L,oceps,A, T,F, v_,ud_,um_, ocmaxmem, op1,op2,op3,op4,op5,op6,op7,op8,op9)
    else
       call oc_column(  L,oceps,A, T,F, v_,ud_,um_, ocmaxmem, op1,op2,op3,op4,op5,op6,op7,op8,op9)
    end if
    call oc_normalize( (/v_/),alfa,ocmaxmem)                     !! v=v / |v|; a=|v|
    call oc_linear( w_,(/v_/),(/rONE/),ocmaxmem)                 !! w=v

    rhobar=alfa
    phibar=beta
    do i = 1, ocniter
       if(ocverb) call message('iteration',i)


       call oc_linear(ud_,(/ud_/),(/-alfa/),ocmaxmem)  !! U =-a U
       call oc_linear(um_,(/um_/),(/-alfa/),ocmaxmem)  
       if(ocverb) call message('Ud= Ud+ W L v; Um= Um+ eA v')
       if(present(W)) then
          call oc_column(W,L,oceps,A, F,T, v_,ud_,um_, ocmaxmem, op1,op2,op3,op4,op5,op6,op7,op8,op9)
       else
          call oc_column(  L,oceps,A, F,T, v_,ud_,um_, ocmaxmem, op1,op2,op3,op4,op5,op6,op7,op8,op9)
       end if
       call oc_normalize((/ud_,um_/),beta,ocmaxmem)    !! U=U / |U|; b=|U|


       call oc_linear( v_,(/ v_/),(/-beta/),ocmaxmem)  !! v =-b v
       if(ocverb) call message('v= v+ L*W* Ud + eA* Um')
       if(present(W)) then
          call oc_column(W,L,oceps,A, T,T, v_,ud_,um_, ocmaxmem, op1,op2,op3,op4,op5,op6,op7,op8,op9)
       else
          call oc_column(  L,oceps,A, T,T, v_,ud_,um_, ocmaxmem, op1,op2,op3,op4,op5,op6,op7,op8,op9)
       end if
       call oc_normalize((/ v_/),alfa,ocmaxmem)            !! v=v / |v|; a=|v|


       rho    = sqrt(rhobar**2+beta**2)
       c      =   rhobar / rho
       s      =   beta   / rho
       teta   =   s      * alfa
       rhobar = - c      * alfa
       phi    =   c      * phibar
       phibar =   s      * phibar
       t1     =   phi    / rho
       t2     = - teta   / rho

       call oc_linear( x_,(/ x_,w_/),(/rONE,t1/),ocmaxmem) !! x = x + t1 w
       call oc_linear( w_,(/ v_,w_/),(/rONE,t2/),ocmaxmem) !! w = v + t2 w

       if(ocmovie) then
          if(ocverb) call message('movie append')
          call oc_append( m_,  x_)
       end if
    end do

  end subroutine oc_lsqrreg
  !----------------------------------------------------------------
end module oc_lsqrreg_mod

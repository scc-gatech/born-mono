module oc_lsqr_mod
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
  logical,                  private :: ocverb, ocmovie

contains
  !----------------------------------------------------------------  
  subroutine oc_lsqr_init(niter,maxmem,verb,movie)
    integer, optional :: niter,maxmem
    logical, optional :: verb, movie
    ocniter=10;     if(present(niter))     ocniter=niter
    ocmaxmem=0;     if(present(maxmem))   ocmaxmem=maxmem
    ocverb=F;       if(present(verb))       ocverb=verb
    ocmovie=F;      if(present(movie))     ocmovie=movie
  end subroutine oc_lsqr_init
  !----------------------------------------------------------------
  subroutine oc_lsqr( L, x_,yy_,op1,op2,op3,op4,op5,op6,op7,op8,op9)
    character(len=128), intent(in) ::  x_, yy_
    integer, external,  optional   :: op1,op2,op3,op4,op5,op6,op7,op8,op9

    character(len=128)             :: name
    character(len=128)             :: uu_, v_, w_, m_
    integer                        :: stat,i
    real(kind=db)                  :: alfa,beta
    real(kind=db)                  :: phibar, rhobar, teta,phi,c,s,rho
    real(kind=db)                  :: t1,t2

    interface
       integer function L( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function L
    end interface

    name=SL_U; uu_ = oc_clone(T, yy_,name, ocmaxmem)
    name=SL_v;  v_ = oc_clone(T,  x_,name, ocmaxmem)
    name=SL_w;  w_ = oc_clone(T,  x_,name, ocmaxmem)

    call oc_linear(uu_,(/yy_/),(/rONE/),ocmaxmem) !! U=Y
    call oc_assign(x_,real(rZERO),ocmaxmem)       !! x=0
    call oc_normalize((/uu_/),beta,ocmaxmem)      !! U=U / |U|; b=|U|
    stat = L(T,F,v_,uu_,op1,op2,op3,op4,op5,op6,op7,op8,op9)    !! v=L* U
    call oc_normalize((/ v_/),alfa,ocmaxmem)      !! v=v / |v|; a=|v|
    call oc_linear( w_,(/ v_/),(/rONE/),ocmaxmem) !! w=v

    if(ocmovie) then
       name=SL_Mmovie; m_ = oc_clone(F, x_, name, ocmaxmem)
       call oc_adddim( m_, ocniter)
    end if

    rhobar=alfa
    phibar=beta
    do i = 1, ocniter
       if(ocverb) call message('iteration',i)
       call oc_linear(uu_,(/uu_/),(/-alfa/),ocmaxmem)  !! U =-a U
       if(ocverb) call message('U=U+L v')
       stat = L(F,T,v_,uu_,op1,op2,op3,op4,op5,op6,op7,op8,op9)      !! U =   U + L v
       call oc_normalize((/uu_/),beta,ocmaxmem)        !! U=U / |U|; b=|U|

       call oc_linear( v_,(/ v_/),(/-beta/),ocmaxmem)  !! v =-b v
       if(ocverb) call message('v=v+L*U')
       stat = L(T,T,v_,uu_,op1,op2,op3,op4,op5,op6,op7,op8,op9)      !! v =   v + L*U
       call oc_normalize((/ v_/),alfa,ocmaxmem)        !! v=v / |v|; a=|v|

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

  end subroutine oc_lsqr
  !----------------------------------------------------------------
end module oc_lsqr_mod







module oc_rescale_mod
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
  implicit none

  integer,                      private :: maxmem
  logical,                      private :: verb

contains
  !! L = linear         operator
  !! A = regularization operator
  !! W = weighting      operator
  !! S = solver

  !----------------------------------------------------------------
  subroutine oc_rescale_init(maxmem_in,verb_in)
    integer, optional :: maxmem_in
    logical, optional :: verb_in
    maxmem=0;     if(present(maxmem_in))   maxmem= maxmem_in
    verb=F;       if(present(verb_in))       verb=   verb_in
  end subroutine oc_rescale_init
  !----------------------------------------------------------------
  subroutine oc_rescale( L, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9)
    character(len=128),    intent(in)        :: x_, yy_
    integer, external,     optional          :: op1,op2,op3,op4,op5,op6,op7,op8,op9

    type(fileinfo)     :: file
    character(len=128) :: name
    integer            :: stat
    character(len=128) :: dd_,rr_
    real(kind=db)      :: a1,a2,aa

    interface
       integer function L( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function L
    end interface

    call oc_allocatefile(file,x_,maxmem)

    !!   mo = L* Do
    !!   m  = L* D
    !!   min || mo - a m ||  =>  a = (D.Do) / (D.D) => mo = a m
    name=RS_dd; dd_ = oc_clone(T, yy_,name, maxmem)
    name=RS_rr; rr_ = oc_clone(T, yy_,name, maxmem)

    if(verb) call message('D = Lm')
    stat = L( F,F, x_,dd_, op1,op2,op3,op4,op5,op6,op7,op8,op9)           !! D
    call oc_linear(rr_, (/yy_,dd_/),(/rONE,-rONE/),maxmem)!! Do - D
    if(verb) call message('data residual magnitude',real(oc_norm((/rr_/),maxmem)))

    if( file%esize /=8) then !! REAL
       a1=sqrt(    oc_rdp((/yy_/),(/dd_/), maxmem) )      !! D.Do
       a2=sqrt(    oc_rdp((/dd_/),(/dd_/), maxmem) )      !! D.D
    else                     !! COMPLEX
       a1=sqrt(abs(oc_cdp((/yy_/),(/dd_/), maxmem)))      !! D.Do
       a2=sqrt(abs(oc_cdp((/dd_/),(/dd_/), maxmem)))      !! D.D
    end if
    aa =a1/a2                                             !! a = D.Do / D.D
    if(verb) call message('rescale: factor',real(aa))
    !
    call oc_linear( x_, (/ x_/),(/aa/), maxmem)           !! mo = a m
    !
  end subroutine oc_rescale
  !----------------------------------------------------------------
end module oc_rescale_mod







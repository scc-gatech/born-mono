module oc_epsilon_mod
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

  integer,                     private :: maxmem
  real,                        private :: bias
  logical,                     private :: verb
  integer,                     private :: method

contains
  !! L = linear         operator
  !! A = regularization operator
  !! W = weighting      operator
  !! S = solver

  !----------------------------------------------------------------
  subroutine oc_epsilon_init(bias_in,maxmem_in,verb_in,method_in)
    integer, optional :: maxmem_in
    real,    optional :: bias_in
    logical, optional :: verb_in
    integer, optional :: method_in
    bias=1.;      if(present(bias_in))       bias=   bias_in
    maxmem=0;     if(present(maxmem_in))   maxmem= maxmem_in
    verb=F;       if(present(verb_in))       verb=   verb_in
    method=1;     if(present(method_in))   method= method_in
  end subroutine oc_epsilon_init
  !----------------------------------------------------------------
  real function oc_epsilon( L,A, x_,yy_, nreg, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(eps)
    character(len=128), intent(in)           :: x_, yy_
    integer, external, optional              :: op1,op2,op3,op4,op5,op6,op7,op8,op9
    integer, dimension(:), intent(in)        :: nreg

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
    end interface

    if(method==1) then
       eps=oc_epsilon_residual( L,A, x_,yy_, nreg, op1,op2,op3,op4,op5,op6,op7,op8,op9)
    else
       eps=oc_epsilon_gradient( L,A, x_,yy_, nreg, op1,op2,op3,op4,op5,op6,op7,op8,op9)
    end if

    !! bias <1 more weight on Rd
    !! bias >1 more weight on Rm
    eps = eps * bias

  end function oc_epsilon
  !----------------------------------------------------------------  
  real function oc_epsilon_residual( L,A, x_,yy_, nreg, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(eps)
    character(len=128), intent(in)           :: x_, yy_
    integer, external, optional              :: op1,op2,op3,op4,op5,op6,op7,op8,op9
    integer, dimension(:), intent(in)        :: nreg

    type(fileinfo)     :: file
    character(len=128) :: rd_,rm_ !! residual
    character(len=128) :: name
    character(len=128) ::  d_, m_
    integer            :: i, stat

    real               :: mnorm, dnorm

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
    end interface

    call oc_allocatefile(file,x_,maxmem)
    name=EPS_Rm;   rm_ = oc_allocate(T, name, file%esize, nreg) !! model residual
    name=EPS_Rd;   rd_ = oc_clone(T, yy_,name, maxmem)          !! data  residual

    name=EPS_m;     m_ = oc_allocate(T, name, file%esize, nreg)
    name=EPS_d;     d_ = oc_clone(T, yy_,name, maxmem)

    !! fill the data  w/ random numbers of ||=1
    call oc_random( d_, real(rONE) , maxmem)          !!  d = random,  |d|=1

    !! find the model by adjoint
    stat = L(T,F, m_, d_, op1,op2,op3,op4,op5,op6,op7,op8,op9)        !!  m = L* d

    !! find the max absolute value in the model
    mnorm = oc_magnitude( (/ m_/), maxmem)            !! |L* d|

    !! create another model file filled with
    !! random numbers so that it has the same
    !! max absolute value as L* d
    call oc_random( m_, mnorm, maxmem)                !!  m = random and |m| = |L* d|

    !! residual files
    call oc_assign( rm_,real(rZERO), maxmem)                !! Rm =   0
    call oc_linear( rd_, (/ d_/), (/-rONE/), maxmem)  !! Rd = - d

    stat = L(F,T, m_,rd_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! Rd = Rd + L m = -d + L m
    stat = A(F,F, m_,rm_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! Rm =      A m

    dnorm = oc_norm( (/rd_/), maxmem) !! |Rd| - data  residual energy
    mnorm = oc_norm( (/rm_/), maxmem) !! |Rm| - model residual energy

    !! eps Rm = Rd
    eps = dnorm / mnorm
  end function oc_epsilon_residual
  !----------------------------------------------------------------
  real function oc_epsilon_gradient( L,A, x_,yy_, nreg, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(eps)
    character(len=128), intent(in)           :: x_, yy_
    integer, external, optional              :: op1,op2,op3,op4,op5,op6,op7,op8,op9
    integer, dimension(:), intent(in)        :: nreg

    type(fileinfo)     :: file
    character(len=128) :: rd_,rm_ !! residual
    character(len=128) :: gd_,gm_ !! gradient
    character(len=128) :: name
    character(len=128) ::  d_, m_
    integer            :: i, stat

    real               :: mnorm, dnorm

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
    end interface

    call oc_allocatefile(file,x_,maxmem)
    name=EPS_Rm;   rm_ = oc_allocate(T, name, file%esize, nreg) !! model residual
    name=EPS_Rd;   rd_ = oc_clone(T, yy_,name, maxmem)          !! data  residual

    name=EPS_gm;   gm_ = oc_clone(T, x_, name, maxmem)          !! model gradient
    name=EPS_gd;   gd_ = oc_clone(T, x_, name, maxmem)          !! data  gradient

    name=EPS_m;     m_ = oc_allocate(T, name, file%esize, nreg)
    name=EPS_d;     d_ = oc_clone(T, yy_,name, maxmem)

    !! fill the data  w/ random numbers of ||=1
    call oc_random( d_, real(rONE) , maxmem)          !!  d = random,  |d|=1

    !! find the model by adjoint
    stat = L(T,F, m_, d_, op1,op2,op3,op4,op5,op6,op7,op8,op9)       !!  m = L* d

    !! find the max absolute value in the model
    mnorm = oc_magnitude( (/ m_/), maxmem)            !! |L* d|

    !! create another model file filled with
    !! random numbers so that it has the same
    !! max absolute value as L* d
    call oc_random( m_, mnorm, maxmem)                !!  m = random and |m| = |L* d|

    !! residual files
    call oc_assign( rm_,real(rZERO), maxmem)                !! Rm =   0
    call oc_linear( rd_, (/ d_/), (/-rONE/), maxmem)  !! Rd = - d

    stat = L(F,T, m_,rd_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! Rd = Rd + L m = -d + L m
    stat = A(F,F, m_,rm_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! Rm =      A m

    stat = L(T,F, gd_,rd_,op1,op2,op3,op4,op5,op6,op7,op8,op9) !! gd = L* Rd
    stat = A(T,F, gm_,rm_,op1,op2,op3,op4,op5,op6,op7,op8,op9) !! gm = A* Rm

    dnorm = oc_norm( (/gd_/), maxmem) !! |gd| - data  gradient energy
    mnorm = oc_norm( (/gm_/), maxmem) !! |gm| - model gradient energy

    !! eps gm = gd
    eps = dnorm / mnorm
  end function oc_epsilon_gradient
  !----------------------------------------------------------------
end module oc_epsilon_mod


















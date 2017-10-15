!!$=head1 NAME
!!$
!!$oc_chain - Chain together several operators
!!$
!!$
!!$=head1 SYNOPSIS
!!$
!!$call oc_chain (adj,add,A,mod,data,maxmem,op1,op2,op3,op4,op5,op6,op7,op8,op9)
!!$
!!$call oc_chain (adj,add,A,B,mod,data,maxmem,op1,op2,op3,op4,op5,op6,op7,op8,op9)
!!$
!!$call oc_chain (adj,add,A,B,C,mod,data,maxmem,op1,op2,op3,op4,op5,op6,op7,op8,op9)
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$
!!$=item adj,add,mod,data - Standatad gee parameters (mod,data tags)
!!$
!!$=item A,B,C - oc operators
!!$
!!$=item op1,op2...,op9  integer, optional - optional operators
!!$
!!$=item maxmem - integer, optional
!!$
!!$Maximum memory to use for blocking operation
!!$
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$ Form a new operator by chaining together multiple operators
!!$
!!$ (e.g.  d = (A B ) m (where A B is performed by chain)
!!$
!!$=head1 SEE ALSO
!!$
!!$L<oc_column>
!!$
!!$=head1 LIBRARY
!!$
!!$sepocf90
!!$
!!$=cut
!!$
!!$>



module oc_combine_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================
  use oc_global_mod
  use oc_file_mod
  use oc_filealgebra_mod
  use oc_scale_mod
  implicit none

  interface oc_chain
     module procedure oc_chain1            !! y = A    x
     module procedure oc_chain2            !! y = AB   x
     module procedure oc_chain3            !! y = ABC  x
  end interface
  !! oc_row                                !! y=[A1 A2] [x1 x2]'
  interface oc_column
     module procedure oc_column11          !! [y1 y2]'=[A1       e.A2]' x
     module procedure oc_column20          !! [y1 y2]'=[A1.B1    e.I ]' x
     module procedure oc_column21          !! [y1 y2]'=[A1.B1    e.A2]' x
     module procedure oc_column30          !! [y1 y2]'=[A1.B1.C1 e.I ]' x
  end interface
!!$=head1 NAME
!!$
!!$oc_column - Form a column (posibly complex operator)
!!$
!!$
!!$=head1 SYNOPSIS
!!$
!!$  call oc_column(A1,eps,A2, adj,add,x_,y1_,y2_, maxmem, op1,op2,op3,op4,op5,op6,op7,op8,op9)
!!$  call oc_column(A1,B1,eps, adj,add, x_,y1_,y2_, maxmem, op1,op2,op3,op4,op5,op6,op7,op8,op9)
!!$  call oc_column(A1,B1,eps,A2, adj,add, x_,y1_,y2_, maxmem, op1,op2,op3,op4,op5,op6,op7,op8,op9)
!!$  call oc_column(A1,B1,C1,eps, adj,add, x_,y1_,y2_, maxmem, op1,op2,op3,op4,op5,op6,op7,op8,op9)
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$
!!$=item adj,add,mod,data - Standatad gee parameters (mod,data tags)
!!$
!!$=item A1,A2,B1,B2,C1 - oc operators
!!$
!!$=item op1,op2...,op9  integer, optional - optional operators
!!$
!!$=item maxmem - integer, optional
!!$
!!$Maximum memory to use for blocking operation
!!$
!!$=item eps - real  Scaling parameter 
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$ Form a new operator by combining together multiple operators
!!$
!!$  [y1 y2]'=[A1       e.A2]' x
!!$  [y1 y2]'=[A1.B1    e.I ]' x
!!$  [y1 y2]'=[A1.B1    e.A2]' x
!!$  [y1 y2]'=[A1.B1.C1 e.I ]' x
!!$
!!$=head1 SEE ALSO
!!$
!!$L<oc_chain>
!!$
!!$=head1 LIBRARY
!!$
!!$sepocf90
!!$
!!$=cut
!!$
!!$>


contains
  !----------------------------------------------------------------
  subroutine oc_chain1(A, adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9)
    logical,            intent(in) :: adj,add
    character(len=128), intent(in) :: x_, yy_
    integer, external,  optional   :: op1,op2,op3,op4,op5,op6,op7,op8,op9
    integer                        :: stat
    interface!!----------------------------------------------------------------
       integer function A( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function A
    end interface!!------------------------------------------------------------

    if (adj) then
       stat = A( T,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! x=A'y
    else
       stat = A( F,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! y=A x
    end if
  end subroutine oc_chain1
  !----------------------------------------------------------------
  subroutine oc_chain2(A,B, adj,add, x_,yy_,t_, op1,op2,op3,op4,op5,op6,op7,op8,op9)
    logical, intent (in) :: adj,add
    character(len=128)            :: x_,yy_,t_
    integer, external, optional   :: op1,op2,op3,op4,op5,op6,op7,op8,op9
    integer                       :: stat
    interface!!----------------------------------------------------------------
       integer function A( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function A
       integer function B( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function B
    end interface!!------------------------------------------------------------

    if (adj) then
       stat = A( T,  F, t_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! t=A'y
       stat = B( T,add, x_, t_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! x=B't = B'A'y
    else
       stat = B( F,  F, x_, t_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! t=B x
       stat = A( F,add, t_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! y=A t = A B x
    end if
  end subroutine oc_chain2
  !----------------------------------------------------------------
  subroutine oc_chain3(A,B,C, adj,add, x_,yy_,t1_,t2_, op1,op2,op3,op4,op5,op6,op7,op8,op9)
    logical,            intent(in) :: adj,add
    character(len=128), intent(in) :: x_,yy_,t1_,t2_
    integer, external,  optional   :: op1,op2,op3,op4,op5,op6,op7,op8,op9
    integer                        :: stat
    interface!!----------------------------------------------------------------
       integer function A( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function A
       integer function B( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function B
       integer function C( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function C
    end interface!!------------------------------------------------------------

    if (adj) then
       stat = A( T,  F, t2_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! t2=A'y
       stat = B( T,  F, t1_,t2_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! t1=B't2 =   B'A'y
       stat = C( T,add,  x_,t1_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! x =C't1 = C'B'A'y
    else
       stat = C( F,  F,  x_,t1_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! t1=C x
       stat = B( F,  F, t1_,t2_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! t2=Bt1 =    B C x
       stat = A( F,add, t2_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! y =At2 =  A B C x
    end if
  end subroutine oc_chain3
  !----------------------------------------------------------------
  subroutine oc_row(A1,A2, adj,add, x1_,x2_,y1_, op1,op2,op3,op4,op5,op6,op7,op8,op9)
    logical,            intent(in) :: adj, add
    character(len=128), intent(in) :: x1_, x2_, y1_
    integer, external,  optional   :: op1,op2,op3,op4,op5,op6,op7,op8,op9
    integer                        :: stat
    interface!!----------------------------------------------------------------
       integer function A1( adj, add, x_, yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function A1
       integer function A2( adj, add, x_, yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function A2
    end interface!!------------------------------------------------------------

    if (adj) then
       stat = A1( T,add, x1_,y1_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! x1=A1'y
       stat = A2( T,add, x2_,y1_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! x2=A2'y
    else
       stat = A1( F,add, x1_,y1_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! y =A1 x1
       stat = A2( F,  T, x2_,y1_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! y+=A2 x2 = A1x1+ A2x2
    end if
  end subroutine oc_row
  !----------------------------------------------------------------
  subroutine oc_column11(A1,eps,A2, adj,add, x_,y1_,y2_, maxmem, op1,op2,op3,op4,op5,op6,op7,op8,op9)
    real,               intent(in) :: eps
    logical,            intent(in) :: adj,add
    character(len=128), intent(in) :: x_,y1_,y2_
    integer,            optional   :: maxmem
    integer, external,  optional   :: op1,op2,op3,op4,op5,op6,op7,op8,op9
    character(len=128)             :: t_, name
    interface!!----------------------------------------------------------------
       integer function A1( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function A1
       integer function A2( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function A2
    end interface!!------------------------------------------------------------
    !!  [ L ]
    !!  [eA ]x

    if(.not.present(maxmem)) maxmem=0
    call oc_scale_init(eps,maxmem)
    name='_t.chain'; t_=oc_clone(T, y2_,name,maxmem)

    if (adj) then
       call oc_chain1(         A1, T,add, x_,y1_,    op1,op2,op3,op4,op5,op6,op7,op8,op9) !! x =  A1' y1
       call oc_chain2(A1,A2, T,  T, x_,y2_,t_, op1,op2,op3,op4,&
          op5,op6,op7,op8,op9) !! x+=e A2' y2
    else
       call oc_chain1(         A1, F,add, x_,y1_,    op1,op2,op3,op4,op5,op6,op7,op8,op9) !! y1=  A1 x
       call oc_chain2(oc_scale,A2, F,add, x_,y2_,t_, &
           op1,op2,op3,op4,op5,op6,op7,op8,op9) !! y2=e A2 x
    end if
  end subroutine oc_column11
  !----------------------------------------------------------------
  subroutine oc_column20(A1,B1,eps, adj,add, x_,y1_,y2_, maxmem, op1,op2,op3,op4,op5,op6,op7,op8,op9)
    real,               intent(in) :: eps
    logical,            intent(in) :: adj,add
    character(len=128), intent(in) :: x_,y1_,y2_
    integer,            optional   :: maxmem
    integer, external,  optional   :: op1,op2,op3,op4,op5,op6,op7,op8,op9
    character(len=128)             :: t_,name
    interface!!----------------------------------------------------------------
       integer function A1( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function A1
       integer function B1( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function B1
    end interface!!------------------------------------------------------------
    !! [ LP]
    !! [eI ]

    if(.not.present(maxmem)) maxmem=0
    call oc_scale_init(eps,maxmem)
    name='_t.chain'; t_=oc_clone(T, x_,name,maxmem)

    if (adj) then
       call oc_chain2(A1,      B1, T,add, x_,y1_,t_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! x =  B1' A1' y1
       call oc_chain1(   oc_scale, T,  T, x_,y2_,    op1,op2,op3,op4,op5,op6,op7,op8,op9) !! x+=e         y2
    else
       call oc_chain2(A1,      B1, F,add, x_,y1_,t_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! y1=  A1 B1 x
       call oc_chain1(   oc_scale, F,add, x_,y2_,    op1,op2,op3,op4,op5,op6,op7,op8,op9) !! y2=e       x
    end if
  end subroutine oc_column20
  !----------------------------------------------------------------
  subroutine oc_column21(A1,B1,eps,A2, adj,add, x_,y1_,y2_, maxmem, op1,op2,op3,op4,op5,op6,op7,op8,op9)
    real,               intent(in) :: eps
    logical,            intent(in) :: adj,add
    character(len=128), intent(in) :: x_,y1_,y2_
    integer,            optional   :: maxmem
    integer, external,  optional   :: op1,op2,op3,op4,op5,op6,op7,op8,op9
    character(len=128)             :: t_, name
    interface!!----------------------------------------------------------------
       integer function A1( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function A1
       integer function B1( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function B1
       integer function A2( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function A2
    end interface!!------------------------------------------------------------
    !! [WL ]
    !! [eA ]

    if(.not.present(maxmem)) maxmem=0
    call oc_scale_init(eps,maxmem)
    name='_t.chain'

    if (adj) then
       t_=oc_clone(T, y1_,name,maxmem)
       call oc_chain2(      A1,B1, T,add, x_,y1_,t_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! x =  B1' A1' y1
       t_=oc_clone(T, y2_,name,maxmem)
       call oc_chain2(oc_scale,A2, T,  T, x_,y2_,t_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! x+=e     A2' y2
    else
       t_=oc_clone(T, y1_,name,maxmem)
       call oc_chain2(      A1,B1, F,add, x_,y1_,t_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! y1=  A1 B1 x
       t_=oc_clone(T, y2_,name,maxmem)
       call oc_chain2(oc_scale,A2, F,add, x_,y2_,t_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! y2=e A2    x
    end if
  end subroutine oc_column21
  !----------------------------------------------------------------
  subroutine oc_column30(A1,B1,C1,eps, adj,add, x_,y1_,y2_, maxmem, op1,op2,op3,op4,op5,op6,op7,op8,op9)
    real,               intent(in) :: eps
    logical,            intent(in) :: adj,add
    character(len=128), intent(in) :: x_,y1_,y2_
    integer,            optional   :: maxmem
    integer, external,  optional   :: op1,op2,op3,op4,op5,op6,op7,op8,op9
    character(len=128)             :: t1_,t2_,name
    interface!!----------------------------------------------------------------
       integer function A1( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function A1
       integer function B1( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function B1
       integer function C1( adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(st)
         integer, external, optional :: op1,op2,op3,op4,op5,op6,op7,op8,op9
         logical, intent (in)        :: adj, add
         character(len=128)          :: x_, yy_
       end function C1
    end interface!!------------------------------------------------------------
    !! [WLP]
    !! [eI ]

    if(.not.present(maxmem)) maxmem=0
    call oc_scale_init(eps,maxmem)
    name='_t1.chain'; t1_=oc_clone(T,  x_,name,maxmem)
    name='_t2.chain'; t2_=oc_clone(T, y1_,name,maxmem)

    if (adj) then
       call oc_chain3(A1,B1,      C1, T,add, x_,y1_,t1_,t2_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! x =  C1' B1' A1' y1
       call oc_chain1(      oc_scale, T,  T, x_,y2_,         op1,op2,op3,op4,op5,op6,op7,op8,op9) !! x+=e             y2
    else
       call oc_chain3(A1,B1,      C1, F,add, x_,y1_,t1_,t2_, op1,op2,op3,op4,op5,op6,op7,op8,op9) !! y1=  A1 B1 C1 x
       call oc_chain1(      oc_scale, F,add, x_,y2_,         op1,op2,op3,op4,op5,op6,op7,op8,op9) !! y2=e          x
    end if
  end subroutine oc_column30
  !----------------------------------------------------------------
end module oc_combine_mod

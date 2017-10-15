module oc_global_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================

  implicit none

  !! double precision
  integer, parameter, public :: db = selected_real_kind(10,50)

  !! short-hand for logical flags
  logical, parameter, public :: T = .true., F = .false.

  !! epsilon
  real   (kind=db), parameter :: rEPS  =           1.0e-30
  complex(kind=db), parameter :: cEPS  = (1.0,0.0)*1.0e-30

  !! commonly-used constants
  real   (kind=db), parameter :: rONE =  1.0
  complex(kind=db), parameter :: cONE = (1.0,0.0)

  real   (kind=db), parameter :: rZERO =  0.0
  complex(kind=db), parameter :: cZERO = (0.0,0.0)

  !! external functions
  integer, external           :: sseek_block

end module oc_global_mod

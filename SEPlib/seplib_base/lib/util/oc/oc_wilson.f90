module oc_wilson_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================
  use oc_global_mod
  use oc_filter_mod
  use oc_helicon_mod
  use oc_polydiv_mod
  implicit none

  integer, private :: ocniter  ! Wilson iterations
  integer, private :: ocnpad   ! Filter padding factor
  integer, private :: n

  interface wilson_factor
     module procedure R_wilson_factor
     module procedure C_wilson_factor
  end interface

contains

  !----------------------------------------------------------------
  subroutine oc_wilfac_init(npad,niter)
    integer, optional :: npad,niter

    ocnpad =2**5; if(present( npad)) ocnpad = npad
    ocniter=10;   if(present(niter)) ocniter=niter
  end subroutine oc_wilfac_init
  !----------------------------------------------------------------
  subroutine oc_wilfac(ss,ff)
    type(rfilter), intent(in)  :: ss
    type(cfilter), intent(out) :: ff
    type(rfilter)              :: aa
    complex :: f0

    call oc_allocatehelix(aa,size(ss%lag)/2)
    aa%lag=ss%lag(size(ss%lag)/2+1:size(ss%lag))
    aa%flt=ss%flt(size(ss%lag)/2+1:size(ss%lag))

    call wilson_init(ocnpad*maxval(ff%lag))
    call wilson_factor(ocniter,1.,aa,f0,ff)

  end subroutine oc_wilfac
  !----------------------------------------------------------------

  !----------------------------------------------------------------
  subroutine wilson_init(n_in)
    integer, intent(in) :: n_in
    n = n_in
  end subroutine wilson_init
  !----------------------------------------------------------------
  subroutine R_wilson_factor(ocniter, s0,ss, a0,aa)
    integer,       intent( in)  :: ocniter ! Newton iterations
    type(rfilter), intent( in)  :: ss    ! autocorrelation, other lags
    type(rfilter)               :: aa    ! factor, other lags
    real,          intent( in)  :: s0    ! autocorrelation zero lag
    real,          intent( out) :: a0    ! factor, zero lag

    real                        :: eps
    integer                     :: i, stat

    real, dimension(:), allocatable :: auto, bb, cc, b, c
    allocate(auto(2*n-1), bb(2*n-1), cc(2*n-1), b(n), c(n))

    auto = 0.
    auto(n) = s0
    b(1) =1.                                     ! initialize
    auto(n+ss%lag) =  ss%flt                     ! autocorrelation
    auto(n-ss%lag) =  ss%flt                     ! symmetrize input auto.
    call oc_helicon_init(aa)                     ! multiply polynoms
    call oc_polydiv_init(aa)                     ! divide   polynoms
    do i = 1, ocniter  
       stat= polydiv(F,F, auto, bb)              ! bb = S/A
       stat= polydiv(T,F, cc,   bb)              ! cc = S/(AA')
       b(2:n) = 0.5*( cc(n+1:2*n-1) + cc(n-1:1:-1) ) / cc(n)  ! b = plusside(1+cc)
       eps = maxval( abs( b(2:n)))               ! "L1 norm"
       if(eps < epsilon(a0)) then
          exit                                   ! convergence
       end if
       stat= helicon(F,F, b,c)                   ! c = A b
       aa%flt = c(1+aa%lag)                      ! put on helix
    end do
    a0 = sqrt(cc(n))

    deallocate(auto, bb,cc, b,c)
  end subroutine R_wilson_factor
  !----------------------------------------------------------------
  subroutine C_wilson_factor(ocniter, s0,ss, a0,aa)
    integer,       intent( in)  :: ocniter ! Newton iterations

    type(rfilter), intent( in)  :: ss    ! autocorrelation, other lags
    type(cfilter)               :: aa    ! factor, other lags
    real,          intent( in)  :: s0    ! autocorrelation zero lag
    complex,       intent( out) :: a0    ! factor, zero lag

    real                        :: eps
    integer                     :: i, stat

    complex, dimension(:), allocatable :: auto, bb, cc, b, c
    allocate(auto(2*n-1), bb(2*n-1), cc(2*n-1), b(n), c(n))

    auto = 0.
    auto(n) = s0
    b(1) =1.                                     ! initialize
    auto(n+ss%lag) =  ss%flt                     ! autocorrelation
    auto(n-ss%lag) =  ss%flt                     ! symmetrize input auto.
    call oc_helicon_init(aa)                     ! multiply polynoms
    call oc_polydiv_init(aa)                     ! divide   polynoms
    do i = 1, ocniter  
       stat= polydiv(F,F, auto, bb)              ! bb = S/A
       stat= polydiv(T,F, cc,   bb)              ! cc = S/(AA')
       b(2:n) = 0.5*( cc(n+1:2*n-1) + cc(n-1:1:-1) ) / cc(n)  ! b = plusside(1+cc)
       eps = maxval(abs(b(2:n)))                 ! "L1 norm"
       if(eps < epsilon(abs(a0))) then
          exit                                   ! convergence
       end if
       stat= helicon(F,F, b, c)                  ! c = A b
       aa%flt = c(1+aa%lag)                      ! put on helix
    end do
    a0 = sqrt(cc(n))

    deallocate(auto, bb,cc, b,c)
  end subroutine C_wilson_factor
  !----------------------------------------------------------------  
end module oc_wilson_mod

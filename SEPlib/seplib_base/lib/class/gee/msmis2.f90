module msmis2  
! multi-scale missing data interpolation
  use mshelicon
  use cgstep_mod
  use mask1
  use solver_smp_mod
  implicit none
  contains
  subroutine mis1( niter, nx, ns, xx, aa, known)
    integer,                intent( in)     :: niter, nx, ns
    logical, dimension( :), intent( in)     :: known
    type( msfilter),        intent( in)     :: aa
    real,    dimension( :), intent( in out) :: xx
    real,    dimension( nx*ns)              :: dd
    logical, dimension(:), pointer :: kk
    dd = 0.
    allocate(kk(size(known)))
    kk=.not. known
    call mask1_init(kk)
    call mshelicon_init( nx,ns, aa)
    call solver_smp(m=xx, d=dd, Fop=mshelicon_lop, stepper=cgstep,&
      & niter=niter, Jop=mask1_lop, m0=xx)
    call cgstep_close()
  end subroutine 
end module 

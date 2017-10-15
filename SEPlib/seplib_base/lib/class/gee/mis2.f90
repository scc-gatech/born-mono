module mis2_mod  
  use mask1 
  use  helicon 
  use  polydiv 
  use  cgstep_mod 
  use solver_smp_mod 
  use  solver_reg_mod 
  use  solver_prc_mod
  implicit none
  contains
  subroutine mis2( niter, xx, aa, known, style)
    integer,                intent( in)     :: style
    integer,                intent( in)     :: niter
    type( filter),          intent( in)     :: aa
    logical, dimension( :), intent( in)     :: known
    real,    dimension( :), intent( in out) :: xx        ! fitting variables
    real,    dimension( :), allocatable     :: dd
    logical, dimension( :), pointer         :: msk
    integer                                 :: nx
    nx = size( xx)
    allocate( dd(nx))
    allocate(msk(nx))
    if (style.eq.0) then
      dd = 0.
      msk = .not.known
      call mask1_init( msk)
      call helicon_init( aa)
      call solver_smp( m=xx, d=dd, Fop=helicon_lop, stepper=cgstep&
        &,niter=niter, Jop=mask1_lop, m0=xx)
      call helicon_close()
    else if (style.eq.1) then
      dd=xx
      msk = known
      call mask1_init( msk)
      call helicon_init( aa)
      call solver_reg( m=xx, d=dd, Fop=mask1_lop, stepper=cgstep,niter=niter&
        &, Aop=helicon_lop, nAop=nx, eps=0.1)
      call helicon_close()
    else
      dd=xx
      msk = known
      call mask1_init( msk)
      call polydiv_init( nx, aa)
      call solver_prc( m=xx, d=dd, Fop=mask1_lop, stepper=cgstep,niter=niter&
        &, Sop=polydiv_lop, nSop=nx, eps=0.0)
      call polydiv_close()
    end if 
    call cgstep_close( )
    deallocate( msk,dd)
  end subroutine 
end module 

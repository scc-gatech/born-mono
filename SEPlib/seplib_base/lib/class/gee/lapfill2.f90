module lapfill  
! fill empty 2-D bins by minimum output of Laplacian operator
  use laplac2
  use cgstep_mod
  use mask1
  use solver_smp_mod
  implicit none
  contains
  subroutine lapfill2( niter, m1, m2, yy, mfixed)
    integer,                    intent (in)     :: niter, m1, m2
    logical, dimension (m1*m2), intent (in)     :: mfixed 
    ! mask for known
    real,    dimension (m1*m2), intent (in out) :: yy     ! model
    real,    dimension (m1*m2)                  :: zero   
    ! laplacian output
    logical, dimension (:),     pointer         :: msk
    allocate(msk(size(mfixed)))
    msk=.not.mfixed
    call mask1_init(msk)
    call laplac2_init ( m1,m2)
    zero = 0.      ! initialize
    call solver_smp(m=yy, d=zero, Fop=laplac2_lop, stepper=cgstep,&
      & niter=niter, m0=yy, Jop=mask1_lop)
    call laplac2_close ()                                 
    ! garbage collection
    call cgstep_close ()                                  
    ! garbage collection
  end subroutine 
end module 

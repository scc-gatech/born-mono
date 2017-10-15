module mask2i  
  use mask1
  use helicon
  use heliarr
  use cgstep_mod
  use solver_reg_mod
  implicit none
  contains
  subroutine maski (niter, dd, xx, known, aa1, aa2)
    integer,                intent (in)  :: niter
    real,    dimension (:), intent (in)  :: dd
    type( filter),          intent (in)  :: aa1, aa2
    real,    dimension (:), intent (out) :: xx
    logical, dimension (:), pointer      :: known
    integer                              :: nx
    optional                             :: aa2
    call mask1_init(known)
    nx = size (dd)
    if (present (aa2)) then
      call heliarr_init (nx, aa1, aa2)
      call solver_reg(m=xx, d=dd, Fop=mask1_lop, stepper=cgstep, niter=niter&
        &,Aop=heliarr_lop, nAop=2*nx, eps=1.)
    else
      call helicon_init (aa1)
      call solver_reg(m=xx, d=dd, Fop=mask1_lop, stepper=cgstep, niter=niter&
        &,Aop=helicon_lop, nAop=nx, eps=1.)
    end if 
    call cgstep_close ()
  end subroutine 
end module 

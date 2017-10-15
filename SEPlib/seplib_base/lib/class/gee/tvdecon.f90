module tv_decon  
! Time Variable (TV) Deconvolution
  use tvdec
  use cgstep_mod
  use solver_mod
  implicit none
  contains
  subroutine tvdecon( rho, gap, na, yy, rr, niter)
    integer,                   intent( in)  :: gap, na, niter
    real,                      intent( in)  :: rho
    real,    dimension( :),    pointer      :: yy
    real,    dimension( :),    intent( out) :: rr
    real,    allocatable                    :: mm(:),dat(:)
    logical, allocatable                    :: mask(:)
    integer                                 :: nt
    allocate(mask(size(yy)*na),mm(size(yy)*na),dat(size(rr)))
    nt = size( yy) 
    dat = 0.                         
    mm = 0. 
    mm( 1) = 1. 
    mm( 2:nt) = 1.- rho
    mask = .false.
    mask( :gap*nt) = .true.
    call tvdec_init( yy, nt, na, rho)
    call solver( tvdec_lop, cgstep, x0 = mm, x = mm, dat = dat,known =&
      & mask, niter = niter, res  = rr) 
    call cgstep_close()
    call tvdec_close()
    deallocate(mask,mm,dat)
  end subroutine 
end module 

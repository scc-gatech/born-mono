module levint  
! LEVeler INverse INTerpolation in 1-D while finding PEF.
  use tcai2
  use peftc
  use lint1
  use cgstep_mod
  use solver_mod
  implicit none
  contains
  subroutine levint1 (niter, coord, dd, o1, d1, aa, mm, mmov, rr, eps)
    integer,               intent (in)  :: niter
    real,                  intent (in)  :: o1, d1, eps
    real, dimension (:),   intent (in)  :: dd
    real, dimension (:)                 :: rr
    real, dimension (:,:), intent (out) :: mmov
    real, dimension (:),   pointer      :: aa, mm, coord
    integer                             :: nr, nm
    logical, dimension (size (rr))      :: ma
    nm = size (mm)
    nr = size (rr)
    call lint1_init (o1, d1, coord)
    call peftc_init (aa, mm)
    call tcai2_init (aa, nm) 
    aa (1 : 3) = (/ 1., -2., 1. /)         ! starting guess
    ma (:nm) = .false.  
    ma (nm+1:) = .true.
    call solver_reg (lint1_lop, cgstep,reg= peftc_lop, nreg= nr, nlreg=&
      & tcai2_lop, eps= eps,niter= niter, x0= rr, x= rr, dat= dd,xmov=&
      & mmov, known= ma)
    ma (nm+2:) = .false.                   ! free filter coefficients
    call cgstep_close ()
    call solver_reg (lint1_lop, cgstep,reg= peftc_lop, nreg= nr, nlreg=&
      & tcai2_lop, eps= eps,niter= niter, x0= rr, x= rr, dat= dd,xmov=&
      & mmov, known= ma)
    call cgstep_close ()
  end subroutine 
end module 

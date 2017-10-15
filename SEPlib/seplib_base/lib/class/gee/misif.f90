module misif  
! MISSIF -- find MISSing Input data and Filter
  use peftc
  use tcai2
  use cgstep_mod
  use solver_mod
  implicit none
  contains
  subroutine misif1 (niter, xx, aa, mm)
    integer, intent (in)                               :: niter  
    ! iterations
    real,    dimension (:)                             :: xx, aa 
    ! data, PEF
    logical, dimension (:)                             :: mm     
    ! mask
    real,    dimension (:), pointer                    :: xk, bb
    real,    dimension (size (xx) + size (aa))         :: dat
    real,    dimension (size (xx) + size (aa)), target :: x
    integer                                            :: nx
    nx = size (xx)
    x = 0.
    dat = 0.        
    xk => x (1 : nx  ) 
    where (mm (1 : nx ))
      xk = xx
end where
    bb => x (1 + nx :) 
    where (mm (1 + nx:))
      bb = 1.
end where
    call peftc_init (bb, xk)
    call tcai2_init (bb, nx)
    call solver (peftc_lop, cgstep, niter = niter, x0 = x, x = x,dat =&
      & dat, nloper = tcai2_lop, known = mm)
    call cgstep_close ()
    xx = xk 
    aa = bb
  end subroutine 
end module 

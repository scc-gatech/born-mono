module invstack  
  use imospray
  use cgstep_mod
  use solver_tiny_mod
implicit none
  contains
! NMO stack by inverse of forward modeling
  subroutine stack( nt, model, nx, gather,     t0,x0,dt,dx,slow, niter&
    &)
    integer           nt,        nx,                              &
      & niter
    real                  model (:), gather (:), t0,x0,dt,dx,slow
    call imospray_init( slow, x0,dx, t0,dt, nt, nx)
    call solver_tiny( m=model, d=gather, Fop=imospray_lop, stepper=cgstep&
      &, niter=niter)
    call cgstep_close ()
    call imospray_close ()   ! garbage collection
  end subroutine 
end module 

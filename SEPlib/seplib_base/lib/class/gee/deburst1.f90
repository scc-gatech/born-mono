module deburst1  
  use copy_mod
  use icai1
  use solver_mod 
  use cgstep_mod
  implicit none
  contains
  subroutine deburst ( niter, weight, eps, dd, hh)
    interface  
      integer function weight( res, w)
        real, dimension (:) :: res, w
      end function
    end interface 
    integer, intent (in)  ::  niter
    real,    intent (in)  ::  eps
    real,    intent (in)  ::  dd(:)
    real,    intent (out) ::  hh(:)
    real,    pointer      ::  aa(:)
    allocate (aa (3))
    aa  = (/-1., 2., -1. /)        ! Laplacian filter
    call icai1_init (aa, 1)        ! initialize convolution
! solver, regularized, weighting function
    call solver_reg(  copy_lop,cgstep, x= hh, dat= dd, niter= niter,wght=&
      & weight,   nfreq= 1,         nmem= 0,reg=  icai1_lop, nreg=&
      & size (dd),  eps=eps) 
    call cgstep_close ()
    deallocate (aa)
  end subroutine 
end module 

module vr_solver  
! virtual-residual fitter
  implicit none
  logical, parameter, private  :: T = .true., F = .false.
  contains
  subroutine solver_prec( oper, solv, prec, nprec, x, dat, niter, eps,&
    & x0)
    optional                                                        ::&
      & x0
    interface  
      integer function oper( adj, add, x, dat)
        logical, intent (in) :: adj, add
        real, dimension (:)  :: x, dat
      end function 
      integer function prec( adj, add, x, dat)
        logical, intent (in) :: adj, add
        real, dimension (:)  :: x, dat
      end function 
      integer function solv( forget, x, g, rr, gg)
        logical             :: forget
        real, dimension (:) :: x, g, rr, gg
      end function
    end interface 
    real, dimension (:), intent (in)  :: dat, x0         
    ! data, initial
    real, dimension (:), intent (out) :: x               ! solution
    real,                intent (in)  :: eps             ! scaling
    integer,             intent (in)  :: niter, nprec    
    ! iterations, size
    real, dimension (size (dat) + nprec), target :: p, g 
    ! new, gradient
    real, dimension (size (dat))      :: rr, gg          
    ! residual, conj grad
    real, dimension (:), pointer      :: pm, pd, gm, gd
    integer                           :: i, stat1, stat2, stat
    pm => p(1:nprec)
    pd => p(1+nprec:)
    pd = 0.      ! compound model
    gm => g(1:nprec)
    gd => g(1+nprec:)                ! data and model grads
    rr = - dat
    if ( present( x0)) then
      pm = x0             ! start with x0
      stat2 = prec( F, F, pm, x)                        ! x0 = S p0
      stat1 = oper( F, T, x, rr)                        
      ! r  = F x0 - dat
    else
      pm = 0.
    end if 
! start with zero
    do i = 1, niter  
      stat1 = oper( T, F, x, rr)                
      stat2 = prec( T, F, gm, x)                    ! g_m = S' F' rr
      gd =     eps*rr           ! g_d = eps rr
      stat2 = prec( F, F, gm, x)                
      stat1 = oper( F, F,  x, gg)                   ! gg = F S g_m
      gg =                               gg + eps*gd          
      ! gg = F S g_m + eps g_d
      stat = solv( F, p, g, rr, gg)                 
      ! step in p and rr
    end do 
    stat2 =    prec( F, F, pm, x)                    ! x = S p
  end subroutine 
end module 

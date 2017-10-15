module prec_solver  
  implicit none
  logical, parameter, private  :: T = .true., F = .false.
  contains
  subroutine solver_prec( oper, solv, prec, nprec, x, dat, niter, eps,&
    & p0)
    optional                                                        ::&
      & p0
    interface  
      integer function oper( adj, add, x, dat)
        logical, intent (in) :: adj, add
        real, dimension (:)  :: x, dat
      end function 
      integer function solv( forget, x, g, rr, gg)
        logical             :: forget
        real, dimension (:) :: x, g, rr, gg
      end function 
      integer function prec( adj, add, x, dat)
        logical, intent (in) :: adj, add
        real, dimension (:)  :: x, dat
      end function
    end interface 
    real, dimension (:), intent (in)  :: dat, p0           
    ! data, initial
    real, dimension (:), intent (out) :: x                 ! solution
    real,                intent (in)  :: eps               ! scaling
    integer,             intent (in)  :: niter, nprec      
    ! iterations, size
    real, dimension (nprec)           :: g, p              
    ! gradient, precond
    real, dimension (size (dat) + nprec), target :: rr, gg 
    ! residual, conj grad
    real, dimension (:), pointer      :: rd, rm, gd, gm
    integer                           :: i, stat1, stat2, stat
    rm => rr (1 : nprec) 
    rd => rr (1 + nprec :)          ! model and data resids
    gm => gg (1 : nprec) 
    gd => gg (1 + nprec :)          ! model and data grads
    rd = -dat                       ! initialize r_d
    if ( present( p0)) then
      stat1 = prec( F, F, p0, x)       ! x = S p
      stat2 = oper( F, T, x, rd)       ! r_d = L x - dat
      p = p0 
      rm = p0*eps
! start with p0
    else
      p = 0. 
      rm = 0.
    end if 
! start with zero
    do i = 1, niter  
      stat2 = oper( T, F, x, rd)    
      stat1 = prec( T, F, g, x)        ! g = S' L' r_d
      g = g + eps*rm                   ! g = S' L' r_d + eps I r_m
      stat1 = prec( F, F, g, x)     
      stat2 = oper( F, F, x, gd)       ! g_d = L S g
      gm = eps*g                       ! g_m = eps I g
      stat = solv (F, p, g, rr, gg)    ! step in p and rr
    end do 
    stat1 = prec( F, F, p, x)           ! x = S p
  end subroutine 
end module 

module reg_solver   
  implicit none
  logical, parameter, private  :: T = .true., F = .false.
  contains
  subroutine solver_reg( oper, solv, reg, nreg, x, dat, niter, eps, x0&
    &, xmov)
    optional                                                     :: x0&
      &, xmov   
    interface  
      integer function oper(  adj, add, x, dat)
        logical, intent( in) :: adj, add
        real, dimension( :)  :: x, dat
      end function 
      integer function reg(   adj, add, x, dat)
        logical, intent( in) :: adj, add
        real, dimension( :)  :: x, dat
      end function 
      integer function solv(  forget, x, g, rr, gg)
        logical             :: forget
        real, dimension( :) :: x, g, rr, gg
      end function
    end interface 
    real, dimension( :),    intent( in)  :: dat, x0       
    ! data, initial
    real, dimension( :),    intent( out) :: x             ! solution
    real,                   intent( in)  :: eps           ! scaling
    integer,                intent( in)  :: niter, nreg   
    ! iterations, size
    real, dimension( :, :), intent (out) :: xmov          
    ! iterations movie
    real, dimension( size( x))        :: g                ! gradient
    real, dimension( size( dat) + nreg), target :: rr, gg 
    ! residual, conj grad
    real, dimension( :), pointer      :: rd,rm, gd,gm
    integer                           :: i, stat1, stat2, stat
    rm => rr( 1: nreg)
    rd => rr( 1 + nreg:)          ! model and data resids
    gm => gg( 1: nreg)
    gd => gg( 1 + nreg:)          ! model and data grads
    rd = - dat                    ! initialize r_d
    if ( present( x0)) then
      stat1 = oper( F, T, x0, rd)   ! r_d = L x0 - dat
      stat2 =  reg( F, F, x0, rm)   ! r_m = eps A x0
      rm = rm * eps                       !
      x = x0
! start with x0
    else
      x = 0.
      rm = 0.
    end if 
! start with zero
    do i = 1, niter   
      stat1 = oper( T, F, g, rd)          ! g   = L' r_d
      gm = rm*eps       !
      stat2 =  reg( T, T, g, gm)          ! g   = L' r_d + eps A' r_m
      stat1 = oper( F, F, g, gd)          ! g_d = L g
      stat2 =  reg( F, F, g, gm)          ! g_m = eps A g
      gm = gm*eps       !
      stat  = solv( F, x, g, rr, gg)      ! step in x and rr
      if (present (xmov)) then
        xmov (:,i) =  x ! save to a movie
      end if
    end do
  end subroutine 
end module 

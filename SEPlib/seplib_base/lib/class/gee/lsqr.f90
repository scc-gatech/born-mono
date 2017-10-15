module lsqr_mod  
  implicit none
  logical, parameter, private :: T = .true., F = .false.
  private                     :: normalize
  contains
  subroutine lsqr( oper, x, dat, niter)
    interface  
      integer function oper( adj, add, x, dat)
        logical, intent( in) :: adj, add
        real, dimension (:)  :: x, dat
      end function
    end interface 
    real, dimension (:), intent (in)    :: dat
    real, dimension (:), intent (out)   :: x
    integer, intent (in)                :: niter
    real, dimension ( size( x  ))       :: w, v
    real, dimension ( size( dat))       :: u
    integer                             :: iter, stat
    double precision                    :: alfa, beta, rhobar, phibar
    double precision                    :: c, s, teta, rho, phi, t1,&
      & t2
    u = dat
    x = 0.    
    call normalize( u, beta)
    stat = oper( T,F,v,u)
    call normalize( v, alfa)
    w = v
    rhobar = alfa
    phibar = beta
    do iter = 1, niter  
      u = - alfa * u
      stat = oper( F, T, v, u)
      call normalize( u, beta)
      v = - beta * v
      stat = oper( T, T, v, u)
      call normalize( v, alfa)
      rho = sqrt( rhobar*rhobar + beta*beta)
      c = rhobar/rho
      s = beta/rho
      teta = s * alfa
      rhobar = - c * alfa
      phi = c * phibar
      phibar = s * phibar
      t1 = phi/rho
      t2 = -teta/rho
      x = x + t1 * w
      w = v + t2 * w
    end do
  end subroutine 
  subroutine normalize( vector, size)
    real, dimension (:), intent (inout) :: vector
    double precision,    intent (out)   :: size
    size = sqrt( sum( dprod( vector, vector)))
    vector = vector / size
  end subroutine 
end module 

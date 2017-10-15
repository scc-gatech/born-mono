module smallsolver   
  implicit none
  logical, parameter, private  :: T = .true., F = .false.
  contains
  subroutine solver( oper, solv, x, dat, niter, x0, res)
    optional                                 :: x0, res   
    interface  
      integer function oper( adj, add, x, dat)
        logical, intent (in) :: adj, add
        real, dimension (:)  :: x, dat
      end function 
      integer function solv( forget, x, g, rr, gg)
        logical             :: forget
        real, dimension (:) :: x, g, rr, gg
      end function
    end interface 
    real,    dimension (:),   intent (in)  :: dat, x0     
    ! data, initial
    real,    dimension (:),   intent (out) :: x, res      
    ! solution, residual
    integer,                  intent (in)  :: niter       
    ! iterations
    real, dimension (size (x))             :: g           ! gradient
    real, dimension (size (dat))           :: rr, gg      
    ! residual, conj grad
    integer                                :: i, stat
    rr = - dat
    if ( present( x0)) then
      stat = oper( F, T, x0, rr)                        
      ! rr <- F x0 - dat
      x = x0                                            
      ! start with x0
    else
      x = 0.                                            
      ! start with zero
    end if 
    do i = 1, niter   
      stat = oper( T, F, g, rr)                          
      ! g  <- F' rr
      stat = oper( F, F, g, gg)                          ! gg <- F  g
      stat = solv( F, x, g, rr, gg)                      
      ! step in x and rr
    end do 
    if ( present( res)) then
      res = rr
    end if
  end subroutine 
end module 

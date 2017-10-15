module weight_solver  
  implicit none
  logical, parameter, private  :: T = .true., F = .false.
  contains
  subroutine solver( oper, solv, x, dat, niter, nmem, nfreq, wght)
    interface  
      integer function wght( res, w)
        real, dimension (:)  :: res, w
      end function 
      integer function oper( adj, add, x, dat)
        logical, intent (in) :: adj, add
        real, dimension (:)  :: x, dat
      end function 
      integer function solv( forget, x, g, rr, gg)
        logical             :: forget
        real, dimension (:) :: x, g, rr, gg
      end function
    end interface 
    real, dimension (:), intent (in)  :: dat                ! data
    real, dimension (:), intent (out) :: x                  
    ! solution
    integer,             intent (in)  :: niter, nmem, nfreq 
    ! iterations
    real, dimension (size (x))        :: g                  
    ! gradient
    real, dimension (size (dat))      :: rr, gg, wt         
    ! res, CG, weight
    integer                           :: i, stat
    logical                           :: forget
    rr = -dat
    x = 0.
    wt = 1.
    forget = F         ! initialize
    do i = 1, niter  
      forget = (i > nmem)                            ! restart
      if ( forget) then
        stat = wght( rr, wt)               ! compute weighting
      end if
      rr = rr * wt                                   
      ! rr = W (Fx - d)
      stat = oper( T, F,  g, wt*rr)                  ! g  = F' W' rr
      stat = oper( F, F,  g, gg)                     ! gg = Fg
      gg = gg * wt                                   ! gg = W F g
      if ( forget) then
        forget = ( mod( i, nfreq) .eq. 0)   ! periodic restart
      end if
      stat = solv( forget, x, g, rr, gg)             
      ! step in x and rr
      rr = - dat
      stat = oper( F, T, x, rr)                      ! rr = Fx - d
    end do
  end subroutine 
end module 

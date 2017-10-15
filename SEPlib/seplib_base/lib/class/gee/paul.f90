module paul  
  use gee_quantile_mod
  implicit none
  real, private :: p = 2.0
  contains
  subroutine paul_init( p_in)
    integer  p_in
    p = p_in
  end subroutine 
  integer function l1 (res, weight)
    real, dimension (:)  :: res, weight
    real                 :: rbar
    rbar = gee_quantile (int(0.5*size(res)), abs (res))                 ! median
    rbar=rbar+(rbar-minval(abs(res)))
    weight =sqrt(p) * sqrt (1. + (res/rbar)**p)**(1/p-1)
    l1 = 0
  end function 
  integer function cauchy (res, weight)
    real, dimension (:)  :: res, weight
    real                 :: rbar
    rbar = gee_quantile (int(0.5*size(res)), abs (res))                 ! median
    rbar=rbar+(rbar-minval(abs(res)))
    weight =p * (1. + (res/rbar)**p)**(1/p-1)
    cauchy = 0
  end function 
end module 

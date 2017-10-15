! Masked Internal Convolution, Adjoint is the Filter
module micaf1 
  use classutil

  real,    dimension (:), pointer, private :: x
  logical, dimension (:), pointer, private :: mx
  integer, private                         :: nx, na, n1, n2

contains

  subroutine micaf1_init (maskx, input, nb, lag)
    logical, dimension (:), pointer :: maskx
    real,    dimension (:), pointer :: input
    integer, intent (in)            :: nb, lag

    x => input
    nx = 1 + size (maskx) ; n2 = nx - lag
    na = 1 + nb           ; n1 = na - lag
    mx => maskx (n1 : n2)
  end subroutine micaf1_init

  function micaf1_op (adj, add, b, y) result (stat)
    integer              :: stat
    logical, intent (in) :: adj, add
    real, dimension (:)  :: b, y

    integer              :: ib

    call adjnull (adj, add, b, y)
    
    do ib = 1, size (b)
       if (adj) then
          b (ib) = b (ib) + sum (y (n1:n2) * x (na-ib:nx-ib), mask = mx)
       else
          where (mx) y (n1:n2) = y (n1:n2) + x (na-ib:nx-ib) * b (ib)
       end if
    end do

    stat = 0
  end function micaf1_op

end module micaf1


module tridiag
  ! tridiagonal equation solver
  implicit none
  complex, dimension (:),   allocatable, private :: cf, ce
  real,    dimension (:),   allocatable, private :: f, e
  integer,                               private :: n

  interface tris
     module procedure ctris
     module procedure rtris_scalar
     module procedure rtris_vector
     module procedure rtris_back
     module procedure vtris
  end interface

contains

  subroutine ctris_init (nd)
    integer, intent (in) :: nd
    n = nd; allocate (cf (n), ce (n))
  end subroutine ctris_init

  subroutine rtris_init (nd)
    integer, intent (in) :: nd
    n = nd; allocate (f (n), e (n))
  end subroutine rtris_init

  subroutine ctris (endl, a, b, c, endr, d, q)
    complex,                intent (in)  :: endl, a, b, c, endr
    complex, dimension (:), intent (in)  :: d
    complex, dimension (:), intent (out) :: q

    integer                              :: i
    complex                              :: den

    ce(1) = -a/endl;   cf(1) = d(1)/endl
    do i= 2, n-1
       den = b+c*ce(i-1);   ce(i) = -a/den;   cf(i) = (d(i)-c*cf(i-1))/den
    end do
    q(n) = (d(n)-c*cf(n-1)) / (endr+c*ce(n-1))
    do i= n-1, 1, -1
       q(i) = ce(i) * q(i+1) + cf(i)
    end do
  end subroutine ctris

  subroutine rtris_vector (el, a, b, c, er, d, q)
    real,                intent (in)  :: el, er
    real, dimension (:), intent (in)  :: d, a, b, c
    real, dimension (:), intent (out) :: q

    integer                           :: i
    real                              :: den, endl, endr

    endl = c(1)*el + b(1)
    endr = a(n)*er + b(n)
    e(1) = -a(1)/endl;   f(1) = d(1)/endl
    do i= 2, n-1
       den = b(i)+c(i)*e(i-1);   
       e(i) = -a(i)/den;   f(i) = (d(i)-c(i)*f(i-1))/den
    end do
    q(n) = (d(n)-c(n)*f(n-1)) / (endr+c(n)*e(n-1))
    do i= n-1, 1, -1
       q(i) = e(i) * q(i+1) + f(i)
    end do
  end subroutine rtris_vector

  subroutine rtris_back (a, b, c, d, q)
    real, dimension (:), intent (in)  :: d, a, b, c
    real, dimension (:), intent (out) :: q

    integer                           :: i
    real                              :: den

    e(n) = -c(n)/b(n);   f(n) = d(n)/b(n)
    do i= n-1, 2, -1
       den = b(i)+a(i)*e(i+1);   
       e(i) = -c(i)/den;   f(i) = (d(i)-a(i)*f(i+1))/den
    end do
    q(1) = (d(1)-a(1)*f(2)) / (b(1)+a(1)*e(2))
    do i= 2, n
       q(i) = e(i) * q(i-1) + f(i)
    end do
  end subroutine rtris_back

  subroutine rtris_scalar (endl, a, b, c, endr, d, q)
    real,                intent (in)  :: endl, a, b, c, endr
    real, dimension (:), intent (in)  :: d
    real, dimension (:), intent (out) :: q

    integer                           :: i
    real                              :: den

    e(1) = -a/endl;   f(1) = d(1)/endl
    do i= 2, n-1
       den = b+c*e(i-1);   e(i) = -a/den;   f(i) = (d(i)-c*f(i-1))/den
    end do
    q(n) = (d(n)-c*f(n-1)) / (endr+c*e(n-1))
    do i= n-1, 1, -1
       q(i) = e(i) * q(i+1) + f(i)
    end do
  end subroutine rtris_scalar

  ! tridiagonal solver for symmetric systems
  ! (Golub and van Loan, p. 157)
  ! d i diagonal
  ! e is superdiagonal
  ! b is overwritten
  subroutine vtris (d, e, b)
    real, dimension (:) :: d, e, b

    integer             :: k, n
    real                :: t

    n = size (d)
    do k = 2, n 
       t = e (k-1) ; e (k-1) = t/d(k-1); d (k) = d (k) - t*e (k-1)
    end do
    do k = 2, n 
       b (k) = b(k) - e (k-1)*b(k-1)
    end do
    b(n) = b(n)/d(n)
    do k = n-1, 1, -1
       b(k) = b(k)/d(k)-e(k)*b(k+1)
    end do
  end subroutine vtris

  subroutine rtris_close ()
    deallocate (f, e)
  end subroutine rtris_close

  subroutine ctris_close ()
    deallocate (cf, ce)
  end subroutine ctris_close

end module tridiag



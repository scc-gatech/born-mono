module ptridiag_mod

  implicit none

  complex, dimension(:,:), allocatable, private :: dd,oo
  integer, private :: nm !! model size

contains

  !----------------------------------------------------------------  
  subroutine ptridiag_init(nth,nm_in)
    integer, intent(in) :: nth,nm_in

    nm = nm_in
    allocate(dd(nth,nm),oo(nth,nm))

  end subroutine ptridiag_init
  !----------------------------------------------------------------
  !! tridiagonal solver for symmetric systems
  !!             (Golub and van Loan, p. 157)
  !! solves (LtL + e^2 AtA) m = Lt d
  !!
  !!    (LtL + e^2 AtA) is tridiagonal and defined by
  !!                                    diag and offd
  !!
  !!  m  enters as                    Lt d   
  !!      exits as (LtL + e^2 AtA)^-1 Lt d
  !!
  !----------------------------------------------------------------  
  subroutine ptridiag(ith,diag,offd,m)
    integer, intent(in) :: ith
    complex, dimension(:,:), pointer    :: diag,offd
    complex, dimension(:,:), intent(inout) :: m
    integer :: k
    complex :: t

    dd(ith,1) = diag(ith,1)
    do k=2,nm
       t=offd(ith,k-1)
       oo(ith,k-1)=t/diag(ith,k-1)
       dd(ith,k  )=  diag(ith,k  ) - t*oo(ith,k-1)
    end do

    do k=2,nm
       m(ith,k) = m(ith,k) - oo(ith,k-1)*m(ith,k-1)
    end do

    m(ith,nm) = m(ith,nm)/dd(ith,nm)
    do k = nm-1, 1, -1
       m(ith,k) = m(ith,k)/dd(ith,k)-oo(ith,k)*m(ith,k+1)
    end do
  end subroutine ptridiag
  !----------------------------------------------------------------
  subroutine ptridiag_close()
    deallocate(dd,oo)
  end subroutine ptridiag_close
  !----------------------------------------------------------------
end module ptridiag_mod

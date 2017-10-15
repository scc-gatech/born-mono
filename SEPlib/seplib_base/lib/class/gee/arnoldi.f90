module arnoldi_mod

  use ddot_mod
  use llist_mod

  implicit none

contains

  subroutine arnoldi (grads, g, go)

    type (list)                  :: grads
    real, dimension (:), pointer :: g
    logical, intent (in)         :: go

    real, dimension (:), pointer :: gi
    integer			 :: i, n
    double precision		 :: beta
    double precision, dimension (:), allocatable	:: alpha
    double precision, parameter	 :: eps = 1.d-20

    n = llist_depth (grads)

    allocate (alpha (n))

    call llist_rewind (grads)
    do i = 1, n
       call llist_down (grads, gi, beta)
       alpha (i) = ddot (g, gi) / beta
    end do

    call llist_rewind (grads)
    do i = 1, n
       call llist_down (grads, gi, beta)
       g = g - alpha (i) * gi
    end do

    deallocate (alpha)

    beta = ddot (g, g)
    if (beta > eps .and. go) call llist_add (grads, g, beta)

  end subroutine arnoldi

  subroutine arnoldi_diag (grads, res)

    type (list)                         :: grads
    real, dimension (:), intent (out)	:: res

    integer				:: i
    real, dimension (:), pointer	:: g
    double precision		        :: gn

    res = 0.
    call llist_rewind (grads)
    do i = 1, llist_depth (grads)
       call llist_down (grads, g, gn)
       res = res + dprod (g, g) / gn
    end do

  end subroutine arnoldi_diag

  subroutine arnoldi_mat (grads, res)

    type (list)                         :: grads
    real, dimension (:,:), intent (out)	:: res

    integer				:: i, j
    real, dimension (:), pointer	:: g
    double precision		        :: gn

    res = 0.
    call llist_rewind (grads)
    do i = 1, llist_depth (grads)
       call llist_down (grads, g, gn)
       do j = 1, size (g)
          res (:,j) = res (:,j) + dprod (g (j), g) / gn
       end do
    end do

  end subroutine arnoldi_mat

end module arnoldi_mod


!!$=head1 NAME
!!$
!!$gauss - solve system by gaussian elimination
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call gauss_init(size)>
!!$
!!$C<call gauss_solver(a,b,x)>
!!$
!!$C<call gaus_close()>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$
!!$item size - integer
!!$
!!$       number of elements in array
!!$
!!$=item a    - C<real(size,size)>
!!$
!!$       matrix
!!$
!!$=item b    - C<real(size)>
!!$
!!$       data
!!$
!!$=item x    - C<real(size)>
!!$
!!$       model
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Solve system of equations by gaussian elimination
!!$
!!$=head1 SEE ALSO
!!$
!!$L<solver>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module gauss

  implicit none
  integer,                            private :: n
  real, dimension (:,:), allocatable, private :: d
  real, dimension (:),   allocatable, private :: drow

contains

  subroutine gauss_init (size)
    integer, intent (in) :: size
    n = size
    if (.not. allocated (d)) allocate (d (n,n+1), drow (n+1))
  end subroutine gauss_init

  function gauss_solve (a,b,x) result (code)
    logical                                :: code
    real,    dimension (:,:), intent (in)  :: a
    real,    dimension (:),   intent (in)  :: b
    real,    dimension (:),   intent (out) :: x

    integer, dimension (1)                 :: mloc
    integer                                :: k

    d (:,1:n) = a
    d (:,n+1) = b

    ! triangularization phase
    do k = 1, n
       ! pivoting
       mloc = maxloc (abs(d(k:n,k)))
       drow (k:n+1) = d (k,k:n+1)
       d (k, k:n+1) = d (k-1+mloc(1), k:n+1)
       d (k-1+mloc(1), k:n+1) = drow (k:n+1)

       if (abs(d(k,k)) < epsilon (d(1,1))) then
          code = .false.
          return
       end if
       d (k,k:n+1)       = d (k,k:n+1) / d (k,k)

       d (k+1:n,k+1:n+1) = &
       d (k+1:n,k+1:n+1) - spread (d (k,k+1:n+1), 1, n-k  ) &
       *                   spread (d (k+1:n,k  ), 2, n-k+1)
    end do

    ! back substitution phase
    do k = n, 1, -1
       x (k) = d (k, n+1) - sum (d(k,k+1:n)*x(k+1:n))
    end do

    code = .true.
  end function gauss_solve

  subroutine gauss_close ()
    if (allocated (d)) deallocate (d, drow)
  end subroutine gauss_close

end module gauss

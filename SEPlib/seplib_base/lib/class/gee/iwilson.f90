module iwilson  
! Wilson's factorization - inverse square root
  use helicon
  use polydiv
  implicit none
  integer,                          private :: n
  real, dimension( :), allocatable, private :: auto, bb, cc, b, c    
  contains
  subroutine iwilson_init( nmax)
    integer, intent (in) :: nmax
    n = nmax
    allocate ( auto( 2*n-1), bb( 2*n-1), cc( 2*n-1), b(n), c(n))
  end subroutine 
  subroutine iwilson_factor( niter, s0, ss, a0, aa)
    integer,       intent( in)  :: niter ! Newton iterations
    real,          intent( in)  :: s0    ! autocorrelation zero lag
    type( filter), intent( in)  :: ss    
    ! autocorrelation, other lags
    real,          intent( out) :: a0    ! factor, zero lag
    type( filter)               :: aa    ! factor, other lags
    real                        :: eps
    integer                     :: i, stat
    real, dimension (size (aa%flt)) :: rand
!    auto = 0.;  auto( n) = s0;  b( 1) =1.       # initialize
!    auto( n+ss%lag) =  ss%flt                   # autocorrelation
!    auto( n-ss%lag) =  ss%flt                   # symmetrize input auto.
    call polydiv_init(2*n-1, aa)
    auto = 0.
    auto( n) = 1.
    b( 1) = 1.
    stat= polydiv_lop(.false.,.false., auto, bb)  ! bb = S*A
    stat= polydiv_lop( .true.,.false., cc,   bb)  ! cc = S*AA'
    auto = cc
    call random_number (rand)
    aa%flt = aa%flt*(1.+0.25*rand)
    call helicon_init( aa)                      ! multiply polynoms
    do i = 1, niter  
      stat= helicon_lop(.false.,.false., auto, bb)  ! bb = S*A
      stat= helicon_lop( .true.,.false., cc,   bb)  ! cc = S*AA'
      b( 2:n) = -0.5*( cc( n+1:2*n-1  ) +cc( n-1:1    :-1)) / cc( n)  
! b = plusside(3-cc)
      eps = maxval( abs( b(2:n)))
      write (0,*) i, eps ! "L1 norm"
      if ( eps < epsilon( a0)) then
        exit                  ! convergence
      end if
      stat= helicon_lop( .false., .false., b, c)      ! c = A b
      aa%flt = c( 1+aa%lag)                           ! put on helix
    end do 
    a0 = sqrt( cc( n))
  end subroutine 
  subroutine iwilson_close ()
    deallocate( auto, bb, cc, b, c)
  end subroutine 
end module 

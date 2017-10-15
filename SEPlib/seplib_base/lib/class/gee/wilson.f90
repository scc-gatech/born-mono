!!$=head1 NAME
!!$
!!$wilson - wilson's factorization
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call wilson_init(nmax)>
!!$
!!$C<call wilson_factor(niter,s0,ss,a0,aa)>
!!$
!!$C<call wilson_close()>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item nmax - integer
!!$
!!$      maximum number of space needed (n1*10 good number)
!!$
!!$=item s0   - real
!!$
!!$      zero lag value of input
!!$
!!$=item ss   - type(filter)
!!$
!!$      auto correlation
!!$
!!$=item a0   - real
!!$
!!$      Output zero lag value
!!$
!!$=item aa   - type(filter)
!!$
!!$      Minimum  phase filter
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Find a minumum phase filter with the given auto-correaltion function
!!$
!!$=head1 SEE ALSO
!!$
!!$L<cross_wilson>,L<lapfac>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module wilson  
! Wilson's factorization
  use helicon
  use polydiv
  implicit none
  integer,                          private :: n
  real, dimension( :), allocatable, private :: auto, bb, cc, b, c    
  contains
  subroutine wilson_init( nmax)
    integer, intent (in) :: nmax
    n = nmax
    allocate ( auto( 2*n-1), bb( 2*n-1), cc( 2*n-1), b(n), c(n))
  end subroutine 
  subroutine wilson_factor( niter, s0, ss, a0, aa, verb)
    integer,       intent( in)  :: niter ! Newton iterations
    real,          intent( in)  :: s0    ! autocorrelation zero lag
    type( filter), intent( in)  :: ss    
    ! autocorrelation, other lags
    real,          intent( out) :: a0    ! factor, zero lag
    type( filter)               :: aa    ! factor, other lags
    logical,       intent( in)  :: verb
    optional                    :: verb
    real                        :: eps
    integer                     :: i, stat
    auto = 0.
    auto( n) = s0
    b( 1) =1.       ! initialize
    auto( n+ss%lag) =  ss%flt                   ! autocorrelation
    auto( n-ss%lag) =  ss%flt                   
    ! symmetrize input auto.
    call helicon_init( aa)                      ! multiply polynoms
    call polydiv_init( 2*n-1, aa)               ! divide   polynoms
    do i = 1, niter  
      stat= polydiv_lop(.false.,.false., auto, bb)  ! bb = S/A
      stat= polydiv_lop( .true.,.false., cc,   bb)  ! cc = S/(AA')
      b( 2:n) = 0.5*( cc( n+1:2*n-1  ) +cc( n-1:1    :-1)) / cc( n)   
! b = plusside(1+cc)
      eps = maxval( abs( b(2:n)))
! "L1 norm"
      if (present (verb)) then
        if (verb) then
          write (0,*) i, eps
        end if
      end if
      stat= helicon_lop( .false., .false., b, c)      ! c = A b
      aa%flt = c( 1+aa%lag)                           ! put on helix
      if ( eps < epsilon( a0)) then
        exit                  ! convergence
      end if
    end do 
    a0 = sqrt( cc( n))
  end subroutine 
  subroutine wilson_close ()
    deallocate( auto, bb, cc, b, c)
    call polydiv_close()
  end subroutine 
end module 

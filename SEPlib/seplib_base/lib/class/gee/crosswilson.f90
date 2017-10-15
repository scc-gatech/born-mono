!!$=head1 NAME
!!$
!!$cross_wilson - wilson factorization of cross-correlation
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call cross_factor( niter, s0, ss, a0, aa, b0, bb) >
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item niter  - integer
!!$
!!$      maximum number of iteratons
!!$
!!$=item s0     - real
!!$
!!$      Initial zero lag coef. value
!!$
!!$=item ss     - type(filter)
!!$
!!$      Input cross correlation
!!$
!!$=item a0,b0  - real
!!$
!!$      Zero lag coef
!!$
!!$=item aa,bb  - type(filter)
!!$
!!$      Output minimum phase filters
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Factorize a cross corelation
!!$
!!$
!!$=head1 SEE ALSO
!!$
!!$L<wilson>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module cross_wilson  
! Wilson's factorization of cross-correlation
  use helicon
  use polydiv
  implicit none
  integer, parameter,         private :: n = 1024        
  real,    dimension( 2*n-1), private :: cross, ab, cc    
  real,    dimension( n),     private :: b, c            
  contains
  subroutine cross_factor( niter, s0, ss, a0, aa, b0, bb)
    integer,                intent( in)     :: niter      
    ! Newton iterations
    real,                   intent( in)     :: s0         
    ! in: cross-correlat.
    type( filter),          intent( in)     :: ss         !
    real,                   intent( in out) :: a0, b0     
    ! out: min. phase
    type( filter),          intent( out)    :: aa, bb     !
    real                                    :: eps
    integer                                 :: i, stat
    cross = 0.
    cross( n) = s0
    b( 1) = 1.                ! initialize
    cross( n+ss%lag) = ss%flt                             
    ! cross-correlation
    do i = 1, niter  
      call   polydiv_init( 2*n-1, aa)                    
      ! divide   polynoms
      stat = polydiv_lop( .false., .false., cross, ab)   ! ab = S/A
!       ab = ab/(a0*b0)                                   # scale
      call   polydiv_init( 2*n-1, bb)                    
      ! divide   polynoms
      stat = polydiv_lop(  .true., .false., cc,    ab)   
      ! cc = S/(AB')
      b( 2:n) = cc( n+1:2*n-1)/cc( n)                    
      ! b = + side
!       b( 1)   = 0.5*(cc( n) + 1.)                       #     of (1 + cc)
      eps = maxval(abs(b(2:n)))
      call   helicon_init( aa)                           
      ! mutliply polynoms
      stat = helicon_lop( .false., .false., b, c)        ! c = A b
!       c = c*a0; a0 = c( 1); aa%flt = c(1+aa%lag)/a0     # scale
      aa%flt = c(1+aa%lag)
      b( 2:n) = cc( n-1:1:-1)/cc( n)                     
      ! b = - side
      eps = max(maxval(abs(b(2:n))),eps)
      write( 0,*) i, eps                                 ! "L1 norm"
      if ( eps < epsilon(a0)) then
        exit                        ! convergence
      end if
      call   helicon_init( bb)                           
      ! mutliply polynoms
      stat = helicon_lop( .false., .false., b, c)        ! c = B b
!       c = c*b0; b0 = c( 1); bb%flt = c(1+bb%lag)/b0     # scale
      bb%flt = c(1+bb%lag)
    end do 
    call polydiv_close()
  end subroutine 
end module 

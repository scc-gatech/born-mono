!!$=head1 NAME
!!$
!!$npef  - find non stationary prediction error filter
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call find_pef(dd,aa,rr,niter,eps,nh)>
!!$
!!$=head1 INPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item dd  -  C<real(:)>
!!$
!!$      Input data
!!$
!!$=item niter - integer
!!$
!!$      Number of itterations
!!$
!!$=item rr   - type(nfilter)
!!$
!!$      Filter (aa) regularizer
!!$
!!$=item eps  -  real
!!$
!!$      Epsilon to apply to filter estimation problem
!!$      larger mean more regularized
!!$
!!$=back
!!$
!!$=head1 OUTPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item  aa - type(filter)
!!$
!!$       output filter
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$ Find a non-stationary prediction-error filter (helix magic)
!!$
!!$=head1 SEE ALSO
!!$
!!$L<pef>,L<nhconest>,L<solver>,L<cgstep>,L<nhelix>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module npef  
! Estimate non-stationary PEF
  use nhconest
  use npolydiv2
  use cgstep_mod
  use solver_mod
  implicit none
  contains
  subroutine find_pef( dd, aa, rr, niter, eps, nh)
    integer,          intent( in)    :: niter, nh      
    ! number of iterations
    real,             intent( in)    :: eps               ! epsilon
    type( nfilter)                   :: aa             
    ! estimated PEF output.
    type( nfilter),   intent( in)    :: rr             
    ! roughening filter.
    real,    dimension(:), pointer   :: dd             ! input data
    integer                          :: ip, ih, np, nr 
    ! filter lengths
    real, dimension (:), allocatable :: flt               
    ! np*na filter coefs
    np = size( aa%hlx)                        ! data   length
    nr = np*nh
    allocate( flt( nr))
    call nhconest_init( dd, aa, nh)
    call npolydiv2_init( nr, rr)
    call solver_prec( nhconest_lop, cgstep, x= flt, dat= -dd, niter=&
      & niter,prec= npolydiv2_lop, nprec= nr, eps= eps)
    call   cgstep_close()
    call npolydiv2_close()
    call nhconest_close()
    do ip = 1, np
      do ih = 1, size( aa%hlx(ip)%lag)
        aa%hlx( ip)%flt( ih) = flt( (ip-1)*nh + ih)
      end do
    end do 
    deallocate( flt)
  end subroutine 
end module 

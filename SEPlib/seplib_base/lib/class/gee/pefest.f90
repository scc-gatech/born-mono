!!$=head1 NAME
!!$
!!$pefest  - find prediction error filter, avoid bursty noise
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call pefest1(dd,aa,niter)>
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
!!$ Find prediction-error filter (helix magic) avoid bursts
!!$
!!$=head1 SEE ALSO
!!$
!!$L<npef>,L<hconest>,L<solver>,L<cgstep>,L<pef>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!$
module pefest  
! Estimate a PEF avoiding zeros and bursty noise on input.
  use helicon
  use misinput
  use pef
  use gee_quantile_mod
  implicit none
  contains
  subroutine pefest1( niter, yy, aa)
    integer, intent( in)         :: niter
    real, dimension( :), pointer :: yy
    type( filter)                :: aa
    real, dimension(:),allocatable  :: rr
    real                         :: rbar
    integer                      :: stat
    allocate(rr(size(yy)))
    call helicon_init( aa)                          ! starting guess
    stat = helicon_lop( .false., .false., yy, rr)
    rbar = gee_quantile( size( yy)/3, abs( rr))         
    ! rbar=(r safe below rbar)
    where( aa%mis)
      yy = 0.
end where
    call find_mask(( yy /= 0. .and. abs( rr) < 5 * rbar), aa)
    call find_pef( yy, aa, niter)  
    deallocate(rr)
  end subroutine 
end module 

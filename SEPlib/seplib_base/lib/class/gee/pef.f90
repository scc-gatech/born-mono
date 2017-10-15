!!$=head1 NAME
!!$
!!$pef  - find prediction error filter
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call find_pef(dd,aa,niter)>
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
!!$ Find prediction-error filter (helix magic)
!!$
!!$=head1 SEE ALSO
!!$
!!$L<npef>,L<hconest>,L<solver>,L<cgstep>,L<pefest>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module pef  
! Find prediction-error filter (helix magic)
  use hconest
  use cgstep_mod
  use solver_smp_mod
  implicit none
  contains
  subroutine find_pef( dd, aa, niter)
    integer,          intent( in)  :: niter        
    ! number of iterations
    type( filter)                  :: aa           ! filter
    real,    dimension(:), pointer :: dd           ! input data
    call hconest_init( dd, aa)
    call solver_smp(m=aa%flt, d=-dd, Fop=hconest_lop, stepper=cgstep,&
      & niter=niter, m0=aa%flt)
    call cgstep_close()
  end subroutine 
end module 

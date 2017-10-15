!!$
!!$=head1 NAME
!!$
!!$cgmeth  - conjugate gradient method
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call cgtest( x, yy, rr, fff, niter)>
!!$
!!$=head1 INPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item yy   - C<real(:)>
!!$
!!$=item rr   - C<real(:)>
!!$
!!$=item fff  - C<real(:)>
!!$
!!$=back
!!$
!!$=head1 OUTPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item x  - C<real(:)>
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$ setup of conjugate gradient descent, minimize  SUM rr(i)**2
!!$             nx
!!$ rr(i)  =   sum fff(i,j) * x(j)  -  yy(i)
!!$            j=1
!!$
!!$
!!$=head1 SEE ALSO
!!$
!!$L<matmult>, L<cgstep>,L<solver>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!$
module cgmeth  
  use matmult_mod
  use cgstep_mod
  use solver_tiny_mod
implicit none
  contains
! setup of conjugate gradient descent, minimize  SUM rr(i)**2
!             nx
! rr(i)  =   sum fff(i,j) * x(j)  -  yy(i)
!            j=1
  subroutine cgtest( x, yy, rr, fff, niter)
    real, dimension (:), intent (out) :: x, rr
    real, dimension (:), intent (in)  :: yy
    real, dimension (:,:), pointer    :: fff
    integer,             intent (in)  :: niter
    call matmult_init( fff)
    call solver_tiny( m=x, d=yy,         Fop=matmult_lop, stepper=cgstep,&
      &         niter=niter, resd=rr)
    call cgstep_close ()
  end subroutine 
end module 

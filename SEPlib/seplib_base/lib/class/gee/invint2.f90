!!$=head1 NAME
!!$
!!$invint2 - Inverse linear interpolation;
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call invint1(niter,coord,ord,o1,d1,mm,mmov,eps,aa,doprec)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item niter - integer
!!$
!!$      Number of itterations
!!$
!!$=item coord -  C<real(:)>
!!$
!!$      Coordinates
!!$
!!$=item o1,d1 -  real
!!$
!!$      First model position and sampling
!!$
!!$=item ord   -  C<real(:)>
!!$
!!$      Data values
!!$
!!$=item mm    -  C<real(:)>
!!$
!!$      Output model
!!$
!!$=item mmov  -  C<real(:,:)>
!!$
!!$      Model movie
!!$
!!$=item eps   -  real
!!$
!!$      Epsilon value if doing preconditioning
!!$
!!$=item aa    -  type(filter)
!!$
!!$      Preconditioning operator
!!$
!!$=item prec  -  logical
!!$
!!$      Whether or not to do preconditioning
!!$
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Perform inverse linear interpolation
!!$
!!$
!!$=head1 SEE ALSO
!!$
!!$L<lint1>,L<helicon>,L<polydiv>,L<solver>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!$
module invint2  
! Inverse linear interpolation
  use lint1
  use helicon                           
  ! regularized    by helix   filtering
  use polydiv                           
  ! preconditioned by inverse filtering
  use cgstep_mod 
  use solver_reg_mod
  use solver_prc_mod
  implicit none
  contains
  subroutine invint( niter, coord,ord, o1,d1, mm,mmov, eps, aa, method&
    &)
    logical,                  intent( in)  :: method
    integer,                  intent( in)  :: niter
    real,                     intent( in)  :: o1, d1, eps
    real,    dimension( :),   intent( in)  :: ord  
    type( filter),            intent( in)  :: aa
    real,    dimension( :),   intent( out) :: mm             
    real,    dimension( :,:), intent( out) :: mmov         
    ! model movie
    real,    dimension( :),   pointer      :: coord        
    ! coordinate
    call lint1_init( o1, d1, coord)
    if ( method) then
! preconditioning
      call polydiv_init( size(mm), aa)
      call solver_prc( Fop=lint1_lop, stepper=cgstep, niter=niter, m=mm&
        &, d=ord,Sop=polydiv_lop, nSop=size(mm), eps=eps, mmov=mmov,&
        & verb=.true.)
      call polydiv_close()
    else
! regularization
      call helicon_init( aa)
      call solver_reg( Fop=lint1_lop, stepper=cgstep, niter=niter, m=mm&
        &, d=ord,Aop=helicon_lop, nAop=size(mm), eps=eps, mmov=mmov,&
        & verb=.true.)
    end if 
    call cgstep_close()
  end subroutine 
end module 

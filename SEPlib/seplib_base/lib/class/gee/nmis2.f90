!!$=head1 NAME
!!$
!!$nmis2 - Fill in missing data using non-stationary filter
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call mis1(niter,xx,aa,known,doprec)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item  niter - integer
!!$
!!$       number of iterations
!!$
!!$=item  xx    -real
!!$
!!$       fitting variable
!!$
!!$=item  aa    -type(nfilter)
!!$
!!$       filter to apply
!!$
!!$=item  known -C<logical(:)>
!!$
!!$       Known data
!!$
!!$=item  doprec -logical
!!$
!!$       Whether or not to run preconditioning
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$fill in missing data by minimizing power out of a given non-stationary filter
!!$ by helix magic works in any number of dimensions
!!$
!!$=head1 SEE ALSO
!!$
!!$L<mis2>,L<msmis>,L<mask1>,L<helicon>,L<solver>,L<nhelix>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!
module nmis2  
  use nhelicon
  use npolydiv
  use mask1
  use cgstep_mod
  use solver_mod
implicit none
  contains
! fill in missing data by minimizing power out of a given filter.
! by helix magic works in any number of dimensions
  subroutine mis1( niter, xx, aa, known, precon)
    logical,                intent( in)     :: precon
    integer,                intent( in)     :: niter
    type( nfilter),         intent( in)     :: aa
    logical, dimension( :), intent( in)     :: known
    real,    dimension( :), intent( in out) :: xx
    real,    dimension( :), allocatable     :: dd
    logical, dimension( :), pointer         :: kk
    integer                                 :: nx
    nx = size( xx)
    if ( precon) then
!  KP p = K y,   m = P p
      allocate( kk( nx))
      kk = known
      call mask1_init( kk)
      call npolydiv_init( nx, aa)
      call solver_prec( mask1_lop, cgstep, niter= niter, x= xx, dat= xx&
        &,prec= npolydiv_lop, nprec= nx, eps= 0.)
      call npolydiv_close()
      deallocate( kk)
    else
!  KA m = 0
      allocate( dd( nx))
      dd = 0.
      call nhelicon_init( aa)
      call solver( nhelicon_lop, cgstep, niter= niter, x= xx, dat= dd&
        &,known = known, x0= xx)
      deallocate( dd)
    end if 
    call cgstep_close( )
  end subroutine 
end module 

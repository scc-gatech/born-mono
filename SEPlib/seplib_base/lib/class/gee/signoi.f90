!!$=head1 NAME
!!$
!!$signoi - signal and noise separation
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call signoi_init(nn,ss,niter,nd,eps)>
!!$
!!$C<ierr=signoi_lop(adj,add,data,sign)>
!!$
!!$C<call signoi_close()>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item  nn   type(filter)
!!$
!!$       Noise filter
!!$
!!$=item  ss   type(filter)
!!$
!!$       Signal filter
!!$
!!$=item niter integer
!!$
!!$      Number of itterations
!!$
!!$=item nd    integer
!!$
!!$      Number of data points
!!$
!!$=item eps   real
!!$
!!$      Epsilon
!!$
!!$=item adj,add,data,sign
!!$
!!$      Standard operator parameters
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Signal an noise separation Claerbout style
!!!$
!!$=head1 SEE ALSO
!!$
!!$L<pef>,L<helicon>,L<solver_prec>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module signoi  
  use classutil
  use helicon
  use polydiv
  use solver_prc_mod
  use cgstep_mod
  implicit none
  integer,   private                       :: i 
  integer, private :: niter 
  integer, private :: nd 
  real, private :: eps 
  type( filter) ,private,  dimension( :), pointer  :: nn 
  type( filter) ,private,  dimension( :), pointer  :: ss 
  real ,private,  dimension (:), allocatable      :: dd 
  contains
  subroutine signoi_init ( nn_in,ss_in,niter_in,nd_in,eps_in )
    type( filter) ,   dimension( :), pointer :: nn_in 
    type( filter) ,   dimension( :), pointer :: ss_in 
    integer    :: niter_in 
    integer    :: nd_in 
    real    :: eps_in 
    nn => nn_in 
    ss => ss_in 
    niter = niter_in 
    nd = nd_in 
    eps = eps_in 
    if (.not. allocated(dd)) then
      allocate(dd ( nd)) 
    end if
    i = 0
  end subroutine
  function signoi_lop ( adj, add, data, sign) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: data,sign 
    call adjnull (adj,add,data,sign )
    call signoi_lop2(adj,add,data,sign )
    stat=0
  end function 
  subroutine signoi_lop2(adj,add,data,sign)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: data 
    real, dimension (:)  :: sign 
    integer stat1
    i = i + 1
    call helicon_init (nn (i))
    call polydiv_init (nd, ss (i)) 
    stat1 = helicon_lop (.false., .false., data, dd)
    call solver_prc(m=sign, d=dd, Fop=helicon_lop, stepper=cgstep&
      &,niter=niter, Sop=polydiv_lop, nSop=nd, eps=eps)
    call cgstep_close()
  end subroutine
  subroutine  signoi_close()
    deallocate( dd )
    call polydiv_close()
  end subroutine 
end module 

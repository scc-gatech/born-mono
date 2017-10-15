!!$
!!$=head1 NAME
!!$
!!$helderiv - helix derivative filter
!!$
!!$=head1 SYNOPSIS
!!$
!!$Initializer: C<call helderiv_init(n1,na)>
!!$
!!$Operator   : C<ierr=helderiv_lop(adj,add,pp,qq)>
!!$
!!$Close      : C<call helderiv_close()>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item  n1 -  int
!!$
!!$       number of samples in n1
!!$
!!$=item  na -  int
!!$
!!$       number of coefs for filter
!!$
!!$=item adj -  logcal
!!$
!!$      wheter or not run the adjoint
!!$
!!$=item add -  logical
!!$
!!$       whether or not to add outout to data
!!$
!!$=item pp  -  C<real(:)>
!!$
!!$      model
!!$
!!$=item qq  -  C<real(:)>
!!$
!!$      data
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$ Create and apply a helix derivative filter
!!$
!!$
!!$=head1 SEE ALSO
!!$
!!$L<lapfac>, L<helicon>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module helderiv  
  use classutil
  use lapfac
  use helicon
  implicit none
  type( filter),   private  :: aa 
  contains
  subroutine helderiv_init ( n1,na,eps )
    integer, intent (in) :: n1, na
    real,    intent (in) :: eps
    aa = lapfac2( eps, n1, na) 
    call helicon_init( aa)
  end subroutine
  function helderiv_lop ( adj, add, pp, qq) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: pp,qq 
    call adjnull (adj,add,pp,qq )
    call helderiv_lop2(adj,add,pp,qq )
    stat=0
  end function 
  subroutine helderiv_lop2(adj,add,pp,qq)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: pp 
    real, dimension (:)  :: qq 
    integer stat1
    stat1 = helicon_lop( adj, .false.,  pp, qq)
  end subroutine
  subroutine  helderiv_close()
    call deallocatehelix( aa)
  end subroutine 
end module 

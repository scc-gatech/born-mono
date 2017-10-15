!!$
!!$=head1 NAME
!!$
!!$heliarr  - two helix convolutions
!!$
!!$=head1 SYNOPSIS
!!$
!!$Initialize: C<call heliarr_init(nx,aa1,aa2)>
!!$
!!$Operator  : C<ierr= heliarr_lop(adj,add, xx,yy)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item  aa1,aa2  - type(filter)
!!$
!!$       Helix filters
!!$
!!$=item  nx1      - integer
!!$
!!$       Size of model space where aa1 will be applied
!!$
!!$=item  adj,add,xx,yy
!!$
!!$       standard operator conventions;
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$An array of helix convolutions.  aa1 Will be applied to  points
!!$(1:nx1) operator two to the remaining data.
!!$
!!$=head1 SEE ALSO
!!$
!!$L<helicon>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!$
module heliarr  
! an array of two helix convolutions
  use classutil
  use helicon
  implicit none
  type (filter), private :: aa1 
  type (filter), private :: aa2 
  integer, private :: nx 
  contains
  subroutine heliarr_init ( nx_in,aa1_in,aa2_in )
    integer    :: nx_in 
    type (filter)    :: aa1_in 
    type (filter)    :: aa2_in 
    nx = nx_in 
    aa1 = aa1_in 
    aa2 = aa2_in
  end subroutine
  function heliarr_lop ( adj, add, xx, yy) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: xx,yy 
    call adjnull (adj,add,xx,yy )
    call heliarr_lop2(adj,add,xx,yy )
    stat=0
  end function 
  subroutine heliarr_lop2(adj,add,xx,yy)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: xx 
    real, dimension (:)  :: yy 
    integer stat1, stat2
    call helicon_init (aa1)
    stat1 = helicon_lop (adj, .true., xx, yy (:nx))
    call helicon_init (aa2)
    stat2 = helicon_lop (adj, .true., xx, yy (1+nx:))
  end subroutine 
  subroutine heliarr_close()
  end subroutine 
end module 

!!$
!!$=head1 NAME
!!$
!!$mshelicon - convolution using mutli scale helix filters
!!$
!!$=head1 SYNOPSIS
!!$
!!$Initializer - C<call mshelicon_init(aa)>
!!$
!!$Operator    - C<ierr=mshelicon_lop(adj,add,xx,yy)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item aa - type(msfilter)
!!$
!!$      helix filter to perform convolution with
!!$
!!$=item adj,add,xx,yy -
!!$
!!$      standard operators parameters
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$ Multi-scale onvolution, inverse to deconvolution.
!!$ Requires the filter be causal with an implicit "1." at the onset.
!!$
!!$
!!$=head1 SEE ALSO
!!$
!!$L<mshelix>,L<mshconest>,L<helicon>,L<nhelicon>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!$
module mshelicon  
! Multi-scale convolution
  use classutil
  use mshelix
  use helicon
  implicit none
  integer, private :: nx 
  integer, private :: ns 
  type( msfilter), private :: msaa 
  contains
  subroutine mshelicon_init ( nx_in,ns_in,msaa_in )
    integer    :: nx_in 
    integer    :: ns_in 
    type( msfilter)    :: msaa_in 
    nx = nx_in 
    ns = ns_in 
    msaa = msaa_in
  end subroutine
  function mshelicon_lop ( adj, add, xx, yy) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: xx,yy 
    call adjnull (adj,add,xx,yy )
    call mshelicon_lop2(adj,add,xx,yy )
    stat=0
  end function 
  subroutine mshelicon_lop2(adj,add,xx,yy)
    logical,intent(in) :: adj,add 
    real, dimension ( nx)  :: xx 
    real, dimension ( nx, ns)  :: yy 
    integer :: is, stat1
    type (filter) :: aa
    do is = 1, ns  
      call onescale( is, msaa, aa)
      call helicon_init( aa)
      stat1 = helicon_lop( adj, .true., xx, yy(:,is))
    end do
  end subroutine 
  subroutine mshelicon_close()
  end subroutine 
end module 

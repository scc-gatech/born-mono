!!$
!!$=head1 NAME
!!$
!!$mshconest - convolution using multi-scale helix filters, adjoint is filter
!!$
!!$=head1 SYNOPSIS
!!$
!!$Initializer - C<call mshconest_init(x,aa)>
!!$
!!$Operator    - C<ierr=mshconest_lop(adj,add,xx,yy)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item x  - C<real(:)>
!!$
!!$      data
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
!!$  Masked multi-scale helix convolution, adjoint is the filter
!!$
!!$=head1 SEE ALSO
!!$
!!$L<mshelix>,L<hconest>,L<mshelicon>,L<nhconest>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module mshconest  
! multi-scale helix convolution, adjoint is the filter.
  use classutil
  use mshelix
  use hconest
  use helix
  implicit none
  integer,   private                    :: nx 
  integer,   private                    :: ns 
  real ,private,     dimension(:),     pointer  :: x 
  type( msfilter), private :: msaa 
  contains
  subroutine mshconest_init ( x_in,msaa_in )
    real ,      dimension(:),     pointer :: x_in 
    type( msfilter)    :: msaa_in 
    x => x_in 
    msaa = msaa_in 
    nx = size( x)
    ns = size( msaa%lag, 2)
  end subroutine
  function mshconest_lop ( adj, add, a, y) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: a,y 
    call adjnull (adj,add,a,y )
    call mshconest_lop2(adj,add,a,y )
    stat=0
  end function 
  subroutine mshconest_lop2(adj,add,a,y)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: a 
    real, dimension (nx,ns)  :: y 
    integer  :: is, stat1
    type (filter) :: aa
    do is = 1, ns  
      call onescale (is, msaa, aa)
      call hconest_init( x, aa)
      stat1 = hconest_lop( adj, .true., a, y(:,is))
    end do
  end subroutine 
  subroutine mshconest_close()
  end subroutine 
end module 

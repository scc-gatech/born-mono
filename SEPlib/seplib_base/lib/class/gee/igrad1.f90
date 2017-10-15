!!$=head1 NAME
!!$
!!$igrad1 - 1D gradient operator
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<ierr=igrad1_lop(adj,add,xx,yy)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item  adj,add,xx,yy -
!!$
!!$       standard operator interface
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$ Calculate 1-D gradient
!!$
!!$=head1 SEE ALSO
!!$
!!$L<igrad2>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!$
module igrad1_mod
! gradient in one dimension
  use classutil
implicit none
  contains
  function igrad1_lop ( adj, add, xx, yy) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: xx,yy 
    call adjnull (adj,add,xx,yy )
    call igrad1_lop2(adj,add,xx,yy )
    stat=0
  end function 
  subroutine igrad1_lop2(adj,add,xx,yy)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: xx 
    real, dimension (:)  :: yy 
    integer i
    do i= 1, size(xx)-1  
      if ( adj) then
        xx(i+1) = xx(i+1) + yy(i)           ! resembles equation (1.2)
        xx(i  ) = xx(i  ) - yy(i)
      else
        yy(i) = yy(i) + xx(i+1) - xx(i)           ! resembles equation (1.1)
      end if
    end do
  end subroutine 
  subroutine igrad1_close()
  end subroutine 
end module 

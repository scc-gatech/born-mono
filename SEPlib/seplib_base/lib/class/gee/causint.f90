!!$=head1 NAME
!!$
!!$causint - Causal integration
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<ierr=causint_lop(adj,add,x,y)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item adj,add,x,y  -
!!$
!!$      standard operator interface
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Causal integration
!!$
!!$=head1 SEE ALSO
!!$
!!$L<cdoubint>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module causint_mod
! causal integration
  use classutil
implicit none
  contains
  function causint_lop ( adj, add, xx, yy) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: xx,yy 
    call adjnull (adj,add,xx,yy )
    call causint_lop2(adj,add,xx,yy )
    stat=0
  end function 
  subroutine causint_lop2(adj,add,xx,yy)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: xx 
    real, dimension (:)  :: yy 
    integer i, n       
    real t
    n = size (xx)
    t = 0.
    if ( adj) then
      do i= n, 1, -1  
        t = t + yy(i)
        xx(i) = xx(i) + t
      end do
    else
      do i= 1, n  
        t   = t + xx(i)
        yy(i) = yy(i) + t
      end do
    end if
  end subroutine 
  subroutine causint_close()
  end subroutine 
end module 

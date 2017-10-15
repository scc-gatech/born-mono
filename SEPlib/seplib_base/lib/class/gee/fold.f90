!!$=head1 NAME
!!$
!!$fold - fold the ends inside
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<ierr=fold_lop(adj,add,x,y)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item adj,add,x,y  -
!!$
!!$      Standard operator interface
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Fold portion of y outside x back in on itself
!!$
!!$=head1 SEE ALSO
!!$
!!$L<triangle>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module fold  
! fold the ends inside
  use classutil
implicit none
  contains
  function fold_lop ( adj, add, x, y) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: x,y 
    call adjnull (adj,add,x,y )
    call fold_lop2(adj,add,x,y )
    stat=0
  end function 
  subroutine fold_lop2(adj,add,x,y)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: x 
    real, dimension (:)  :: y 
    integer                             :: nu, nr, np, nt
    nu = size (y) 
    nt = size (x)
    np = (nt + nu) /2
    nr = (nt - nu) /2 + 1
    if (adj) then
      x (nt:np+1:-1)      = y (nu-nt+np+1:nu  ) + x (nt:np+1:-1)
      x (nr-1: 1:-1)      = y (         1:nr-1) + x (nr-1: 1:-1) 
      x (nr:np)           = y                   + x (nr:np)
    else
      y                   = y                   + x (nr:np)
      y (         1:nr-1) = y (         1:nr-1) + x (nr-1: 1:-1) 
      y (nu-nt+np+1:nu  ) = y (nu-nt+np+1:nu  ) + x (nt:np+1:-1)
    end if
  end subroutine 
  subroutine fold_close()
  end subroutine 
end module 

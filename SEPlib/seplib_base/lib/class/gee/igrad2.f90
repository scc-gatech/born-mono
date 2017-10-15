!!$=head1 NAME
!!$
!!$igrad2 - 2D gradient operator
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<ierr=igrad2_init(n1,n2)>
!!$
!!$C<ierr=igrad2_lop(adj,add,xx,yy)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item n1,n2  - integer
!!$
!!$      dimensions of dataset
!!$
!!$=item adj,add,xx,yy -
!!$
!!$      standard operator interface
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$ Calculate 2-D gradient
!!$
!!$=head1 SEE ALSO
!!$
!!$L<igrad1>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!$
module igrad2  
! 2-D gradient with adjoint,  r= grad( p)
  use classutil
  implicit none
  integer, private :: n1 
  integer, private :: n2 
  contains
  subroutine igrad2_init ( n1_in,n2_in )
    integer    :: n1_in 
    integer    :: n2_in 
    n1 = n1_in 
    n2 = n2_in
  end subroutine
  function igrad2_lop ( adj, add, p, r) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: p,r 
    call adjnull (adj,add,p,r )
    call igrad2_lop2(adj,add,p,r )
    stat=0
  end function 
  subroutine igrad2_lop2(adj,add,p,r)
    logical,intent(in) :: adj,add 
    real, dimension (n1, n2)  :: p 
    real, dimension (n1,n2,2)  :: r 
    integer i,j
    do i= 1, n1-1  
      do j= 1, n2-1  
        if ( adj) then
          p(i+1,j  )  =                p(i+1,j  )  +  r(i,j,1) 
          p(i  ,j  )  =                p(i  ,j  )  -  r(i,j,1) 
          p(i  ,j+1)  =                p(i  ,j+1)  +  r(i,j,2) 
          p(i  ,j  )  =                p(i  ,j  )  -  r(i,j,2)
        else
          r(i,j,1)  =   r(i,j,1)  +  ( p(i+1,j) - p(i,j)) 
          r(i,j,2)  =                r(i,j,2)  +  ( p(i,j+1) - p(i,j))
        end if
      end do
    end do
  end subroutine 
  subroutine igrad2_close()
  end subroutine 
end module 

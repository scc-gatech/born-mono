!!$=head1 NAME
!!$
!!$igrad3 - 3D gradient operator
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
!!$=item n1,n2 - integer
!!$
!!$ dimensions of dataset
!!$
!!$=item adj,add,xx,yy -
!!$
!!$ standard operator interface
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
module igrad3_epst
! 3-D gradient with adjoint, r= grad( p)
  use classutil
  implicit none
  integer, private :: n1
  integer, private :: n2
  integer, private :: n3
  real, private :: eps_t
  contains
  subroutine igrad3_epst_init ( n1_in,n2_in,n3_in,eps_t_in )
    integer :: n1_in
    integer :: n2_in
    integer :: n3_in
    real :: eps_t_in
    n1 = n1_in
    n2 = n2_in
    n3 = n3_in
    eps_t = eps_t_in
  end subroutine
  function igrad3_epst_lop ( adj, add, p, r) result(stat)
    integer :: stat
    logical,intent(in) :: adj,add
    real,dimension(:) :: p,r
    call adjnull (adj,add,p,r )
    call igrad3_epst_lop2(adj,add,p,r )
    stat=0
  end function
  subroutine igrad3_epst_lop2(adj,add,p,r)
    logical,intent(in) :: adj,add
    real, dimension (n1, n2, n3) :: p
    real, dimension (n1,n2,n3,3) :: r
    integer i,j,t
    do i= 1, n1-1
      do j= 1, n2-1
        do t= 1, n3-1
          if ( adj) then
            p(i+1,j ,t ) = p(i+1,j ,t ) + eps_t*r(i,j,t,1)
            p(i ,j ,t ) = p(i ,j ,t ) - eps_t*r(i,j&
              &,t,1)
            p(i ,j+1,t) = p(i ,j+1,t) + r(i,j,t,2)
            p(i ,j ,t) = p(i ,j ,t) - r(i,j,t,2)
            p(i ,j,t+1) = p(i ,j,t+1) + r(i,j,t,3)
            p(i ,j ,t) = p(i ,j ,t) - r(i,j,t,3)
          else
            r(i,j,t,1) = r(i,j,t,1) + eps_t*( p(i+1,j,t) - p(i,j,t&
              &))
            r(i,j,t,2) = r(i,j,t,2) + ( p(i,j+1,t) -&
              & p(i,j,t))
            r(i,j,t,3) = r(i,j,t,3) + ( p(i,j,t+1) - p(i,j,t))
          end if
        end do
      end do
    end do




  end subroutine
  subroutine igrad3_epst_close()
  end subroutine
end module

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
!$
module igrad2_2d_reg
! 2-D gradient applied to 3-Dcube
  use classutil
  implicit none
  integer, private :: n1
  integer, private :: n2
  integer, private :: n3
  real, private :: eps_t
  contains
  subroutine igrad2_2d_reg_init ( n1_in,n2_in,eps_t_in )
    integer :: n1_in
    integer :: n2_in
    real :: eps_t_in
    n1 = n1_in
    n2 = n2_in
    eps_t = eps_t_in
  end subroutine
  function igrad2_2d_reg_lop ( adj, add, p, r) result(stat)
    integer :: stat
    logical,intent(in) :: adj,add
    real,dimension(:) :: p,r
    call adjnull (adj,add,p,r )
    call igrad2_2d_reg_lop2(adj,add,p,r )
    stat=0
  end function
  subroutine igrad2_2d_reg_lop2(adj,add,p,r)
    logical,intent(in) :: adj,add
    real, dimension (n1, n2) :: p
    real, dimension (n1,n2,2) :: r
    integer i1,i2

    do i2=1,n2
      do i1=1,n1-1
        if(.not.adj) then 
          r(i1,i2,1)=r(i1,i2,1)+eps_t*(p(i1+1,i2)-p(i1,i2))
        else
          p(i1+1,i2)=p(i1+1,i2)+eps_t*r(i1,i2,1)
          p(i1,i2)=p(i1,i2)-eps_t*r(i1,i2,1)
        end if
      end do
      if(.not. adj) then
        r(n1,i2,1)=r(n1,i2,1)+eps_t*(p(1,i2)-p(n1,i2))
      else
        p(n1,i2)=p(n1,i2)-eps_t*r(n1,i2,1)
        p(1,i2)=p(1,i2)+eps_t*r(n1,i2,1)
      end if 
    end do 

    do i2=1,n2-1
      do i1=1,n1
        if(.not.adj) then 
          r(i1,i2,2)=r(i1,i2,2)+eps_t*(p(i1,i2+1)-p(i1,i2))
        else
          p(i1,i2+1)=p(i1,i2+1)+eps_t*r(i1,i2,2)
          p(i1,i2)=p(i1,i2)-eps_t*r(i1,i2,2)
        end if
      end do
    end do 
    do i1=1,n1
      if(.not.adj) then 
        r(i1,n2,2)=r(i1,n2,2)+eps_t*(p(i1,1)-p(i1,n2))
      else
        p(i1,n2)=p(i1,n2)-eps_t*r(i1,n2,2)
        p(i1,1)=p(i1,1)+eps_t*r(i1,n2,2)
      end if
    end do
  end subroutine
  subroutine igrad2_2d_reg_close()
  end subroutine
end module

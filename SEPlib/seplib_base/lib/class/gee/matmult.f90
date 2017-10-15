!!$
!!$=head1 NAME
!!$
!!$matmult - matrix multiplication
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call matmult_init(bb)>
!!$
!!$C<ierr=matmult_lop(adj,add,model,data)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item  bb  C<real(:,:)>
!!$
!!$       Matrix
!!$
!!$=item adj,add,model,data
!!$
!!$      Standard operator interface
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Matrix multiplication
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!$
module matmult_mod
! matrix multiply and its adjoint
  use classutil
  implicit none
  real ,private,  dimension (:,:), pointer  :: bb 
  contains
  subroutine matmult_init ( bb_in )
    real ,   dimension (:,:), pointer :: bb_in 
    bb => bb_in
  end subroutine
  function matmult_lop ( adj, add, x, y) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: x,y 
    call adjnull (adj,add,x,y )
    call matmult_lop2(adj,add,x,y )
    stat=0
  end function 
  subroutine matmult_lop2(adj,add,x,y)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: x 
    real, dimension (:)  :: y 
    integer ix, iy
    do ix= 1, size(x)
      do iy= 1, size(y)
        if ( adj) then
          x(ix) = x(ix) + bb(iy,ix) * y(iy)
        else
          y(iy) = y(iy) + bb(iy,ix) * x(ix)
        end if
      end do
    end do
  end subroutine 
  subroutine matmult_close()
  end subroutine 
end module 

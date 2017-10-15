!!$=head1 NAME
!!$
!!$lint1  -  linear 1D interpolation
!!$
!!$=head1 SYNOPSIS
!!$
!!$Initializer : C<call lint1_init(o1,d1,coordinate)>
!!$
!!$Operator    : C<ierr=lint1_lop(adj,add,model,data)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item o1 -  real
!!$
!!$      first sample of model space
!!$
!!$=item d1 -  real
!!$
!!$      sampling of model space
!!$
!!$=item coordinte - C<real(:)>
!!$
!!$      location of data
!!$
!!$=item adj,add,model,data -
!!$
!!$       standard operator
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$1-D linear interpolation
!!$
!!$=head1 SEE ALSO
!!$
!!$L<binpull2>,L<binpull1>,L<lint2>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!! Nearest-neighbor interpolation would do this:  data = model( 1.5 + (t-t0)/dt)
!!                             This is likewise but with _linear_ interpolation.
module lint  
  use classutil
  use tridiag
  implicit none
  integer, private :: n1 
  logical, private :: inv 
  real, private :: o1 
  real, private :: d1 
  real,   parameter, private  :: eps = 0.01 
  real ,private,  dimension (:), pointer  :: coordinate 
  real ,private,  dimension (:), pointer  :: weight 
  real ,private,  dimension (:), allocatable  :: diagg 
  real ,private,  dimension (:), allocatable  :: offd 
  contains
  subroutine lint_init ( inv_in,o1_in,d1_in,n1_in,coordinate_in,weight_in&
    & )
    logical    :: inv_in 
    real    :: o1_in 
    real    :: d1_in 
    integer    :: n1_in 
    real ,   dimension (:), pointer :: coordinate_in 
    real ,   dimension (:), pointer :: weight_in 
    inv = inv_in 
    o1 = o1_in 
    d1 = d1_in 
    n1 = n1_in 
    coordinate => coordinate_in 
    weight => weight_in 
    if (.not. allocated(diagg)) then
      allocate(diagg ( n1)) 
    end if
    if (.not. allocated(offd)) then
      allocate(offd ( n1)) 
    end if
  end subroutine
  function lint_lop ( adj, add, mm, dd) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: mm,dd 
    call adjnull (adj,add,mm,dd )
    call lint_lop2(adj,add,mm,dd )
    stat=0
  end function 
  subroutine lint_lop2(adj,add,mm,dd)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: mm 
    real, dimension (:)  :: dd 
    integer :: i, im,  id
    real    :: f, fx,gx, wmax
    wmax = eps*maxval(weight*weight)
    diagg = 2.*wmax
    offd = -wmax   ! regularization
    do id= 1, size(dd)
      f = (coordinate(id)-o1)/d1
      i=f  
      im= 1+i
      if ( 1<=im .and. im< size(mm)) then
        fx=(f-i)*weight(id)
        gx= (1.-fx)*weight(id)        
        if ( adj) then
          mm(im  ) =                        mm(im  ) +  gx * dd(id)
          mm(im+1) =                        mm(im+1) +  fx * dd(id)
          if (inv) then
            diagg (im  ) =                                diagg (im  ) + gx * gx
            diagg (im+1) =                                diagg (im+1) + fx * fx
            offd (im  ) =                                offd (im  ) + gx * fx
          end if
        else
          dd(id)   =                        dd(id)   +  gx * mm(im)  +&
            &  fx * mm(im+1)
        end if
      end if
    end do 
    if (adj .and. inv) then
      call tris (diagg, offd, mm) ! tridiagonal solver
    end if
  end subroutine 
  subroutine lint_close()
    deallocate( diagg ,offd)
  end subroutine 
end module 

!!$=head1 NAME
!!$
!!$lint2  -  2-D linear interpolation
!!$
!!$=head1 SYNOPSIS
!!$
!!$Initializer : C<call lint2_init(m1,m2,o1,d1,o2,d2,xy)>
!!$
!!$Operator    : C<ierr=binpull1_lop(adj,add,model,data)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item m1   - integer
!!$
!!$      number of model points,axis 1
!!$
!!$=item m2   - integer
!!$
!!$      number of model points,axis 2
!!$
!!$=item o1 -  real
!!$
!!$      first sample of model space,axis 1
!!$
!!$=item d1 -  real
!!$
!!$      sampling of model space,axis 1
!!$
!!$=item o1 -  real
!!$
!!$      first sample of model space,axis 2
!!$
!!$=item d1 -  real
!!$
!!$      sampling of model space,axis 2
!!$
!!$=item xy - C<real(:,2)>
!!$
!!$      location of data
!!$
!!$=item adj,add,model,data -
!!$
!!$      standard operator
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$2-D linear  interpolation
!!$
!!$=head1 SEE ALSO
!!$
!!$L<binpull2>,L<binpull1>,L<lint1>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!$
module lint2  
! (Bi)Linear interpolation in 2-D
  use classutil
  implicit none
  integer, private :: m1 
  integer, private :: m2 
  real, private :: o1 
  real, private :: d1 
  real, private :: o2 
  real, private :: d2 
  real ,private,  dimension (:,:), pointer  :: xy 
  contains
  subroutine lint2_init ( m1_in,m2_in,o1_in,d1_in,o2_in,d2_in,xy_in )
    integer    :: m1_in 
    integer    :: m2_in 
    real    :: o1_in 
    real    :: d1_in 
    real    :: o2_in 
    real    :: d2_in 
    real ,   dimension (:,:), pointer :: xy_in 
    m1 = m1_in 
    m2 = m2_in 
    o1 = o1_in 
    d1 = d1_in 
    o2 = o2_in 
    d2 = d2_in 
    xy => xy_in
  end subroutine
  function lint2_lop ( adj, add, mm, dd) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: mm,dd 
    call adjnull (adj,add,mm,dd )
    call lint2_lop2(adj,add,mm,dd )
    stat=0
  end function 
  subroutine lint2_lop2(adj,add,mm,dd)
    logical,intent(in) :: adj,add 
    real, dimension (m1,m2)  :: mm 
    real, dimension (:)  :: dd 
    integer i, ix,iy, id
    real    f, fx,gx, fy,gy
    do id= 1, size(dd)
      f = (xy(id,1)-o1)/d1
      i=f
      ix= 1+i
      if ( 1>ix .or. ix>=m1) then
        cycle
      end if
      fx=f-i
      gx= 1.-fx
      f = (xy(id,2)-o2)/d2
      i=f
      iy= 1+i
      if ( 1>iy .or. iy>=m2) then
        cycle
      end if
      fy=f-i
      gy= 1.-fy
      if ( adj) then
        mm(ix  ,iy  ) =                        mm(ix  ,iy  ) + gx * gy&
          & * dd(id)
        mm(ix+1,iy  ) =                        mm(ix+1,iy  ) + fx * gy&
          & * dd(id)
        mm(ix  ,iy+1) =                        mm(ix  ,iy+1) + gx * fy&
          & * dd(id)
        mm(ix+1,iy+1) =                        mm(ix+1,iy+1) + fx * fy&
          & * dd(id)
      else
        dd(id) = dd(id) + gx * gy * mm(ix  ,iy  ) +fx * gy * mm(ix+1,iy&
          &  ) +gx * fy * mm(ix  ,iy+1) +fx * fy * mm(ix+1,iy+1)
      end if
    end do
  end subroutine 
  subroutine lint2_close()
  end subroutine 
end module 

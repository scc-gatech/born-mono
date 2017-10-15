!!$=head1 NAME
!!$
!!$binpull2  -  nearest neighbor interpolation, 2D
!!$
!!$=head1 SYNOPSIS
!!$
!!$Initializer : C<call binpull2_init(m1,m2,o1,d1,o2,d2,xy)>
!!$
!!$Operator    : C<ierr=binpull2_lop(adj,add,model,data)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item m1   - integer
!!$
!!$      number of model points, axis 1
!!$
!!$=item m2   - integer
!!$
!!$      number of model points, axis 2
!!$
!!$=item o1 -  real
!!$
!!$      first sample of model space, axis 1
!!$
!!$=item d1 -  real
!!$
!!$      sampling of model space, axis 1
!!$
!!$=item o2 -  real
!!$
!!$      first sample of model space, axis 2
!!$
!!$=item d2 -  real
!!$
!!$      sampling of model space, axis 2
!!$
!!$=item xy - C<real(:,2)
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
!!$2-D nearest neighbor interpolation
!!$
!!$=head1 SEE ALSO
!!$
!!$L<binpull1>,L<lint1>,L<lint2>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!$
module binpull2  
! From each bin on model mesh, pull nearest data value.
  use classutil
  implicit none
  integer, private :: m1 
  integer, private :: m2 
  integer ,private,  dimension (:,:), allocatable  :: jd 
  contains
  subroutine binpull2_init ( m1_in,m2_in,o1,d1,o2,d2,xy )
    integer    :: m1_in 
    integer    :: m2_in 
    real, dimension (:,:) :: xy
    real  o1,d1, o2,d2, x,y, near,dist
    integer i1,i2, id
    m1 = m1_in 
    m2 = m2_in 
    if (.not. allocated(jd)) then
      allocate(jd ( m1,m2)) 
    end if
    do i1= 1, m1  
      x = o1 + (i1-1)*d1
      do i2= 1, m2  
        y = o2 + (i2-1)*d2
        near = 1.e30
        do id= 1, size(xy,1)
          dist = (x-xy(id,1))**2 + (y-xy(id,2))**2
          if ( dist < near) then
            near = dist
            jd(i1,i2) = id
          end if
        end do
      end do
    end do
  end subroutine
  function binpull2_lop ( adj, add, mm, dd) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: mm,dd 
    call adjnull (adj,add,mm,dd )
    call binpull2_lop2(adj,add,mm,dd )
    stat=0
  end function 
  subroutine binpull2_lop2(adj,add,mm,dd)
    logical,intent(in) :: adj,add 
    real, dimension (m1,m2)  :: mm 
    real, dimension (:)  :: dd 
    integer i1,i2, id
    do i1= 1, m1  
      do i2= 1, m2  
        id = jd(i1,i2)
        if ( adj) then
          mm(i1,i2) = mm(i1,i2) + dd(id)
        else
          dd(id)    = dd(id)    + mm(i1,i2) 
        end if
      end do
    end do
  end subroutine 
  subroutine binpull2_close()
    deallocate( jd )
  end subroutine 
end module 

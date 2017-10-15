!!$=head1 NAME
!!$
!!$binpull1  -  nearest neighbor interpolation
!!$
!!$=head1 SYNOPSIS
!!$
!!$Initializer : C<call binpull1_init(o1,d1,coordinate,m1)>
!!$
!!$Operator    : C<ierr=binpull1_lop(adj,add,model,data)>
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
!!$=item m1   - integer
!!$
!!$      number of model points
!!$
!!$=item adj,add,model,data -
!!$
!!$      standard operator
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$1-D nearest neighbor interpolation
!!$
!!$=head1 SEE ALSO
!!$
!!$L<binpull2>,L<lint1>,L<lint2>
!!$
!!$=head1 LIBRARY
!$
!!$B<geef90>
!!$
!!$=cut
!!$
module binpull1  
! From each bin on model mesh, pull nearest data value.
  use classutil
  implicit none
  integer ,private,  dimension (:), allocatable  :: jd 
  integer, private :: m1 
  contains
  subroutine binpull1_init ( o1,d1,coordinate,m1_in )
    integer    :: m1_in 
    integer im, id
    real  x, near, dist,  o1,d1,coordinate(:)
    m1 = m1_in 
    if (.not. allocated(jd)) then
      allocate(jd ( m1)) 
    end if
    do im= 1, m1  
      x = o1 + (im-1)*d1
      near = 1.e30
      do id= 1, size(coordinate)
        dist = abs( x -coordinate(id))
        if ( dist < near) then
          jd(im) = id
          near = dist
        end if
      end do
    end do
  end subroutine
  function binpull1_lop ( adj, add, mm, dd) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: mm,dd 
    call adjnull (adj,add,mm,dd )
    call binpull1_lop2(adj,add,mm,dd )
    stat=0
  end function 
  subroutine binpull1_lop2(adj,add,mm,dd)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: mm 
    real, dimension (:)  :: dd 
    integer im,id
    do im= 1, m1  
      id =  jd(im)
      if ( adj) then
        mm(im) = mm(im) + dd(id)
      else
        dd(id) = dd(id) + mm(im) 
      end if
    end do
  end subroutine 
  subroutine binpull1_close()
    deallocate( jd )
  end subroutine 
end module 

!!$=head1 NAME
!!$
!!$refine2 -refine mesh
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call refine2_init(co1,cd1,co2,cd2,m1,m2,fo1,fd1,fo2,fd2,n1,n2)>
!!$
!!$C<ierr= refine2_lop(adj,model,data)>
!!$
!!$C<call refine2_close()>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item co1,co2 - real
!!$
!!$      Coarse grain origin
!!$
!!$=item cd1,cd2 - real
!!$
!!$      Coarse grain sampling
!!$
!!$=item ro1,ro2 - real
!!$
!!$      Refined grain origin
!!$
!!$=item rd1,rd2 - real
!!$
!!$      Refined grain sampling
!!$
!!$=item m1,m2   - integer
!!$
!!$      Size of coarse grain input
!!$
!!$=item n1,n2   - integer
!!$
!!$      Size of fine grain output
!!$
!!$=item adj,add,model,data -
!!$
!!$      Standard operator interface
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$ Refine a mesh by linear interpolation
!!$
!!$=head1 SEE ALSO
!!$
!!$L<lint2>
!!$
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!$
module refine2  
! Refine mesh.
  use classutil
! Input mm(m1,m2) is coarse.  Output dd(n1,n2) linear interpolated.
!
  use lint2
  implicit none
  real,   dimension( :, :), pointer, private  :: xy 
  contains
  subroutine refine2_init ( co1,cd1,co2,cd2,m1,m2,fo1,fd1,fo2,fd2,n1,n2&
    & )
    integer, intent( in)  :: m1,m2, n1,n2
    real,    intent( in)  :: co1,cd1,co2,cd2                 
    ! coarse grid
    real,    intent( out) :: fo1,fd1,fo2,fd2                 
    ! fine grid
    integer               :: i1,i2, id
    real                  :: xmin,xmax, ymin,ymax, x,y
    allocate (xy( n1*n2, 2))
    xmin = co1-cd1/2
    xmax = co1 +cd1*(m1-1) +cd1/2        ! Great formula!
    ymin = co2-cd2/2
    ymax = co2 +cd2*(m2-1) +cd2/2
    fd1= (xmax-xmin)/n1
    fo1= (xmin*(n1-.5) + xmax/2)/n1    ! Great formula!
    fd2= (ymax-ymin)/n2
    fo2= (ymin*(n2-.5) + ymax/2)/n2
    do i2=1,n2  
      y = fo2 + fd2*(i2-1)
      do i1=1,n1  
        x = fo1 + fd1*(i1-1)
        id = i1+n1*(i2-1)
        xy( id, :) = (/ x, y /)
      end do
    end do 
    call lint2_init( m1,m2, co1,cd1, co2,cd2, xy)
  end subroutine
  function refine2_lop ( adj, add, mm, dd) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: mm,dd 
    call adjnull (adj,add,mm,dd )
    call refine2_lop2(adj,add,mm,dd )
    stat=0
  end function 
  subroutine refine2_lop2(adj,add,mm,dd)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: mm 
    real, dimension (:)  :: dd 
    integer stat1
    stat1 = lint2_lop( adj, .true., mm, dd)
  end subroutine
  subroutine  refine2_close()
    deallocate (xy)
  end subroutine 
end module 

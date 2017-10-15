!!$
!!$=head1 NAME
!!$
!!$helicon - convolution using helix filters
!!$
!!$=head1 SYNOPSIS
!!$
!!$Initializer - C<call helicon_init(aa)>
!!$
!!$Operator    - C<ierr=helicon_lop(adj,add,xx,yy)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item aa - type(filter)
!!$
!!$      helix filter to perform convolution with
!!$
!!$=item adj,add,xx,yy -
!!$
!!$      standard operators parameters
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$ Convolution, inverse to deconvolution.
!!$ Requires the filter be causal with an implicit "1." at the onset.
!!$
!!$
!!$=head1 SEE ALSO
!!$
!!$L<helix>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!$
module helicon  
! Convolution, inverse to deconvolution.
  use classutil
!             Requires the filter be causal with an implicit "1." at the onset.
  use helix
  implicit none
  type( filter), private :: aa 
  contains
  subroutine helicon_init ( aa_in )
    type( filter)    :: aa_in 
    aa = aa_in
  end subroutine
  function helicon_lop ( adj, add, xx, yy) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: xx,yy 
    call adjnull (adj,add,xx,yy )
    call helicon_lop2(adj,add,xx,yy )
    stat=0
  end function 
  subroutine helicon_lop2(adj,add,xx,yy)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: xx 
    real, dimension (:)  :: yy 
    integer iy, ix, ia
    if ( adj) then
! zero lag
      xx =        xx + yy
    else
      yy =        yy + xx
    end if 
    do ia = 1, size( aa%lag)
      do iy = 1  + aa%lag( ia), size( yy)
        if ( associated( aa%mis)) then
          if ( aa%mis( iy)) then
            cycle
          end if
        end if
        ix = iy - aa%lag( ia)
        if ( adj) then
          xx(ix) =                        xx(ix) + yy(iy) * aa%flt(ia)
        else
          yy(iy) =                        yy(iy) + xx(ix) * aa%flt(ia)
        end if
      end do
    end do
  end subroutine 
  subroutine helicon_close()
  end subroutine 
end module 

!!$
!!$=head1 NAME
!!$
!!$nhelicon - non stationary convolution
!!$
!!$=head1 SYNOPSIS
!!$
!!$Initializer - C<call nhelicon_init(aa)>
!!$
!!$Operator    - C<ierr=nhelicon_lop(adj,add,xx,yy)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item aa - type(nfilter)
!!$
!!$      nhelix filter to perform convolution with
!!$
!!$=item adj,add,xx,yy -
!!$
!!$      standard operators parameters
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$ Nonstationary convolution, inverse to deconvolution.
!!$ Requires the filter be causal with an implicit "1." at the onset.
!!$
!!$=head1 SEE ALSO
!!$
!!$L<nhelix>,L<nhconest>,L<npolydiv>,L<helicon>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module nhelicon  
! Nonstationary convolution, inverse to deconvolution.
  use classutil
!             Requires the filter be causal with an implicit "1." at the onset.
  use nhelix
  implicit none
  type( nfilter), private :: aa 
  contains
  subroutine nhelicon_init ( aa_in )
    type( nfilter)    :: aa_in 
    aa = aa_in
  end subroutine
  function nhelicon_lop ( adj, add, xx, yy) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: xx,yy 
    call adjnull (adj,add,xx,yy )
    call nhelicon_lop2(adj,add,xx,yy )
    stat=0
  end function 
  subroutine nhelicon_lop2(adj,add,xx,yy)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: xx 
    real, dimension (:)  :: yy 
    integer                         :: iy, ix, ia, ip
    integer, dimension( :), pointer :: lag
    real,    dimension( :), pointer :: flt
    if ( adj) then
! zero lag
      xx =        xx + yy
    else
      yy =        yy + xx
    end if 
    do iy = 1, size( yy)
      if ( associated( aa%mis)) then
        if ( aa%mis( iy)) then
          cycle
        end if
      end if
      ip = aa%pch( iy)
      lag => aa%hlx( ip)%lag
      flt => aa%hlx( ip)%flt
      do ia = 1, size( lag)
        ix = iy - lag( ia)
        if ( ix < 1) then
          cycle
        end if
        if ( adj) then
          xx(ix) =                        xx(ix) + yy(iy) * flt( ia)
        else
          yy(iy) =                        yy(iy) + xx(ix) * flt( ia)
        end if
      end do
    end do
  end subroutine 
  subroutine nhelicon_close()
  end subroutine 
end module 

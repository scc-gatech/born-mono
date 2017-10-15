!!$
!!$=head1 NAME
!!$
!!$npolydiv - non stationary polynomial division
!!$
!!$=head1 SYNOPSIS
!!$
!!$Initializer - C<call npolydiv_init(nd,aa)>
!!$
!!$Operator    - C<ierr=npolydiv_lop(adj,add,xx,yy)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item nd - integer
!!$
!!$      number of data points
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
!!$ Polynomial division (deconvolution), inverse to convolution using
!!$ space varying filter.
!!$ Requires the filter be causal with an implicit "1." at the onset.
!!$
!!$
!!$=head1 SEE ALSO
!!$
!!$L<nhelix>,L<nhconest>,L<nhelicon>,L<polydiv>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module npolydiv  
! Helix polynomial division
  use classutil
  use nhelix
  implicit none
  integer, private :: nd 
  type( nfilter), private :: aa 
  real ,private,  dimension (:), allocatable  :: tt 
  contains
  subroutine npolydiv_init ( nd_in,aa_in )
    integer    :: nd_in 
    type( nfilter)    :: aa_in 
    nd = nd_in 
    aa = aa_in 
    if (.not. allocated(tt)) then
      allocate(tt ( nd)) 
    end if
  end subroutine
  function npolydiv_lop ( adj, add, xx, yy) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: xx,yy 
    call adjnull (adj,add,xx,yy )
    call npolydiv_lop2(adj,add,xx,yy )
    stat=0
  end function 
  subroutine npolydiv_lop2(adj,add,xx,yy)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: xx 
    real, dimension (:)  :: yy 
    integer                        :: ia, ix, iy, ip
    integer, dimension(:), pointer :: lag
    real,    dimension(:), pointer :: flt
    tt = 0.
    if ( adj) then
      tt = yy
      do iy= nd, 1, -1  
        ip = aa%pch( iy)
        lag => aa%hlx( ip)%lag
        flt => aa%hlx( ip)%flt
        do ia = 1, size( lag)
          ix = iy - lag( ia)
          if ( ix < 1) then
            cycle
          end if
          tt( ix) =                        tt( ix) -  flt( ia) * tt( iy)
        end do
      end do 
      xx =        xx + tt
    else
      tt = xx
      do iy= 1, nd  
        ip = aa%pch( iy)
        lag => aa%hlx( ip)%lag
        flt => aa%hlx( ip)%flt
        do ia = 1, size( lag)
          ix = iy - lag( ia)
          if ( ix < 1) then
            cycle
          end if
          tt( iy) =                        tt( iy) -  flt( ia) * tt( ix)
        end do
      end do 
      yy =        yy + tt
    end if
  end subroutine 
  subroutine npolydiv_close()
    deallocate( tt )
  end subroutine 
end module 

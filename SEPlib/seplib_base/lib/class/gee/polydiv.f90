!!$
!!$=head1 NAME
!!$
!!$polydiv - polynomial division
!!$
!!$=head1 SYNOPSIS
!!$
!!$Initializer - C<call polydiv_init(nd,aa)>
!!$
!!$Operator    - C<ierr=polydiv_lop(adj,add,xx,yy)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item nd - integer
!!$
!!$      number of data points
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
!!$ Polynomial division (deconvolution), inverse to convolution.
!!$ Requires the filter be causal with an implicit "1." at the onset.
!!$
!!$
!!$=head1 SEE ALSO
!!$
!!$L<helix>,L<hconest>,L<helicon>,L<npolydiv>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module polydiv  
! Helix polynomial division
  use classutil
  use helix
  implicit none
  integer, private :: nd 
  type( filter), private :: aa 
  real ,private,  dimension (:), allocatable  :: tt 
  contains
  subroutine polydiv_init ( nd_in,aa_in )
    integer    :: nd_in 
    type( filter)    :: aa_in 
    nd = nd_in 
    aa = aa_in 
    if (.not. allocated(tt)) then
      allocate(tt ( nd)) 
    end if
  end subroutine
  function polydiv_lop ( adj, add, xx, yy) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: xx,yy 
    call adjnull (adj,add,xx,yy )
    call polydiv_lop2(adj,add,xx,yy )
    stat=0
  end function 
  subroutine polydiv_lop2(adj,add,xx,yy)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: xx 
    real, dimension (:)  :: yy 
    integer  ia, ix, iy
    tt = 0.
    if ( adj) then
      do ix= nd, 1, -1  
        tt( ix) = yy( ix)
        do ia = 1, size( aa%lag)
          iy = ix + aa%lag( ia)
          if ( iy > nd) then
            cycle
          end if
          tt( ix) =                        tt( ix) -  aa%flt( ia) * tt( iy)
        end do
      end do 
      xx =        xx + tt
    else
      do iy= 1, nd  
        tt( iy) = xx( iy)
        do ia = 1, size( aa%lag)
          ix = iy - aa%lag( ia)
          if ( ix < 1) then
            cycle
          end if
          tt( iy) =                        tt( iy) -  aa%flt( ia) * tt( ix)
        end do
      end do 
      yy =        yy + tt
    end if
  end subroutine 
  subroutine polydiv_close()
    deallocate( tt )
  end subroutine 
end module 

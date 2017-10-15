!!$=head1 NAME
!!$
!!$compress - compress a helix filter
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call resize(aa)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item aa  - type(filter)
!!$
!!$      helix filter
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Compress a helix filter, remove all filter coefs < epsilon
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module compress  
  use helix
  implicit none
  contains
  subroutine resize( aa, eps)
    real, optional, intent (in)        :: eps
    type( filter)                      :: aa, bb
    logical, dimension( size( aa%lag)) :: keep
    if (present (eps)) then
      keep = ( abs( aa%flt) > eps)
    else
      keep = ( abs( aa%flt) > epsilon( aa%flt))
    end if 
    call allocatehelix( bb, count( keep))
    bb%flt = pack( aa%flt, keep)
    bb%lag = pack( aa%lag, keep)
    call deallocatehelix( aa)
    aa = bb
  end subroutine 
end module 

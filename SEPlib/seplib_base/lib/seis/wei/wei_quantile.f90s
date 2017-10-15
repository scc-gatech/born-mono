!#$=head1 NAME
!#$
!#$quantile - find data quantile
!#$
!#$=head1 SYNOPSIS
!#$
!#$C<val=quantile(k,a)>
!#$
!#$=head1 PARAMETERS
!#$
!#$=over 4
!#$
!#$
!#$=item k - integer
!#$
!#$      position in array
!#$
!#$=item a - real(:)
!#$
!#$      array
!#$
!#$=back
!#$
!#$=head1 RETURN VALUE
!#$
!#$val - the  a(k) value of the array if the array was sorted
!#$
!#$=head1 DESCRIPTION
!#$
!#$Find the a(k) value of the array if the array was sorted
!#$
!#$=head1 LIBRARY
!#$
!#$B<geef90>
!$
!#$=cut
!#$
module quantile_mod  
! quantile finder.    median = quantile( size(a)/2, a)
  implicit none
  contains
  recursive function quantile( k, a) result( value)
    integer,             intent (in)  :: k          
    ! position in array
    real, dimension (:), intent (in)  :: a
    real                              :: value      
    ! output value of quantile
    integer                           :: j
    real                              :: ak
    ak = a( k)
    j = count( a < ak)                              
    ! how many a(:) < ak
    if ( j >= k) then
      value = quantile( k, pack( a, a < ak))
    else
      j = count( a > ak) + k - size( a)
      if ( j > 0) then
        value = quantile( j, pack( a, a > ak))
      else
        value = ak
      end if
    end if
  end function 
end module 

!!$
!!$=head1 NAME
!!$
!!$autocorr -compute a filter auto-correlation
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<filter=autocorr1(aa,s0,a0)>
!!$
!!$=head1 INPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item aa - C<type(filter)>
!!$
!!$      filter to compute auto-correlation of
!!$
!!$=item a0 - real
!!$
!!$      zero lag value of input
!!$
!!$=back
!!$
!!$=head1 INPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item s0  - real
!!$
!!$      zero lag of output filter
!!$
!!$=back
!!$
!!$=head1 RETURN VALUE
!!$
!!$=over 4
!!$
!!$=item filter  -
!!$
!!$      autocorrelation of helix filter
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Compute the autocorrealtion of filter. Useful for input into
!!$wilson.
!!$
!!$=head1 SEE ALSO
!!$
!!$L<helix>,L<compress>,L<wilson>
!!$
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!$
module autocorr  
  use helix
  use compress
  implicit none
  contains
  function autocorr1( aa, s0, a0) result ( ss)
    type( filter)                       :: aa, ss
    real, intent (in), optional         :: a0
    real, intent (out)                  :: s0
    integer                             :: i, j, k, n, na
    if (present (a0)) then
      s0 = a0*a0 + dot_product (aa%flt,aa%flt)
    else
      s0 = 1.    + dot_product (aa%flt,aa%flt)
    end if 
    na = size (aa%lag)
    call allocatehelix (ss, na*(na+1)/2) 
    k = na
    if (present (a0)) then
      ss%flt (:na) = aa%flt * a0
    else
      ss%flt (:na) = aa%flt
    end if 
    ss%lag (:na) = aa%lag
    do i = 1, na  
      do j = i+1, na  
        k = k + 1
        ss%flt (k) = aa%flt (j) * aa%flt (i)
        ss%lag (k) = aa%lag (j) - aa%lag (i)
        do n = 1, k-1  
          if (ss%lag (n) .eq. ss%lag (k) .and. ss%flt (n) /= 0.) then
            ss%flt (n) = ss%flt (n) + ss%flt (k)
            ss%flt (k) = 0.
          end if
        end do
      end do
    end do 
    call resize (ss)
  end function 
end module 

!!$=head1 NAME
!!$
!!$conv - convolve helix filters
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<ss=conv(aa,bb)>
!!$
!!$=head1 INPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item aa,bb - type(filter)
!!$
!!$      Helix filters
!!$
!!$=back
!!$
!!$=head1 RETURN VALUE
!!$
!!$=over 4
!!$
!!$=item  ss - type(filter)
!!$
!!$       Result of helix filter convolution
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Convolves two helix filters and returns result
!!$
!!$=head1 SEE ALSO
!!$
!!$L<helix>,L<compress>,L<helicon>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module conv  
  use helix
  use compress
  implicit none
  contains
  function conv1( aa, bb) result ( ss)
    type( filter) :: aa, bb, ss
    integer       :: na, nb, ns, ia, ib, i        
    na = size (aa%lag)
    nb = size (bb%lag)
    ns = maxval (aa%lag) + maxval (bb%lag)
    call allocatehelix (ss, ns)
    ss%lag = (/ (i, i = 1, ns) /) 
    do ia = 1, na  
      i = aa%lag (ia)      
      ss%flt (i) = aa%flt (ia)
    end do 
    do ib = 1, nb  
      i = bb%lag (ib)      
      ss%flt (i) = ss%flt (i) + bb%flt (ib)
    end do 
    do ia = 1, na  
      do ib = 1, nb  
        i = aa%lag (ia) + bb%lag (ib) 
        ss%flt (i) = ss%flt (i) + aa%flt (ia) * bb%flt (ib)
      end do
    end do 
    call resize (ss)
  end function 
end module 

!!$=head1 NAME
!!$
!!$misinput - find a mask of missing filter inputs
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call find_mask(known,aa)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item  known - logical(:)
!!$
!!$       Known data locations
!!$
!!$=item  aa    - type(filter)
!!$
!!$       Helix filter
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Given known locations of data, mark on filter where convolution
!!$result is valid.
!!$
!!$=head1 SEE ALSO
!!$
!!$L<lopef>,L<helicon>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module misinput  
! find a mask of missing filter inputs
  use helicon
  implicit none
  contains
  subroutine find_mask( known, aa)
    logical,  intent( in)          :: known(:)
    type( filter)                  :: aa
    real, dimension( size (known)) :: rr, dfre
    integer                        :: stat
    where( known)
      dfre = 0.
    elsewhere
      dfre = 1.    
    end where 
    call helicon_init( aa)
    aa%flt = 1.
    stat = helicon_lop( .false., .false., dfre, rr)
    aa%flt = 0. 
    where ( rr > 0.)
      aa%mis = .true.        
end where
  end subroutine 
end module 

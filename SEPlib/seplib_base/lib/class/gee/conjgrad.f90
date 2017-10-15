!!$
!!$=head1 NAME
!!$
!!$conjgrad  - on step of conjugate gradient
!!$
!!$=head1 SYNOPSIS
!!$
!!$OPERATOR:C<ierr= conjgrad_init(forget,x,g,rr,gg)>
!!$
!!$CLOSE:  C<call conjgrad_close()>
!!$
!!$=head1 PARAMETERS
!!$
!!$
!!$=over 4
!!$
!!$=item forget - logical
!!$
!!$      Wheter or not to forget previous step
!!$
!!$=item x      - C<real(:)>
!!$
!!$      Model
!!$
!!$=item g      - C<real(:)>
!!$
!!$      Gradient
!!$
!!$=item rr      - C<real(:)>
!!$
!!$=item gg      - C<real(:)>
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$One step of conjugate gradient method
!!$
!!$=head1 SEE ALSO
!!$
!!$L<cgmeth>, L<cdstep>,L<solver_reg>,L<cgstep>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!$
module conjgrad_mod  
  implicit none
  real, dimension (:), allocatable, private         :: s, ss
  contains
  subroutine conjgrad_close ()
    if ( allocated( s)) then
      deallocate( s, ss)
    end if
  end subroutine 
  function conjgrad( forget, x, g, rr, gg)  result( stat)
    integer              :: stat
    real, dimension (:)  :: x, g, rr, gg
    logical              :: forget
    real, save           :: rnp
    double precision     :: rn, alpha, beta
    rn = sum( dprod( g, g))
    if ( .not. allocated( s)) then
      forget = .true.
      allocate( s  (size (x )))
      s  = 0. 
      allocate( ss (size (rr)))
      ss = 0.
    end if
    if ( forget .or. rnp < epsilon (rnp)) then
      alpha = 0.d0
    else
      alpha = rn / rnp
    end if 
    s  =  g + alpha *  s
    ss = gg + alpha * ss
    beta = sum( dprod( ss, ss))
    if ( beta > epsilon( beta)) then
      alpha = - rn / beta
      x =  x  + alpha *  s
      rr = rr + alpha * ss
    end if
    rnp = rn
    forget = .false.
    stat = 0
  end function 
end module 

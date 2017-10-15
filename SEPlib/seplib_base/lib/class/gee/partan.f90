!!$=head1 NAME
!!$
!!$partan - partan step
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call partan_close()>
!!$
!!$C<ierr=partan(forget,x,g,rr,gg)>
!!$
!!$  
!!$=head1 PARAMETERS
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
!!$One step of partan method
!!$
!!$=head1 SEE ALSO
!!$
!!$L<cgmeth>, L<cdstep>,L<solver_reg>,L<conjgrad>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!$

module partan_mod		
  use ddot_mod
  implicit none
  real, dimension (:), allocatable, private 	:: s, ss, x1, rr1
contains
  subroutine partan_close ()
    deallocate (s, ss, x1, rr1)
  end subroutine partan_close

  function partan (forget, x, g, rr, gg) result (stat)
    integer					:: stat
    real, dimension (:)				:: x, g, rr, gg
    logical					:: forget

    double precision 				:: alpha, beta
    double precision, parameter			:: eps = 1.d-30
    real, dimension (size (x))			:: tx
    real, dimension (size (rr))			:: tr

    if (.not. allocated (s)) then
       forget = .true.
       allocate (s  (size (x))) 
       allocate (ss  (size (rr)))

       allocate (x1 (size (x)))
       allocate (rr1 (size (rr)))
    end if

    if (forget) then
       s = 0 
       ss = 0.

       rr1 = rr
       x1 = x
    end if

    beta = ddot (gg, gg)
    if (beta > eps) then
       alpha =  ddot (rr,gg) / beta
       s  = s  + alpha * g
       ss = ss + alpha * gg
    end if

    if (.not. forget) then
       beta = ddot (ss, ss)
       if (beta > eps) then
          alpha =  ddot (rr1,ss) / beta
          s  = alpha * s
          ss = alpha * ss
       end if
    end if

    tx =  x;  x =  x1 +  s;  x1 = tx;  s =  x - tx
    tr = rr; rr = rr1 - ss; rr1 = tr; ss = tr - rr

    forget = .false.
    stat = 0
  end function partan
end module partan_mod





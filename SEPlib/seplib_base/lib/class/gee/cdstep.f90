!!$
!!$=head1 NAME
!!$
!!$cdstep  - on step of conjugate direction
!!$
!!$=head1 SYNOPSIS
!!$
!!$OPEN:  C<call cdstep_init()>
!!$
!!$OPERATOR:C<ierr= cdstep(forget,x,g,rr,gg)>
!!$
!!$CLOSE:  C<call cdstep_close()>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item forget - logical
!!$
!!$  Wheter or not to forget previous step
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
!!$One step of conjugate direction method
!!$
!!$=head1 SEE ALSO
!!$
!!$L<cgmeth>, L<cgstep>,L<solver_reg>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!$

module cdstep_mod		

  use ddot_mod
  use llist_mod

  implicit none
  type (list), private	:: steps

contains
  subroutine cdstep_init ()
    steps = llist_init ()
  end subroutine cdstep_init

  subroutine cdstep_close ()
    call llist_close (steps)
  end subroutine cdstep_close

  function cdstep (forget, x, g, rr, gg) result (stat)
    integer                       :: stat
    real, dimension(:)            :: x, g, rr, gg
    logical                       :: forget	

    real, dimension(:), pointer   :: s, ss, si
    double precision              :: alpha, beta
    double precision, parameter	  :: eps = 1.d-30
    integer                       :: i, n, nm, nr

    nm = size (x) 
    nr = size (rr)

    allocate (s (nm + nr)) ; ss => s (: nr)
    s (nr + 1:) = g   ; ss = gg

    call llist_rewind (steps)
    n = llist_depth (steps)
    do i = 1, n
       call llist_down (steps, si, beta)
       alpha = ddot (gg, si (:nr)) / beta
       s = s - alpha * si
    end do

    beta = ddot (ss, ss)
    if (beta > eps) then
       call llist_add (steps, s, beta)
       if (forget) call llist_chop (steps)
       alpha = - ddot (rr, ss) / beta
       x  =  x + alpha * s  (nr+1:)
       rr = rr + alpha * ss 
    end if

    stat = 0
  end function cdstep

  subroutine cdstep_diag(res)
    real, dimension (:), intent (out)	:: res

    integer				:: i, nr
    real, dimension (:), pointer	:: s, ss
    double precision		        :: sn

    res = 0. ; nr = size (res)
    call llist_rewind (steps)
    do i = 1, llist_depth (steps)
       call llist_down (steps, s, sn) ; ss => s (:nr)
       res = res + dprod (ss, ss) / sn
    end do
  end subroutine cdstep_diag

  subroutine cdstep_mat (res)
    real, dimension (:,:), intent (out)	:: res

    integer				:: i, j, nr
    real, dimension (:), pointer	:: s, ss
    double precision		        :: sn

    res = 0. ; nr = size (res, 1)
    call llist_rewind (steps)
    do i = 1, llist_depth (steps)
       call llist_down (steps, s, sn) ; ss => s (:nr)
       do j = 1, nr
          res (:,j) = res (:,j) + dprod (ss (j), ss) / sn
       end do
    end do
  end subroutine cdstep_mat
end module cdstep_mod









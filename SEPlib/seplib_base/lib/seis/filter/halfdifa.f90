!!$
!!$=head1 NAME
!!$
!!$ halfdifa - half causal derivative
!!$
!!$=head1 SYNOPSIS
!!$
!!$ call halfdifa(adj,add,n,xx,yy)
!!$
!!$=head1  PARAMETERS
!!$
!!$=over 4
!!$
!!$=item adj - integer
!!$
!!$      1=adjoint, 0=forward
!!$
!!$=item add - integer
!!$
!!$       1=add to output, 0=erase output
!!$
!!$=item n - integer
!!$
!!$      size of data
!!$
!!$=item xx - real(n)
!!$
!!$      model
!!$
!!$=item yy - real(n)
!!$
!!!$      data
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$ Half order causal derivative.  OK to equiv(xx,yy)
!!$
!!$
!!$=head1 SEE ALSO
!!$
!!$L<Halfint>
!!$
!!$=head1 LIBRARY
!!$
!!$B<sepfilterf90>
!!$
!!$=cut
!>
!
subroutine halfdifa( adj, add, n, xx,    yy  )
integer n2, i,       adj, add, n
real omega,                       xx(n), yy(n)
complex cz, cv(n*2)
n2=1
do while (n2<n)
  n2=2*n2
end do 
do i= 1, n2  
  cv(i) = 0.
end do 
do i= 1, n
  if ( adj .eq. 0) then
    cv(i) = xx(i)
  else
    cv(i) = yy(i)
  end if
end do 
call adjnull(       adj, add,    xx,n,  yy,n)
call ftu( +1., n2, cv)
do i= 1, n2  
  omega = (i-1.) * 2.*3.14159265 / n2
  cz = csqrt( 1. - cexp( cmplx( 0., omega)))
  if ( adj .ne. 0) then
    cz = conjg( cz)
  end if
  cv(i) = cv(i) * cz
end do 
call ftu( -1., n2, cv)
do i= 1, n
  if ( adj .eq. 0) then
    yy(i) = yy(i) + cv(i)
  else
    xx(i) = xx(i) + cv(i)
  end if
end do 
return
end  

!!$
!!$=head1 NAME
!!$
!!$ boxconv - smooth by apllying box filter
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call boxconv(nb,nx,xx,yy)>
!!$
!!$
!$=head1 INPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item nb - integer
!!$
!!$      box length
!!$
!!$=item nx - integer
!!$
!!$      data length
!!$
!!$=item xx - real(nx)
!!$
!!$      input data
!!$
!!$=back
!!$
!!$=head1 OUTPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item yy - real(nx+nb-1)
!!$
!!$      smoothed data
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$ Smooths data by applying box filter
!!$
!!$=head1 SEE ALSO
!!$
!!$B<Smooth>
!!$
!!$=head1 LIBRARY
!!$
!!$B<sepfilterf90>
!!$
!!$=cut
!!$
!!
!
subroutine boxconv( nb, nx, xx, yy)
use sep_mod
! inputs:       nx,  xx(i), i=1,nx      the data
!               nb                      the box length
! output:       yy(i),i=1,nx+nb-1       smoothed data
integer nx, ny, nb, i
real xx(nx), yy(1)
real bb(nx+nb)
if ( nb < 1 .or. nb > nx) then
  call erexit('boxconv')  ! "||" means .OR.
end if
ny = nx+nb-1
do i= 1, ny
  bb(i) = 0.
end do 
bb(1) = xx(1)
do i= 2, nx
  bb(i) = bb(i-1) + xx(i)         ! make B(Z) = X(Z)/(1-Z)
end do 
do i= nx+1, ny
  bb(i) = bb(i-1)
end do 
do i= 1, nb
  yy(i) = bb(i)
end do 
do i= nb+1, ny
  yy(i) = bb(i) - bb(i-nb)        ! make Y(Z) = B(Z)*(1-Z**nb)
end do 
do i= 1, ny
  yy(i) = yy(i) / nb
end do 
return
end  

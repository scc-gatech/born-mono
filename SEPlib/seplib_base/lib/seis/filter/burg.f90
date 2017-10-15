!!$
!!$=head1 NAME
!!$
!!$ burg - burg decon
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call burg(lx,x,la,a)>
!!$
!!$
!!$
!$=head1  INPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item lx - integer
!!$
!!$      input length
!!$
!!$=item x  - real(lx)
!!$
!!$      input
!!$
!!$=item la - integer
!!$
!!$      output length
!!$
!!$=item a  - real(la)
!!$
!!$      output
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$ Burg routione adopted from FGDP page 137
!!$
!!$=head1 SEE ALSO
!!$
!!$B<wilson>
!!$
!!$=head1 LIBRARY
!!$
!!$B<sepfilterf90>
!!$
!!$=cut
!!$
!>
subroutine burg( lx, x, la, a)
integer          lx,    la
real                 x(lx), a(la)
real  top, bot, epi
integer i, j
real ep(lx), em(lx), c(la), ap(la)
!if( lx>2000 || la>1000 )  call err('dimension error in burg')
a(1) = 1.
do i= 1, la  
  ap(i) = 0.
end do 
do i= 1, lx  
  ep(i) = x(i)
  em(i) = x(i)
end do 
do j= 2, la  
  top = 0.
  bot = 0.
  do i= j, lx  
    bot = bot + ep(i) * ep(i)    +    em(i-j+1) * em(i-j+1)
    top = top + ep(i) * em(i-j+1)
  end do 
  c(j) = 2. * top / (bot + 1.e-30)
  do i= j, lx  
    epi       = ep(i)
    ep(i)     = ep(i)     - c(j) * em(i-j+1)
    em(i-j+1) = em(i-j+1) - c(j) * epi
  end do 
  a(j) = 0.
  do i= 1, j
    ap(i) = a(i) - c(j) * a(j-i+1)
  end do 
  do i= 1, j
    a(i) = ap(i)
  end do
end do 
return
end  

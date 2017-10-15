!<
! r2codeven
!
! USAGE
! call r2codeven(sig,n1o,n1s)
!
! INPUT PARAMETERS
!  sig   -  complex(n1o,n1s)  signal
!  n1o   -  integer           size-dim1
!  n1s   -  integer           size-dim2
!
! DESCRIPTION
! This subroutine pads frequency axis
! and distinguishes odd/even number of points for the FFT
!
! CATEGORY
! Lib:Aux
!
! COMPILE LEVEL
! DISTR
!>
subroutine r2codeven(sig,n1o,n1s)
integer n1s,iw,i1s,n1o,n1oh,m1oh,is
complex sig(n1o,n1s)
!                Author:    Clement Kostov
! KEYWORDS :   Fourier transform
if (  (n1o/2)*2 .eq. n1o ) then
  n1oh = n1o/2 + 1 
  m1oh = 1
else
  n1oh = n1o/2+1 
  m1oh = 0
end if 
if ( m1oh .eq. 1 ) then
! even
  do is=1,n1s
    do iw = 1,n1oh - 2
      sig(n1oh+iw,is) = conjg( sig(n1oh-iw, is) )
    end do
  end do
else
! odd
  do is=1,n1s
    do iw = 1,n1oh - 1
      sig(n1oh+iw,is) = conjg( sig(n1oh+1-iw, is) )
    end do
  end do
end if 
return 
end
subroutine zero(n,x)
integer n,i
real x(n)
do i=1,n
  x(i)=0. 
end do 
return 
end
subroutine zero8(n,x)
integer n,i
double precision x(n)
do i=1,n
  x(i)=0. 
end do 
return 
end  

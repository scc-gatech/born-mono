module ft3d_mod
use sep_fft_mod

contains
!<
! ft3d_sub
!
! Description
!        1-d, 2-d, or 3-d Fourier Transform subroutine
!
! USAGE
!        subroutine ft3d_sub( n1, n2, n3, cx,sign1,sign2,sign3,center1, center2, center3)
!
! Input parameter
! n1   -     integer            axis-1 length
! n2   -     integer            axis-2 length
! n3   -     integer            axis-3 length
!        cx   -     complex(n1,n2,n3)  dataset to transform
! sign1..3-  real               fourier tranform signs
! center1..3-real               where to put the reflection axis
!
! CATEGORY
! Lib:Fft
!
! COMPILE LEVEL
! DISTR
!>
! KEYWORDS   subroutine Ft3d  Fourier-transform
!
!
! ft3d.r - 1-d, 2-d, or 3-d Fourier Transform subroutine
! copyright (c) 1991 Stanford University
!
! Modified   7/27/87    steve   Moved header file operations to main program
!                               Ft3d.c so that this subroutine may be called
!                                by any program.
!
! Modified  12/17/88    dave   Added center?=-1 option to post-multiply by
!                               1,-1,1,-1...   so that a centered transform
!                                can be inverted.
!
subroutine ft3d_sub( n1, n2, n3, cx, sign1, sign2, sign3, center1, center2&
  &, center3 )
use sep_mod
! ----------
!  Keyword:  subroutine Ft3d  Fourier-transform
! ----------
integer i1,n1, i2,n2, i3,n3, n
complex cx(n1,n2,n3)
real sign1,sign2,sign3, center1,center2,center3
do i2=1,n2 
  do i1=1,n1 
    if (real(cx(i1,i2,1)).ne.0.) then
      write(0,*) "here",i1,i2,cx(i1,i2,1) 
    end if
  end do
end do 
if ( (sign1 .ne. 0.0) .and. (n1 .ne.1) ) then
  write(0,*) "in Ft31"
  n = n1
  do while ( n .ne.1 )
    if ( (n/2)*2 .ne. n ) then
      call erexit(' n1 not a power of 2 ')
    end if
    n = n / 2
  end do 
  if (center1 > 0.0) then
    do i3=1,n3
      do i2=1,n2
        do i1=2,n1,2
          cx(i1,i2,i3) = - cx(i1,i2,i3)
        end do
      end do
    end do
  end if
  do i2=1,n2 
    do i1=1,n1 
      if (real(cx(i1,i2,1)).ne.0.) then
        write(0,*) "part 1b",i1,i2,cx(i1,i2,1) 
      end if
    end do
  end do 
  do i3=1,n3
    do i2=1,n2
      call cefft( cx(1,i2,i3), n1, int(sign1) ,1./sqrt(1.*n1))
    end do
  end do
end if
do i2=1,n2 
  do i1=1,n1 
    if (real(cx(i1,i2,1)).ne.0.) then
      write(0,*) "part 2",i1,i2,cx(i1,i2,1) 
    end if
  end do
end do 
if (center1 < 0.0) then
  write(0,*) "in Ft32"
  do i3=1,n3
    do i2=1,n2
      do i1=2,n1,2
        cx(i1,i2,i3) = - cx(i1,i2,i3)
      end do
    end do
  end do
end if
do i2=1,n2 
  do i1=1,n1 
    if (real(cx(i1,i2,1)).ne.0.) then
      write(0,*) "part 2nb",i1,i2,cx(i1,i2,1) 
    end if
  end do
end do 
if ( (sign2 .ne. 0.0) .and. (n2 .ne.1) ) then
  write(0,*) "in Ft33"
  n = n2
  do while ( n .ne.1 )
    if ( (n/2)*2 .ne. n ) then
      call erexit(' n2 not a power of 2 ')
    end if
    n = n / 2
  end do 
  if (center2 > 0.0) then
    do i3=1,n3
      do i2=2,n2,2
        do i1=1,n1
          cx(i1,i2,i3) = - cx(i1,i2,i3)
        end do
      end do
    end do
  end if
  do i2=1,n2 
    do i1=1,n1 
      if (real(cx(i1,i2,1)).ne.0.) then
        write(0,*) "part 3a",i1,i2,cx(i1,i2,1) 
      end if
    end do
  end do 
  write(0,*) "in Ft33-2"
  do i3=1,n3 
    write(0,*) "in Ft33-4",i3
    do i2=1,n2 
      do i1=1,n1 
        if (real(cx(i1,i2,i3)).ne.0.) then
          write(0,*) i1,i2,cx(i1,i2,i3) 
        end if
      end do
    end do 
    call rowcc( n1, n2, cx(1,1,i3), sign2, 1./sqrt(1.*n2) )
    do i2=1,n2 
      do i1=1,n1 
        if (real(cx(i1,i2,i3)).ne.0.) then
          write(0,*) i1,i2,cx(i1,i2,i3) 
        end if
      end do
    end do
  end do
end if
if (center2 < 0.0) then
  write(0,*) "in Ft34"
  do i3=1,n3
    do i2=2,n2,2
      do i1=1,n1
        cx(i1,i2,i3) = - cx(i1,i2,i3)
      end do
    end do
  end do
end if
if ( (sign3 .ne. 0.0) .and. (n3 .ne.1) ) then
  write(0,*) "in Ft35"
  n = n3
  do while ( n .ne.1 )
    if ( (n/2)*2 .ne. n ) then
      call erexit(' n3 not a power of 2 ')
    end if
    n = n / 2
  end do 
  if (center3 > 0.0) then
    do i3=2,n3,2
      do i2=1,n2
        do i1=1,n1
          cx(i1,i2,i3) = - cx(i1,i2,i3)
        end do
      end do
    end do
  end if
  call rowcc( n1*n2, n3, cx, sign3, 1./sqrt(1.*n3) )
  if (center3 < 0.0) then
    do i3=2,n3,2
      do i2=1,n2
        do i1=1,n1
          cx(i1,i2,i3) = - cx(i1,i2,i3)
        end do
      end do
    end do
  end if
end if
return
end subroutine
!<
! ft3D1axis
!
! Usage
! call ft3D1axis(adj,sign1,n1,n2,n3,cx)
!
! Description
! 1D Fourier transform on a 3D data set along the 1-axis
!
! Input Parameters
! adj - integer 1 adjoint, 0 forward
! sign - real  sign for fft
! n1   - integer number of elemnts fast axis
! n2   - integer number of elements second axis
! n3   - integer number of elements third axis
! cx   - complex data
!
! CATEGORY
! Lib:Fft
!
! COMPILE LEVEL
! DISTR
!>
! Mihai Nov 92
!
subroutine  ft3D1axis(adj,sign1,n1,n2,n3,cx)
use sep_mod
integer  i2,i3,adj,n1,n2,n3
complex cx(n1,n2,n3)
real sign1
do i3=1,n3
  do i2=1,n2
    call fth( adj, sign1, 1,n1, cx(1,i2,i3))
  end do
end do 
return
end subroutine
! 1D Fourier transform on a 3D data set along the 2-axis
!
subroutine  ft3D2axis( adj, sign2, n1,n2,n3, cx)
use sep_mod
integer i1,i3,adj,n1,n2,n3
complex cx(n1,n2,n3)
real    sign2
do i3=1,n3
  do i1=1,n1
    call fth( adj, sign2, n1,n2, cx(i1,1,i3))
  end do
end do 
return
end subroutine
! 1D Fourier transform on a 3D data set along the 3-axis
!
subroutine  ft3D3axis( adj, sign3, n1,n2,n3, cx)
use sep_mod
integer i1,i2,adj,n1,n2,n3
complex cx(n1,n2,n3)
real    sign3
do i2=1,n2
  do i1=1,n1
    call fth( adj, sign3, n1*n2,n3, cx(i1,i2,1))
  end do
end do 
return
end   subroutine
end module

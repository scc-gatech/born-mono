!<
! rowcc
!
! USAGE
! call rowcc(n1,n2,cx,sign2,scale)
!
! DESCRIPTION
! Rocca's row Fourier transform.
!
! INPUT PARAMETERS
! n1 - integer data size fast axis
! n2 - integer data size second axis
! cx - complex(:) data
! sign2 - integer  should be +1. or -1. it is the sign of i.
! scale - real  scale
!
! CATEGORY
! Lib:Fft
!
! COMPILE LEVEL
! DISTR
!>
subroutine rowcc(n1,n2,cx,sign2,scale)
complex cx(n1,n2),cmplx,cw,cdel
integer i1,n1, i2,n2, j,i,m,lstep,istep
real sign2, scale, arg
do i1 = 1,n1
  do i2 = 1,n2
    cx(i1,i2) = cx(i1,i2)*scale
  end do
end do 
j = 1
do i = 1,n2  
  if (i<=j) then
    call twid1(n1,cx(1,i),cx(1,j))
  end if
  m = n2/2
  do while  (j>m)
    j = j-m
    m = m/2
    if (m<1) then
      exit 
    end if
  end do 
  j = j+m
end do 
lstep = 1
do while (1.eq.1)
  istep = 2*lstep
  cw = 1.
  arg = sign2*3.14159265/lstep
  cdel = cmplx(cos(arg),sin(arg))
  do m = 1,lstep  
    do i = m,n2,istep
      call twid2(n1,cw,cx(1,i),cx(1,i+lstep))
    end do 
    cw = cw*cdel
  end do 
  lstep = istep
  if (lstep >=n2) then
    exit
  end if
end do 
return
end
subroutine twid1(n,cx,cy)
complex cx(n),cy(n),ct
integer i,n
do i = 1,n  
  ct = cx(i)
  cx(i) = cy(i)
  cy(i) = ct
end do 
return
end
! If you feel like optimizing, this is the place.
subroutine twid2(n,cw,cx,cy)
complex cx(n),cy(n),ctemp,cw
integer i,n
do i = 1,n  
  ctemp = cw*cy(i)
  cy(i) = cx(i)-ctemp
  cx(i) = cx(i)+ctemp
end do 
return
end  

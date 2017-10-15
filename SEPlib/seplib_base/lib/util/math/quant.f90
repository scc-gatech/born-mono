!<
! quantile
!
! Usage
! call quantile(k,n,bb,value)
!
! INPUT PARAMETERS
! k - integer kth value of bb you want to find
! n - integer number of elements in bb
! bb - real(:) array
!
! OUTPUT PARAMETERS
! value - real bb(k)
!
!DESCRIPTION
! Finds the kth value of a real array
! value = value of bb(k) if bb(1...n) were sorted into increasing order.
!
! CATEGORY
! Lib:Math
!
! COMPILE LEVEL
! DISTR
!>
subroutine quantile( k, n, bb, value)
integer   i, k, n
real bb(n), value
real aa(n)
do i= 1, n
  aa(i) = bb(i)
end do 
call quantinternal( k, n, aa)
value = aa(k)
return
end
!<
! quantabs
!
! Usage
! call quantabs(k,n,bb,value)
!
! INPUT PARAMETERS
! k - integer kth value of bb you want to find
! n - integer number of elements in bb
! bb - real(:) array
!
! OUTPUT PARAMETERS
! value - real bb(k)
!
!DESCRIPTION
! Finds the kth value of a real array (sorted accoring to absolute value)
! value = value of bb(k) if bb(1...n) were sorted into increasing order.
!
! CATEGORY
! Lib:Math
!
! COMPILE LEVEL
! DISTR
!>
subroutine quantabs( k, n, bb, value)
integer   i, k, n
real bb(n), value
real aa(n)
do i= 1, n
  aa(i) = abs( bb(i))
end do 
call quantinternal( k, n, aa)
value = aa(k)
return
end
subroutine quantinternal( k, n, a)
use sep_func_mod
integer k, n
real a(n)
integer i, j, low, hi
real ak, aa
if ( k>n .or. k<1) then
  call erexit("quant: inputs not in range  1 <= k <= n ")
end if
low = 1
hi = n
do while ( low < hi)
  ak = a(k)
  i = low
  j = hi
  do   
    if ( a(i) < ak) then
      i = i+1
    else
      do while ( a(j) > ak)
        j = j-1
      end do 
      if ( i > j) then
        exit 
      end if
      aa = a(i)
      a(i) = a(j)
      a(j) = aa
      i = i+1
      j = j-1
      if ( i > j) then
        exit 
      end if
    end if
  end do 
  if ( j < k) then
    low = i
  end if
  if ( k < i) then
    hi = j
  end if
end do 
return
end  

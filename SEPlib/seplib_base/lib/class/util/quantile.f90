! Two quantile utilities.  Changed since formally tested.
!
! value = value of bb(k) if bb(1...n) were sorted into increasing order.
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
! value = value of abs(bb(k)) if abs(bb(1...n)) were sorted to increasing order.
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

subroutine diag( adj, add, lambda,n,  pp,    qq)
integer i,       adj, add,        n                   
! equivalence (pp,qq) OK
real                       lambda(n), pp(n), qq(n)
if ( adj .eq. 0 ) then
  if ( add .eq. 0 ) then
    do i=1,n  
      qq(i) =         lambda(i) * pp(i)
    end do
  else
    do i=1,n  
      qq(i) = qq(i) + lambda(i) * pp(i)
    end do
  end if
else
  if ( add .eq. 0 ) then
    do i=1,n  
      pp(i) =         lambda(i) * qq(i)
    end do
  else
    do i=1,n  
      pp(i) = pp(i) + lambda(i) * qq(i)
    end do
  end if
end if 
return
end  

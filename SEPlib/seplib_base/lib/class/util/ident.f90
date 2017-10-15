subroutine ident( adj, add, epsilon, n, pp,    qq   )
integer i,        adj, add,          n
real                        epsilon,    pp(n), qq(n)  
! equivalence (pp,qq) OK
if ( adj .eq. 0 ) then
  if ( add .eq. 0 ) then
    do i=1,n  
      qq(i) =         epsilon * pp(i)
    end do
  else
    do i=1,n  
      qq(i) = qq(i) + epsilon * pp(i)
    end do
  end if
else
  if ( add .eq. 0 ) then
    do i=1,n  
      pp(i) =         epsilon * qq(i)
    end do
  else
    do i=1,n  
      pp(i) = pp(i) + epsilon * qq(i)
    end do
  end if
end if 
return
end  

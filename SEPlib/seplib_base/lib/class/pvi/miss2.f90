
! fill in missing data in 2-D by minimizing power out of a given filter.
!
subroutine miss2(   lag1,lag2, a1,a2, aa, n1,n2, ww, pp, known, rr, niter&
  &)
integer i1,i2,iter, lag1,lag2, a1,a2,     n1,n2,                    niter&
  &, n12
real    pp(    n1, n2)          
! in: known data with zeros for missing values
! out: known plus missing data.
real    known( n1, n2)          
! in: known(ip) vanishes where pp(ip) is missing
real    ww(    n1, n2)          ! in: weighting function on data pp
real    aa(    a1, a2)          ! in: roughening filter
real    rr(    n1, n2*2)        ! out: residual
real  dp( n1, n2), dr( n1, n2*2)
real  sp( n1, n2), sr( n1, n2*2)
n12 = n1 * n2
call null( rr, n12*2)
call null( dp, n12)
call null( dr, n12*2)
call cinloi( 0, 0, lag1,lag2,a1,a2,aa, n1,n2, pp, rr        )
call diag  ( 0, 0,                 ww,  n12,  pp, rr(1,n2+1))
call scaleit (-1.,                      2*n12,      rr        )
do iter= 0, niter  
  call cinloi( 1, 0, lag1,lag2,a1,a2,aa, n1,n2, dp, rr        )
  call diag  ( 1, 1,                 ww,  n12,  dp, rr(1,n2+1))
  do i1= 1, n1  
    do i2= 1, n2  
      if ( known(i1,i2) .ne. 0.) then
        dp(i1,i2) = 0.
      end if
    end do
  end do 
  call cinloi( 0, 0, lag1,lag2,a1,a2,aa, n1,n2, dp, dr        )
  call diag  ( 0, 0,                 ww,  n12,  dp, dr(1,n2+1))
  call cgstep( iter,   n12, pp,dp,sp,                            2*n12&
    &, rr,dr,sr )
end do 
return
end  

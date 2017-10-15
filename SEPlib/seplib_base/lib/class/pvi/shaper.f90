! shaping filter
! minimize  SUM rr(i)**2  by finding ff and rr where
!
! rr = yy - xx (convolve) ff
!
subroutine shaper( nf,ff, nx,xx, ny, yy, rr, niter)
use sep_func_mod
integer i, iter,   nf,    nx,    ny,         niter
real    ff(nf), xx(nx), yy(ny), rr(ny)
real  df(nf), dr(ny), sf(nf), sr(ny)
if ( ny .ne. nx+nf-1) then
  call erexit('data length error')
end if
do i= 1, nf
  ff(i) = 0.
end do 
do i= 1, ny
  rr(i) =  yy(i)
end do 
do iter= 0, niter  
  call contran( 1, 0, nx,xx,  nf,df,  rr)         ! df=xx*rr
  call contran( 0, 0, nx,xx,  nf,df,  dr)         ! dr=xx*df
  call cgstep( iter, nf,ff,df,sf,  ny,rr,dr,sr)   
  ! rr=rr-dr; ff=ff+df
end do 
return
end  

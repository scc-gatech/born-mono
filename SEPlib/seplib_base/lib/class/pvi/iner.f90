! weighted interpolation-error filter
!
subroutine iner( nf,f, nr,yy,rr, ww, niter, lag, gap1, gapn)
use sep_func_mod
integer i, iter, nf,   nr,           niter, lag, gap1, gapn
real     f(nf), yy(nr), rr(nr), ww(nr)
real  df(nf), sf(nf), dr(nr), wr(nr), sr(nr)
if ( lag < gap1 .or. lag > gapn ) then
  call erexit('input fails gap1<=lag<=gapn')
end if
do i= 1, nf
  f(i) = 0.
end do 
f(lag) = 1.                                                  
! set output lag
call         wcontrunc( 0,0, ww, lag, nr,yy, nf, f, nr,wr)
call scaleit( -1.,                                    nr,wr)   
! negative
do iter= 0, niter  
  call wcontrunc( 1,0, ww, lag, nr,yy, nf,df, nr,wr)   ! df=yy*wr
  do i= gap1, gapn
    df(i) = 0.                                   ! constrained lags
  end do 
  call wcontrunc( 0,0, ww, lag, nr,yy, nf,df, nr,dr)   ! dr=yy*df
  call cgstep( iter,  nf, f,df,sf,                             nr,wr,dr&
    &,sr )                    ! f=f+df
end do 
call          contrunc( 0,0,     lag, nr,yy, nf,f,  nr,rr)   
! unweighted res
return
end  

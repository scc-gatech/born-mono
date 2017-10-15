! filter and weight.
!
subroutine wcontrunc( adj, add, ww,    lag, nx, xx, nf,ff,  nn,yy )
use classutil
integer i,            adj, add,        lag, nx,     nf,     nn
real                            ww(nn),     xx(nx), ff(nf), yy(nn)
real                  ss(nn)
call adjnull(         adj, add,                     ff,nf,  yy,nn)
if ( adj .eq. 0) then
  call contrunc( 0,0, lag,  nx,xx,   nf,ff,  nn,ss)      
  do i= 1, nn
    yy(i) =  yy(i) + ss(i) * ww(i)
  end do
else
  do i= 1, nn
    ss(i) =          yy(i) * ww(i)
  end do 
  call contrunc( 1,1, lag,  nx,xx,   nf,ff,  nn,ss)
end if 
return
end  

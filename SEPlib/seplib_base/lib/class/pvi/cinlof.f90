
! CINLOF --- Convolution INternal with Lags.  Output is adjoint to FILTER.
!
subroutine cinlof( adj, add, lag1,lag2,  n1,n2,xx,  nb1,nb2,bb,  yy)
use classutil
integer            adj, add, lag1,lag2,  n1,n2,     nb1,nb2
real                                      xx(n1,n2), bb(nb1,nb2), yy(n1&
  &,n2)
integer y1,y2, x1,x2, b1, b2
call adjnull(      adj, add,                         bb,nb1*nb2,  yy,n1&
  &*n2)
if ( adj .eq. 0 ) then
  do b2=1,nb2  
    do y2= 1+nb2-lag2, n2-lag2+1  
      x2= y2 - b2 + lag2
      do b1=1,nb1  
        do y1= 1+nb1-lag1, n1-lag1+1  
          x1= y1 - b1 + lag1
          yy(y1,y2) = yy(y1,y2) + bb(b1,b2) * xx(x1,x2)
        end do
      end do
    end do
  end do
else
  do b2=1,nb2  
    do y2= 1+nb2-lag2, n2-lag2+1  
      x2= y2 - b2 + lag2
      do b1=1,nb1  
        do y1= 1+nb1-lag1, n1-lag1+1  
          x1= y1 - b1 + lag1
          bb(b1,b2) = bb(b1,b2) + yy(y1,y2) * xx(x1,x2)
        end do
      end do
    end do
  end do
end if 
return
end  

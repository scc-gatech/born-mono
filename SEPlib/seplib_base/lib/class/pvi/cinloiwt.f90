
! cinloiwt --- get triangle tent weights for 2-D convolution (cinloi()) output
!
subroutine cinloiwt( a1,a2, lag1,lag2, windwt, w1,w2)
integer              a1,a2, lag1,lag2,         w1,w2,     i1,i2, s1,s2&
  &, e1,e2
real                                   windwt( w1,w2)
real mid1,mid2, wide1,wide2, x, y
call null( windwt, w1*w2)
s1= 1+a1-lag1
e1= w1-lag1+1
mid1=(e1+s1)/2.
wide1=(e1-s1+1.)/2.
s2= 1+a2-lag2
e2= w2-lag2+1
mid2=(e2+s2)/2.
wide2=(e2-s2+1.)/2.
do i1= s1, e1  
  x = abs((i1-mid1)/wide1)
  do i2= s2, e2  
    y = abs((i2-mid2)/wide2)
!       windwt(i1,i2) = amax1( 0., 2. - abs(x+y) - abs(x-y) )           # Cheop
    windwt(i1,i2) = amax1( 0., 1. - abs(x))  *amax1( 0., 1. - abs(y))
  end do
end do 
return
end  

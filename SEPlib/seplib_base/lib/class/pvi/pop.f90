subroutine pop( nn, n, x, ep, em, y, dp, w, lc, sbar, s)
!
!              n                         2   n+lc=nn             2
!       min   sum w(i)*(x convolve s - y)  +   sum  w(i)*(s-sbar)
!             i=1                             i=n+1
!
integer i, j, n, nn, jp, lc, n1
real x(n), y(n), w(nn), sbar(lc)        ! inputs
real ep(nn), em(nn), dp(nn), s(lc)      ! outputs
real dotw, dpp, dmm, dpm, dde, ept, cp, cm, cde
n1 = n+1
do i= 1, n  
  dp(i) = -y(i)
  ep(i) =  x(i)
  em(i) =  x(i)
end do 
ep(n1) = 1.
em(n1) = 1.
do i= 1, lc
  dp(i+n) = -sbar(i)
end do 
do jp= 1, lc  
  if ( jp .ne. 1 ) then
    ep( n+jp) = 0.
    em( n-jp+2) = 0.
    dpp = dotw( n1, ep(jp), ep(jp), w(jp))
    dmm = dotw( n1, em,     em,     w(jp))
    dpm = dotw( n1, ep(jp), em,     w(jp))
    cp = dpm / dpp
    cm = dpm / dmm
    do i= 1, n1  
      j = i+jp-1
      ept   = ep(j) - em(i) * cm
      em(i) = em(i) - ep(j) * cp
      ep(j) = ept
    end do
  end if
  dde = dotw( n1, dp(jp), em, w(jp))
  dmm = dotw( n1, em,     em, w(jp))
  cde = dde / dmm
  do i= 1, n1
    dp( i+jp-1) = dp( i+jp-1) - em(i) * cde
  end do
end do 
do i= 1, lc
  s(i) = dp( n+i) + sbar(i)
end do 
return
end
real function dotw( n, x, y, w)
integer i, n
real x(n), y(n), w(n), ans
ans = 0.
do i= 1, n
  ans = ans + w(i) * x(i) * y(i)
end do 
dotw = ans
return
end  

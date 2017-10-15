subroutine hope( gap, h1,h2,hh, t1,t2,tt, a1,a2,aa, p1,p2,pp, known,&
  & niter)
integer         h1,h2,h12,  t1,t2,t12,  a1,a2,a12,  p1,p2,p12
integer         i, gap, iter, niter, midpt, nx,nr, px,ax,  qr,tr,hr
real            hh(h1,h2), tt(t1,t2), aa(a1,a2), pp(p1*p2), known(p1*p2&
  &), dot
real  x( p1*p2 +a1*a2),  rr( p1*p2 +p1*p2 +t1*t2)
real  g( p1*p2 +a1*a2),  gg( p1*p2 +p1*p2 +t1*t2)
real  s( p1*p2 +a1*a2),  ss( p1*p2 +p1*p2 +t1*t2)
p12 = p1*p2
a12 = a1*a2
t12 = t1*t2
h12= h1*h2
nx  = p12 + a12
px= 1
ax= 1+p12
nr  = p12 + p12 + t12
qr= 1
hr= 1+p12
tr= 1+p12+p12
call zero( a12, aa)
midpt= a1/2
aa( midpt, 1 ) = sqrt( dot( p12,pp,pp))
call zero( nx, x)
call zero( nr, rr)
call copy( p12, pp, x(px))
call zero( nx, g)
call zero( nr, gg)
call copy( a12, aa, x(ax))
do iter= 0, niter  
  call cinloi( 0, 0, midpt,1, a1,a2,aa, p1,p2,pp,    rr(qr))
  call cinloi( 0, 0, midpt,1, h1,h2,hh, p1,p2,pp,    rr(hr))
  call cinloi( 0, 0, midpt,1, a1,a2,aa, t1,t2,tt,    rr(tr))
  call scaleit (                     -1., nr,          rr    )
  call cinloi( 1, 0, midpt,1, a1,a2,aa, p1,p2,g(px), rr(qr))
  call cinlof( 1, 0, midpt,1, p1,p2,pp, a1,a2,g(ax), rr(qr))
  call cinloi( 1, 1, midpt,1, h1,h2,hh, p1,p2,g(px), rr(hr))
  call cinlof( 1, 1, midpt,1, t1,t2,tt, a1,a2,g(ax), rr(tr))
  do i= 1, p12        
    if ( known(i) .ne. 0.) then
      g( i + (px-1)) = 0.
    end if
  end do 
  do i= 1, midpt+gap  
    g( i + (ax-1)) = 0.
  end do 
  call cinloi( 0, 0, midpt,1, a1,a2,aa, p1,p2,g(px), gg(qr))
  call cinlof( 0, 1, midpt,1, p1,p2,pp, a1,a2,g(ax), gg(qr))
  call cinloi( 0, 0, midpt,1, h1,h2,hh, p1,p2,g(px), gg(hr))
  call cinlof( 0, 0, midpt,1, t1,t2,tt, a1,a2,g(ax), gg(tr))
  call cgstep( iter, nx,  x, g, s,                            nr, rr,gg&
    &,ss )
  call copy( p12, x(px), pp)
  call copy( a12, x(ax), aa)
end do 
return
end  

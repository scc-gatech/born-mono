      MODULE PVI
      INTERFACE
      SUBROUTINE ADVANCE(adj,add,jump,nx,xx,ny,yy)
         INTEGER, INTENT(IN) :: adj,add,jump,nx,ny
         REAL, DIMENSION(1:nx), INTENT(INOUT) :: xx
         REAL, DIMENSION(1:ny), INTENT(INOUT) :: yy
      END SUBROUTINE ADVANCE
      SUBROUTINE BURG2(adj,add,lag1,lag2,data,n1,n2,aa,a1,a2,residual)
         INTEGER, INTENT(IN) :: adj,add,lag1,lag2,n1,n2,a1,a2
         REAL, DIMENSION(1:n1,1:n2), INTENT(IN) :: data
         REAL, DIMENSION(1:a1,1:a2), INTENT(INOUT) :: aa
         REAL, DIMENSION(1:n1,1:2*n2), INTENT(INOUT) :: residual
      END SUBROUTINE BURG2
      SUBROUTINE BURG(lx,x,la,a)
         INTEGER, INTENT(IN) :: lx,la
         REAL, DIMENSION(1:lx), INTENT(IN) :: x
         REAL, DIMENSION(1:la), INTENT(OUT) :: a
      END SUBROUTINE BURG
      SUBROUTINE BUTTER(hilo,cutoff,npoly,num,den)
         REAL, INTENT(IN) :: hilo,cutoff
         INTEGER, INTENT(IN) :: npoly
         REAL, DIMENSION(1:npoly), INTENT(OUT) :: num,den
      END SUBROUTINE BUTTER
      SUBROUTINE CAUSINT2(adj,add,n,xx,yy)
         INTEGER, INTENT(IN) :: adj,add,n
         REAL, DIMENSION(1:n), INTENT(INOUT) :: xx,yy
      END SUBROUTINE CAUSINT2
      SUBROUTINE CGMETH(nx,x,nr,yy,rr,aaa,niter)
         INTEGER, INTENT(IN) :: nx,nr,niter
         REAL, DIMENSION(1:nr,1:nx), INTENT(IN) :: aaa
         REAL, DIMENSION(1:nx), INTENT(OUT) :: x
         REAL, DIMENSION(1:nr), INTENT(IN) :: yy
         REAL, DIMENSION(1:nr), INTENT(OUT) :: rr
      END SUBROUTINE CGMETH
      SUBROUTINE CGSTEP(iter,n,x,g,s,m,rr,gg,ss)
         INTEGER, INTENT(IN) :: iter,n,m
         REAL, DIMENSION(1:n), INTENT(INOUT) :: x
         REAL, DIMENSION(1:n), INTENT(IN) :: g
         REAL, DIMENSION(1:n), INTENT(OUT) :: s
         REAL, DIMENSION(1:m), INTENT(INOUT) :: rr
         REAL, DIMENSION(1:m), INTENT(IN) :: gg
         REAL, DIMENSION(1:m), INTENT(OUT) :: ss
      END SUBROUTINE CGSTEP
      SUBROUTINE CINJOF(adj,add,jump,n1,n2,xx,nb1,nb2,bb,yy)
         INTEGER, INTENT(IN) :: adj,add,jump,n1,n2,nb1,nb2
         REAL, DIMENSION(1:n1,1:n2), INTENT(IN) :: xx
         REAL, DIMENSION(1:nb1,1:nb2), INTENT(INOUT) :: bb
         REAL, DIMENSION(1:n1,1:n2), INTENT(INOUT) :: yy
      END SUBROUTINE CINJOF
      SUBROUTINE CINLOF(adj,add,lag1,lag2,n1,n2,xx,nb1,nb2,bb,yy)
         INTEGER, INTENT(IN) :: adj,add,lag1,lag2,n1,n2,nb1,nb2
         REAL, DIMENSION(1:n1,1:n2), INTENT(IN) :: xx
         REAL, DIMENSION(1:nb1,1:nb2), INTENT(INOUT) :: bb
         REAL, DIMENSION(1:n1,1:n2), INTENT(INOUT) :: yy
      END SUBROUTINE CINLOF
!skipped cinloip, cinloiwt
      SUBROUTINE CINLOI(adj,add,lag1,lag2,nb1,nb2,bb,n1,n2,xx,yy)
         INTEGER, INTENT(IN) :: adj,add,lag1,lag2,n1,n2,nb1,nb2
         REAL, DIMENSION(1:nb1,1:nb2), INTENT(IN) :: bb
         REAL, DIMENSION(1:n1,1:n2), INTENT(INOUT) :: xx
         REAL, DIMENSION(1:n1,1:n2), INTENT(INOUT) :: yy
      END SUBROUTINE CINLOI
      SUBROUTINE CINTERP0(x,nx,x0,dx,cb,cbx)
         INTEGER, INTENT(IN) :: nx
         REAL, INTENT(IN) :: x,x0,dx
         COMPLEX, DIMENSION(1:nx), INTENT(IN) :: cb
         COMPLEX, INTENT(OUT) :: cbx
      END SUBROUTINE CINTERP0
      SUBROUTINE CONJNULL(conj,add,x,nx,y,ny)
         INTEGER, INTENT(IN) :: conj,add,nx,ny
         REAL, DIMENSION(1:nx), INTENT(INOUT) :: x
         REAL, DIMENSION(1:ny), INTENT(INOUT) :: y
      END SUBROUTINE CONJNULL
      SUBROUTINE CONTRUNC(conj,add,lag,np,pp,nf,ff,nq,qq)
         INTEGER, INTENT(IN) :: conj,add,lag,np,nf,nq
         REAL, DIMENSION(1:np), INTENT(IN) :: pp
         REAL, DIMENSION(1:nf), INTENT(INOUT) :: ff
         REAL, DIMENSION(1:nq), INTENT(INOUT) :: qq
      END SUBROUTINE CONTRUNC
      SUBROUTINE CONVIN(adj,add,nx,xx,nb,bb,yy)
         INTEGER, INTENT(IN) :: adj,add,nx,nb
         REAL, DIMENSION(1:nx), INTENT(IN) :: xx
         REAL, DIMENSION(1:nb), INTENT(INOUT) :: bb
         REAL, DIMENSION(1:1+nx-nb), INTENT(INOUT) :: yy
      END SUBROUTINE CONVIN
      SUBROUTINE CONVOLVE(nb,bb,nx,xx,yy)
         INTEGER, INTENT(IN) :: nb,nx
         REAL, DIMENSION(1:nb), INTENT(IN) :: bb
         REAL, DIMENSION(1:nx), INTENT(IN) :: xx
         REAL, DIMENSION(1:nx+nb-1), INTENT(OUT) :: yy
      END SUBROUTINE CONVOLVE
      SUBROUTINE DEGHOST(eps,nb,bb,n,yy,xx,rr,niter)
         INTEGER, INTENT(IN) :: nb,n,niter
         REAL, INTENT(IN) :: eps
         REAL, DIMENSION(1:nb), INTENT(IN) :: bb
         REAL, DIMENSION(1:n), INTENT(IN) :: yy
         REAL, DIMENSION(1:n), INTENT(INOUT) :: xx
         REAL, DIMENSION(1:2*n), INTENT(OUT) :: rr
      END SUBROUTINE DEGHOST
      REAL FUNCTION DOT(n,x,y)
         INTEGER, INTENT(IN) :: n
         REAL, DIMENSION(1:n), INTENT(IN) :: x,y
      END FUNCTION DOT
      REAL FUNCTION DOTW(n,x,y,w)
         INTEGER, INTENT(IN) :: n
         REAL, DIMENSION(1:n), INTENT(IN) :: x,y,w
      END FUNCTION DOTW
      SUBROUTINE FTDERIVSLOW(nyq,t0,dt,ntf,ctt,cdd)
         INTEGER, INTENT(IN) :: nyq,ntf
         REAL, INTENT(IN) :: t0,dt
         COMPLEX, DIMENSION(1:ntf), INTENT(IN) :: ctt
         COMPLEX, DIMENSION(1:ntf), INTENT(OUT) :: cdd
      END SUBROUTINE FTDERIVSLOW
      SUBROUTINE FTLAGSLOW(nyq,lag,t0,dt,n1,ctt)
         INTEGER, INTENT(IN) :: nyq,n1
         REAL, INTENT(IN) :: lag,t0,dt
         REAL, DIMENSION(1:n1), INTENT(INOUT) :: ctt
      END SUBROUTINE FTLAGSLOW
      SUBROUTINE HESTENES(nx,x,nr,yy,rr,aaa,niter)
         INTEGER, INTENT(IN) :: nx,nr,niter
         REAL, DIMENSION(1:nx), INTENT(OUT) :: x
         REAL, DIMENSION(1:nr), INTENT(INOUT) :: yy
         REAL, DIMENSION(1:nr), INTENT(OUT) :: rr
         REAL, DIMENSION(1:nr,1:nx), INTENT(IN) :: aaa
      END SUBROUTINE HESTENES
      SUBROUTINE HOPE(gap,h1,h2,hh,t1,t2,tt,a1,a2,aa,p1,p2,pp,&
     &                known,niter)
         INTEGER, INTENT(IN) :: gap,h1,h2,t1,t2,a1,a2,p1,p2,niter
         REAL, DIMENSION(1:h1,1:h2), INTENT(INOUT) :: hh
         REAL, DIMENSION(1:t1,1:t2), INTENT(INOUT) :: tt
         REAL, DIMENSION(1:a1,1:a2), INTENT(INOUT) :: aa
         REAL, DIMENSION(1:p1*p2), INTENT(INOUT) :: pp
         REAL, DIMENSION(1:p1*p2), INTENT(IN) :: known
      END SUBROUTINE HOPE
      SUBROUTINE IMO1(adj,add,xs,t0,dt,nt,zz,tt)
         INTEGER, INTENT(IN) :: adj,add,nt
         REAL, INTENT(IN) :: xs,t0,dt
         REAL, DIMENSION(1:nt), INTENT(INOUT) :: zz,tt
      END SUBROUTINE IMO1
      SUBROUTINE IMOSPRAY(adj,add,slow,x0,dx,t0,dt,nx,nt,stack,gather)
         INTEGER, INTENT(IN) :: adj,add,nx,nt
         REAL, INTENT(IN) :: slow,x0,dx,t0,dt
         REAL, DIMENSION(1:nt), INTENT(INOUT) :: stack
         REAL, DIMENSION(1:nt,1:nx), INTENT(INOUT) :: gather
      END SUBROUTINE IMOSPRAY
      SUBROUTINE INER(nf,f,nr,yy,rr,ww,niter,lag,gap1,gapn)
         INTEGER, INTENT(IN) :: nf,nr,niter,lag,gap1,gapn
         REAL, DIMENSION(1:nf), INTENT(INOUT) :: f
         REAL, DIMENSION(1:nr), INTENT(INOUT) :: yy,rr
         REAL, DIMENSION(1:nr), INTENT(IN) :: ww
      END SUBROUTINE INER
      SUBROUTINE INVSTACK(nt,model,nx,gather,rr,t0,x0,dt,dx,slow,niter)
         INTEGER, INTENT(IN) :: nt,nx,niter
         REAL, INTENT(IN) :: t0,x0,dt,dx,slow
         REAL, DIMENSION(1:nt,1:nx), INTENT(IN) :: gather
         REAL, DIMENSION(1:nt), INTENT(OUT) :: model
         REAL, DIMENSION(1:nt,1:nx), INTENT(OUT) :: rr
      END SUBROUTINE INVSTACK
      SUBROUTINE KOLMOGOROFF(n,cx)
         INTEGER, INTENT(IN) :: n
         COMPLEX, DIMENSION(1:n), INTENT(INOUT) :: cx
      END SUBROUTINE KOLMOGOROFF
      SUBROUTINE LEAK(rho,n,xx,yy)
         INTEGER, INTENT(IN) :: n
         REAL, INTENT(IN) :: rho
         REAL, DIMENSION(1:n), INTENT(IN) :: xx
         REAL, DIMENSION(1:n), INTENT(OUT) :: yy
      END SUBROUTINE LEAK
      SUBROUTINE LEAKY(distance,m1,n12,uu,vv)
         INTEGER, INTENT(IN) :: m1,n12
         REAL, INTENT(IN) :: distance
         REAL, DIMENSION(1:m1,1:n12), INTENT(IN) :: uu
         REAL, DIMENSION(1:m1,1:n12), INTENT(OUT) :: vv
      END SUBROUTINE LEAKY
      SUBROUTINE LOMOPLAN(agc,eps,w1,w2,data,n1,n2,resid,gap,&
     &                    aa,a1,a2,k1,k2)
         INTEGER, INTENT(IN) :: agc,w1,w2,n1,n2,gap,a1,a2,k1,k2
         REAL, INTENT(IN) :: eps
         REAL, DIMENSION(1:n1,1:n2), INTENT(IN) :: data
         REAL, DIMENSION(1:n1,1:n2), INTENT(OUT) :: resid
         REAL, DIMENSION(1:a1,1:a2,1:k1,1:k2), INTENT(OUT) :: aa
      END SUBROUTINE LOMOPLAN
      SUBROUTINE MISFIP(nt,tt,na,aa,np,pp,known,niter)
         INTEGER, INTENT(IN) :: nt,na,np,niter
         REAL, DIMENSION(1:nt), INTENT(INOUT) :: tt
         REAL, DIMENSION(1:na), INTENT(OUT) :: aa
         REAL, DIMENSION(1:np), INTENT(INOUT) :: pp
         REAL, DIMENSION(1:np), INTENT(IN) :: known
      END SUBROUTINE MISFIP
      SUBROUTINE MISS1(na,a,np,p,copy,niter)
         INTEGER, INTENT(IN) :: na,np,niter
         REAL, DIMENSION(1:na), INTENT(IN) :: a
         REAL, DIMENSION(1:np), INTENT(INOUT) :: p
         REAL, DIMENSION(1:np), INTENT(IN) :: copy
      END SUBROUTINE MISS1
      SUBROUTINE MISS2(lag1,lag2,a1,a2,aa,n1,n2,ww, pp,known,rr,niter)
         INTEGER, INTENT(IN) :: lag1,lag2,a1,a2,n1,n2,niter
         REAL, DIMENSION(1:n1,1:n2), INTENT(INOUT) :: pp
         REAL, DIMENSION(1:n1,1:n2), INTENT(IN) :: known
         REAL, DIMENSION(1:n1,1:n2), INTENT(IN) :: ww
         REAL, DIMENSION(1:a1,1:a2), INTENT(IN) :: aa
         REAL, DIMENSION(1:n1,1:n2*2), INTENT(OUT) :: rr
      END SUBROUTINE MISS2
      SUBROUTINE MISSIF(na,lag,aa,np,pp,known,niter)
         INTEGER, INTENT(IN) :: na,lag,np,niter
         REAL, DIMENSION(1:np), INTENT(INOUT) :: pp
         REAL, DIMENSION(1:np), INTENT(IN) :: known
         REAL, DIMENSION(1:na), INTENT(INOUT) :: aa
      END SUBROUTINE MISSIF
      SUBROUTINE MKWALLWT(k1,k2,windwt,w1,w2,wallwt,n1,n2)
         INTEGER, INTENT(IN) :: k1,k2,w1,w2,n1,n2
         REAL, DIMENSION(1:w1,1:w2), INTENT(INOUT) :: windwt
         REAL, DIMENSION(1:n1,1:n2), INTENT(OUT) :: wallwt
      END SUBROUTINE MKWALLWT
      SUBROUTINE MOPLAN(agc,eps,data,n1,n2,niter,lag1,lag2,gap,&
     &                  aa,a1,a2,rr)
         INTEGER, INTENT(IN) :: agc,n1,n2,niter,lag1,lag2,a1,a2,gap
         REAL, INTENT(IN) :: eps
         REAL, DIMENSION(1:n1,1:n2), INTENT(IN) :: data
         REAL, DIMENSION(1:a1,1:a2), INTENT(OUT) :: aa
         REAL, DIMENSION(1:n1*n2*2+a1*a2), INTENT(OUT) :: rr
      END SUBROUTINE MOPLAN
      SUBROUTINE MPWAVE(n,cx)
         INTEGER, INTENT(IN) :: n
         COMPLEX, DIMENSION(n), INTENT(INOUT) :: cx
      END SUBROUTINE MPWAVE
      SUBROUTINE NONSTAT2(n1,n2,w1,w2,j1,j2,data,output,weight)
         INTEGER, INTENT(IN) :: n1,n2,w1,w2,j1,j2
         REAL, DIMENSION(1:n1,1:n2), INTENT(IN) :: data
         REAL, DIMENSION(1:n1,1:n2), INTENT(OUT) :: weight
         REAL, DIMENSION(1:n1,1:n2), INTENT(OUT) :: output
      END SUBROUTINE NONSTAT2
      SUBROUTINE NONSTAT(n1,n2,w1,w2,j1,j2,count)
         INTEGER, INTENT(IN) :: n1,n2,w1,w2,j1,j2
         REAL, DIMENSION(1:n1,1:n2), INTENT(OUT) :: count
      END SUBROUTINE NONSTAT
      SUBROUTINE PARCEL(adj,add,n1,n2,w1,w2,k1,k2,wall,wind)
         INTEGER, INTENT(IN) :: adj,add,n1,n2,w1,w2,k1,k2
         REAL, DIMENSION(1:n1,1:n2), INTENT(INOUT) :: wall
         REAL, DIMENSION(1:w1,1:w2,1:k1,1:k2), INTENT(INOUT) :: wind
      END SUBROUTINE PARCEL
      SUBROUTINE PATCH(adj,add,j1,j2,k1,k2,wall,n1,n2,wind,w1,w2)
         INTEGER, INTENT(IN) :: adj,add,j1,j2,k1,k2,n1,n2,w1,w2
         REAL, DIMENSION(1:n1,1:n2), INTENT(INOUT) :: wall
         REAL, DIMENSION(1:w1,1:w2), INTENT(INOUT) :: wind
      END SUBROUTINE PATCH
      SUBROUTINE PE2(eps,a1,a2,aa,n1,n2,pp,rr,niter,jump)
         INTEGER, INTENT(IN) :: a1,a2,n1,n2,niter,jump
         REAL, INTENT(IN) :: eps
         REAL, DIMENSION(1:a1,1:a2), INTENT(OUT) :: aa
         REAL, DIMENSION(1:n1,1:n2), INTENT(IN) :: pp
         REAL, DIMENSION(1:n1,1:n2*2), INTENT(OUT) :: rr
      END SUBROUTINE PE2
      SUBROUTINE POLYDIV(na,aa,nx,xx,ny,yy)
         INTEGER, INTENT(IN) :: na,nx,ny
         REAL, DIMENSION(1:na), INTENT(IN) :: aa
         REAL, DIMENSION(1:nx), INTENT(IN) :: xx
         REAL, DIMENSION(1:ny), INTENT(OUT) :: yy
      END SUBROUTINE POLYDIV
      SUBROUTINE POLYFT(nt,tt,nw,cww)
         INTEGER, INTENT(IN) :: nt,nw
         REAL, DIMENSION(1:nt), INTENT(IN) :: tt
         COMPLEX, DIMENSION(1:nw), INTENT(OUT) :: cww
      END SUBROUTINE POLYFT
      SUBROUTINE POP(nn,n,x,ep,em,y,dp,w,lc,sbar,s)
         INTEGER, INTENT(IN) :: nn,n,lc
         REAL, DIMENSION(1:n), INTENT(IN) :: x,y
         REAL, DIMENSION(1:nn), INTENT(IN) :: w
         REAL, DIMENSION(1:lc), INTENT(IN) :: sbar
         REAL, DIMENSION(1:nn), INTENT(OUT) :: ep,em,dp
         REAL, DIMENSION(1:lc), INTENT(OUT) :: s
      END SUBROUTINE POP
      SUBROUTINE PUCK(n1,n2,uu,coh,pp,res)
         INTEGER, INTENT(IN) :: n1,n2
         REAL, INTENT(OUT) :: coh,pp
         REAL, DIMENSION(1:n1,1:n2), INTENT(IN) :: uu
         REAL, DIMENSION(1:n1,1:n2), INTENT(OUT) :: res
      END SUBROUTINE PUCK
      SUBROUTINE SCALEIT(factor,n,data)
         INTEGER, INTENT(IN) :: n
         REAL, INTENT(IN) :: factor
         REAL, DIMENSION(1:n), INTENT(INOUT) :: data
      END SUBROUTINE SCALEIT
      SUBROUTINE SHAPER(nf,ff,nx,xx,ny,yy,rr,niter)
         INTEGER, INTENT(IN) :: nf,nx,ny,niter
         REAL, DIMENSION(1:nf), INTENT(OUT) :: ff
         REAL, DIMENSION(1:nx), INTENT(IN) :: xx
         REAL, DIMENSION(1:ny), INTENT(IN) :: yy
         REAL, DIMENSION(1:ny), INTENT(OUT) :: rr
      END SUBROUTINE SHAPER
      SUBROUTINE SLIDER(n1,n2,w1,w2,k1,k2,data,coh,pp,res)
         INTEGER, INTENT(IN) :: n1,n2,w1,w2,k1,k2
         REAL, DIMENSION(1:n1,1:n2), INTENT(IN) :: data
         REAL, DIMENSION(1:n1,1:n2), INTENT(OUT) :: coh,pp
         REAL, DIMENSION(1:n1,1:n2), INTENT(OUT) :: res
      END SUBROUTINE SLIDER
      SUBROUTINE SLOWFT(adj,add,nyq,t0,dt,nt,tt,f0,df,nf,ff)
         INTEGER, INTENT(IN) :: adj,add,nyq,nt,nf
         REAL, INTENT(IN) :: t0,dt,f0,df
         COMPLEX, DIMENSION(1:nt), INTENT(INOUT) :: tt
         COMPLEX, DIMENSION(1:nf), INTENT(INOUT) :: ff
      END SUBROUTINE SLOWFT
      SUBROUTINE TRIS(n,endl,a,b,c,endr,d,t)
         INTEGER, INTENT(IN) :: n
         REAL, INTENT(IN) :: endl,a,b,c,endr
         REAL, DIMENSION(1:n), INTENT(IN) :: d
         REAL, DIMENSION(1:n), INTENT(OUT) :: t
      END SUBROUTINE TRIS
      SUBROUTINE VSPRAY(adj,nt,dt,t0,nx,dx,x0,tx,ns,ds,s0,zs)
         INTEGER, INTENT(IN) :: adj,nt,nx,ns
         REAL, INTENT(IN) :: dt,t0,dx,x0,ds,s0
         REAL, DIMENSION(1:nt,1:nx), INTENT(INOUT) :: tx
         REAL, DIMENSION(1:nt,1:ns), INTENT(INOUT) :: zs
      END SUBROUTINE VSPRAY
      SUBROUTINE WAVEKILL(aa,pp,n1,n2,uu,vv)
         INTEGER, INTENT(IN) :: n1,n2
         REAL, INTENT(IN) :: aa,pp
         REAL, DIMENSION(1:n1,1:n2), INTENT(IN) :: uu
         REAL, DIMENSION(1:n1-1,1:n2-1), INTENT(OUT) :: vv
      END SUBROUTINE WAVEKILL
      SUBROUTINE WCONTRUNC(adj,add,ww,lag,nx,xx,nf,ff,nn,yy)
         INTEGER, INTENT(IN) :: adj,add,lag,nx,nf,nn
         REAL, DIMENSION(1:nn), INTENT(IN) :: ww
         REAL, DIMENSION(1:nn), INTENT(INOUT) :: yy
         REAL, DIMENSION(1:nx), INTENT(IN) :: xx
         REAL, DIMENSION(1:nf), INTENT(INOUT) :: ff
      END SUBROUTINE WCONTRUNC
      SUBROUTINE ZERO(n,xx)
         INTEGER, INTENT(IN) :: n
         REAL, DIMENSION(1:n), INTENT(OUT) :: xx
      END SUBROUTINE ZERO
      END INTERFACE
      END MODULE PVI

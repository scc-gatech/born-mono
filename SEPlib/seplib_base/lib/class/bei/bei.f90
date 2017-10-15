      MODULE BEI
      INTERFACE
      SUBROUTINE AAMIG(adj,add,slow,antialias,t0,dt,dx,nt,nx,image,data)
         INTEGER, INTENT(IN) :: adj, add, nx, nt
         REAL, DIMENSION(1:nt), INTENT(IN) :: slow
         REAL, DIMENSION(1:nt,1:nx), INTENT(INOUT) :: image, data
         REAL, INTENT(IN) :: antialias,t0,dt,dx
      END SUBROUTINE AAMIG
      SUBROUTINE CAUSINT2(adj,add,n,xx,yy)
         INTEGER, INTENT(IN) :: adj,add,n
         REAL, DIMENSION(1:n), INTENT(INOUT) :: xx,yy
      END SUBROUTINE CAUSINT2
      SUBROUTINE COSMIG(adj,add,velhalf,h,t0,dt,dx,modl,nt,nx,data)
         INTEGER, INTENT(IN) :: adj,add,nt,nx
         REAL, INTENT(IN) :: velhalf,h,t0,dt,dx
         REAL, DIMENSION(1:nt,1:nx), INTENT(INOUT) :: modl,data
      END SUBROUTINE COSMIG
      SUBROUTINE DMOKIRCH(adj,add,velhalf,h,t0,dt,dx,modl,nt,nx,data)
         INTEGER, INTENT(IN) :: adj,add,nt,nx
         REAL, INTENT(IN) :: velhalf,h,t0,dt,dx
         REAL, DIMENSION(1:nt,1:nx), INTENT(INOUT) :: modl,data
      END SUBROUTINE DMOKIRCH
      SUBROUTINE DOUBINT(adj,add,n,pp,qq)
         INTEGER, INTENT(IN) :: adj,add,n
         REAL, DIMENSION(1:n), INTENT(INOUT) :: pp,qq
      END SUBROUTINE DOUBINT
      SUBROUTINE DPBIN2(adj,add,o1,d1,o2,d2,xy,mm,m1,m2,dd,nd)
         INTEGER, INTENT(IN) :: adj,add,m1,m2,nd
         REAL, INTENT(IN) :: o1,d1,o2,d2
         REAL, DIMENSION(1:2,1:nd), INTENT(IN) :: xy
         REAL, DIMENSION(1:m1,1:m2), INTENT(INOUT) :: mm
         REAL, DIMENSION(1:nd), INTENT(INOUT) :: dd
      END SUBROUTINE DPBIN2
      COMPLEX FUNCTION EIKTAU(dt,w,vkx,qi)
         REAL, INTENT(IN) :: dt,w,vkx,qi
      END FUNCTION EIKTAU
      SUBROUTINE FLATHYP(adj,add,vel,h,t0,dt,dx,modl,nt,nx,data)
         INTEGER, INTENT(IN) :: adj,add,nt,nx
         REAL, INTENT(IN) :: h,t0,dt,dx
         REAL, DIMENSION(1:nt), INTENT(IN) :: vel
         REAL, DIMENSION(1:nt,1:nx), INTENT(INOUT) :: modl,data
      END SUBROUTINE FLATHYP
      SUBROUTINE FT1AXE(adj,add,sign1,n1,n2,cx,cy)
         INTEGER, INTENT(IN) :: adj,add,n1,n2
         REAL, INTENT(IN) :: sign1
         COMPLEX, DIMENSION(1:n1,1:n2), INTENT(INOUT) :: cx,cy
      END SUBROUTINE FT1AXE
      SUBROUTINE GAZDAGJ(adj,dt,dx,v,nt,nx,modl,data)
         INTEGER, INTENT(IN) :: adj,nt,nx
         REAL, INTENT(IN) :: dt,dx
         REAL, DIMENSION(1:nt), INTENT(IN) :: v
         REAL, DIMENSION(1:nt,1:nx), INTENT(INOUT) :: modl,data
      END SUBROUTINE GAZDAGJ
      SUBROUTINE KDMOSLOW(adj,add,v,h,t0,dt,dx,nt,nx,p0,pn)
         INTEGER, INTENT(IN) :: adj,add,nt,nx
         REAL, INTENT(IN) :: v,h,t0,dt,dx
         REAL, DIMENSION(1:nt,1:nx), INTENT(INOUT) :: p0,pn
      END SUBROUTINE KDMOSLOW
      SUBROUTINE KIRCHFAST(adj,add,vrms,t0,dt,dx,modl,nt,nx,data)
         INTEGER, INTENT(IN) :: adj,add,nt,nx
         REAL, INTENT(IN) :: t0,dt,dx
         REAL, DIMENSION(1:nt), INTENT(IN) :: vrms
         REAL, DIMENSION(1:nt,1:nx), INTENT(INOUT) :: modl,data
      END SUBROUTINE KIRCHFAST
      SUBROUTINE KIRCHSLOW(adj,add,velhalf,t0,dt,dx,modl,nt,nx,data)
         INTEGER, INTENT(IN) :: adj,add,nt,nx
         REAL, INTENT(IN) :: velhalf,t0,dt,dx
         REAL, DIMENSION(1:nt,1:nx), INTENT(INOUT) :: modl,data
      END SUBROUTINE KIRCHSLOW
      SUBROUTINE LINT1(adj,add,o1,d1,coordinate,mm,m1,dd,nd)
         INTEGER, INTENT(IN) :: adj,add,m1,nd
         REAL, INTENT(IN) :: o1,d1
         REAL, DIMENSION(1:nd), INTENT(IN) :: coordinate
         REAL, DIMENSION(1:m1), INTENT(INOUT) :: mm
         REAL, DIMENSION(1:nd), INTENT(INOUT) :: dd
      END SUBROUTINE LINT1
      SUBROUTINE LMO(adj,add,slow,tau0,t0,dt,x0,dx,modl,nt,nx,data)
         INTEGER, INTENT(IN) :: adj,add,nt,nx
         REAL, INTENT(IN) :: slow,tau0,t0,dt,x0,dx
         REAL, DIMENSION(1:nt,1:nx), INTENT(INOUT) :: modl,data
      END SUBROUTINE LMO
      SUBROUTINE MATMUL(adj,bb,nx,x,ny,y)
         INTEGER, INTENT(IN) :: adj,nx,ny
         REAL, DIMENSION(1:ny,1:nx), INTENT(IN) :: bb
         REAL, DIMENSION(1:nx), INTENT(INOUT) :: x
         REAL, DIMENSION(1:ny), INTENT(INOUT) :: y
      END SUBROUTINE MATMUL
      SUBROUTINE MUTTER1(tp,slope0,slopep,t0,dt,x,data,nt)
         INTEGER, INTENT(IN) :: nt
         REAL, INTENT(IN) :: tp,slope0,slopep,t0,dt,x
         REAL, DIMENSION(1:nt), INTENT(INOUT) :: data
      END SUBROUTINE MUTTER1
      SUBROUTINE MUTTER(tp,slope0,slopep,dt,dx,t0,x0,data,nt,nx)
         INTEGER, INTENT(IN) :: nt,nx
         REAL, INTENT(IN) :: tp,slope0,slopep,dt,dx,t0,x0
         REAL, DIMENSION(1:nt,1:nx), INTENT(INOUT) :: data
      END SUBROUTINE MUTTER
      SUBROUTINE NMO0(adj,add,slow,x,t0,dt,n,zz,tt)
         INTEGER, INTENT(IN) :: adj,add,n
         REAL, INTENT(IN) :: t0,dt
         REAL, DIMENSION(1:n), INTENT(IN) :: slow
         REAL, DIMENSION(1:n), INTENT(INOUT) :: zz,tt
      END SUBROUTINE NMO0
      SUBROUTINE PHASEMIG(up,nt,nx,dt,dx,image,ntau,dtau,vel)
         INTEGER, INTENT(IN) :: nt,nx,ntau
         REAL, INTENT(IN) :: dt,dx,dtau,vel
         COMPLEX, DIMENSION(1:nt,1:nx), INTENT(INOUT) :: up
         COMPLEX, DIMENSION(1:ntau,1:nx), INTENT(OUT) :: image
      END SUBROUTINE PHASEMIG
      SUBROUTINE PHASEMOD(image,nz,nx,dz,dx,up,nt,dt,vel)
         INTEGER, INTENT(IN) :: nz,nx,nt
         REAL, INTENT(IN) :: dz,dx,dt,vel
         COMPLEX, DIMENSION(1:nt,1:nx), INTENT(OUT) :: up
         COMPLEX, DIMENSION(1:nz,1:nx), INTENT(INOUT) :: image
      END SUBROUTINE PHASEMOD
      SUBROUTINE RMS2INT(inverse,dt,vrms,nt,vint)
         INTEGER, INTENT(IN) :: inverse,nt
         REAL, INTENT(IN) :: dt
         REAL, DIMENSION(1:nt), INTENT(IN) :: vrms
         REAL, DIMENSION(1:nt), INTENT(OUT) :: vint
      END SUBROUTINE RMS2INT
      SUBROUTINE RTRIS(n,endl,a,b,c,endr,d,q)
         INTEGER, INTENT(IN) :: n
         REAL, INTENT(IN) :: endl,endr
         REAL, INTENT(IN) :: a,b,c
         REAL, DIMENSION(1:n), INTENT(IN) :: d
         REAL, DIMENSION(1:n), INTENT(OUT) :: q
      END SUBROUTINE RTRIS
      SUBROUTINE SCALE(factor,n,data)
         INTEGER, INTENT(IN) :: n
         REAL, INTENT(IN) :: factor
         REAL, DIMENSION(1:n), INTENT(INOUT) :: data
      END SUBROUTINE SCALE
      SUBROUTINE SLOWFIT(vsurface,alpha,t0,dt,s0,ds,scan,nt,ns,reg,slow)
         INTEGER, INTENT(IN) :: nt,ns
         REAL, INTENT(IN) :: vsurface,alpha,t0,dt,s0,ds
         REAL, DIMENSION(1:nt), INTENT(OUT) :: reg,slow
         REAL, DIMENSION(1:nt,1:ns), INTENT(OUT) :: scan
      END SUBROUTINE SLOWFIT
      SUBROUTINE SPOTW(adj,add,scale,nt,t0,dt,t,val,vec)
         INTEGER, INTENT(IN) :: adj,add,nt
         REAL, INTENT(IN) :: scale,t0,dt,t
         REAL, INTENT(INOUT) :: val
         REAL, DIMENSION(1:nt), INTENT(INOUT) :: vec
      END SUBROUTINE SPOTW
      SUBROUTINE STACK0(adj,add,slow,t0,dt,x0,dx,nt,nx,stack,gather)
         INTEGER, INTENT(IN) :: adj,add,nt,nx
         REAL, INTENT(IN) :: t0,dt,x0,dx
         REAL, DIMENSION(1:nt), INTENT(IN) :: slow
         REAL, DIMENSION(1:nt), INTENT(INOUT) :: stack
         REAL, DIMENSION(1:nt,1:nx), INTENT(INOUT) :: gather
      END SUBROUTINE STACK0
      SUBROUTINE STOLTMIG(nt,nz,nx,dt,dz,dx,vel,cftdata,cftimage)
         INTEGER, INTENT(IN) :: nt,nz,nx
         REAL, INTENT(IN) :: dt,dz,dx,vel
         REAL, DIMENSION(1:nt,1:nx), INTENT(IN) :: cftdata
         REAL, DIMENSION(1:nz,1:nx), INTENT(OUT) :: cftimage
      END SUBROUTINE STOLTMIG
      SUBROUTINE STOLTMOD(nz,nt,nx,dz,dt,dx,vel,cftimage,cftdata)
         INTEGER, INTENT(IN) :: nt,nz,nx
         REAL, INTENT(IN) :: dt,dz,dx,vel
         REAL, DIMENSION(1:nt,1:nx), INTENT(OUT) :: cftdata
         REAL, DIMENSION(1:nz,1:nx), INTENT(IN) :: cftimage
      END SUBROUTINE STOLTMOD
      SUBROUTINE SYNMARINE(data,nt,nh,ny,nz)
         INTEGER, INTENT(IN) :: nt,nh,ny,nz
         REAL, DIMENSION(1:nt,1:nh,1:ny), INTENT(OUT) :: data
      END SUBROUTINE SYNMARINE
      SUBROUTINE TRIMO(adj,add,t0,dt,dx,x,nt,slow,s02,wt,anti,zz,tt)
         INTEGER, INTENT(IN) :: adj,add,nt
         REAL, INTENT(IN) :: t0,dt,dx,x,s02,wt,anti
         REAL, DIMENSION(1:nt), INTENT(IN) :: slow
         REAL, DIMENSION(1:nt), INTENT(INOUT) :: zz,tt
      END SUBROUTINE TRIMO
      SUBROUTINE TRISTACK(adj,add,slow,anti,t0,dt,x0,dx,nt,nx,stack,    &
     &                    gather)
         INTEGER, INTENT(IN) :: adj,add,nt,nx
         REAL, INTENT(IN) :: t0,dt,x0,dx,anti
         REAL, DIMENSION(1:nt), INTENT(IN) :: slow
         REAL, DIMENSION(1:nt), INTENT(INOUT) :: stack
         REAL, DIMENSION(1:nt,1:nx), INTENT(INOUT) :: gather
      END SUBROUTINE TRISTACK
      SUBROUTINE VELSIMP(adj,add,t0,dt,x0,dx,s0,ds,nt,nx,ns,modl,data)
         INTEGER, INTENT(IN) :: adj,add,nt,nx,ns
         REAL, INTENT(IN) :: t0,dt,x0,dx,s0,ds
         REAL, DIMENSION(1:nt,1:ns), INTENT(INOUT) :: modl
         REAL, DIMENSION(1:nt,1:nx), INTENT(INOUT) :: data
      END SUBROUTINE VELSIMP
      SUBROUTINE VELTRAN(adj,add,psun,s02,anti,t0,dt,x0,dx,s0,ds,nt,nx, &
     &                   ns,model,data)
         INTEGER, INTENT(IN) :: adj,add,psun,nt,nx,ns
         REAL, INTENT(IN) :: s02,anti,t0,dt,x0,dx,s0,ds
         REAL, DIMENSION(1:nt,1:ns), INTENT(INOUT) :: model
         REAL, DIMENSION(1:nt,1:nx), INTENT(INOUT) :: data
      END SUBROUTINE VELTRAN
      SUBROUTINE VINT2RMS(inverse,vminallow,dt,vint,nt,vrms)
         INTEGER, INTENT(IN) :: inverse,nt
         REAL, INTENT(IN) :: vminallow,dt
         REAL, DIMENSION(1:nt), INTENT(INOUT) :: vint
         REAL, DIMENSION(1:nt), INTENT(INOUT) :: vrms
      END SUBROUTINE VINT2RMS
      SUBROUTINE WAVEMOVIE(nt,nx,nz,nw,nlam,dx,dz,v,p,cd,q)
         INTEGER, INTENT(IN) :: nt,nx,nz,nw,nlam
         REAL, INTENT(IN) :: dx,dz,v
         REAL, DIMENSION(1:nz,1:nx,1:nz), INTENT(OUT) :: p
         COMPLEX, DIMENSION(1:nx), INTENT(OUT) :: cd,q
      END SUBROUTINE WAVEMOVIE
      SUBROUTINE ZERO(n,xx)
         INTEGER, INTENT(IN) :: n
         REAL, DIMENSION(1:n), INTENT(OUT) :: xx
      END SUBROUTINE ZERO
      END INTERFACE
      END MODULE BEI

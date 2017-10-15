      MODULE CLASSUTIL
      INTERFACE
!     SUBROUTINE ADJNULL(adj,add,x,nx,y,ny)
!        INTEGER, INTENT(IN) :: adj,add,nx,ny
!        REAL, DIMENSION(1:nx), INTENT(INOUT) ::  x
!        REAL, DIMENSION(1:ny), INTENT(INOUT) ::  y
!     END SUBROUTINE ADJNULL
      SUBROUTINE LADJNULL(adj,add,x,y)
         LOGICAL, INTENT(IN) :: adj,add
         REAL, DIMENSION(1:), INTENT(INOUT) ::  x
         REAL, DIMENSION(1:), INTENT(INOUT) ::  y
      END SUBROUTINE LADJNULL
      SUBROUTINE BOXCONV(nb,nx,xx,yy)
         INTEGER, INTENT(IN) :: nb, nx
         REAL, DIMENSION(1:nx), INTENT(IN) :: xx
         REAL, DIMENSION(1:), INTENT(OUT) :: yy !nx+nb-1
      END SUBROUTINE BOXCONV
      SUBROUTINE BOXMO(adj,add,t0,dt,dx,x,nt,slow,antialias,zz,tt)
         INTEGER, INTENT(IN) :: adj,add,nt
         REAL, INTENT(IN) :: t0,dt,dx,x,antialias
         REAL, DIMENSION(1:nt), INTENT(IN) :: slow
         REAL, DIMENSION(1:nt), INTENT(INOUT) :: zz,tt
      END SUBROUTINE BOXMO
      SUBROUTINE CADD(n,xx,yy)
         INTEGER, INTENT(IN) :: n
         REAL, DIMENSION(n), INTENT(IN) :: xx
         REAL, DIMENSION(n), INTENT(INOUT) :: yy
      END SUBROUTINE CADD
      SUBROUTINE CAUSINT(adj,add,n,xx,yy)
         INTEGER, INTENT(IN) :: adj,add,n
         REAL, DIMENSION(1:n), INTENT(INOUT) :: xx,yy
      END SUBROUTINE CAUSINT
      SUBROUTINE CINTERP1(x,nx,x0,dx,cb,cbx)
         INTEGER, INTENT(IN) :: nx
         REAL, INTENT(IN) :: x,x0,dx
         COMPLEX, DIMENSION(1:nx), INTENT(IN) :: cb
         COMPLEX, INTENT(OUT) :: cbx
      END SUBROUTINE CINTERP1
      SUBROUTINE CONTRAN(adj,add,nx,xx,nb,bb,yy)
         INTEGER, INTENT(IN) :: adj,add,nx,nb
         REAL, DIMENSION(1:nx), INTENT(IN) :: xx
         REAL, DIMENSION(1:nb), INTENT(INOUT) :: bb
         REAL, DIMENSION(1:), INTENT(INOUT) :: yy
      END SUBROUTINE CONTRAN
      SUBROUTINE COPY(n,xx,yy)
         INTEGER, INTENT(IN) :: n
         REAL, DIMENSION(1:n), INTENT(IN) :: xx
         REAL, DIMENSION(1:n), INTENT(OUT) :: yy
      END SUBROUTINE COPY
      SUBROUTINE CTRIS(n,endl,a,b,c,endr,d,q)
         INTEGER, INTENT(IN) :: n
         COMPLEX, INTENT(IN) :: endl,endr
         COMPLEX, INTENT(IN) :: a,b,c
         COMPLEX, DIMENSION(1:n), INTENT(IN) :: d
         COMPLEX, DIMENSION(1:n), INTENT(OUT) :: q
      END SUBROUTINE CTRIS
      SUBROUTINE DIAG(adj,add,lambda,n,pp,qq)
         INTEGER, INTENT(IN) :: adj,add,n
         REAL, DIMENSION(1:n), INTENT(IN) :: lambda
         REAL, DIMENSION(1:n), INTENT(INOUT) :: pp,qq
      END SUBROUTINE DIAG
      SUBROUTINE FT1AXIS(adj,sign1,n1,n2,cx)
         INTEGER, INTENT(IN) :: adj,n1,n2
         REAL, INTENT(IN) :: sign1
         COMPLEX, DIMENSION(1:n1,1:n2), INTENT(INOUT) :: cx
      END SUBROUTINE FT1AXIS
      SUBROUTINE FT2AXIS(adj,sign2,n1,n2,cx)
         INTEGER, INTENT(IN) :: adj,n1,n2
         REAL, INTENT(IN) :: sign2
         COMPLEX, DIMENSION(1:n1,1:n2), INTENT(INOUT) :: cx
      END SUBROUTINE FT2AXIS
      SUBROUTINE FT3D1AXIS(adj,sign1,n1,n2,n3,cx)
         INTEGER, INTENT(IN) :: adj,n1,n2,n3
         REAL, INTENT(IN) :: sign1
         COMPLEX, DIMENSION(1:n1,1:n2,1:n3), INTENT(INOUT) :: cx
      END SUBROUTINE FT3D1AXIS
      SUBROUTINE FT3D2AXIS(adj,sign2,n1,n2,n3,cx)
         INTEGER, INTENT(IN) :: adj,n1,n2,n3
         REAL, INTENT(IN) :: sign2
         COMPLEX, DIMENSION(1:n1,1:n2,1:n3), INTENT(INOUT) :: cx
      END SUBROUTINE FT3D2AXIS
      SUBROUTINE FT3D3AXIS(adj,sign3,n1,n2,n3,cx)
         INTEGER, INTENT(IN) :: adj,n1,n2,n3
         REAL, INTENT(IN) :: sign3
         COMPLEX, DIMENSION(1:n1,1:n2,1:n3), INTENT(INOUT) :: cx
      END SUBROUTINE FT3D3AXIS
      SUBROUTINE FTH(adj,sign,m1,n12,cx)
         INTEGER, INTENT(IN) :: adj,m1,n12
         REAL, INTENT(IN) :: sign
         COMPLEX, DIMENSION(1:m1,1:n12), INTENT(INOUT) :: cx
      END SUBROUTINE FTH
      SUBROUTINE FTU(signi,nx,cx)
         INTEGER, INTENT(IN) :: nx
         REAL, INTENT(IN) :: signi
         COMPLEX, DIMENSION(1:nx), INTENT(INOUT) :: cx
      END SUBROUTINE FTU
      SUBROUTINE HALFDIFA(adj,add,n,xx,yy)
         INTEGER, INTENT(IN) :: adj,add,n
         REAL, DIMENSION(1:n), INTENT(INOUT) :: xx,yy
      END SUBROUTINE HALFDIFA
      SUBROUTINE IDENT(adj,add,epsilon,n,pp,qq)
         INTEGER, INTENT(IN) :: adj,add,n
         REAL, INTENT(IN) :: epsilon
         REAL, DIMENSION(1:n), INTENT(INOUT) :: pp,qq
      END SUBROUTINE IDENT
      SUBROUTINE IGRAD1(adj,add,xx,n,yy)
         INTEGER, INTENT(IN) :: adj,add,n
         REAL, DIMENSION(1:n), INTENT(INOUT) :: xx,yy
      END SUBROUTINE IGRAD1
      SUBROUTINE MATMULT(adj,add,bb,nx,x,ny,y)
         INTEGER, INTENT(IN) :: adj,add,nx,ny
         REAL, DIMENSION(1:ny,1:nx), INTENT(IN) :: bb
         REAL, DIMENSION(1:nx), INTENT(INOUT) :: x
         REAL, DIMENSION(1:ny), INTENT(INOUT) :: y
      END SUBROUTINE MATMULT
      SUBROUTINE NMO1(adj,add,slow,x,t0,dt,n,zz,tt)
         INTEGER, INTENT(IN) :: adj,add,n
         REAL, INTENT(IN) :: t0,dt
         REAL, DIMENSION(1:n), INTENT(IN) :: slow
         REAL, DIMENSION(1:n), INTENT(INOUT) :: zz,tt
      END SUBROUTINE NMO1
      SUBROUTINE NULL(xx,n)
         INTEGER, INTENT(IN) :: n
         REAL, DIMENSION(1:n), INTENT(OUT) :: xx
      END SUBROUTINE NULL
      INTEGER FUNCTION PAD2(n)
         INTEGER, INTENT(IN) :: n
      END FUNCTION PAD2
      SUBROUTINE QUANTILE(k,n,bb,value)
         INTEGER, INTENT(IN) :: k,n
         REAL, DIMENSION(1:n), INTENT(IN) :: bb
         REAL, INTENT(OUT) :: value
      END SUBROUTINE QUANTILE
      SUBROUTINE QUANTABS(k,n,bb,value)
         INTEGER, INTENT(IN) :: k,n
         REAL, DIMENSION(1:n), INTENT(IN) :: bb
         REAL, INTENT(OUT) :: value
      END SUBROUTINE QUANTABS
      SUBROUTINE QUANTINTERNAL(k,n,a)
         INTEGER, INTENT(IN) :: k,n
         REAL, DIMENSION(1:n), INTENT(INOUT) :: a
      END SUBROUTINE QUANTINTERNAL
      REAL FUNCTION RAND01(iseed)
         INTEGER, INTENT(INOUT) :: iseed
      END FUNCTION RAND01
      SUBROUTINE RUFFEN1(adj,n,xx,yy)
         INTEGER, INTENT(IN) :: adj,n
         REAL, DIMENSION(1:n), INTENT(INOUT) :: xx
         REAL, DIMENSION(1:n-1), INTENT(INOUT) :: yy
      END SUBROUTINE RUFFEN1
      REAL FUNCTION SIGNUM(x)
         REAL, INTENT(IN) :: x
      END FUNCTION SIGNUM
      SUBROUTINE SIMPLEFT(adj,add,t0,dt,tt,nt,f0,df,ff,nf)
         INTEGER, INTENT(IN) :: adj,add,nt,nf
         REAL, INTENT(IN) :: t0,dt,f0,df
         COMPLEX, DIMENSION(1:nt), INTENT(INOUT) :: tt
         COMPLEX, DIMENSION(1:nf), INTENT(INOUT) :: ff
      END SUBROUTINE SIMPLEFT
      SUBROUTINE SPOT0(adj,add,nt,t0,dt,t,val,vec)
         INTEGER, INTENT(IN) :: adj,add,nt
         REAL, INTENT(IN) :: t0,dt,t
         REAL, INTENT(INOUT) :: val
         REAL, DIMENSION(1:nt), INTENT(INOUT) :: vec
      END SUBROUTINE SPOT0
      SUBROUTINE SPOT1(adj,add,nt,t0,dt,t,val,vec)
         INTEGER, INTENT(IN) :: adj,add,nt
         REAL, INTENT(IN) :: t0,dt,t
         REAL, INTENT(INOUT) :: val
         REAL, DIMENSION(1:nt), INTENT(INOUT) :: vec
      END SUBROUTINE SPOT1
      SUBROUTINE STACK1(adj,add,slow,t0,dt,x0,dx,nt,nx,stack,gather)
         INTEGER, INTENT(IN) :: adj,add,nt,nx
         REAL, INTENT(IN) :: t0,dt,x0,dx
         REAL, DIMENSION(1:nt), INTENT(IN) :: slow
         REAL, DIMENSION(1:nt), INTENT(INOUT) :: stack
         REAL, DIMENSION(1:nt,1:nx), INTENT(INOUT) :: gather
      END SUBROUTINE STACK1
      SUBROUTINE TRIANGLE2(rect1,rect2,n1,n2,uu,vv)
         INTEGER, INTENT(IN) :: rect1,rect2,n1,n2 
         REAL, DIMENSION(1:n1,1:n2), INTENT(IN) :: uu
         REAL, DIMENSION(1:n1,1:n2), INTENT(OUT) :: vv
      END SUBROUTINE TRIANGLE2
      SUBROUTINE TRIANGLE(nr,m1,n12,uu,vv)
         INTEGER, INTENT(IN) :: nr,m1,n12 
         REAL, DIMENSION(1:m1,1:n12), INTENT(IN) :: uu
         REAL, DIMENSION(1:m1,1:n12), INTENT(OUT) :: vv
      END SUBROUTINE TRIANGLE
      SUBROUTINE ZPAD1(adj,add,data,nd,padd,np)
         INTEGER, INTENT(IN) :: adj,add,nd,np
         REAL, DIMENSION(1:nd), INTENT(INOUT) :: data
         REAL, DIMENSION(1:np), INTENT(INOUT) :: padd
      END SUBROUTINE ZPAD1
      END INTERFACE
      INTERFACE ADJNULL
         MODULE PROCEDURE ADJNULL771D1DR
         MODULE PROCEDURE ADJNULL771D1DC
         MODULE PROCEDURE ADJNULL771D2DR
         MODULE PROCEDURE ADJNULL772D1DR
         MODULE PROCEDURE ADJNULL772D2DR
         MODULE PROCEDURE ADJNULL771D3DR
         MODULE PROCEDURE ADJNULL773D1DR
         MODULE PROCEDURE ADJNULL772D3DR
         MODULE PROCEDURE ADJNULL773D2DR
         MODULE PROCEDURE ADJNULL773D3DR
         MODULE PROCEDURE ADJNULL771D4DR
         MODULE PROCEDURE ADJNULL774D1DR
         MODULE PROCEDURE ADJNULL772D4DR
         MODULE PROCEDURE ADJNULL774D2DR
         MODULE PROCEDURE ADJNULL773D4DR
         MODULE PROCEDURE ADJNULL774D3DR
         MODULE PROCEDURE ADJNULL774D4DR
         MODULE PROCEDURE ADJNULL901D1DR
         MODULE PROCEDURE ADJNULL902D2DR
      END INTERFACE
      CONTAINS
      SUBROUTINE ADJNULL771D1DR( ADJ, ADD, X, NX,  Y, NY )
      INTEGER IX, IY,     ADJ, ADD,    NX,     NY
      REAL                          X( : ), Y( :  )
      IF( ADD .EQ. 0 ) THEN
          IF( ADJ .EQ. 0 ) THEN
              Y = 0.
          ELSE
              X = 0.
          ENDIF
      ENDIF
      RETURN
      END SUBROUTINE ADJNULL771D1DR
      SUBROUTINE ADJNULL771D1DC( ADJ, ADD, X, NX,  Y, NY )
      INTEGER IX, IY,     ADJ, ADD,    NX,     NY
      COMPLEX                       X( : ), Y( :  )
      IF( ADD .EQ. 0 ) THEN
          IF( ADJ .EQ. 0 ) THEN
              Y = 0.
          ELSE
              X = 0.
          ENDIF
      ENDIF
      RETURN
      END SUBROUTINE ADJNULL771D1DC
      SUBROUTINE ADJNULL771D2DR( ADJ, ADD, X, NX,  Y, NY )
      INTEGER IX, IY,     ADJ, ADD,    NX,     NY
      REAL                          X( : ), Y( :,: )
      IF( ADD .EQ. 0 ) THEN
          IF( ADJ .EQ. 0 ) THEN
              Y = 0.
          ELSE
              X = 0. 
          ENDIF
      ENDIF
      RETURN
      END SUBROUTINE ADJNULL771D2DR
      SUBROUTINE ADJNULL772D1DR( ADJ, ADD, X, NX,  Y, NY )
      INTEGER IX, IY,     ADJ, ADD,    NX,     NY
      REAL                          X( :,: ), Y( : )
      IF( ADD .EQ. 0 ) THEN
          IF( ADJ .EQ. 0 ) THEN
              Y = 0.
          ELSE
              X = 0. 
          ENDIF
      ENDIF
      RETURN
      END SUBROUTINE ADJNULL772D1DR
      SUBROUTINE ADJNULL772D2DR( ADJ, ADD, X, NX,  Y, NY )
      INTEGER IX, IY,     ADJ, ADD,    NX,     NY
      REAL                          X( :,: ), Y( :,: )
      IF( ADD .EQ. 0 ) THEN
          IF( ADJ .EQ. 0 ) THEN
              Y = 0.
          ELSE
              X = 0. 
          ENDIF
      ENDIF
      RETURN
      END SUBROUTINE ADJNULL772D2DR
      SUBROUTINE ADJNULL771D3DR( ADJ, ADD, X, NX,  Y, NY )
      INTEGER IX, IY,     ADJ, ADD,    NX,     NY
      REAL                          X( : ), Y( :,:,: )
      IF( ADD .EQ. 0 ) THEN
          IF( ADJ .EQ. 0 ) THEN
                  Y = 0.
          ELSE
                  X = 0.
          ENDIF
      ENDIF
      RETURN
      END SUBROUTINE ADJNULL771D3DR
      SUBROUTINE ADJNULL773D1DR( ADJ, ADD, X, NX,  Y, NY )
      INTEGER IX, IY,     ADJ, ADD,    NX,     NY
      REAL                          X( :,:,: ), Y( : )
      IF( ADD .EQ. 0 ) THEN
          IF( ADJ .EQ. 0 ) THEN
                  Y = 0.
          ELSE
                  X = 0.
          ENDIF
      ENDIF
      RETURN
      END SUBROUTINE ADJNULL773D1DR
      SUBROUTINE ADJNULL772D3DR( ADJ, ADD, X, NX,  Y, NY )
      INTEGER IX, IY,     ADJ, ADD,    NX,     NY
      REAL                          X( :,: ), Y( :,:,: )
      IF( ADD .EQ. 0 ) THEN
          IF( ADJ .EQ. 0 ) THEN
                  Y = 0.
          ELSE
                  X = 0.
          ENDIF
      ENDIF
      RETURN
      END SUBROUTINE ADJNULL772D3DR
      SUBROUTINE ADJNULL773D2DR( ADJ, ADD, X, NX,  Y, NY )
      INTEGER IX, IY,     ADJ, ADD,    NX,     NY
      REAL                          X( :,:,: ), Y( :,: )
      IF( ADD .EQ. 0 ) THEN
          IF( ADJ .EQ. 0 ) THEN
                  Y = 0.
          ELSE
                  X = 0.
          ENDIF
      ENDIF
      RETURN
      END SUBROUTINE ADJNULL773D2DR
      SUBROUTINE ADJNULL773D3DR( ADJ, ADD, X, NX,  Y, NY )
      INTEGER IX, IY,     ADJ, ADD,    NX,     NY
      REAL                          X( :,:,: ), Y( :,:,: )
      IF( ADD .EQ. 0 ) THEN
          IF( ADJ .EQ. 0 ) THEN
                  Y = 0.
          ELSE
                  X = 0.
          ENDIF
      ENDIF
      RETURN
      END SUBROUTINE ADJNULL773D3DR
      SUBROUTINE ADJNULL771D4DR( ADJ, ADD, X, NX,  Y, NY )
      INTEGER IX, IY,     ADJ, ADD,    NX,     NY
      REAL                          X( : ), Y( :,:,:,: )
      IF( ADD .EQ. 0 ) THEN
          IF( ADJ .EQ. 0 ) THEN
                  Y = 0.
          ELSE
                  X = 0.
          ENDIF
      ENDIF
      RETURN
      END SUBROUTINE ADJNULL771D4DR
      SUBROUTINE ADJNULL774D1DR( ADJ, ADD, X, NX,  Y, NY )
      INTEGER IX, IY,     ADJ, ADD,    NX,     NY
      REAL                          X( :,:,:,: ), Y( : )
      IF( ADD .EQ. 0 ) THEN
          IF( ADJ .EQ. 0 ) THEN
                  Y = 0.
          ELSE
                  X = 0.
          ENDIF
      ENDIF
      RETURN
      END SUBROUTINE ADJNULL774D1DR
      SUBROUTINE ADJNULL772D4DR( ADJ, ADD, X, NX,  Y, NY )
      INTEGER IX, IY,     ADJ, ADD,    NX,     NY
      REAL                          X( :,: ), Y( :,:,:,: )
      IF( ADD .EQ. 0 ) THEN
          IF( ADJ .EQ. 0 ) THEN
                  Y = 0.
          ELSE
                  X = 0.
          ENDIF
      ENDIF
      RETURN
      END SUBROUTINE ADJNULL772D4DR
      SUBROUTINE ADJNULL774D2DR( ADJ, ADD, X, NX,  Y, NY )
      INTEGER IX, IY,     ADJ, ADD,    NX,     NY
      REAL                          X( :,:,:,: ), Y( :,: )
      IF( ADD .EQ. 0 ) THEN
          IF( ADJ .EQ. 0 ) THEN
                  Y = 0.
          ELSE
                  X = 0.
          ENDIF
      ENDIF
      RETURN
      END SUBROUTINE ADJNULL774D2DR
      SUBROUTINE ADJNULL773D4DR( ADJ, ADD, X, NX,  Y, NY )
      INTEGER IX, IY,     ADJ, ADD,    NX,     NY
      REAL                          X( :,:,: ), Y( :,:,:,: )
      IF( ADD .EQ. 0 ) THEN
          IF( ADJ .EQ. 0 ) THEN
                  Y = 0.
          ELSE
                  X = 0.
          ENDIF
      ENDIF
      RETURN
      END SUBROUTINE ADJNULL773D4DR
      SUBROUTINE ADJNULL774D3DR( ADJ, ADD, X, NX,  Y, NY )
      INTEGER IX, IY,     ADJ, ADD,    NX,     NY
      REAL                          X( :,:,:,: ), Y( :,:,: )
      IF( ADD .EQ. 0 ) THEN
          IF( ADJ .EQ. 0 ) THEN
                  Y = 0.
          ELSE
                  X = 0.
          ENDIF
      ENDIF
      RETURN
      END SUBROUTINE ADJNULL774D3DR
      SUBROUTINE ADJNULL774D4DR( ADJ, ADD, X, NX,  Y, NY )
      INTEGER IX, IY,     ADJ, ADD,    NX,     NY
      REAL                          X( :,:,:,: ), Y( :,:,:,: )
      IF( ADD .EQ. 0 ) THEN
          IF( ADJ .EQ. 0 ) THEN
                  Y = 0.
          ELSE
                  X = 0.
          ENDIF
      ENDIF
      RETURN
      END SUBROUTINE ADJNULL774D4DR
      SUBROUTINE ADJNULL901D1DR( ADJ, ADD, X, Y )
      LOGICAL ADJ, ADD
      REAL X(:), Y(:)
      IF(.NOT. ADD) THEN
         IF( .NOT. ADJ) THEN
             Y = 0.0
         ELSE
             X = 0.0
         ENDIF
      ENDIF
      RETURN
      END SUBROUTINE ADJNULL901D1DR
      SUBROUTINE ADJNULL902D2DR( ADJ, ADD, X, Y )
      LOGICAL ADJ, ADD
      REAL X(:,:), Y(:,:)
      IF(.NOT. ADD) THEN
         IF( .NOT. ADJ) THEN
             Y = 0.0
         ELSE
             X = 0.0
         ENDIF
      ENDIF
      RETURN
      END SUBROUTINE ADJNULL902D2DR
      END MODULE CLASSUTIL

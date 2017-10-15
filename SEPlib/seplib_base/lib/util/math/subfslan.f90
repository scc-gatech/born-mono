!#if defined (CRAY)
!#define cdabs CABS
!#define DREAL REAL
!#define DIMAG AIMAG
!#define DCONJG CONJG
!#endif
!!$
!!$=head1 NAME
!!$
!!$ hermtoep - toeplitz solver
!!$
!!$=head1 SYNOPSIS
!!$
!!$ call  HERMTOEP (M,T0,T,Z,X,ISTAT)
!!$
!!$=head1 DESCRIPTION
!!$
!!$ Solve hermitian toeplitz system
!!$
!!$=head1 INPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item m     - integer         
!!$
!!$      size
!!$
!!$=item t     - complex*16(m+1) 
!!$
!!$      ????
!!$
!!$=item z     - complex*16(m+1) 
!!$
!!$      ????
!!$
!!$=item x     - complex*16(m+1) 
!!$
!!$      ????
!!$
!!$=item istat - integer         
!!$
!!$      status
!!$
!!$=back
!!$
!!$=head1 LIBRARY
!!$
!!$B<sepmathf90>
!!$
!!$=cut
!>




      SUBROUTINE HERMTOEP (M,T0,T,Z,X,ISTAT)
use sep_mod
use sep_func_mod
!
!
      PARAMETER ( NAMAX = 10000 )
      COMPLEX*16  T(M),X(M+1),Z(M+1)
      COMPLEX*16  TEMP,SAVE,ALPHA,BETA,A(NAMAX)
      DOUBLE PRECISION T0
      DOUBLE PRECISION P

	if( M .GT. NAMAX ) call erexit('parameter statement')

      P=T0
      ISTAT=1
      IF (P .EQ. 0.)  RETURN
!     Handle  M=0  as a special case
      X(1)=Z(1)/T0                            
      IF (M .LE. 0)  RETURN
!
!   Main recursion
!
      K=0
100   K=K+1
      SAVE=T(K)
      BETA=X(1)*T(K)
      IF (K .EQ. 1)  GO TO 20
      DO 10 J=1,K-1
        SAVE=SAVE+A(J)*T(K-J)                 
10      BETA=BETA+X(J+1)*T(K-J)               
20    TEMP = -SAVE/P

!      if( cdabs(TEMP) .GT. 0.99D0 ) RETURN

      P=P*(1.-DREAL(TEMP)**2-DIMAG(TEMP)**2)   

      IF (P .LE. 1.D-10 )  THEN
	RETURN
	ENDIF

30    A(K)=TEMP
      ALPHA=(Z(K+1)-BETA)/P                   
      IF (K .EQ. 1)  GO TO 50
      KHALF=K/2
      DO 40 J=1,KHALF
        KJ=K-J
        SAVE=A(J)
        A(J)=SAVE+TEMP*DCONJG(A(KJ))           
        IF (J .EQ. KJ)  GO TO 40
        A(KJ)=A(KJ)+TEMP*DCONJG(SAVE)          
40      CONTINUE
	
50    X(K+1)=ALPHA

      DO 60 J=1,K
60      X(J)=X(J)+ALPHA*DCONJG(A(K-J+1))       
      IF (K .LT. M)  GO TO 100

      ISTAT=0
      RETURN
      END

      SUBROUTINE HERMTOEPREFC (M,T0,T,refc)
use sep_mod
use sep_func_mod
!
!     reflection coefficients for the Toeplitz matrix
!
	PARAMETER ( NAMAX = 10000 )
      COMPLEX*16  T(M)
      COMPLEX*16  TEMP,SAVE,ALPHA,BETA,A(NAMAX)
      DOUBLE PRECISION T0
      DOUBLE PRECISION P

	complex*16 refc(M) 

	if( M .GT. NAMAX ) call erexit('parameter statement')

	do ii=1,M
	refc(ii) = 0.D0
	enddo

      P=T0
      IF (P .EQ. 0.)  RETURN
!     Handle  M=0  as a special case
      IF (M .LE. 0)  RETURN
!
!   Main recursion
!
      K=0
100   K=K+1
      SAVE=T(K)
      IF (K .EQ. 1)  GO TO 20
      DO 10 J=1,K-1
        SAVE=SAVE+A(J)*T(K-J)                 
10      CONTINUE
20    refc(K) = -SAVE/P

      if( cdabs(refc(K)) .GT. 0.99D0 ) RETURN

      P=P*(1.-DREAL(refc(K))**2-DIMAG(refc(K))**2)   

      IF (P .LE. 1.D-10 )  THEN
	RETURN
	ENDIF

30    A(K)=refc(K)
      IF (K .EQ. 1)  GO TO 50
      KHALF=K/2
      DO 40 J=1,KHALF
        KJ=K-J
        SAVE=A(J)
        A(J)=SAVE+refc(K)*DCONJG(A(KJ))           
        IF (J .EQ. KJ)  GO TO 40
        A(KJ)=A(KJ)+refc(K)*DCONJG(SAVE)          
40      CONTINUE
	
50    CONTINUE

      IF (K .LT. M)  GO TO 100

      ISTAT=0
      RETURN
      END

      SUBROUTINE REFCAAC16 (M,T0,T,refc,aa,ldaa)
use sep_mod
!
!
	PARAMETER ( NAMAX = 10000 ) 
      COMPLEX*16  T(M), a(NAMAX), SAVE
      DOUBLE PRECISION T0
      DOUBLE PRECISION P

	integer ldaa,K
	complex*16 refc(ldaa) , aa(ldaa,m+1)

	if( M .GT. NAMAX ) call erexit('parameter statement')
      P=T0
      ISTAT=1
      IF (P .EQ. 0.)  RETURN
!   Handle  M=0  as a special case
      IF (M .LE. 0)  RETURN
!
!   Main recursion
!

	do ii = 1, (M+1)
	do jj = 1, (M+1)
	aa(ii,jj) = 0.D0
	enddo
	enddo

	aa(M+1,M+1) = 1./dsqrt(P) 

      K=0
100   K=K+1

      P=P*(1.-DREAL(refc(K))**2-DIMAG(refc(K))**2)   

      IF (P .LE. 1.D-10 )  THEN
	RETURN
	ENDIF

30    A(K)=refc(K)

      IF (K .EQ. 1)  GO TO 50
      KHALF=K/2
      DO 40 J=1,KHALF
        KJ=K-J
        SAVE=A(J)
        A(J)=SAVE+refc(K)*DCONJG(A(KJ))           
        IF (J .EQ. KJ)  GO TO 40
        A(KJ)=A(KJ)+refc(K)*DCONJG(SAVE)          
40      CONTINUE
	
50    CONTINUE

	aa(M+1-K,M+1-K)  = 1.D0/dsqrt(P)
	do j=1,K
	aa(M+1-K+j,M+1-K) = a(j)/dsqrt(P)
	enddo

      IF (K .LT. M)  GO TO 100

      ISTAT=0
      RETURN
      END

      SUBROUTINE HERMTOEPREFSL (M,T0,T,Z,X,refc)
use sep_mod
!
!
	PARAMETER ( NAMAX = 10000 )
      COMPLEX*16  T(M),X(M+1),Z(M+1)
      COMPLEX*16  TEMP,SAVE,ALPHA,BETA,A(NAMAX)
      DOUBLE PRECISION T0
      DOUBLE PRECISION P
	complex*16 refc(M) 

	if( M .GT. NAMAX )  call erexit('parameter statement')

      P=T0
!     Handle  M=0  as a special case
      X(1)=Z(1)/T0                            
!
!   Main recursion
!
      K=0
100   K=K+1
      SAVE=T(K)
      BETA=X(1)*T(K)
      IF (K .EQ. 1)  GO TO 20
      DO 10 J=1,K-1
10      BETA=BETA+X(J+1)*T(K-J)               
20    CONTINUE

      P=P*(1.-DREAL(refc(K))**2-DIMAG(refc(K))**2)   

30    A(K)=refc(K)
      ALPHA=(Z(K+1)-BETA)/P                   
      IF (K .EQ. 1)  GO TO 50
      KHALF=K/2
      DO 40 J=1,KHALF
        KJ=K-J
        SAVE=A(J)
        A(J)=SAVE+refc(K)*DCONJG(A(KJ))           
        IF (J .EQ. KJ)  GO TO 40
        A(KJ)=A(KJ)+refc(K)*DCONJG(SAVE)          
40      CONTINUE
	
50    X(K+1)=ALPHA

      DO 60 J=1,K
60      X(J)=X(J)+ALPHA*DCONJG(A(K-J+1))       
      IF (K .LT. M)  GO TO 100

      RETURN
      END

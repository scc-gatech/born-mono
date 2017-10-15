!  edit history:
! 10-28-90 jon	changed iand to and so it would compile on sun.
! 03-20-91 W. Bauske IBM added block data to initialize init in common/oconft/

	 subroutine cfft1 (a, npts,iop)

	parameter (len=8192, len2=len/2, lenlg2=13)
!   nu is the order of the FFT to do.  nu = 10 means 1024 points.
!   nu = 8 means 256 points.


	 real a(2,*)
	 real xr1(len), xr2(len), xi1(len), xi2(len)

!   The coefficients are precomputed and are passed in a common
!   array.  The routine COEFF must be called before the first
!   call to FFT in order to initialize the coefficients.  The array
!   iscramb is a vector of indeces that is used to unscramble the
!   output of the FFT routine at the end.

	 real twr(len2,lenlg2), twi(len2,lenlg2)
	 integer iscramb(len,lenlg2)
	 common /coef/ twr, twi, iscramb
	 common /oconft/ init

	 if(npts.gt.len) then
	    write(0,*)&
      	    ' Sorry, too many points for cfft1(), max is ', &
           len,' you have ',npts
	    stop
	 endif
	 if(init.eq.0) then
	    call coeff
	    init=1
	 endif
	 nu=nint(alog(float(npts))/alog(2.))
	 n2 = 2**(nu-1)

!   In the first pass, all of the coefficients are (1.0, 0.0), thus
!   there is no need to do any multiplication.
!******************************************************************************
!	NOTE! The division by 32. in the first pass is not part of the normal
!	FFT routine.  This mod was made in order for the convex FFT to match
!	answers of the Stanford "fork" fft.
!******************************************************************************

	 if(iop.lt.0) then
	 	scale=.5/n2
!		compute asymmetric part and negate
		do 5 i=2,n2
			xr2(i)=a(1,i)-a(1,npts-i+2)
5			xi2(i)=a(2,i)-a(2,npts-i+2)
		do 6 i=2,n2
			a(1,i)=(a(1,i)-xr2(i))*scale
6			a(2,i)=(a(2,i)-xi2(i))*scale
		do 7 i=2,n2
			a(1,npts-i+2)=(a(1,npts-i+2)+xr2(i))*scale
7			a(2,npts-i+2)=(a(2,npts-i+2)+xi2(i))*scale
		a(1,1)=a(1,1)*scale
		a(2,1)=a(2,1)*scale
		a(1,n2+1)=a(1,n2+1)*scale
		a(2,n2+1)=a(2,n2+1)*scale
	 endif
	 do 10 i = 1, n2
		 xr2(2*i-1) = (a(1,i) + a(1,i+n2))
		 xi2(2*i-1) = (a(2,i) + a(2,i+n2))
		 xr2(2*i)   = (a(1,i) - a(1,i+n2))
10		 xi2(2*i)   = (a(2,i) - a(2,i+n2))

!   In the second pass, the coefficients alternate between (1.0, 0.0)
!   and (0.0, -1.0).  Again, no actual multiplication takes place, but
!   this pass is performed in two segments, corresponding to the
!   coefficient values above.

	 do 20 i = 1, n2, 2
		 xr1(2*i-1) = xr2(i) + xr2(i+n2)
		 xi1(2*i-1) = xi2(i) + xi2(i+n2)
		 xr1(2*i)   = xr2(i) - xr2(i+n2)
20		 xi1(2*i)   = xi2(i) - xi2(i+n2)

	 do 30 i = 2, n2, 2
		 xr1(2*i-1) = xr2(i) + xi2(i+n2)
		 xi1(2*i-1) = xi2(i) - xr2(i+n2)
		 xr1(2*i)   = xr2(i) - xi2(i+n2)
30		 xi1(2*i)   = xi2(i) + xr2(i+n2)

!   All the remaining passes perform multiplies using twiddle factors
!   obtained from the coefficient arrays.  Note that the imaginary
!   factor is actually the negative of the coefficient used in the
!   Cooley-Tukey FFTs.  This is accounted for in the algorithms below.

!   Do all but the last pass.

	 do 60 l = 3, nu-1

	if ((l-2*(l/2)) .ne. 0) then

		 do 40 i = 1, n2
		   treal = xr1(i+n2) * twr(i,l) + xi1(i+n2) * twi(i,l)
		   timag = xi1(i+n2) * twr(i,l) - xr1(i+n2) * twi(i,l)
		   xr2(2*i-1) = xr1(i) + treal
		   xi2(2*i-1) = xi1(i) + timag
		   xr2(2*i)   = xr1(i) - treal
40		   xi2(2*i)   = xi1(i) - timag

	 else

		 do 50 i = 1, n2
		   treal = xr2(i+n2) * twr(i,l) + xi2(i+n2) * twi(i,l)
		   timag = xi2(i+n2) * twr(i,l) - xr2(i+n2) * twi(i,l)
		   xr1(2*i-1) = xr2(i) + treal
		   xi1(2*i-1) = xi2(i) + timag
		   xr1(2*i)   = xr2(i) - treal
50		   xi1(2*i)   = xi2(i) - timag

	 endif
60	continue


!   In the last pass, we store the result back in the original input
!   array, and we reorder the output according to the bit reversed
!   indexed array.

	if ((nu-2*(nu/2)) .ne. 0) then

!$dir no_recurrence
		 do 70 i = 1, n2
		   treal = xr1(i+n2) * twr(i,nu) + xi1(i+n2) * twi(i,nu)
		   timag = xi1(i+n2) * twr(i,nu) - xr1(i+n2) * twi(i,nu)
		   a(1,iscramb(2*i-1,nu)+1) = xr1(i) + treal
		   a(2,iscramb(2*i-1,nu)+1) = xi1(i) + timag
		   a(1,iscramb(2*i,nu)+1)   = xr1(i) - treal
70		   a(2,iscramb(2*i,nu)+1)   = xi1(i) - timag

	 else

!$dir no_recurrence
		 do 80 i = 1, n2
		   treal = xr2(i+n2) * twr(i,nu) + xi2(i+n2) * twi(i,nu)
		   timag = xi2(i+n2) * twr(i,nu) - xr2(i+n2) * twi(i,nu)
		   a(1,iscramb(2*i-1,nu)+1) = xr2(i) + treal
		   a(2,iscramb(2*i-1,nu)+1) = xi2(i) + timag
		   a(1,iscramb(2*i,nu)+1)   = xr2(i) - treal
80		   a(2,iscramb(2*i,nu)+1)   = xi2(i) - timag

	 endif

	 return
	 end


	 subroutine coeff

	 parameter (len=8192,len2=len/2,lenlg2=13)

!   The coefficients are precomputed and are passed in a common
!   array.  The routine COEFF must be called before the first
!   call to FFT in order to initialize the coefficients.  The array
!   iscramb is a vector of indeces that is used to unscramble the
!   output of the FFT routine at the end.

	 real twr(len2,lenlg2), twi(len2,lenlg2)
	 integer iscramb(len,lenlg2)
	 common /coef/ twr, twi, iscramb

	 double precision arg, twopi, p

	 twopi = 6.2831853071795864769252866d0
	 n2 = len2

	 ibit = n2
	 iscramb (1,lenlg2) = 0
	 iscramb (2,lenlg2) = ibit
	 iscramb (n2+1,lenlg2) = 1
	 iscramb (n2+2,lenlg2) = ibit + 1

	 do 120 i = 2,  lenlg2-1
		 ibit = ibit / 2

		ix = (2**(i-1)) + 1
	 	do 110 j = ix, 2**i
			 iscramb(j,lenlg2) = iscramb(j-ix+1,lenlg2) + ibit
110			 iscramb(j+n2,lenlg2) = iscramb(j,lenlg2) + 1

120		 continue

	 do 140 i = 3, lenlg2-1

		 ifac = 2**(lenlg2-i)

		 do 140 j = 1, len
140			 iscramb(j,i) = iscramb(j,lenlg2) / ifac




	imsk = len2 + (len2 / 2)
        nmsk = len2 / 4
	 do 150 l = 3, lenlg2
		imsk = imsk + nmsk
		nmsk = nmsk / 2

		 do 150 i = 1, n2
			 p = iand ((iscramb(i,lenlg2) / 2), imsk)
!			 p = and ((iscramb(i,lenlg2) / 2), imsk)
			 arg = twopi * p / dfloat (len)
			 twr(i,l) = dcos (arg)
150			 twi(i,l) = dsin (arg)


	 return
	 end
	block data stinit
	 common /oconft/ init
	 data init/0/
	 end

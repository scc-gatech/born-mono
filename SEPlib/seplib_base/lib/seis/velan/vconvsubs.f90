!***********************************************************************
!
!  Subroutines for program Vconvert
!
!***********************************************************************
!
!  Subroutine to convert one kind of velocity field to another
!  note that Vrms(z) is not understood properly; must be stopped in
!  calling routine.
!
!  Author: Paul Fowler
!  Date: October 1986
!
      subroutine vc3443(iarray,rarray,data1,data2,osmpwt,work1,&
     &                  work2,work3,zin)
use sep
      integer iarray(16)
      real rarray(5)
      real data1(*),data2(*),osmpwt(*),work1(*),work2(*),work3(*)
      real zin(*)
!      call vconv(iarray(1),iarray(2),data1,data2,iarray(3),&
!     &      iarray(4), iarray(5),iarray(6),rarray(1),rarray(2),&
!     &      rarray(3),rarray(4),iarray(7),iarray(8),iarray(9),&
!     &      iarray(10),iarray(11),iarray(12),iarray(13),iarray(14),&
!     &      zin,osmpwt,work1,work2,work3,&
!     &      iarray(15),iarray(16),rarray(5))
      return
      end
!<
! vconv
! 
! call vconv(data1,data2,n1in,n1out,n2,n3,d1in,d1out,o1in,o1out,vopt1,vopt2,vpar1,vpar2,vtype1,vtype2,lsinc,points,zin,osmpwt,work1,work2,work3,n1pad,ovrsmp,alpha)
!
! INPUT PARAMETERS
! data1 -real(n1in,n2)  input data
! n1in  -integer        number of samples input
! n1out -integer        number of samples output
! n2    -integer        number of traces in gather
! n3    -integer        number of gathers
! d1in  -real           sampling of input
! d1out -real           sampling of out
! o1in  -real           first value of input
! o1out -real           first value of input
! vopt1 -integer        input type 1=velocity 2=slowness
! vopt2 -integer        output type 1=velocity 2=slowness
! vpar1 -integer        input 1=time 2=depth
! vpar2 -integer        output 1=time 2=depth
! vtype1-integer        input 1=interval 2=rms
! vtype2-integer        output 1=interval 2=rms
! lsinc -integer        oversampling interpolator length
! points-integer        oversampling rate
! zin   -real(n1pad)    converted values of input
! osmpwt-real(lsinc,n1pad) weights for oversampling
! work1 -real(n1in)     work array
! work2 -real(n1pad)    work array
! work3 -real(n1out)   work array
! n1pad -integer       padded length
! ovrsmp-integer       over-sampling rate
! alpha -real          fraction of peak at end of interpolator
!
! OUTPUT PARAMETERS
! data2 -real(n1out,n2) output data
!
! CATEGORY
! Lib:Velan
!
! COMPILE LEVEL
! DISTR
!>

      subroutine vconv(  data1,  data2, n1in, &
     &                  n1out,  n2,     n3,     d1in,  d1out, &
     &                  o1in,   o1out,  vopt1,  vopt2, vpar1, &
     &                  vpar2,  vtype1, vtype2, lsinc, points, &
     &                  zin,    osmpwt, work1,  work2, work3, &
     &                  n1pad,  ovrsmp, alpha)
use sep
!
      implicit none
!
      integer count	
!           !count of bytes read/written
!           !file descriptor for input
      integer i3	
!           !loop counter over slow axis
      integer lsinc	
!           !length of sinc interpolator
      integer nbyte1	
!           !number of bytes input
      integer nbyte2	
!           !number of bytes output
      integer nsamp	
!           !number of points in gather
      integer n1in	
!           !number of fast axis points on input
      integer n1out	
!           !number of fast axis points on output
      integer n1pad	
!           !number of fast axis points after oversampling
      integer n2	
!           !number of intermediate axis points
      integer n3	
!           !number of points on slow axis
      integer outfd	
!           !file descriptor for output
      integer ovrsmp	
!           !oversampling rate
      integer points	
!           !ovrsmp-1
!           !seplib input function
!           !seplib output function
      integer vopt1	
!           !input velocity, slowness, etc. option
      integer vopt2	
!           !output velocity, slowness, etc. option
      integer vpar1	
!           !input time or depth option
      integer vpar2	
!           !output time or depth option
      integer vtype1	
!           !input interval or rms option
	    integer ia,ib
      integer vtype2	
!           !output interval or rms option
!
      real alpha	
!           !fraction of peak at end of interpolator taper
      real d1in		
!           !fast axis input sampling interval
      real d1out	
!           !fast axis output sampling interval
      real o1in		
!           !fast axis input first sample value
      real o1out	
!           !fast axis output first sample value
      real data1(n1in,n2)	
!           !input gather
      real data2(n1out,n2)	
!           !output gather
      real osmpwt(lsinc,points)	
!           !weights for oversampling interpolator
      real work1(n1in)		
!           !work vector
      real work2(n1pad)		
!           !work vector
      real work3(n1out)		
!           !work vector
      real zin(n1pad)		
!           !converted values of input parameter
!
      nbyte1 = n1in * n2 * 4
      nbyte2 = n1out * n2 * 4
      nsamp = n1out * n2
!
      if( (vpar2 .ne. vpar1) .and. (ovrsmp .gt. 1) )then
         call mkosmp( osmpwt, points, lsinc, alpha )
      endif
!
      do 100 i3=1,n3
!
         count = sreed( "in", data1, nbyte1 )	
!           !read in a gather
         if( count .ne. nbyte1 ) then
            call seperr('Vconvert: unexpected end of data')
         endif
!
         if( vtype1 .eq. 1 )then	
!          !input is interval velocities
!
            if( vpar2 .ne. vpar1 )then	
!             !changing v(z) to v(t) or vice versa
!
               call newpar( data1, data2, work1, work2,  osmpwt,&
     &                      zin,   n1in,  n1out, d1in,   ovrsmp,&
     &                      d1out, o1in,  o1out, n2,     vpar1, &
     &                      vpar2, vopt1, lsinc, points, n1pad   )
!
            else
!
							data2(1:n1out,:)=data1(1:n1out,:)	
!	             call copy(nsamp,data1,data2)
!               call rvcopy( data1, data2, nsamp ) 
!
            endif
!
            if( vtype2 .ne. vtype1 )then	
!             !changing interval to rms 
!
               call newtyp( data2,  work3, n1out, n2,   vtype1, &
     &                      vtype2, vopt1, o1out, d1out        )
!
            endif
!
         elseif( vtype1 .eq. 2 )then	
!             !input is rms velocities
!
            if( vtype2 .ne. vtype1 )then	
!             !changing rms to interval 
!
               call newtyp( data1,  work1, n1in, n2,   vtype1, &
     &                      vtype2, vopt1, o1in, d1in        )
!
            endif
!
            if( vpar2 .ne. vpar1 )then	
!             !changing v(t) to v(z)
!
               call newpar( data1, data2, work1, work2,  osmpwt,&
     &                      zin,   n1in,  n1out, d1in,   ovrsmp,&
     &                      d1out, o1in,  o1out, n2,     vpar1, &
     &                      vpar2, vopt1, lsinc, points, n1pad   )
!
            else
!
!							data2(1:nsamp)=data1(1:nsamp)	
							data2(1:n1out,:)=data1(1:n1out,:)	
!               call copy(nsamp,data1,data2)
!               call rvcopy( data1, data2, nsamp ) 
!
             endif
!
         else
!
            call seperr &
     &		('Vconvert: illegal value of vtype1 encountered.' )
!
         endif
!
         if( vopt2 .ne. vopt1 )then	
!             !changing velocity to slowness, etc.
            call newopt(data2, n1out, n2, vopt1, vopt2 )
         endif   
!
         count = srite( "out" , data2, nbyte2 )	
!             !write out the gather
         if( count .ne. nbyte2 ) then
            call seperr('Vconvert: error in data write.')
         endif
!
 100  continue
!
      return
      end
!****************************************************************************
!
!  Subroutine to convert velocity fields to slowness etc.
!
!  Author: Paul Fowler
!  Date: October 1986
!
      subroutine newopt( data, n1, n2, vopt1, vopt2 )
use sep
!
      implicit none
!
      integer n1	
!           !number of points in trace
      integer n2	
!           !number of traces in gather
      integer nsamp	
!           !number of points in gather
      integer vopt1	
!           !input velocity option
      integer vopt2	
!           !output velocity option
!
      real data(n1*n2)	
!           !input gather
!
      nsamp = n1*n2
!
      if( vopt1 .eq. 1 )then
         if( vopt2 .eq. 2 )then
						data=1/data
!            call inv( data, data, nsamp ) 
         elseif( vopt2 .eq. 3 )then
						data=1/data
!            call inv( data, data, nsamp ) 
						data=data**2
!            call ipow( data, 2, data, nsamp )
         elseif( vopt2 .eq. 4 )then
						data=data**2
!            call ipow( data, 2, data, nsamp )
         endif
      elseif( vopt1 .eq. 2 )then
         if( vopt2 .eq. 1 )then
						data=1/data
!            call inv( data, data, nsamp ) 
         elseif( vopt2 .eq. 3 )then
						data=data**2
!            call ipow( data, 2, data, nsamp )
         elseif( vopt2 .eq. 4 )then
						data=1/data
!            call inv( data, data, nsamp ) 
						data=data**2
!            call ipow( data, 2, data, nsamp )
         endif
      elseif( vopt1 .eq. 3 )then
         if( vopt2 .eq. 1 )then
						data=data**2
						data=sqrt(data)
!            call sqr( data, data, nsamp )
						data=1/data
!            call inv( data, data, nsamp ) 
         elseif( vopt2 .eq. 2 )then
						data=sqrt(data)
!            call sqr( data, data, nsamp )
         elseif( vopt2 .eq. 4 )then
						data=1/data
!            call inv( data, data, nsamp ) 
         endif
      elseif( vopt1 .eq. 4 )then
         if( vopt2 .eq. 1 )then
						data=sqrt(data)
!            call sqr( data, data, nsamp )
         elseif( vopt2 .eq. 2 )then
						data=sqrt(data)
!            call sqr( data, data, nsamp )
						data=1/data
!            call inv( data, data, nsamp ) 
         elseif( vopt2 .eq. 3 )then
						data=1/data
!            call inv( data, data, nsamp ) 
         endif
      endif   
!
      return
      end
!
!****************************************************************************
!
!  Subroutine to convert velocity fields from interval to rms and 
!  vice-versa.  Assumes input is velocity (slowness, etc.)
!  as evenly sampled function of time.
!
!  Author: Paul Fowler
!  Date: October 1986
!
      subroutine newtyp( data,   work, n1, n2, vtype1, &
     &                   vtype2, vopt, t0, dt        )
use sep
!
      implicit none
!
      integer it	
!           !loop counter over t
      integer ix	
!           !loop counter over x
      integer n1	
!           !number of points in trace
      integer n2	
!           !number of traces in gather
      integer nsamp	
!           !number of points in gather
      integer vopt	
!           !velocity, slowness, slowness squared option
      integer vtype1	
!           !input velocity option
      integer vtype2	
!           !output velocity option
!
      real dt		
!           !time sampling interval
      real t0		
!           !time of first sample
      real t		
!           !time
      real tsum		
!           !running time sum
      real vsum		
!           !running velocity sum
      real data(n1,n2)	
!           !input gather
      real work(n1)	
!           !work trace
!
      nsamp = n1*n2
!
      if( vtype1 .eq. vtype2 )then
         return
      endif
!
!     convert to velocity squared
!
      if( vopt .ne. 4 )then
         call newopt( data, n1, n2, vopt, 4 )
      endif
!
      if( (vtype1 .eq. 1) .and. (vtype2 .eq. 2) )then	
!           !interval to rms
!
         do 100 ix=1,n2
            vsum = data(1,ix) * t0
            do 200 it=2,n1
               t = t0 + (it-1)*dt
               vsum = vsum + data(it,ix) * dt
               data(it,ix) = vsum / t
 200        continue
 100    continue
!
      elseif( (vtype1 .eq. 2) .and. (vtype2 .eq. 1) )then	
!           !rms to interval
!
         do 300 ix=1,n2
            do 400 it=1,n1
               t = t0 + (it-1)*dt
               work(it) = data(it,ix) * t
 400        continue
            do 500 it=2,n1
               data(it,ix) = ( work(it) - work(it-1) ) / dt
 500        continue
 300    continue
!
      else
!
         call seperr('illegal vtype in newtyp')
! 
      endif
!
!     convert back from velocity squared
!
      if( vopt .ne. 4 )then
         call newopt( data, n1, n2, 4, vopt )
      endif
!
      return
      end
!
!**************************************************************************
!
!    subroutine to convert interval velocity (slowness, etc.)
!    data from v(t) to v(z) and vice versa.
!    Data is oversampled and then linearly interpolated.
!    Subroutine mkosmp must be invoked before first call to newpar
!    to compute array osmpwt of interpolation weights.
!
      subroutine newpar( data1, data2, work1, work2,  osmpwt,&
     &                   zin,   n1in,  n1out, d1in,   ovrsmp,&
     &                   d1out, o1in,  o1out, n2,     vpar1, &
     &                   vpar2, vopt,  lsinc, points, n1pad   )
use sep
!
      integer i1	
!           !loop counter over fast axis
      integer i2	
!           !loop counter over slower axis
      integer lsinc	
!           !length of sinc interpolator
      integer n1in	
!           !number of fast axis points on input
      integer n1out	
!           !number of fast axis points on output
      integer n1pad	
!           !number of points on fast axis after oversampling
      integer n2	
!           !number of intermediate axis points
      integer n3	
!           !number of points on slow axis
      integer ovrsmp	
!           !oversampling rate
      integer point	
!           !pointer to location in zin array
      integer points	
!           !ovrsmp-1
      integer vopt	
!           !velocity, slowness, etc. option
      integer vpar1	
!           !input time or depth option
      integer vpar2	
!           !output time or depth option
!
      real delta	
!           !fractional distance between samples for interpolation
      real d1in		
!           !fast axis input sampling interval
      real d1out	
!           !fast axis output sampling interval
      real o1in		
!           !fast axis input first sample value
      real o1out	
!           !fast axis output first sample value
      real z		
!           !computed depth or time value
      real data1(n1in,n2)	
!           !input gather
      real data2(n1out,n2)	
!           !output gather
      real work1(n1in)		
!           !work vector
      real work2(n1pad)		
!           !work vector
      real osmpwt(lsinc,points)	
!           !interpolator coefficient table
      real zin(n1pad)		
!           !converted values of input parameter
!
!     !onvert to velocity if it isnt already
!
      if( vopt .ne. 1 )then
         call newopt( data1, n1in, n2, vopt, 1 )
      endif
!
      do 100 i2=1,n2
!
!        Oversample the input data
!
					work1(1:n1in)=0.
					work2(1:n1pad)=0.
!         call setvalf( work1, n1in,0. )
!         call setvalf( work2, n1pad, 0. )
					work1(1:n1in)=data1(1:n1in,i2)	
!         call copy( n1in,data1(1,i2), work1 )
         if( ovrsmp .gt. 1 )then
            call ovsamp( work1, work2,  osmpwt, n1in,   n1pad,&
     &                   lsinc, ovrsmp, points          )
         else
!            call rvcopy( work1, work2, n1in )
						work2(1:n1in)=work1(1:n1in)
!            call copy( n1in,work1, work2 )
         endif
!
!        Now build table of times (or depths) equivalent to input
!        sampling in depth (or time).
!
         if((vpar1 .eq. 1) .and. (vpar2 .eq. 2))then  
!           !time to depth
!
            zin(1) = 0.5*o1in*work2(1)
!
            do 200 i1=2,n1pad
               zin(i1) = zin(i1-1) + 0.5*d1in*work2(i1)/ovrsmp
 200        continue
!
         elseif((vpar1 .eq. 2) .and. (vpar2 .eq. 1))then  
!           !depth to time
!
            zin(1) = 2.0*o1in/work2(1)
!
            do 400 i1=2,n1pad
               zin(i1) = zin(i1-1) + 2.0*d1in/(work2(i1)*ovrsmp)
 400        continue
!
         endif
!
!        Now interpolate data onto an evenly spaced output grid.
!        If output grid extends beyond input data, first and/or last
!        input values are just repeated.  If input grid extends
!        beyond output, data is just discarded.
!
         point = 1
         do 500 i1=1,n1out
            z = o1out + (i1-1)*d1out
            if( z .le. zin(1) )then
               data2(i1,i2) = work2(1)
            elseif( z .lt. zin(n1pad) )then
 999           if( zin(point) .lt. z )then
                  point = point + 1
                  go to 999
               endif
               point = point -1
               delta = ( z - zin(point) ) / &
     &			( zin(point+1) - zin(point) )
               data2(i1,i2) = (1.0-delta) * work2(point) &
     &                            + delta * work2(point+1)
            else
               data2(i1,i2) = work2(n1pad)
            endif
 500     continue
!
 100  continue
!
!     convert back from velocity squared
!
      if( vopt .ne. 1 )then
         call newopt( data2, n1out, n2, 1, vopt )
      endif
!
      return
      end

      subroutine mkosmp( osmpwt, points, lsinc, alpha )
use sep
!
!     Builds lookup table of sinc interpolator coefficients for oversampling
!
      implicit none
!
      integer hlsinc	
!           !half length of interpolator
      integer hlntrp	
!           !half length of interpolator
      integer i		
!           !loop counter 
      integer j		
!           !loop counter 
      integer k		
!           !loop counter 
      integer lsinc	
!           !maximum length of sinc interpolator
      integer points	
!           !number of interpolated points for oversampling
      real alpha	
!           !gaussian decay factor; taper ranges from 1.0 to alpha
      real delta	
!           !fractional distance to be interpolated
      real pi		
!           !the usual constant
      real sinpd	
!           !sin(pi*delta)/pi
      real osmpwt(lsinc,points)	
!           !coefficient table
!
      pi = 3.14159265
      hlsinc = lsinc/2
!
			osmpwt=0.
!      call setvalf( osmpwt, lsinc*points,0. )
!
      do 100 j=1,points
!
         delta = j * ( 1.0/(points+1) )
         sinpd = sin( pi*delta ) / pi
!
         do 200 k=1,lsinc
!
            osmpwt(k,j) = (-1) ** ( hlsinc - k ) * sinpd *&
     &         alpha**( ((hlsinc-k+delta)/hlsinc)**2 ) / &
     &		(hlsinc-k+delta)
!
 200     continue
!
 100  continue
!
      return
      end
      subroutine ovsamp( trace, outdat,   osmpwt, nt,    ntpad, &
     &                   lsinc, factor, points    )
use sep
!
!     Oversamples a trace 
!
      implicit none
!
      integer frac	
!           !fractional part of interpolated position
      integer hlsinc	
!           !half length of maximum sinc interpolator
      integer it	
!           !loop counter
      integer ik	
!           !loop counter
      integer j		
!           !array index 
      integer k		
!           !array index 
      integer lsinc	
!           !length of sinc interpolator
      integer n		
!           !integer part of interpolated position
      integer nt	
!           !length of trace before oversampling
      integer ntpad	
!           !length of trace after oversampling
      integer factor	
!           !oversampling factor
      integer points	
!           !factor - 1
      real outdat(ntpad)	
!           !data vector
      real osmpwt(lsinc,points)	
!           !coefficient table
      real trace(nt)	
!           !data trace vector
!
      hlsinc = lsinc/2
!
			outdat=0.
!      call setvalf(outdat,ntpad,0.)
!
      do 100 it=1,ntpad
!
         n = ( (it-1)/factor ) + 1
         frac = (it-1) - (n-1)*factor
!
         if( frac .eq. 0 ) then		
!           !no interpolation needed
!
            outdat(it) = trace(n)
!
         else
!
            do 200 ik=1,lsinc
!
               k = ik - hlsinc + n
               if( k .le. 1 )then
                  j = 1
               elseif( k .ge. nt )then
                  j = nt
               else 
                  j = k
               endif
!
               outdat(it) = outdat(it) + trace(j) * osmpwt(ik,frac)
!
 200        continue
            
         endif
!
 100  continue
!
      return
      end
!  $Id: vconvsubs.f90,v 1.1.1.1 2004/03/25 06:37:25 cvs Exp $

!<
! randm
!
! Description
!  random number generators
!
! Usage
!   call randm(randum)
!
! Output Parmaeters
! randum   -   real   return a number between 0.0 and 1.0.
!
! Comments
!   call rseed(seedin), seedin is an integer
!              set the seed for the random number generator.
!
! Category
! libsepmath subroutine
!>
!<
! grand
!
! Description
! generate gaussian random number
!
! Usage
!   call grand(gr)
!
! Output Parameters
! gr    -    real return a number with normal distribution,
!                   mean=0, standard deviation=1
!
! Comments
!   call rseed(seedin), seedin is an integer
!              set the seed for the random number generator.
!
! Revised - Ray Abma
!       set seed to 12345 if seed was not set by call to rseed
!
!
! Category
! libsepmath subroutine
!>
!
subroutine randm(randum)
!
  common /randumx/ seed
!
integer seed
real randum   
integer a,m,q,r,lo,hi,test
a = 16807
m=2147483647
q=127773
r=2836
if ( seed .eq. 0.0) then
  seed = 12345
end if
!
hi = seed/q
lo = mod(seed,q)
test = a * lo - r * hi
if ( test > 0 ) then
  seed = test
else
  seed = test + m
end if 
randum = float(seed)/float(m)
return
end
! set the seed for the generator
subroutine rseed(seedin)
!
  common /randumx/ seed
!
integer seed,seedin
seed = seedin
return
end 
!
!   produce a number with normal distribution, mean=0, standard deviation=1
!
subroutine grand(gr)
real r1,r2,gr,s
!
do while (1.eq.1)
  call randm(r1)
  call randm(r2)
  r1 = 2 * r1 -1.
  r2 = 2 * r2 -1.
  s = r1**2 + r2**2
  if (s < 1.) then
    exit
  end if
end do 
!
gr = r1 * sqrt( -2 * log(s)/s )
return
end  

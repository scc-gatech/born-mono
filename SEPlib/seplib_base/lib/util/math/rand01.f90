!<
! rand01
!
! Usage
! real rand01(iseed)
!
! INPUT PARAMETERS
! iseed - integer  integer seed to random number generator
!
! RETURN VALUES
! x= random value, uniform  distribution (0,1)
!
! DESCRIPTION
! Uniform random number generator
!
! CATEGORY
! Lib:Math:Random
!
! COMPILE LEVEL
! DISTR
!>
real function rand01( iseed)
integer ia, im,       iseed
parameter(ia = 727,im = 524287)
iseed = mod(iseed*ia,im)
rand01 =(float(iseed) - 0.5)/float(im - 1)
return
end  

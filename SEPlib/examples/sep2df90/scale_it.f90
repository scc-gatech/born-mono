!<
!scale_it
!
!Usage:
!scale_it.x  <in.H >out.H  scale=
!
!Input Parameters:
! scale - real scale value
!
!
!Description:
! Intro program taht simply scles the dataset
!
!
!>
!%end of self-documentation
!-------------------------------------------------
!
!Author: Robert Clapp, ESMB 463, 7230253
!
!Date Created:Mon Jul  7 16:22:44 PDT 1997
!
!Sergey: style modifications Jan 14 1998
!
!Purpose: 
!
!
program scale_it
  !use sep module containg pars, reed, rite, etc
  use sep

  implicit none
  integer                       :: n1,n2, i2
  real,dimension(:),allocatable :: data  
  real                          :: scale

  !start sep enviornment
  call sep_init(SOURCE)

  !call to get information from the history file
  call from_history(n1, n2, esize=4, compress = .true.)

  !obtain scale factor from the command line 
  call from_param("scale",scale)

  !close the output history file
  call sep_close()

  !allocate storage array 
  allocate(data(n1))

  !loop over calculated subcubes
  do i2=1,n2
     call sep_read(data)
     !scale the data and write it out
     call sep_write(data*scale)
  end do

  !free memory 
  deallocate(data)
  call exit (0)
end program scale_it

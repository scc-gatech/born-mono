
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
!Purpose: 
!
!
program scale_it
  implicit none
  integer :: n(2) ,i1,i2,esize,ndim,logic,mem
  real,dimension(:),pointer:: data  
  real scale,o,d
  character(len=128) :: temp_ch,label
  integer            :: tempi
  integer sep_get_number_data_axes
  integer getch,hetch,putch,sep_get_data_axis_par
 
  !start sep parameter grabing
  call initpar()

 
  !call to get information from the history file
  if(0 .eq. hetch("esize","d",esize)) esize=4

  !error checking mechanism
  if(esize .ne. 4 .and. esize .ne. 8)  &
    call erexit("Unacceptable esize value")

  !Get the number of data axes
  if(0 .ne. sep_get_number_data_axes("in",ndim)) &
    call erexit("Trouble obtaining number of data axes")

  n(2)=1; mem=1;

  if(0 .eq. hetch("n1","d",n(1))) &
    call erexit("can not obtain n1 required for SEP datasets")

  !read in cube dimensions, calculating what size subcube we
  !  can reasonably read in at one time 

  do i1=2,ndim
    if(0 .ne. sep_get_data_axis_par("in",i1,tempi,o,d,label)) &
       call erexit("trouble obtaining dimension the dataset ")
      if(mem * tempi < 2000000 .and. logic==0) then
        mem=mem*tempi
      n(1)=n(1)*tempi;
      else
      logic=1 
      n(2)=n(2)*tempi;
    end if
  end do

  !fake a real dataset if data is complex 
  n(1)=n(1)*esize/4

  !obtain scale factor from the command line 
  if(0 .eq. getch("scale","f",scale)) &
    call erexit("trouble obtaining scale factor.")

  !write scale factor to output history file 
  if(0 .ne. putch("scale","f",scale)) &
    call erexit("trouble putting scale factor into output history file.")

  !close the output history file
  call hclose()


  !allocate storage array 
  allocate(data(n(1)))


  !loop over calculated subcubes
  do i2=1,n(2)
    call sreed("in",data,n(1)*4)
    !scale the data 
    data=data*scale
    !write out data  to stdout ("out")
    call srite("out", data , n(1) *4)
  end do

  !free memory 
  deallocate(data)
end program scale_it

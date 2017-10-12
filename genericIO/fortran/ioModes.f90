module ioModes_mod
  use generic_io_mod
  use cio_mod
  !use sepio_mod
   type :: ioModes

   contains
     procedure,pass :: init
     procedure,pass :: getIO
     procedure,pass ::getDefaultIO
 end type
  contains

 
  subroutine init(struct)
      class(ioModes) :: struct
      integer :: narg,i
      integer,dimension(:) ,allocatable:: nlens
      character(len=1000) ,allocatable :: ar(:)
      character(len=99000) :: tmp
      narg=command_argument_count()
      allocate(ar(narg+1));
      allocate(nlens(narg+1))
      do i=0,narg
        call get_command_argument(i,ar(i+1))
      end do

      call to1DCharArray(ar,nlens,narg+1,tmp)
      call initializeIO(narg+1,nlens,trim(tmp)//C_NULL_CHAR)
      deallocate(ar)
      deallocate(nlens)
 end subroutine
 
 subroutine getIO(struct,myIO,nm)
   class(ioModes) :: struct
   class(genericIO),pointer :: myIO
   character(len=*) :: nm
    type(cIO)  :: my_CIO
!    type(sepIO)  :: my_SEP
!   if(trim(nm)=="SEP") then
!      allocate(myIO,source=my_SEP)
!   else
      allocate(myIO,source=my_CIO)
!   end if
   call myIO%init(nm)
 end subroutine
 
 subroutine getDefaultIO(struct,myIO)
    class(ioModes) :: struct
   class(genericIO),pointer :: myIO
   character(len=128) :: nm
    call getDefaultIOName(nm)
    call struct%getIO(myIO,nm)
 end subroutine

end module

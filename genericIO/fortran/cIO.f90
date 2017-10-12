module cio_mod
 use c_regfile_mod
 use generic_io_mod
 use c_param_mod

 implicit none
 
 
 
 type,extends(genericIO) :: cio
   contains
    procedure  :: init=>cInit
 end type
 
 contains
    subroutine cInit(struct,IO)
      class(cIO) :: struct
      character(len=*) IO
      type(cFile) :: dumF
      type(cParam) :: dumP
 
       struct%myType=IO
      allocate(struct%regFile,source=dumF)
      allocate(struct%param,source=dumP)
      struct%param%myIOName=IO
      struct%regFile%myIOName=IO
    end subroutine
    

    


end module

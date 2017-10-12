module rsfio_mod
 use rsf_regfile_mod
 use generic_io_mod
 use rsf_param_mod
 implicit none
 
 
 
 type,extends(genericIO) :: rsfIO
   type(file) :: myf
   contains
    procedure init=>sepInit
 end type
 
 contains
    subroutine sepInit(struct,IO)
      character(len=*) ::IO
      class(rsfIO) :: struct
      type(rsfFile) :: dumF
      type(rsfParam) :: dumP
      allocate(struct%regfile,source=dumF)
      allocate(struct%param,source=dumP)
      call sf_input()
    end subroutine
    


end module

module sepio_mod
 use sep_regfile_mod
 use generic_io_mod
 use sep_param_mod
 implicit none
 
 
 
 type,extends(genericIO) :: sepIO
   contains
    procedure  :: init=>sepInit
 end type
 
 contains
    subroutine sepInit(struct,IO)
      character(len=*) ::IO
      class(sepIO) :: struct
      type(sepFile) :: dumF
      type(sepParam) :: dumP
      allocate(struct%regFile,source=dumF)
      allocate(struct%param,source=dumP)
      call initpar()
    end subroutine
    


end module
